#include"Threadpool.h"

#pragma warning( push )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4018 ) // Optimization off warning of mine

std::atomic<uint32_t> Create{ 0 };
std::atomic<uint32_t> Delete{ 0 };
std::atomic<uint32_t> GotFuture{ 0 };


namespace Core
{
	namespace Threading
	{
		bool ThreadPool::JobQueue::try_Pop(Wrapper_Base*& func)
		{/* Try to aquire a function off the Queue to run */
			std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
			if (!Lock || TaskQueue.empty())
			{
				return false;
			}
			func = TaskQueue.front();//std::forward(TaskQueue.front());// std::move(
		 	TaskQueue.pop_front();
			return true;
		}
		bool ThreadPool::JobQueue::pop(Wrapper_Base*& func)
		{
			std::unique_lock<std::mutex> Lock{ QueueMutex };
			while (TaskQueue.empty() && !is_Done)
			{
				is_Ready.wait(Lock);
			}
			if (TaskQueue.empty())
			{
				return false;
			}
			func = std::move(TaskQueue.front());
			TaskQueue.pop_front();
			return true;
		}
		void ThreadPool::JobQueue::Done()
		{
			{
				std::unique_lock<std::mutex> Lock{ QueueMutex };
				is_Done = true;
			}
			is_Ready.notify_all();
		}

		ThreadPool::ThreadPool()
		{
			for (int N{ 0 }; N < ThreadCount; ++N)
			{
				Worker_Threads.emplace_back([&, N] {Run(N); });
			}
		}
		ThreadPool::~ThreadPool()
		{
			for (auto& Q : ThreadQueue)
			{
				Q.Done();
			}
			for (auto& WT : Worker_Threads)
			{
				WT.join();
			}
		}
		void ThreadPool::Run(unsigned int _i)
		{
			while (true)
			{
				Wrapper_Base* Func{ nullptr };
				for (unsigned int N{ 0 }; N != ThreadCount; ++N)
				{
					if (ThreadQueue[static_cast<size_t>((_i + N) % ThreadCount)].try_Pop(Func))
					{
						break;
					}
				}
				if (!Func && !ThreadQueue[_i].pop(Func)){break;}
				Func->Invoke();
			 	delete &(*Func);
			}
		}
	}// End NS Threading
}// End NS Core

#pragma warning( pop )

/*NOTES:


Intel Game Engine Design:
https://software.intel.com/en-us/articles/designing-the-framework-of-a-parallel-game-engine


Faster STD::FUNCTION Implementation
https://github.com/skarupke/std_function/blob/master/function.h


Lock Free Ring Buffer
https://github.com/tempesta-tech/blog/blob/master/lockfree_rb_q.cc


Lock-Free Programming
https://www.cs.cmu.edu/~410-s05/lectures/L31_LockFree.pdf


A Fast Lock-Free Queue for C++
http://moodycamel.com/blog/2013/a-fast-lock-free-queue-for-c++


Introduction to Multithreaded Algorithms
http://ccom.uprrp.edu/~ikoutis/classes/algorithms_12/Lectures/MultithreadedAlgorithmsApril23-2012.pdf


A Thread Pool with C++11
http://progsch.net/wordpress/?p=81


Parallelizing the Naughty Dog Engine
https://www.gdcvault.com/play/1022186/Parallelizing-the-Naughty-Dog-Engine


C++11 threads, affinity and hyperthreading
https://eli.thegreenplace.net/2016/c11-threads-affinity-and-hyperthreading/


Thread pool worker implementation
https://codereview.stackexchange.com/questions/60363/thread-pool-worker-implementation


Thread pool implementation using c++11 threads
https://github.com/mtrebi/thread-pool


C++11 Multithreading – Part 8: std::future , std::promise and Returning values from Thread
https://thispointer.com/c11-multithreading-part-8-stdfuture-stdpromise-and-returning-values-from-thread/


CppCon 2015: Fedor Pikus PART 2 “Live Lock-Free or Deadlock (Practical Lock-free Programming) ”
Queue
https://www.youtube.com/watch?v=1obZeHnAwz4&t=3055s


Thread Pool Implementation on Github:
https://github.com/mtrebi/thread-pool/blob/master/README.md#queue


Threadpool with documentation:
https://www.reddit.com/r/cpp/comments/9lvji0/c_threadpool_with_documentation/


Original White paper on Work stealing Queues:
http://supertech.csail.mit.edu/papers/steal.pdf


Code overview - Thread Pool & Job System
https://www.youtube.com/watch?v=Df-6ws_EZno


Type Traits Reference
https://code.woboq.org/llvm/libcxx/include/type_traits.html

Aquiring results of a templated function:
template<typename Function, typename ...Args>
result_type = std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>;

MSVC Threadpool implementation for Concurrency:
https://docs.microsoft.com/en-us/cpp/parallel/concrt/task-scheduler-concurrency-runtime?view=vs-2019


Simple Threadpool Implementation:
https://riptutorial.com/cplusplus/example/15806/create-a-simple-thread-pool


Use this switch with Compiler Explorer inorder to allow it to compile: -std=c++17 -O3


//template<typename _F, typename ...ARGS>
//uint16_t Wrapper<_F, ARGS...>::Offset{ 0 };

https://www.youtube.com/watch?v=zULU6Hhp42w&list=PLl8Qc2oUMko_FMAaK7WY4ly0ikLFrYCE3&index=4

*/