#pragma once


/*




int LOOP_COUNT{ 1000000 };

#define NUMBER_OF_THREADS 200
#define _TEST_THREADPOOL_SPEED    1

bool THREAD_POOL_TEST()
{
#if 0
    /// TEST_UNIT(TEST_PROFILE_WINDOW());
    DEBUG_CODE(_Trace("Testing Trace Macro", 100000));
    DEBUGPrint(CON_Red, "Testing Print");
 
    while (true)
    {
        TestAsyncSort SortTest(1024); // 4096); // 262144); //
        Print("Running 40");
        ThreadPool::get().Async(TestRecursion, 40);
        Print("Running 30");
        ThreadPool::get().Async(TestRecursion, 30);
        Print("Running 20");
        ThreadPool::get().Async(TestRecursion, 20);
        Print("Running 10");
        ThreadPool::get().Async(TestRecursion, 10);
        {
            Timing::Profiling::Profile_Timer Bench("My Linear Merge Sort");
            // 	SortTest.LinearMergeSort();
        }
        {// Currently freezes if one attempts to recurse to many levels to the point it overwhelms the threadpool as it can never return until it is capable of recursing deeper.
            Timing::Profiling::Profile_Timer Bench("My MT Merge Sort");// Dont use the current Threaded Version its broke.
        //	SortTest.AsyncMergeSort();
        }
        {
            Timing::Profiling::Profile_Timer Bench("std::async Merge Sort");
            //SortTest.StdMergeSort();
        }


#if _TEST_THREADPOOL_SPEED
        Function_Counter = 0;
        //	LOOP_COUNT += 1000;// 22100 is when Threadpool and Linear start to become one.
        Print("/n/n/n/n Loop Counter:" << LOOP_COUNT << " iterations in the Worker Functions/n");
        {
            Timing::Profiling::Profile_Timer Bench("My Threadpool");
            std::vector<std::vector<uint32_t>> Test;

            auto A = ThreadPool::get().Async(TestFunctionE, std::move(LOOP_COUNT));
            auto B = ThreadPool::get().Async(TestFunctionE, std::move(LOOP_COUNT));
            auto C = ThreadPool::get().Async(TestFunctionB, 1431);
            auto D = ThreadPool::get().Async(TestFunctionD, 123.321f, 10);
            auto E = ThreadPool::get().Async(TestFunctionA);
            auto F = ThreadPool::get().Async(TestFunctionC, 3.14159f, 123);
            auto G = ThreadPool::get().Async(TestFunctionF, std::move(LOOP_COUNT));
            auto H = ThreadPool::get().Async(TestFunctionG, std::move(LOOP_COUNT));
            auto I = ThreadPool::get().Async(TestFunctionH, std::move(LOOP_COUNT));
            auto J = ThreadPool::get().Async(TestFunctionI, std::move(LOOP_COUNT));
            auto K = ThreadPool::get().Async(TestFunctionJ, std::move(LOOP_COUNT));

            Print("Thread Pool Cluster");
            std::vector<std::future<float>> Fut;
            //std::vector<Future<float>> Fut;
            for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
            {
                auto F = ThreadPool::get().Async(TestFunctionC, 123.321f, std::move(rand() % NUMBER_OF_THREADS));
                //Fut.push_back(std::forward<std::future<float>>(F));
                ///Fut.push_back(std::forward<Future<float>>(F));
            }
            uint64_t result{ 0 };
            uint64_t counter = Fut.size();
            while (counter)
            {
                for (auto& F : Fut)
                {
                    if (!is_ready(F))
                    {
                        continue;
                    }
                    result += (uint64_t)F.get();
                    --counter;
                }
            }
            Print("End Thread Pool Cluster: " << result);

            //while (Function_Counter < 10) {}// SpinLock until every single function called returns as measured via the atomic int Function_Counter.

            Print("Threadpool: " << result);
        }

        Function_Counter = 0;
        {

            Timing::Profiling::Profile_Timer ThreadBM("std::Async");
            auto  TPTest5T = std::async(std::launch::async | std::launch::deferred, TestFunctionE, LOOP_COUNT);
            auto  TPTest1T = std::async(std::launch::async | std::launch::deferred, TestFunctionB, 1431);
            auto  TPTest4T = std::async(std::launch::async | std::launch::deferred, TestFunctionD, 123.321f, 10);
            auto  TPTest3T = std::async(std::launch::async | std::launch::deferred, TestFunctionA);
            auto  TPTest2T = std::async(std::launch::async | std::launch::deferred, TestFunctionC, 3.14159f, 123);
            auto  TPTest6T = std::async(std::launch::async | std::launch::deferred, TestFunctionF, LOOP_COUNT);
            auto  TPTest7T = std::async(std::launch::async | std::launch::deferred, TestFunctionG, LOOP_COUNT);
            auto  TPTest8T = std::async(std::launch::async | std::launch::deferred, TestFunctionH, LOOP_COUNT);
            auto  TPTest9T = std::async(std::launch::async | std::launch::deferred, TestFunctionI, LOOP_COUNT);
            auto TPTest10T = std::async(std::launch::async | std::launch::deferred, TestFunctionJ, LOOP_COUNT);

            Print("Async Cluster");
            std::vector<std::future<float>> Fut;
            for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
            {
                auto TPTest4loop = std::async(std::launch::async, TestFunctionC, 123.321f, rand() % NUMBER_OF_THREADS);
                Fut.push_back(std::move(TPTest4loop));
            }
            uint64_t result{ 0 };
            uint64_t counter = Fut.size();
            while (counter)
            {
                for (auto& F : Fut)
                {
                    if (!is_ready(F))
                    {// if not ready yet, check the next  
                        continue;
                    }
                    result += (uint64_t)F.get(); // it is ready 
                    --counter;
                }
            }

            Print("End Async Cluster");
            Print("Async :" << result);
            while (Function_Counter < 10) {}// SpinLock until every single function called returns as measured via the atomic int Function_Counter. 

            std::vector<std::vector<uint64_t>> Test;

            Test.push_back(TPTest5T.get());
            Test.push_back(TPTest6T.get());
            Test.push_back(TPTest7T.get());
            Test.push_back(TPTest8T.get());
            Test.push_back(TPTest9T.get());
            Test.push_back(TPTest10T.get());
            ///Print("Async: " << TPTest4T.get() << " : " << TestCompile(Test));
        }


        Function_Counter = 0;
        {
            Timing::Profiling::Profile_Timer LBench("Linear Benchmark");
            auto Test5 = TestFunctionE(std::move(LOOP_COUNT));// .5ms
            auto Test4 = TestFunctionD(123.321f, 10);
            auto Test1 = TestFunctionB(1431);
            auto Test3 = TestFunctionA();
            auto Test2 = TestFunctionC(3.14159f, 123);
            auto Test6 = TestFunctionF(std::move(LOOP_COUNT));//.3
            auto Test7 = TestFunctionG(std::move(LOOP_COUNT));//3.21
            auto Test8 = TestFunctionH(std::move(LOOP_COUNT));// .32
            auto Test9 = TestFunctionI(std::move(LOOP_COUNT));//2.8
            auto Test10 = TestFunctionJ(std::move(LOOP_COUNT));//2.6


            while (Function_Counter < 10) {}// SpinLock until every single function called returns as measured via the atomic int Function_Counter. 


            uint64_t result{ 0 };
            for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
            {
                result += (uint64_t)TestFunctionC(123.321f, rand() % NUMBER_OF_THREADS);
            }
            Print("Linear :" << result);

            std::vector<std::vector<uint64_t>> Test;
            Test.push_back(Test5);
            Test.push_back(Test6);
            Test.push_back(Test7);
            Test.push_back(Test8);
            Test.push_back(Test9);
            Test.push_back(Test10);
            ///Print("Linear: " << Test4 << " : " << TestCompile(Test));
        }
        Sleep(1500);
#endif //_TEST_THREADPOOL_SPEED
    }
#endif // IF 0 to turn all this off for now.
    return true;
}
bool TEST_PROFILE_WINDOW()
{
    {
        Profiling::DisplayWindow Test({ 0,0 }, { 10, 150 }, { 30, 450 });
        {	// TEST SET PIXEL

            Test.setPixel(1, 1, Pixel(255, 255, 255, 255));
        }
        {	// TEST UPDATE

            Test.Update(1);
            if (Test.getPixel(1, 2) _NOT_EQUAL_TO_ 0xffffffff)return false;
        }
        {	// REPEATEDLY TEST TO ENSURE UPDATE IS MOVING THE VALUES DOWN

            Test.Update(1);
            if (Test.getPixel(1, 3) _NOT_EQUAL_TO_ 0xffffffff)return false;
            Test.Update(1);
            if (Test.getPixel(1, 4) _NOT_EQUAL_TO_ 0xffffffff)return false;
            Test.Update(1);
            if (Test.getPixel(1, 5) _NOT_EQUAL_TO_ 0xffffffff)return false;
            Test.Update(1);
            if (Test.getPixel(1, 6) _NOT_EQUAL_TO_ 0xffffffff)return false;
            Test.Update(1);
            if (Test.getPixel(1, 7) _NOT_EQUAL_TO_ 0xffffffff)return false;
            Test.Update(1);
            if (Test.getPixel(1, 8) _NOT_EQUAL_TO_ 0xffffffff)return false;
        }
        {	// TEST TO MAKE SURE IT IS NOT LEAVING A VALUE BEHIMD AND IS NOT JUST SETTING NEW VALUE

            if (Test.getPixel(1, 7) _EQUALS_ 0xffffffff)return false;
        }
    }

    return true;
}


*/























///await-expression
///e.await_resume().
//Print("Suspending Thread: " << ThreadID);
//_func(args...);
//jmp_buf Context;
//setjmp(Context);
//auto _function = new Suspend<_FUNC, ARGS... >(&Context, std::move(_func), std::forward<ARGS>(args)...);  // Create our task which binds the functions parameters
// 
//auto result = _function->get_future();                                                           // Get the future of our async task for later use
// 
//int Attempts = 5;
//for (unsigned int n{ 0 }; n != ThreadCount * Attempts; ++n)                                      // K is Tunable for better work distribution
//{// Cycle over all Queues K times and attempt to push our function to one of them
//
//	if (ThreadQueue[static_cast<size_t>((i + n) % ThreadCount)].try_push_front(static_cast<Executor*>(_function)))
//	{// If succeeded return our functions Future
//		//_func(args...);
//		//result.get();
//		return result;
//	}
//}
//
//// In the rare instance that all attempts at adding work fail just push it to the Owned Queue for this thread
//ThreadQueue[i % ThreadCount].push_front(static_cast<Executor*>(_function));
////_func(args...);
//result.get();
//return result;






/*

#include<future>
// STL Async Declaration 
template<class _Fty, class... _ArgTypes>
[[nodiscard]] inline std::future<std::_Invoke_result_t<std::decay_t<_Fty>, std::decay_t<_ArgTypes>...>>
async(_Fty&& _Fnarg, _ArgTypes&&... _Args){}



template<class _Ty>
struct __declspec(novtable) _Deleter_base
{	// abstract base class for managing deletion of state objects
	virtual void _Delete(_Associated_state<_Ty> *) = 0;
	virtual ~_Deleter_base() noexcept
	{	// destroy the object
	}
};


template<class _Ty>
class _Associated_state
{
	using _State_type = _Ty;
	using _Mydel = _Deleter_base<_Ty>;
	_Mydel *_Deleter;

	virtual ~_Associated_state() noexcept
	{	// destroy
		if (_Has_stored_result && !_Ready)
		{	// registered for release at thread exit
			_Cond._Unregister(_Mtx);
		}
	}

	volatile long _Refs;

	_Ty _Result;
	std::mutex _Mtx;
	std::condition_variable _Cond;
	bool _Retrieved;
	int _Ready;
	bool _Ready_at_thread_exit;
	bool _Has_stored_result;
	bool _Running;
};

template<class _Ty>
class _State_manager
{
	_Associated_state<_Ty> *_Assoc_state;
	bool _Get_only_once;
};

template<typename T>
struct _Promise
{

	_State_manager<T> _State;
	bool _Future_retrieved;
};

template<typename T>
struct Promise
{
	_Promise<T> MyPromise;
};








template<class _Ty,
	class _Alloc> inline
	_Associated_state<_Ty> *_Make_associated_state(const _Alloc& _Al) {}


*/


