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

#include <setjmp.h>
#include <csetjmp>
#include <intrin.h>
#include"../Common.h" // Comment out this header to stand alone

//#include "Future.h"

/*
THREADING PATTERNS:
 Pipeline: Lock stepped
 Dedicated Thread
 Schedular: What is neing done here
*/

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

extern std::vector<int*> DebugStack;
extern int DebugCounter;
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
 * It may be possible to implement a recursive Mutex and/or Condition Variable so that execution of
 * a function can be suspended and the Queue unlocked so that subsequent calls to our Threadpool 
 * can be made. This should allow recursive functions or Functions that call other functions
 * because currently if our functions make further threadpool calls it ties up a slot on our 
 * Threadpool and if that number reaches a level greater than the number of threads we have then
 * our program will Lock as it waits for functions to return which can never return until those in the
 * Queue waiting return.
 */


 /* Will be used later so Threadpool can accept functions that return void
    template<typename _Ty> struct FixReturn       { using type = _Ty ;}
    template<typename _Ty> struct FixReturn<_Ty&> { using type = _Ty*;}
    template<>             struct FixReturn<>     { using type = int ;}
 */



#include<functional>


namespace Core
{
    namespace Threading
    {
        class CREATURE_API ThreadPool
        {  
            NO_COPY_OR_ASSIGNMENT(ThreadPool);

            std::thread::id Main_ThreadID{ std::this_thread::get_id() }; // Thread ID of the Main Thread
            static std::atomic<int> RunningThreads;
            /*      WRAPPER_BASE: Allows us to make a polymorphic object and derive from it with the various
                Function types the user may invoke. We store the Base class pointer in Queues to Erase the type 
                While Polymorphically calling each Functions specific invoke method */
            struct NO_VTABLE Executor
            {
                virtual ~Executor() noexcept = default;

                /* Function responsible to properly invoking our derived class */
                virtual void Invoke() noexcept = 0;

				/* Mainly for Debug information Gives the Current status of a Function passed into our Queue */
                enum asyncStatus
                {
                    Empty, Valid, Waiting, Busy, Submitted, Ready, Aquired
                } Status{ Empty };

                std::thread::id LaunchThread{ std::this_thread::get_id() };
            }; // End Wrapper_Base Class
            

            template<typename _Ret, typename _Class, typename ...ARGS>
            struct member_func
            {
                using Return_t = _Ret;
                _Ret(_Class::*mem_ptr)(ARGS...);
                member_func(_Ret(_Class::*mptr)(ARGS...)):
                    mem_ptr{mptr}
                {}

                template<typename...ARGS>
                _Ret operator()(_Class&& _obj, ARGS&&... args) const
                {
                    return (_obj.*mem_ptr)(std::forward<ARGS>(args)...);
                }
            };
            template<typename _Ret, typename _Class, typename ...ARGS>
            member_func < _Ret, _Class, ARGS...>mem_fn(_Ret(_Class::*mptr)(ARGS...))
            {
                return { mptr };
            }

            /*      ASYNC TASK: Object Binds Function Pointers as well as Arguments into a single unit 
                and stores its return value inside of an std::promise<_Rty> With _Rty being functions return type */
            
            template<typename _Func, typename ...ARGS>
            struct asyncTask final
                : public Executor
            {	
                NO_COPY_OR_ASSIGNMENT(asyncTask);

            public:													   
                using type = std::invoke_result_t<_Func, ARGS...>; // Return type of our function

                virtual ~asyncTask() noexcept = default; // Virtual destructor to ensure proper Deallocation of object
                                                               
                /* Accepts Functions and their arguments */
                asyncTask(_Func&& _function, ARGS&&... _args) noexcept
                    :
                    Function(std::forward<_Func>(_function)),
                    Arguments(std::forward<ARGS>(_args)...)
                {// Signals to user the object is now completed and valid
                    Status = Valid;
                }

               // asyncTask(mem_fn& _function) 
                //{}
 
                  //  template< class Res, class T >
               // std::mem_fun_t<Res, T> mem_fun(Res(T::*f)());
 
                /*     Calls the Objects Stored function along with its parameters using std::apply
                    Sets the value of the Promise and signals to the User that the value is waiting */
                virtual void Invoke() noexcept override
                {
                    Status = Busy;
                    auto result = std::apply(Function, Arguments);
                    set_return(result);
                    Status = Waiting;
                }

                void set_return(type& _value)
                {
                    ReturnValue.set_value(_value);
                }

                /*      To ensure familiarity and usability get_future works to retrieve the
                    std::future object associated with the return values std::promise */
                auto get_future() noexcept
                {
                    Status = Submitted;
                    return ReturnValue.get_future();
                }

            private:
                using Fptr = type(*)(ARGS...);                             // Function pointer type for our function
                const Fptr Function;                                       // Pointer to our Function
                const std::tuple<ARGS...> Arguments;                       // Tuple which Binds the Parameters to the Function call				
                std::promise<type> ReturnValue;                            // Return Value of our function stored as a Promise
            };// End asyncTask Class




        public:

            /*
             JOB QUEUE: Stores a Deque of Base Pointers to asyncTask Objects. 
             Pointers are pushed and popped off the stack and properly deallocated after no longer needed */
            struct JobQueue
            {
            public:
                std::thread::id QueueID{ std::this_thread::get_id() };

                JobQueue() = default;

                std::mutex QueueMutex;
                std::deque<Executor*> TaskQueue;
                std::condition_variable is_Ready;
                bool is_Done{ false };

                /* Triggers the Threadpool to shut down when the application ends or user ask it to */
                void Done();
                
                /* Try to Pop a function off the Queue if it fails return false */
                bool try_Pop(Executor*& _func);
                
                /* Pop function from Queue if fails wait for it */
                bool pop(Executor*& _func);
                
                /* Attempts to add a function to the Queue if unable to lock return false */
                bool try_push(Executor* _func);
                
                /* Adds a Function to our Queue */
                void push(Executor* _func);



                /* Try to Pop a function off the BACK OF the Queue if it fails return false */
                bool try_Pop_back(Executor*& _func);            // Lower Priority Function

                /* Pop function from Queue if fails wait for it */
                bool pop_back(Executor*& _func);                // Lower Priority Function

                /* Attempts to add a function to the FRONT OF the Queue if unable to lock return false */
                bool try_push_front(Executor* _func);           // Higher Priority Function

                /* Adds a Function to the FRONT OF our Queue */
                void push_front(Executor* _func);               // Higher Priority Function
            };
            
            
            const uint32_t           ThreadCount{ std::thread::hardware_concurrency() }; 
            std::vector<JobQueue>    ThreadQueue    { ThreadCount };
            std::vector<std::thread> Worker_Threads; // Each Thread is responsible for an instance of Run which owns a specific Queue
            std::atomic<uint32_t>    Index          { 0 };
            
            /*
             * Create a set number of Threads and Add Job Queues to the Threadpool 
             *     NOTE: May Possibly add a Number here to create a specific number of threads */
            ThreadPool();

            /* Properly shuts down our Threadpool and joins any Threads running */
            ~ThreadPool();
            
            /* Initializes Thread and starts the Queue running */
            void Run(unsigned int _i);
            
            /*  Since C++11, initialization of function scope static variables is thread safe :
            the first tread calling get() will initialize instance, 
            blocking other threads until the initialization is completed. All subsequent calls will use the initialized value.
            Source: https://stackoverflow.com/questions/27181645/is-publishing-of-magic-statics-thread-safe 				*/

            /* Returns a singleton instance of our Threadpool */
            static ThreadPool& get()
            {
                static ThreadPool __instance;
                return __instance;
            }


            /* Executor for our Threadpool Allocating our Asyncronous objects, 
               returning their Futures an handles work sharing throughout all the available Queues */
            template<typename _FUNC, typename...ARGS >
            auto Async(_FUNC&& _func, ARGS&&... args)->std::future<typename asyncTask<_FUNC, ARGS... >::type>
            {// Accept arbitrary Function signature, Bind its arguments and add to a Work pool for Asynchronous execution

				auto ThreadID = std::this_thread::get_id();
				//Executor* _function;
				auto i = Index++;

                auto  _function = new asyncTask<_FUNC, ARGS... >(std::move(_func), std::forward<ARGS>(args)...);  // Create our task which binds the functions parameters
			    auto result = _function->get_future(); // Get the future of our async task for later use
				
                int Attempts = 5;// Ensure fair work distribution
                for (unsigned int n{ 0 }; n != ThreadCount * Attempts; ++n) // Attempts is Tunable for better work distribution
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
        }; // End ThreadPool Class
    }// End NS Threading
}// End NS Core 


#pragma warning( pop )
#endif// THREADPOOL_H

 
 
//Promise<type> ReturnValue;                               // Return Value of our function stored as a Promise

 
 
 /*
==========================================================================================================================================================================
                                                           NOTES:
==========================================================================================================================================================================
Co-Routine Specs
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0057r2.pdf

Executors proposal
https://github.com/chriskohlhoff/executors

Very similar work stealer to this one.
https://codereview.stackexchange.com/questions/169377/work-stealing-queue

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




Events can be used in place of Locks and CV:
Event.workAvalible;

workAvalible.signal();

Signaled: Threads pass through
Reset:    Threads Wait
*/























/* ==========================================================================
     REMOVED ON 5/17/2020
   ==========================================================================


   ==========================================================================
   Taken from Async Function
   ==========================================================================
/// if (ThreadID != Main_ThreadID)
/// {
///
/// 	auto _function = new Suspend<_FUNC, ARGS... >(GetCurrentThread(), std::move(_func), std::forward<ARGS>(args)...); //Suspend(GetCurrentThread());  // Create our task which binds the functions parameters
/// 	auto result = _function->get_future(); // Get the future of our async task for later use
///
/// 	int Attempts = 5;// Ensure fair work distribution
/// 	for (unsigned int n{ 0 }; n != ThreadCount * Attempts; ++n) // Attempts is Tunable for better work distribution
/// 	{// Cycle over all Queues K times and attempt to push our function to one of them
///
/// 		if (ThreadQueue[static_cast<size_t>((i + n) % ThreadCount)].try_push_front(static_cast<Executor*>(_function)))
/// 		{// If succeeded return our functions Future
/// 			return result;
/// 		}
/// 	}
/// 	// In the rare instance that all attempts at adding work fail just push it to the Owned Queue for this thread
/// 	ThreadQueue[i % ThreadCount].push_front(static_cast<Executor*>(_function));
/// 	return result;
///
/// 	return result;
/// }







template<typename _Func, typename ...ARGS>
class Suspend
    : public Executor
{

public:
    using type = std::invoke_result_t<_Func, ARGS...>; // Return type of our function

    Suspend(std::thread::native_handle_type _thread, _Func _function, ARGS...args)
        :
        ThreadID(_thread)
    {
        Context.ContextFlags = CONTEXT_ALL;
        if (setjmp(JumpBuffer) != 1)
        {
            GetThreadContext(ThreadID, &Context);
            Print("Storing Jump Context");
        }
        else
        {
            Print("Running Child Function");
            type Result = _function(args...);
            set_return(Result);
        }
    }
    virtual void Invoke() noexcept override
    {
        Print("Restoring Jump Context");
        SetThreadContext(ThreadID, &Context);
        longjmp(JumpBuffer, 1);
    }
    void set_return(type& _value)
    {
        Print("Setting Return Value");
        ReturnValue.set_value(_value);
    }

    /*      To ensure familiarity and usability get_future works to retrieve the
        std::future object associated with the return values std::promise
    auto get_future() noexcept
    {
        Print("Getting the Future");
        Status = Submitted;
        return ReturnValue.get_future();
    }

    std::jmp_buf JumpBuffer;
    std::promise<type> ReturnValue;                            // Return Value of our function stored as a Promise


    std::thread::native_handle_type ThreadID;
    CONTEXT Context;
};
*/