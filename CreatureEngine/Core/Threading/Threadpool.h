#pragma once
#include <thread>
#include <functional>
#include <future>
#include <deque>
#include <queue>

typedef void(*Worker_Function)();
// NOTE: https://riptutorial.com/cplusplus/example/15806/create-a-simple-thread-pool
namespace Core
{
	namespace Threading
	{

		struct JobQueue
		{
		public:
			JobQueue() = default;

			std::deque<std::packaged_task<void()>> TaskQueue;
			std::vector<std::future<void>> finished;


			std::condition_variable is_Ready;
			std::mutex QueueMutex;
			bool is_Done{ false };

			bool Try_Pop(std::packaged_task<void()> &func);
			
			void Done();
			bool pop(std::packaged_task<void()> &func);
			template<typename _FUNC> bool try_push(_FUNC&& func)
			{
				{
					std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
					if (!Lock)
					{
						return false;
					}
					TaskQueue.emplace_back(std::forward<_FUNC>(func));
				}
				is_Ready.notify_one();
				return true;
			}
			template<typename _FUNC> void push(_FUNC&& func)
			{
				{
					std::unique_lock<std::mutex> Lock{ QueueMutex };
					TaskQueue.emplace_back(std::forward<_FUNC>(func));
				}
			}
		};

		class ThreadPool
		{
			const size_t ThreadCount{ std::thread::hardware_concurrency() };
			std::vector<std::thread> Worker_Threads;
			std::vector<JobQueue> ThreadQueue{ ThreadCount };
			std::atomic<unsigned int> Index{ 0 };

			ThreadPool();
			~ThreadPool();
// class _RET = std::result_of_t<_FUNC>,class _RET =std::result_of_t<_FUNC()>, 

			void Run(unsigned int _i);
		public:

			static ThreadPool &get()
			{
				static ThreadPool instance;
				return instance;
			}
			template<typename _FUNC, typename...ARGS >
			auto Async(_FUNC&& _func, ARGS&&...args)->std::future<decltype(_func(args...))>//  //
			{
				auto i = Index++;

			//	std::function<decltype(_func(args...))()> func = ;
				auto task_ptr = 
					std::make_shared<std::packaged_task<decltype(_func(args...))()>>
					(
						std::bind
						(
							std::forward<_FUNC>(_func),
							std::forward<ARGS>(args)...
						)
						);
				std::function<void()> wrapper_func = [task_ptr]() {
					(*task_ptr)();
				};


				int K = 8;	
				for (unsigned int n = 0; n != ThreadCount * K; ++n) // K is Tunable 
				{
					if (ThreadQueue[(i + n) % ThreadCount].try_push(wrapper_func))
					{
						return task_ptr->get_future();//taskPTR->get_future();
					}
				}
				 
				ThreadQueue[i % ThreadCount].push(std::forward<std::function<void()>>(wrapper_func));
				return  task_ptr->get_future();
			}
		};

	}// End NS Threading
}// End NS Core 