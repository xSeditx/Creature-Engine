#include"Threadpool.h"

#pragma warning( push )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4018 ) // Optimization off warning of mine



_static uint32_t Core::Threading::ThreadPool::JobQueue::queue_ID{0};


namespace Core
{
    namespace Threading
    {
		std::mutex DEBUGMutex;

		/* ============================================================
		 *                    Initializer                                    
		 * ============================================================ */
		ThreadPool::ThreadPool()
		{// Create a set number of Threads and Add Job Queues to the Threadpool 

			for (int N{ 0 }; N < ThreadCount; ++N)
			{// Creates and Runs Schedular

				Worker_Threads.emplace_back([&, N] {Run(N); });
			}
		}
		/* ============================================================ */


		/* ============================================================
		 *                    Executors                                     
		 * ============================================================ */
		void ThreadPool::Run(unsigned int _i)
		{ // Initializes Thread and starts the Queue running 
			while (true)
			{// Constantly run until application or user shuts it down

				Executor* Func{ nullptr };
				int QueID{ -1 };
				for (unsigned int N{ 0 }; N != ThreadCount; ++N)
				{// Cycle over all available Queues until one returns some work 

					if (ThreadQueue[static_cast<size_t>((_i + N) % ThreadCount)].try_Pop(Func))
					{// If Queue N succeeded at returning a function break the for loop and run the function
						QueID = ThreadQueue[static_cast<size_t>((_i + N) % ThreadCount)].g_ID();
						break;
					}
				}

				if (!Func && !ThreadQueue[_i].pop(Func))
				{// If there is no Function and the Queue fails to Pop it means that it is quiting time
					break;
				}
				if (QueID == -1)QueID = ThreadQueue[_i].g_ID();
				DEBUGPrint("Running: " << QueID );
				Func->Invoke();  // Invoke the returned function 
				delete &(*Func); // Destroy the Object which our Async Class Allocated
				DEBUGPrint("Finished: " << QueID);

			}
		}
		bool ThreadPool::JobQueue::try_Pop(Executor*& func)
		{// Try to pop a function off the Queue if it fails return false

			/* ~   CRITICAL SECTION   ~ */

			std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
			if (!Lock || TaskQueue.empty())
			{
				return false;
			}

			DEBUGPrint("Popped " << g_ID());
			func = std::move(TaskQueue.front());
			TaskQueue.pop_front();
			return true;
		}
		bool ThreadPool::JobQueue::pop(Executor*& func)
		{ /*  Pop function from Queue if previous Try pops failed wait on it

              Entire Scope is protected by the Queue Mutex */

			/* ~   CRITICAL SECTION   ~ */

			std::unique_lock<std::mutex> Lock{ QueueMutex };
			while (TaskQueue.empty() && !is_Done)
			{// If Queue is Empty and we are not Done Wait until there is work to do
				is_Ready.wait(Lock);
			}
			if (TaskQueue.empty())
			{// If Task Queue is empty and we are done, return false to Initiate shut down process
				return false;
			}

			DEBUGPrint("Popped " << g_ID());
			func = std::move(TaskQueue.front());
			TaskQueue.pop_front();
			return true;
		}
		/* ============================================================ */


		/* ============================================================
		 *                    Submitters                                
		 * ============================================================ */
		bool ThreadPool::JobQueue::try_push(Executor* func)
		{// Attempts to add a function to the Queue if unable to lock return false 

			{/* ~   CRITICAL SECTION   ~ */

				std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
				if (!Lock)
				{// If our mutex is already locked simply return 
					return false;
				}

				DEBUGPrint("Pushed " << g_ID());
				TaskQueue.push_back(std::move(func));    // Else place on the back of our Queue

			}/* ~ END CRITICAL SECTION ~ */              // Unlock the Mutex 

			is_Ready.notify_one();                       // Tell the world about it 
			return true;                                 // Lets Async know you succeeded
		}
		void ThreadPool::JobQueue::push(Executor* func)
		{// Adds a Function to our Queue

			{/* ~   CRITICAL SECTION   ~ */

				DEBUGPrint("Pushed " << g_ID());
				std::unique_lock<std::mutex> Lock{ QueueMutex };
				TaskQueue.emplace_back(std::move(func));

			}/* ~ END CRITICAL SECTION ~ */
		}
		/* ============================================================ */
		

		/* ============================================================
		 *                    Destructors
		 * ============================================================ */
		void ThreadPool::JobQueue::Done()
		{// Triggers the Threadpool to shut down when the application ends or user ask it to

			{/* ~   CRITICAL SECTION   ~ */

				std::unique_lock<std::mutex> Lock{ QueueMutex };
				is_Done = true;

			}/* ~ END CRITICAL SECTION ~ */

			is_Ready.notify_all();
		}
		ThreadPool::~ThreadPool()
		{
			for (auto& Q : ThreadQueue)
			{// Signal to all Queues that we are closing down Shop
				Q.Done();
			}
			for (auto& WT : Worker_Threads)
			{// Once those threads Finish we Join all our threads and close the Threadpool
				WT.join();
			}
		}
		/* ============================================================ */

	}// End NS Threading
}// End NS Core

#pragma warning( pop )




/*
==========================================================================================================================================================================
                                                           Trash:
==========================================================================================================================================================================
*/
