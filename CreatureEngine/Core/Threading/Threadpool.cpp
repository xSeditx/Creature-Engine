#include"Threadpool.h"


namespace Core
{
	namespace Threading
	{

		bool JobQueue::Try_Pop(std::packaged_task<void()> &func)
		{/* Try to aquire a function off the Queue to run */
			std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
			if (!Lock || TaskQueue.empty())
			{
				return false;
			}
			func = std::move(TaskQueue.front());
			TaskQueue.pop_front();
			return true;
		}
		bool JobQueue::pop(std::packaged_task<void()> &func)
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



		void JobQueue::Done()
		{
			{
				std::unique_lock<std::mutex> Lock{ QueueMutex }; is_Done = true;
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
				std::packaged_task<void()> Func;
				for (unsigned int N = 0; N != ThreadCount; ++N)
				{
					if (ThreadQueue[(size_t)(_i + N) % ThreadCount].Try_Pop(Func))
					{
						break;
					}
				}
				if (!ThreadQueue[_i].pop(Func))break;
				Func();
			}
		}



	}// End NS Threading
}// End NS Core