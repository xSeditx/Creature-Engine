#pragma once
#ifndef THREADPOOL_H
#define THREADPOOL_H

/*=======================================================================
                     # ThreadPool Module  #

     Handles the Acceptance and dispatching of Asynchronous Function 
  calls via Threaded Queues which store pointers to functions and
  systematically keeps every core of the CPU busy at all times. As soon
  as one function returns another is popped from a stack and ran and the 
  user synchronizes these efforts via a future object which becomes valid
  with the return value after the desired function has been properly
  run.

 =======================================================================*/


 /*************************************************************************/
 /*                       This file is part of:                           */
 /*                       Creature Game Engine                            */
 /*              https://github.com/xSeditx/Creature-Engine               */
 /*************************************************************************/
 /* Copyright (c) 2019 Sedit                                              */
 /*                                                                       */
 /* Permission is hereby granted, free of charge, to any person obtaining */
 /* a copy of this software and associated documentation files (the       */
 /* "Software"), to deal in the Software without restriction, including   */
 /* without limitation the rights to use, copy, modify, merge, publish,   */
 /* distribute, sublicense, and/or sell copies of the Software, and to    */
 /* permit persons to whom the Software is furnished to do so, subject to */
 /* the following conditions:                                             */
 /*                                                                       */
 /* The above copyright notice and this permission notice shall be        */
 /* included in all copies or substantial portions of the Software.       */
 /*                                                                       */
 /* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
 /* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
 /* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
 /* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
 /* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
 /* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
 /* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
 /*=======================================================================*/


#include <thread>
#include <future>
#include <deque>
#include <tuple>
#include <iostream>
#include <type_traits> 

#include"../Common.h" // Comment out this header to stand alone

#define DEBUGPrint(x)   DEBUGMutex.lock();\
std::cout << x << std::endl;\
DEBUGMutex.unlock();


//  Defines Which allow Threadpool.h and .cpp to stand along from Common.h If desired
#ifndef CREATURE_API
#    define CREATURE_API // Will determine export type later on
#endif
/* Denotes that Object Can not be Copied or Assigned */
#ifndef NO_COPY_OR_ASSIGNMENT
#    define NO_COPY_OR_ASSIGNMENT(Class_X)	void operator=(const Class_X&) = delete;\
Class_X(const Class_X&) = delete
#endif
#ifndef NO_VTABLE
#    define NO_VTABLE __declspec(novtable) 
#endif
//======================================================================================




#pragma warning( push )
#pragma warning( disable : 4244 ) // Type conversions
#pragma warning( disable : 4018 ) // Optimization off warning of mine



//TODO: Should wrap this in a Namespace somewhere
/*      Non-blocking test of std::future to see if value is avalible yet
    NOTE: Performance of this is not the best use sparingly our outside of hot loops */
template<typename _R>
bool is_ready(std::future<_R> const& _fut)
{
    return _fut.valid() ? _fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready : false;
}
/* 
 *TODO: Implement a Then, When_all and Deferred Function handling 
 *Note: Must create my own Promise/ Future inorder to make that happen I think
 *       Possibly derive from std::future and build on that would be possible
 *TODO: Call Chains where user can Generate Call chains and trees for execution without interference
 * Possible co-Routine functionality to handle recursive and nested Task Calls                                                                                                                         
 */


 /* Will be used later so Threadpool can accept functions that return void
	template<typename _Ty> struct FixReturn       { using type = _Ty ;}
	template<typename _Ty> struct FixReturn<_Ty&> { using type = _Ty*;}
	template<>             struct FixReturn<>     { using type = int ;}
 */




namespace Core
{
    namespace Threading
    {
		extern std::mutex DEBUGMutex;

        class CREATURE_API ThreadPool
        {
            NO_COPY_OR_ASSIGNMENT(ThreadPool);
            
            /*      Executor: Allows us to make a polymorphic object and derive from it with the various 
                Function types the user may invoke. We store the Base class pointer in Queues to Erase the type 
                While Polymorphically calling each Functions specific invoke method */
            struct NO_VTABLE Executor
            {
                virtual ~Executor() noexcept {}

				/* Function responsible to properly invoking our derived class */
				virtual void Invoke() noexcept = 0;

				/* Mainly for Debug information Gives the Current status of a Function passed into our Queue */
				enum asyncStatus
				{
					Empty, Valid, Waiting, Busy, Submitted, Ready, Aquired
				} Status{ Empty };
            }; // End Wrapper_Base Class
            
            /*      ASYNC TASK: Object Binds Function Pointers as well as Arguments into a single unit 
                and stores its return value inside of an std::promise<_Rty> With _Rty being functions return type */
            template<typename _Func, typename ...ARGS>
            struct asyncTask final
                : public Executor
            {
			public:													   
				using type = std::invoke_result_t<_Func, ARGS...>;         // Return type of our function

                virtual ~asyncTask() noexcept = default;                   // Virtual destructor to ensure proper Deallocation of object
															   
				/* Accepts Functions and their arguments */
                asyncTask(_Func&& _function, ARGS&&... _args) noexcept
                    :
                    Function(std::forward<_Func>(_function)),
                    Arguments(std::forward<ARGS>(_args)...)
                {// Signals to user the object is now completed and valid
                    Status = Valid;
                }


				/*     Calls the Objects Stored function along with its parameters using std::apply
					Sets the value of the Promise and signals to the User that the value is waiting */
				virtual void Invoke() noexcept override
				{
					Status = Busy;
					
					auto result = std::apply(Function, Arguments);
					ReturnValue.set_value(result);
					Status = Waiting;
				}


				/*      To ensure familiarity and usability get_future works to retrieve the
					std::future object associated with the return values std::promise */
				std::future<type> get_future() noexcept
				{
					Status = Submitted;
					return ReturnValue.get_future();
				}


			private:
				using Fptr = type(*)(ARGS...);                             // Function pointer type for our function
				const Fptr Function;                                       // Pointer to our Function
				const std::tuple<ARGS...> Arguments;                       // Tuple which Binds the Parameters to the Function call
				
				std::promise<type> ReturnValue;                            // Return Value of our function stored as a Promise

                asyncTask(const asyncTask&) = delete;                      // Prevent copying
				asyncTask(asyncTask&& _other) = delete;                    // Prevent move   
                asyncTask& operator=(const asyncTask& _other) = delete;    // Prevent Assignment
				asyncTask& operator=(asyncTask&& _other) = delete;         // Prevent move assignment
			};// End asyncTask Class


        public:

            /*
             JOB QUEUE: Stores a Deque of Base Pointers to asyncTask Objects. 
             Pointers are pushed and popped off the stack and properly deallocated after no longer needed */
            struct JobQueue
            {
            public:
                JobQueue()
					:
					idTag(queue_ID++)
				{}
                
                std::deque<Executor*> TaskQueue;
                std::condition_variable is_Ready;
				std::mutex QueueMutex;
				bool is_Done{ false };
                
                /* Triggers the Threadpool to shut down when the application ends or user ask it to */
                void Done();
                
                /* Try to Pop a function off the Queue if it fails return false */
                bool try_Pop(Executor*& func);
                
                /* Pop function from Queue if fails wait for it */
                bool pop(Executor*& func);
                
                /* Attempts to add a function to the Queue if unable to lock return false */
                bool try_push(Executor* func);
                
                /* Adds a Function to our Queue */
                void push(Executor* func);

				uint32_t g_ID() { return idTag; }
			private:
				uint32_t idTag;
				static uint32_t queue_ID;
            };
            
            
            const uint32_t           ThreadCount    {  std::thread::hardware_concurrency() * 3};
            std::vector<JobQueue>    ThreadQueue    { ThreadCount };
            std::atomic<uint32_t>    Index          { 0 };
            std::vector<std::thread> Worker_Threads;
            
            /*
             * Create a set number of Threads and Add Job Queues to the Threadpool 
             *     NOTE: May Possibly add a Number here to create a specific number of threads */
            ThreadPool();

            /* Properly shuts down our Threadpool and joins any Threads running */
            ~ThreadPool();
            
            /* Initializes Thread and starts the Queue running */
            void Run(unsigned int _i);
            
            
            /* Returns a singleton instance of our Threadpool */
            static ThreadPool &get()
            {
                static ThreadPool instance;
                return instance;
            }
        





			enum class launch_policy
			{
				immediate,
				async,
				deferred
			};
            /* Executor for our Threadpool Allocating our Asyncronous objects, returning their Futures an handles work sharing throughout all the available Queues*/
            template<typename _FUNC, typename...ARGS >
            auto Async(launch_policy _policy, _FUNC&& _func, ARGS&&... args)->std::future<typename std::invoke_result_t<std::decay_t<_FUNC>, std::decay_t <ARGS>...>>
            {// Accept arbitrary Function signature, Bind its arguments and add to a Work pool for Asynchronous execution

				switch (_policy)
				{
				case launch_policy::async:
				{
						auto _function = new asyncTask<_FUNC, ARGS... >(std::move(_func), std::forward<ARGS>(args)...);  // Create our task which binds the functions parameters
						auto result = _function->get_future();                                                           // Get the future of our async task for later use
						auto i = Index++;                                                                                // Ensure fair work distribution

						int Attempts = 5;
						for (unsigned int n{ 0 }; n != ThreadCount * Attempts; ++n)                                      // K is Tunable for better work distribution
						{// Cycle over all Queues K times and attempt to push our function to one of them

							if (ThreadQueue[static_cast<size_t>((i + n) % ThreadCount)].try_push(static_cast<Executor*>(_function)))
							{// If succeeded return our functions Future
								return result;
							}
						}

						// In the rare instance that all attempts at adding work fail just push it to the Owned Queue for this thread
						ThreadQueue[i % ThreadCount].push(static_cast<Executor*>(_function));
						return result;
					}

				case launch_policy::immediate:
				return Execute_Immediately(std::forward <_FUNC>( _func), std::forward<ARGS>(args)...);
					break;

				}
            }
			/// NOTE: invoke_result_t = template<class... _Types>{	using type = decltype(std::invoke(std::declval<_Types>()...)); }

			template<typename _Func, typename...ARGS >
			inline std::future<std::invoke_result_t<std::decay_t<_Func>, std::decay_t<ARGS>...>>
			 Async( _Func&& _func, ARGS&&... args)
			{// Accept arbitrary Function signature, Bind its arguments and add to a Work pool for Asynchronous execution
				static_assert(std::is_invocable<_Func, ARGS...>::value, "Template param <_Func> is not callable with arguments <ARGS...> ");
				return (Async(launch_policy::async, 
					std::forward<_Func>(_func), 
					std::forward<ARGS>(args)...
				));
			}

			/// STL Async. 
//template<class _Fty, class... _ArgTypes>
//	inline std::future<std::invoke_result_t<std::decay_t<_Fty>, std::decay_t<_ArgTypes>...>>
//	std::async(_Fty&& _Fnarg, _ArgTypes&&... _Args)
//{	// return a future object whose associated asynchronous state
//	// manages a callable object launched with default policy
//	return (_STD async(launch::async | launch::deferred,
//		_STD forward<_Fty>(_Fnarg),
//		_STD forward<_ArgTypes>(_Args)...
//	));
//}




//->std::future<typename std::invoke_result_t<_Func,ARGS...>>
			template<typename _Func, typename...ARGS >
			auto Execute_Immediately( _Func&& _func, ARGS&&... args)->std::future<typename std::invoke_result_t<_Func, ARGS...>>
			{
				using type = std::invoke_result_t<_Func, ARGS...>;
				std::promise<type> Promise;
				std::future<type> result;
				result = Promise.get_future();
				std::thread Thr = std::thread(
					[&]()
				{
					Promise.set_value(_func(args...));
				}
				);
				return result;
			}




        }; // End ThreadPool Class

    }// End NS Threading
}// End NS Core 


#pragma warning( pop )
#endif// THREADPOOL_H





/*
==========================================================================================================================================================================
                                                  Future Implementations:
==========================================================================================================================================================================

Type of executor	Where, when and how
System	             Any thread in the process.
Thread pool	         Any thread in the pool, and nowhere else.
Strand	             Not concurrent with any other function object sharing the strand, and in FIFO order.
Future / Promise	 Any thread. Capture any exceptions thrown by the function object and store them in the promise.

*/

/*
==========================================================================================================================================================================
                                                           NOTES:
==========================================================================================================================================================================

Parallelizing the Naughty Dog Engine
https://www.gdcvault.com/play/1022186/Parallelizing-the-Naughty-Dog-Engine

CppCon 2015: Fedor Pikus PART 2 “Live Lock-Free or Deadlock (Practical Lock-free Programming) ”
Queue
https://www.youtube.com/watch?v=1obZeHnAwz4&t=3055s

Code overview - Thread Pool & Job System
https://www.youtube.com/watch?v=Df-6ws_EZno

Better Code Concurrency
https://www.youtube.com/watch?v=zULU6Hhp42w&list=PLl8Qc2oUMko_FMAaK7WY4ly0ikLFrYCE3&index=4

GotW #95: Thread Safety and Synchronization
https://herbsutter.com/2014/01/06/gotw-95-thread-safety-and-synchronization/

Thread Pool Implementation on Github:
https://github.com/mtrebi/thread-pool/blob/master/README.md#queue

Thread pool implementation using c++11 threads
https://github.com/mtrebi/thread-pool

Faster STD::FUNCTION Implementation
https://github.com/skarupke/std_function/blob/master/function.h

Lock Free Ring Buffer
https://github.com/tempesta-tech/blog/blob/master/lockfree_rb_q.cc



Learning C++
https://riptutorial.com/Download/cplusplus.pdf

Performance Analysis of Multithreaded Sorting Algorithms
http://www.diva-portal.org/smash/get/diva2:839729/FULLTEXT02

Double Check Locking is Fixed
https://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/

Intel Game Engine Design:
https://software.intel.com/en-us/articles/designing-the-framework-of-a-parallel-game-engine

Programming with Threads
Parallel Sorting
https://cseweb.ucsd.edu/classes/fa13/cse160-a/Lectures/Lec02.pdf

Lock-Free Programming
https://www.cs.cmu.edu/~410-s05/lectures/L31_LockFree.pdf

Introduction to Multithreaded Algorithms
http://ccom.uprrp.edu/~ikoutis/classes/algorithms_12/Lectures/MultithreadedAlgorithmsApril23-2012.pdf

A Fast Lock-Free Queue for C++
http://moodycamel.com/blog/2013/a-fast-lock-free-queue-for-c++


A Thread Pool with C++11
http://progsch.net/wordpress/?p=81

C++11 threads, affinity and hyperthreading
https://eli.thegreenplace.net/2016/c11-threads-affinity-and-hyperthreading/

Thread pool worker implementation
https://codereview.stackexchange.com/questions/60363/thread-pool-worker-implementation

C++11 Multithreading – Part 8: std::future , std::promise and Returning values from Thread
https://thispointer.com/c11-multithreading-part-8-stdfuture-stdpromise-and-returning-values-from-thread/

Threadpool with documentation:
https://www.reddit.com/r/cpp/comments/9lvji0/c_threadpool_with_documentation/


Original White paper on Work stealing Queues:
http://supertech.csail.mit.edu/papers/steal.pdf

Type Traits Reference
https://code.woboq.org/llvm/libcxx/include/type_traits.html

MSVC Threadpool implementation for Concurrency:
https://docs.microsoft.com/en-us/cpp/parallel/concrt/task-scheduler-concurrency-runtime?view=vs-2019


Simple Threadpool Implementation:
https://riptutorial.com/cplusplus/example/15806/create-a-simple-thread-pool


Use this switch with Compiler Explorer inorder to allow it to compile: -std=c++17 -O3

Aquiring results of a templated function:
template<typename Function, typename ...Args>
result_type = std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>;


*/


