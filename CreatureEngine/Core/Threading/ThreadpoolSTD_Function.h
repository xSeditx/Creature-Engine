#pragma once
#include <thread>
#include <functional>
#include <future>
#include <deque>
#include <queue>
#include <array>
#include <tuple>


#include<Windows.h>
#include"Core\Common.h"
#include<iostream>

#pragma warning( push )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4018 ) // Optimization off warning of mine

#define BLOCK_SIZE    1024 * 100
#define MY_WRAPPER

static void* Wrap_MemoryBlock = malloc(BLOCK_SIZE * sizeof(char)); // nullptr;
static uint16_t  Wrap_Offset{ 0 };

template<typename R>
bool is_ready(std::future<R> const& f)
{
	return f.valid() ? f.wait_for(std::chrono::seconds(0)) == std::future_status::ready : false;
}

/// template<typename Function, typename ...Args>
/// result_type = std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>;

extern std::atomic<int> Function_Counter;


typedef std::function<void()> Worker_Function;

//Type Traits: https://code.woboq.org/llvm/libcxx/include/type_traits.html
// NOTE: https://riptutorial.com/cplusplus/example/15806/create-a-simple-thread-pool
namespace Core
{
	namespace Threading
	{

		struct JobQueue
		{
		public:
			JobQueue() = default;

			std::condition_variable is_Ready;
			std::mutex QueueMutex;
			bool is_Done{ false };
			void Done();


			std::deque<Worker_Function> TaskQueue;

			bool Try_Pop(Worker_Function& func);
			bool pop(Worker_Function& func);
			template<typename _FUNC> bool try_push(_FUNC&& func)
			{
				{
					std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
					if (!Lock)
					{/* If our mutex is already locked simply return */
						return false;
					}
					/* Else place on the back of our Queue*/
					TaskQueue.emplace_back(std::forward<_FUNC>(func));
				}
				is_Ready.notify_one();
				return true;
			}
			template<typename _FUNC> void push(_FUNC&& func)
			{/* Adds a Function to our Queue */
				{
					std::unique_lock<std::mutex> Lock{ QueueMutex };
					TaskQueue.emplace_back(std::forward<_FUNC>(func));//std::move<_FUNC> std::forward<_FUNC&>(&((Worker_Function *)
				}
			}
		};
		class ThreadPool
		{
			const unsigned int ThreadCount{ std::thread::hardware_concurrency() - 1 };
			std::vector<std::thread> Worker_Threads;
			std::vector<JobQueue> ThreadQueue{ ThreadCount };
			std::atomic<unsigned int> Index{ 0 };

			ThreadPool();
			~ThreadPool();

			void Run(unsigned int _i);
		public:

			static ThreadPool& get()
			{
				static ThreadPool instance;
				return instance;
			}
			
			template<typename _FUNC, typename...ARGS >
			auto Async(_FUNC&& _func, ARGS&&...args)->std::future<decltype(_func(args...))>//  //
			{// Accept arbitrary Function signature and arguments
				auto i = Index++;

				auto task_PTR =
					std::make_shared<std::packaged_task<decltype(_func(args...))()>>
					(// Bind the argument and make a new packaged task to retrieve a future from on completion. 
						std::bind
						(
							std::forward<_FUNC>(_func),
							std::forward<ARGS>(args)...
						)
						);
				//	if(_q[(i + n) % _count]
				std::function<void()> wrapper_func = [task_PTR]()
				{// Utilize std::functions type erasure to wrap the Task pointer
					(*task_PTR)();
				};


				int K = 8;
				for (unsigned int n{ 0 }; n != ThreadCount * K; ++n) // K is Tunable 
				{
					if (ThreadQueue[static_cast<size_t>((i + n) % ThreadCount)].try_push(wrapper_func))
					{
						return task_PTR->get_future();
					}
				}

				ThreadQueue[i % ThreadCount].push(wrapper_func);
				return  task_PTR->get_future();
			}
		};
	}// End NS Threading
}// End NS Core 


int TestFunctionA();
int TestFunctionB(int _param);
uint64_t TestFunctionLarge(uint64_t _p1, uint64_t _p2, uint64_t _p3, uint64_t _p4, uint64_t _p5);

float TestFunctionC(float _paramA, int _paramB);

std::string TestFunctionD(float _paramA, int _paramB);
std::vector<uint32_t> TestFunctionE(int _paramA);
std::vector<uint32_t> TestFunctionF(int _paramA);
std::vector<uint32_t> TestFunctionG(int _paramA);
std::vector<uint32_t> TestFunctionH(int _paramA);
std::vector<uint32_t> TestFunctionI(int _paramA);
std::vector<uint32_t> TestFunctionJ(int _paramA);

uint64_t TestCompile(std::vector<std::vector<uint32_t>> _input);
uint64_t Worker_TestFunction(size_t _count);




template<typename _Ty, size_t _SZ>
class ring_buffer
{
public:
	using value_type = _Ty;
	using reference = _Ty&;
	using pointer = const _Ty*;

	ring_buffer() = default;

	bool push_back(reference _element)
	{/// Just deferring this function to make it compatible with older Queue system for Threadpool
		return push(_element);
	}
	bool push(reference _element)
	{// Adds new Element to Queue
		std::atomic<size_t> OldWritePosition;/// = WritePosition.load();
		std::atomic<size_t> NewWritePosition = NextElement(OldWritePosition);
		if (NewWritePosition)/// == ReadPosition.load())
		{/* If Read position and Write position are the same Buffer is Full */
			return false;
		}
		///		Data[OldWritePosition] = _element;
		WritePosition = NewWritePosition;
	}

	/// WE MIGHT BE ABLE TO PACK THE READER AND WRITER INTO THE SAME ATOMIC INTEGER WHICH WILL REDUCE THE OVERHEAD
	bool pop(reference _returnElement)
	{/* Returns True if more elements in Queue*/
		while (true)
		{/* READER Multiple in Existance */
			std::atomic<size_t> OldReadPosition = ReadPosition.load();

			if (WritePosition.load() == OldReadPosition.load())
			{// If attempting to read the same position again or buffer is full return false;
				return false;
			}

			_returnElement = Data[OldReadPosition];// Perhaps std::move() would be better for performance
			if (ReadPosition.compare_exchange_strong(OldReadPosition, NextElement(OldReadPosition)))
			{
				return true;
			}
		}
	}

	auto front()
	{/// Possibly wrong currently a place holder to make the experimental ringbuffer compatible with older style of queue
		return &Data[ReadPosition];
	}
	void pop_front()
	{/// Possibly wrong currently a place holder to make the experimental ringbuffer compatible with older style of queue
		WritePosition = NextElement(WritePosition);
	}
	bool empty()
	{/// Possibly wrong currently a place holder to make the experimental ringbuffer compatible with older style of queue
		return (ReadPosition == WritePosition);
	}

	reference operator[](const int _offset)
	{
		return Data[_offset]; //[ReadPosition % BufferSize];
	}

	bool destroy()
	{
		delete[](Data);
	}

	size_t size()
	{
		return BufferSize;
	}

private:

	std::array<std::atomic<_Ty*>, _SZ + 1> Data;

	size_t NextElement(size_t _pos)
	{
		return ++_pos == BufferSize ? 0 : _pos;
	}

	size_t
		ReadPosition{ 0 },
		WritePosition{ 0 };
	size_t Length, Size, Start, End, Elements;
	size_t BufferSize = _SZ + 1;
};

#pragma warning( pop )