#include"Threadpool.h"
#include<stdio.h>
#pragma warning( push )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4018 ) // Optimization off warning of mine

std::vector<int*> DebugStack;
int DebugCounter{ 0 };

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

				Worker_Threads.emplace_back([&, N]
				{
					ThreadQueue[N].QueueID = std::this_thread::get_id();
					Run(N); 
				});
			}
		}
		/* ============================================================ */

		/* ============================================================
		 *                    Executors                                     
		 * ============================================================ */
		void ThreadPool::Run(unsigned int _i)
		{ // Initializes Thread and starts the Queue running 
			DEBUG_CODE(Print("Running: "<<_i));
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
				DEBUG_CODE(++RunningThreads);
				Function->Invoke();  // Invoke the returned function
				DEBUG_CODE(--RunningThreads);
				delete &(*Function); // Destroy the Object which our Async Class Allocated
			}
		}

		/* Pop Front of the Queue */
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
		/* Pop Back of the Queue 
		NOTE: Likely will not need this as much but will need the Push Front
		So included this for completeness. Functions with Low Priority could 
		very well be used here I guess. */
		bool ThreadPool::JobQueue::try_Pop_back(Executor*& _func)
		{// Try to pop a function off the Queue if it fails return false

			/* ~   CRITICAL SECTION   ~ */
			std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
			if (!Lock || TaskQueue.empty())
			{
				return false;
			}

			_func = std::move(TaskQueue.back()); // Gather from the back of the queue this time
			TaskQueue.pop_back(); // Remove it from the Queue
			return true;
		}
		bool ThreadPool::JobQueue::pop_back(Executor*& _func)
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

			_func = std::move(TaskQueue.back());
			TaskQueue.pop_back();
			return true;
		}

		/* ============================================================ */

		/* ============================================================
		 *                    Submitters                                
		 * ============================================================ */
		/* Push onto the Back of the Queue*/
		bool ThreadPool::JobQueue::try_push(Executor* _func)
		{// Attempts to add a function to the Queue if unable to lock return false 

			{/* ~   CRITICAL SECTION   ~ */
				std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
				if (_func->LaunchThread == QueueID) Print("Job Pushed to Thread it was Launched from. Possible Error if Recursive Call made ");
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

                DEBUG_CODE
                (
                    if (_func->LaunchThread == QueueID)
                    {
                        Print("Job Pushed to Thread it was Launched from. Possible Error if Recursive Call made ");
                    }
                );

				TaskQueue.emplace_back(std::move(_func));

			}/* ~ END CRITICAL SECTION ~ */

			is_Ready.notify_one();                       // Why did I not have this before?
		}
		/* Push onto the Front of the Queue 
		NOTE: Needed for higher priority calls such as forking the main
		thread and directly calling the child function. A Suspend Ops.*/
		bool ThreadPool::JobQueue::try_push_front(Executor* _func)
		{// Attempts to add a function to the Queue if unable to lock return false 

			{/* ~   CRITICAL SECTION   ~ */

				std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
				if (!Lock)
				{// If our mutex is already locked simply return 
					return false;
				}
				// Push... Emplaced_front instead????
				TaskQueue.push_front(std::move(_func));  // Else place on the back of our Queue

			}/* ~ END CRITICAL SECTION ~ */              // Unlock the Mutex 

			is_Ready.notify_one();                       // Tell the world about it 
			return true;                                 // Lets Async know you succeeded
		}
		void ThreadPool::JobQueue::push_front(Executor* _func)
		{// Adds a Function to our Queue

			{/* ~   CRITICAL SECTION   ~ */

				std::unique_lock<std::mutex> Lock{ QueueMutex };
				TaskQueue.emplace_front(std::move(_func));

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

        /* ============================================================
         *                State and Utility
         * ============================================================ */

        bool   ThreadPool::JobQueue::is_empty()
        {// Test if the Queue is empty 
            {/* ~   CRITICAL SECTION   ~ */
               // std::unique_lock<std::mutex> Lock{ QueueMutex };
                return TaskQueue.empty();
            }
        }
        size_t ThreadPool::JobQueue::size()
        {  // Returns the amount of Elements still waiting in the Queue
            {/* ~   CRITICAL SECTION   ~ */
                std::unique_lock<std::mutex> Lock{ QueueMutex };
                return TaskQueue.size();
            }
        }
     
        void ThreadPool::Flush()
        {  // Wait until all Queues are empty 
            while (std::any_of(ThreadQueue.begin(), ThreadQueue.end(), [](auto &x) {return !x.is_empty(); }))
            {  // While any queue is not empty
            }
        }

        
	}// End NS Threading
}// End NS Core
                          
#pragma warning( pop )













//#include<intrin.h>

            //template<typename _FUNC, typename...ARGS >

/*
==========================================================================================================================================================================
                                                           Trash:
==========================================================================================================================================================================
*/
 ////* Executor for our Threadpool Allocating our Asyncronous objects, returning their Futures an handles work sharing throughout all the available Queues*/
//////<Possibly to avoid Code bloat Make Async create the asynTask and Future and cast the task to Executionwer before sending it to the rest of the function to process>
//////<This way it will only duplicate the Async code and not the rest when not needed>
///template<typename _FUNC, typename...ARGS >
///auto Async(_FUNC&& _func, ARGS&&... args)->std::future<typename asyncTask<_FUNC, ARGS... >::type>
///{// Accept arbitrary Function signature, Bind its arguments and add to a Work pool for Asynchronous execution
///
///
///    auto _function = new asyncTask<_FUNC, ARGS... >(std::move(_func), std::forward<ARGS>(args)...);  // Create our task which binds the functions parameters
///    auto result = _function->get_future();                                                           // Get the future of our async task for later use
///    Submit(static_cast<Executor*>(_function));
///    return result;
///}
///
///void Submit(Executor *_task)
///{
///    auto i = Index++;
///    auto ThreadID = std::this_thread::get_id();
///
///    //===================================== IF TASK WAS LANCHED FROM ANOTHER RUNNING TASK ==================================================
///    //if (Main_ThreadID != ThreadID)                // If this is being call from one of the Threadpool Threads.
///    //{// If not our main thread run now
///    //	_function->Invoke();                                                                         // Invoke Immediately as our Thread is alreadylocked up
///    //	delete& (*_function);                                                                       // Destroy the Object which our Async Class Allocated
///    //	return result;
///    //}
///    //======================================================================================================================================
///                                                                                                     // Ensure fair work distribution
///    int Attempts = 5;
///    for (unsigned int n{ 0 }; n != ThreadCount * Attempts; ++n)                                      // Attempts is Tunable for better work distribution
///    {// Cycle over all Queues K times and attempt to push our function to one of them
///
///        if (ThreadQueue[static_cast<size_t>((i + n) % ThreadCount)].try_push(_task))
///        {// If succeeded return our functions Future
///            return;
///        }
///    }
///
///    // In the rare instance that all attempts at adding work fail just push it to the Owned Queue for this thread
///    ThreadQueue[i % ThreadCount].push(_task);
///    return;
///}
///#else
//auto Async(_FUNC&& _func, const ARGS&... args)->std::future<typename asyncTask<_FUNC, ARGS... >::type>
//{// Accept arbitrary Function signature, Bind its arguments and add to a Work pool for Asynchronous execution
//    return Async(_func, std::move(args...));
//}
            //template<typename _CON, typename _FUNC, typename...ARGS >
            //void For_Each(_CON _container, _FUNC&& _func, ARGS&&... arg)
            //{
            //    for (auto& C : _container)
            //    {
            //
            //    }
            //
            //}
          //
          // template<class _InIt,
          //     class _Fn> inline
          //     _Fn For_each(_InIt _First, _InIt _Last, _Fn  _Func)
          // {	// perform function for each element [_First, _Last)
          //    // _Adl_verify_range(_First, _Last);
          //     auto _UFirst =  _First;// _Get_unwrapped(_First);
          //     const auto _ULast = _Last;// _Get_unwrapped(_Last);
          //     for (; _UFirst != _ULast; ++_UFirst)
          //     {
          //         Async(_Func, (_UFirst));
          //     }
          //
          //     return (_Func);
          // }
                    //PoolWaiting |= ThreadQueue[i].is_empty();
                 //for (uint32_t i{ 0 }; i < ThreadCount; ++i)
                //{
                //    ThreadQueue[i].is_Ready.notify_all();
                //}
          //  } //                  // std::unique_lock<std::mutex> Lock(ThreadQueue[i].QueueMutex);
   

//std::condition_variable CV;
//for (uint32_t i{ 0 }; i < ThreadCount; ++i)
//{
//
//    ThreadQueue[i].QueueMutex.try_lock();
//}

//while (!PoolWaiting)
//{
//    for (uint32_t i{ 0 }; i < ThreadCount; ++i)
//    {
//        ThreadQueue[i].is_Ready;
//    }
//// }
 //
 //for (uint32_t i{ 0 }; i < ThreadCount; ++i)
 //{
 //    ThreadQueue[i].QueueMutex.unlock();
 //}
//// CV.notify_all();





//===================================== IF TASK WAS LANCHED FROM ANOTHER RUNNING TASK ==================================================
//if (Main_ThreadID != ThreadID)                // If this is being call from one of the Threadpool Threads.
//{// If not our main thread run now
//	_function->Invoke();                                                                         // Invoke Immediately as our Thread is alreadylocked up
//	delete& (*_function);                                                                       // Destroy the Object which our Async Class Allocated
//	return result;
//}
//======================================================================================================================================
                                                                                                 // Ensure fair work distribution


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

