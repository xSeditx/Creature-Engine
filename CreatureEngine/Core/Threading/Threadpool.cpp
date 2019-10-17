#include"Threadpool.h"



bool JobQueue::Try_Pop(Worker_Function &func)
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
void JobQueue::Done()
{
	{
		std::unique_lock<std::mutex> Lock{ QueueMutex }; is_Done = true;
	}
	is_Ready.notify_all();
}
bool JobQueue::pop(Worker_Function &func)
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
template<typename F> bool JobQueue::try_push(F&& func)
{
	{
		std::unique_lock<std::mutex> Lock{ QueueMutex ,std::try_to_lock };
		if (!Lock)
		{
			return false;
		}
		TaskQueue.emplace_back(std::forward<F>(func));
	}
	is_Ready.notify_one();
	return true;
}
template<typename F> void JobQueue::push(F&& func)
{
	{
		std::unique_lock<std::mutex> Lock{ QueueMutex };
		TaskQueue.emplace_back(std::forward<F>(func));
	}
}







ThreadPool::ThreadPool()
{
	for_loop(N, ThreadCount)
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
		Worker_Function Func;
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

template<typename F> void ThreadPool::Async(F&& _func)
{
	auto i = Index++;
	int K = 8;
	for (unsigned int n = 0; n != ThreadCount * K; ++n) // K is Tunable 
	{
		if (ThreadQueue[(i + n) % ThreadCount].try_push(Worker_Function(_func)))
		{
			return;
		}
	}
	ThreadQueue[i % ThreadCount].push(std::forward<F>(_func));
}
