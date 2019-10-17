#pragma once
#include <thread>
#include <functional>
#include <future>
#include <deque>
#include <queue>

typedef void(*Worker_Function)();

struct JobQueue
{
public:
	JobQueue() = default;
	std::deque< Worker_Function> TaskQueue;
	std::condition_variable is_Ready;
	std::mutex QueueMutex;
	bool is_Done{ false };

	bool Try_Pop(Worker_Function &func)
	{// Try to aquire a function off the Queue to run
		std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
		if (!Lock || TaskQueue.empty())
		{
			return false;
		}
		func = std::move(TaskQueue.front());
		TaskQueue.pop_front();
		return true;
	}

	void Done()
	{
		{
			std::unique_lock<std::mutex> Lock{ QueueMutex }; is_Done = true;
		}
		is_Ready.notify_all();
	}
	bool pop(Worker_Function &func)
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
	template<typename F> bool try_push(F&& func)
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
	template<typename F> void push(F&& func)
	{
		{
			std::unique_lock<std::mutex> Lock{ QueueMutex };
			TaskQueue.emplace_back(std::forward<F>(func));
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

	void Run(unsigned int _i);
public:

	static ThreadPool &get()
	{
		static ThreadPool instance;
		return instance;
	}

	template<typename F>
	void Async(F&& _func);
};
