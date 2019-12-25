#include"Threadpool.h"
#include<stdio.h>
#pragma warning( push )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4018 ) // Optimization off warning of mine

namespace Core
{
    namespace Threading
    {
		_static std::atomic<int> ThreadPool::RunningThreads{ 0 };

		/* ============================================================
		 *                    Initializer                                    
		 * ============================================================ */
		ThreadPool::ThreadPool()
		{// Create a set number of Threads and Add Job Queues to the Threadpool 

			for (int N{ 0 }; N < ThreadCount; ++N)
			{// Creates and Runs Schedular

				Worker_Threads.emplace_back([&, N] {ThreadQueue[N].ThreadID = std::thread::id(); Run(N); });
			}
		}
		/* ============================================================ */

		/* ============================================================
		 *                    Executors                                     
		 * ============================================================ */
		void ThreadPool::Run(unsigned int _i)
		{ // Initializes Thread and starts the Queue running 
			Print("Running: "<<_i);
			while (true)
			{// Constantly run until application or user shuts it down

				Executor* Function{ nullptr };

				for (unsigned int N{ 0 }; N != ThreadCount; ++N)
				{// Cycle over all available Queues until one returns some work 

					if (ThreadQueue[static_cast<size_t>((_i + N) % ThreadCount)].try_Pop(Function))
					{// If Queue N succeeded at returning a function break the for loop and run the function
						break;
					}
				}

				if (!Function && !ThreadQueue[_i].pop(Function))
				{// If there is no Function and the Queue fails to Pop it means that it is quiting time
					break;
				}
				++RunningThreads;
				Function->Invoke();  // Invoke the returned function
				--RunningThreads;
				delete &(*Function); // Destroy the Object which our Async Class Allocated
			}
		}
 		bool ThreadPool::JobQueue::try_Pop(Executor*& _func)
		{// Try to pop a function off the Queue if it fails return false

			/* ~   CRITICAL SECTION   ~ */
			std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
			if (!Lock || TaskQueue.empty())
			{
				return false;
			}

			_func = std::move(TaskQueue.front());
			TaskQueue.pop_front();
			return true;
		}
		bool ThreadPool::JobQueue::pop(Executor*& _func)
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
			// Move the pointer to the function pointer from our Queue into our _func object

			_func = std::move(TaskQueue.front());
			TaskQueue.pop_front();
			return true;
		}
		/* ============================================================ */

		/* ============================================================
		 *                    Submitters                                
		 * ============================================================ */
		bool ThreadPool::JobQueue::try_push(Executor* _func)
		{// Attempts to add a function to the Queue if unable to lock return false 

			{/* ~   CRITICAL SECTION   ~ */

				std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
				if (!Lock)
				{// If our mutex is already locked simply return 
					return false;
				}
				TaskQueue.push_back(std::move(_func));    // Else place on the back of our Queue

			}/* ~ END CRITICAL SECTION ~ */              // Unlock the Mutex 

			is_Ready.notify_one();                       // Tell the world about it 
			return true;                                 // Lets Async know you succeeded
		}
		void ThreadPool::JobQueue::push(Executor* _func)
		{// Adds a Function to our Queue

			{/* ~   CRITICAL SECTION   ~ */

				std::unique_lock<std::mutex> Lock{ QueueMutex };
				TaskQueue.emplace_back(std::move(_func));

			}/* ~ END CRITICAL SECTION ~ */

			is_Ready.notify_one();                       // Why did I not have this before?
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


//DEBUGPrint(CON_DarkRed, "Created Queue: ");

//======================================================================================
//========================= DEBUG INFOMATION ======================================================
// #include<Windows.h>
// 
// extern HANDLE hConsole;
// #define SetColor(x) SetConsoleTextAttribute(hConsole, x);
// //
// //#define DEBUGPrint(col, x) {DEBUGMutex.lock();\
// //SetColor(col);\
// //std::thread::id CurrentThread = std::this_thread::get_id();\
// //std::cout <<  x << " : " << CurrentThread<< std::endl;\
// //SetColor(7);\
// DEBUGMutex.unlock();}
// #define DEBUGPrint(col,x)
// extern std::mutex DEBUGMutex;
// 
//======================================================================================
//======================================================================================
//	_function->ThreadID = std::this_thread::get_id();
//				std::thread::id ThreadID{ std::this_thread::get_id() };
//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
// template<typename _FUNC, typename...ARGS >
// auto Launch_Now(_FUNC&& _func, ARGS&&... args)
// {
// 	return _func(args...);					DEBUGPrint(CON_DarkPurple, " Call from a Different Thread");
// 
// }

//std::printf(x );\
//DEBUGPrint(CON_DarkYellow, "Async Called: " << &_func);
//	Print(typeid(_func).
//int PushColor = 2;
//int PopColor = 3;
//std::mutex DEBUGMutex;

//DEBUGPrint(CON_Green, "Function Called from " << _func->ThreadID << "- Pop CurrentThread: \n");
//DEBUGPrint(PopColor,"%d \n", _func->ThreadID);
//DEBUGPrint(PopColor, "- Pop CurrentThread: \n");

//DEBUGPrint(CON_Blue, "Func Address: 0x" << &_func);
//DEBUGPrint(CON_Red, "Try Push CurrentThread: ");

//DEBUGPrint(CON_DarkBlue, "Func Address: 0x" << &_func);
//DEBUGPrint(CON_DarkRed, "Push CurrentThread: ");

//DEBUGPrint(PopColor, "Function Called from "); DEBUGPrint(PopColor,"%d \n", _func->ThreadID); DEBUGPrint(PopColor, "- Pop CurrentThread: \n");
//DEBUGPrint(CON_DarkGreen, "Function Called from " << _func->ThreadID << "- Pop CurrentThread:");

				//Print(Main_ThreadID << std::thread::this_thread.get_id());
				//DEBUGPrint(CON_Yellow, "Running Function: " << Function->ThreadID);

