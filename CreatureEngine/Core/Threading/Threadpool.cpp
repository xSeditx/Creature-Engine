#include"Threadpool.h"

#pragma warning( push )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4018 ) // Optimization off warning of mine


namespace Core
{
    namespace Threading
    {
        
        bool ThreadPool::JobQueue::try_Pop(Wrapper_Base*& func)
        {// Try to pop a function off the Queue if it fails return false
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
        {// Pop function from Queue if previous Try pops failed wait on it

            std::unique_lock<std::mutex> Lock{ QueueMutex };
            while (TaskQueue.empty() && !is_Done)
            {// If Queue is Empty and we are not Done Wait until there is work to do
                is_Ready.wait(Lock);
            }
            if (TaskQueue.empty())
            {// If Task Queue is empty and we are done, return false to Initiate shut down process
                return false;
            }
            func = std::move(TaskQueue.front());
            TaskQueue.pop_front();
            return true;
        }


        bool ThreadPool::JobQueue::try_push(Wrapper_Base* func)
        {// Attempts to add a function to the Queue if unable to lock return false 
            {
                std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
                if (!Lock)
                {// If our mutex is already locked simply return 
                    return false;
                }
                                                     
                TaskQueue.push_back(std::move(func));    // Else place on the back of our Queue
            }                                            // Unlock the Mutex 
            is_Ready.notify_one();                       // Tell the world about it 
            return true;                                 // Lets Async know you succeeded
        }


        void ThreadPool::JobQueue::push(Wrapper_Base* func)
        {// Adds a Function to our Queue
            {
                std::unique_lock<std::mutex> Lock{ QueueMutex };
                TaskQueue.emplace_back(std::move(func));//std::move<_FUNC> std::forward<_FUNC&>(&((Worker_Function *)
            }
        }


        void ThreadPool::JobQueue::Done()
        {// Triggers the Threadpool to shut down when the application ends or user ask it to
            {
                std::unique_lock<std::mutex> Lock{ QueueMutex };
                is_Done = true;
            }
            is_Ready.notify_all();
        }
    

        ThreadPool::ThreadPool()
        {// Create a set number of Threads and Add Job Queues to the Threadpool 

            for (int N{ 0 }; N < ThreadCount; ++N)
            {// Creates and Runs Schedular

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
        { // Initializes Thread and starts the Queue running 
            while (true)
            {// Constantly run until application or user shuts it down

                Wrapper_Base* Func{ nullptr };

                for (unsigned int N{ 0 }; N != ThreadCount; ++N)
                {// Cycle over all available Queues until one returns some work 
                    if (ThreadQueue[static_cast<size_t>((_i + N) % ThreadCount)].try_Pop(Func))
                    {// If Queue N succeeded at returning a function break the for loop and run the function
                        break;
                    }
                }

                if (!Func && !ThreadQueue[_i].pop(Func))
                {// If there is no Function and the Queue fails to Pop it means that it is quiting time
                    break;
                }
                Func->Invoke(); // Invoke the returned function 
                delete &(*Func); // Destroy the Object which our Async Class Allocated
            }
        }

    }// End NS Threading
}// End NS Core

#pragma warning( pop )




/*
==========================================================================================================================================================================
                                                           Trash:
==========================================================================================================================================================================
*/
