#pragma once
#include <thread>
#include <functional>
#include <future>
#include <deque>
#include <queue>
#include <array>
#include <tuple>
#include <type_traits>

#include"Core\Common.h"
#include<iostream>

#pragma warning( push )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4018 ) // Optimization off warning of mine

#define BLOCK_SIZE    1024 * 100
#define MY_WRAPPER

static void* Wrap_MemoryBlock = malloc(BLOCK_SIZE * sizeof(char)); // nullptr;
static uint16_t  Wrap_Offset{ 0 };

///================ Test Code Deprecated ==============================
extern std::atomic<uint32_t> Create;
extern std::atomic<uint32_t> Delete;
extern std::atomic<uint32_t> GotFuture;
///====================================================================

/* Non-blocking test of std::future to see if value is avalible yet */
template<typename R>
bool is_ready(std::future<R> const& f)
{
	return f.valid() ? f.wait_for(std::chrono::seconds(0)) == std::future_status::ready : false;
}

namespace Core
{
	namespace Threading
	{
		class ThreadPool
		{
		public:
			NO_COPY_OR_ASSIGNMENT(ThreadPool);

//
			struct Wrapper_Base
			{/// __declspec(novtable) USE THIS
				virtual ~Wrapper_Base() {
				}//Print("Base");

				virtual void Invoke() = 0;
				void operator()()
				{
					Invoke();
				}
				/* Implement a Then, When_all and Deferred Function handling */

				enum asyncStatus
				{
					Empty, Valid, Waiting, Busy, Submitted, Ready, Aquired
				};

				asyncStatus Status{ Empty };
			};

			template<typename _Func, typename ...ARGS>
			struct asyncTask final
				: public Wrapper_Base 
			{
				using type = std::invoke_result_t<_Func, ARGS...>;// typename std::decay_t<_Func>, typename std::decay_t<ARGS>... > ; //std::invoke_result_t<_Func&, ARGS&...>;
				using Fptr = type(*)(ARGS...);
				Fptr Function;

				std::tuple<ARGS...> Arguments;
				std::promise<type> ReturnValue;

				asyncTask(_Func&& _function, ARGS&&... _args) noexcept
					:
					Function(std::forward<_Func>(_function)),
					Arguments(std::forward<ARGS>(_args)...)
				{
					Status = Valid;
				}
				virtual ~asyncTask() = default;

				asyncTask(asyncTask&& _other) noexcept
					:
					Function(std::move(_other.Function)),
					Arguments(std::forward<ARGS>(_other.Arguments)),
					ReturnValue(std::move(_other.ReturnValue))
				{
					std::cout << "Called the Forward Function" << "\n";
				}
				asyncTask& operator=(asyncTask&& _other) noexcept
				{
					Function = std::move(_other.Function);
					Arguments = std::forward<ARGS>(_other.Arguments);
					ReturnValue = std::move(_other.ReturnValue);
					std::cout << "Called Assignment Operator " << "\n";
				}


				virtual void Invoke() override
				{
					Status = Busy;
					auto result = std::apply(Function, Arguments);
					ReturnValue.set_value(result);
					Status = Waiting;
				}

				std::future<type> get_future()
				{
					Status = Submitted;
					++Create;
					GotFuture++;
//					RefCount++;
					return ReturnValue.get_future();
				}

				asyncTask(const asyncTask&) = delete;
				asyncTask& operator=(const asyncTask& _other) = delete;
			};

			struct JobQueue
			{
			public:
				JobQueue() = default;

				std::condition_variable is_Ready;
				std::mutex QueueMutex;
				bool is_Done{ false };
				void Done();

				std::deque<Wrapper_Base*> TaskQueue;

				bool try_Pop(Wrapper_Base*& func);
				bool pop(Wrapper_Base*& func);

				bool try_push(Wrapper_Base* func)
				{
					{
						std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
						if (!Lock)
						{/* If our mutex is already locked simply return */
							return false;
						}
						/* Else place on the back of our Queue*/
						TaskQueue.push_back(std::move(func));//(func));std::move<_FUNC>
					}/* Unlock the Mutex */
					is_Ready.notify_one(); /* Tell the world about it */
					return true;/* Lets Async know u*/
				}
				void push(Wrapper_Base* func)
				{/* Adds a Function to our Queue */
					{
						std::unique_lock<std::mutex> Lock{ QueueMutex };
						TaskQueue.emplace_back(std::move(func));//std::move<_FUNC> std::forward<_FUNC&>(&((Worker_Function *)
					}
				}
			};


			const unsigned int ThreadCount{  std::thread::hardware_concurrency() * 3};
			std::vector<std::thread> Worker_Threads;
			std::vector<JobQueue> ThreadQueue{ ThreadCount };
			std::atomic<unsigned int> Index{ 0 };


			ThreadPool();
			~ThreadPool();

			void Run(unsigned int _i);

		public:
			static ThreadPool &get()
			{
				static ThreadPool instance;
				return instance;
			}

			template<typename _FUNC, typename...ARGS >
			auto Async(_FUNC&& _func, ARGS&&... args)->std::future<typename asyncTask<_FUNC, ARGS... >::type>
			{// Accept arbitrary Function signature and arguments into a Work pool for Asynchronous execution
				auto _function = new asyncTask<_FUNC, ARGS... >(std::move(_func), std::forward<ARGS>(args)...);
				auto result = _function->get_future();
				
				auto i = Index++;
				int K = 1;
				for (unsigned int n{ 0 }; n != ThreadCount * K; ++n) // K is Tunable 
				{
					if (ThreadQueue[static_cast<size_t>((i + n) % ThreadCount)].try_push(static_cast<Wrapper_Base*>(_function)))
					{
						return result;
					}
				}

				ThreadQueue[i % ThreadCount].push(static_cast<Wrapper_Base*>(_function));
				return result;
			}
		};
	}// End NS Threading
}// End NS Core 



#pragma warning( pop )


