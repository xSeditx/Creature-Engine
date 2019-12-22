#include<iostream>
#include<Windows.h>
#include<array>
#pragma optimize( "", off )

#define MY_WRAPPER
#define _PROFILE_MEMORY

#include"Profiling\SystemInfo.h"
#include"Profiling\MemoryPerf\MemTracker.h"
#include"Core\Threading\Threadpool.h"
#include"Profiling\Timing\Benchmark.h"

#include"Core\Common.h"

using namespace Core;
using namespace Threading;

int LOOP_COUNT{ 1000000 };
#include<string>

#define NUMBER_OF_THREADS 20000

#include"Core/Threading/TestFunctions.h"

//Nice
///http://eel.is/c++draft/meta.trans.other
auto func(char a, char b) {
	return a + b;
}


std::array<float,10> TestVec(std::vector<uint32_t>& _input)
{
	return std::array<float, 10>();
}
int TestNot(int _input)
{
	Print("Called Test Not with a " << _input);
	return 42;
}


//#include <intrin.h>

// Getting the Stack frame caller
// https://social.msdn.microsoft.com/Forums/vstudio/en-US/da4e85c5-407e-4a57-87e7-14bf994504d9/error-asm-keyword-not-supported-on-this-architecture?forum=vcgeneral
// void * memoryAllocator = _ReturnAddress();

//#pragma message (__FILE__ "[" STRING(__LINE__) "]: test")
//https://docs.microsoft.com/en-us/cpp/preprocessor/pragma-directives-and-the-pragma-keyword?view=vs-2019
#include<utility>
int main()
{
	_Trace("Testing Trace Macro", 100000);
    while (true)
	{
		TestAsyncSort SortTest(64);// 4096); // 262144);
		ThreadPool::get().Async(TestNot, 5);

		{
			Timing::Profiling::Profile_Timer Bench("My Linear Merge Sort");
		 	SortTest.LinearMergeSort();
		}
		{// Currently freezes if one attempts to recurse to many levels to the point it overwhelms the threadpool as it can never return until it is capable of recursing deeper.
			Timing::Profiling::Profile_Timer Bench("My Multithreaded Sort");// Dont use the current Threaded Version its broke.
			//SortTest.AsyncMergeSort();
//auto A = SortTest.MTSwapSort();//MTAdd(std::vector<int>& _input);
			//Print(A);
		}
        {
        	Timing::Profiling::Profile_Timer Bench("My Linear Bubble Sort");
        	//SortTest.LinearBubbleSort();
        }


#if _TEST_THREADPOOL_SPEED
		Function_Counter = 0;
	//	LOOP_COUNT += 1000;// 22100 is when Threadpool and Linear start to become one.
		Print("\n\n\n\n Loop Counter:" << LOOP_COUNT << " iterations in the Worker Functions\n");
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
			for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
			{
				auto F = ThreadPool::get().Async(TestFunctionC, 123.321f, std::move(rand() % NUMBER_OF_THREADS));
				Fut.push_back(std::forward<std::future<float>>(F));
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

			while (Function_Counter < 10) {}// SpinLock until every single function called returns as measured via the atomic int Function_Counter.

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
	
			std::vector<std::vector<uint32_t>> Test;

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

			std::vector<std::vector<uint32_t>> Test;
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


	//	Profiling::Memory::TrackDumpBlocks();
	//	Profiling::Memory::TrackListMemoryUsage();
	return 0;
}
 
			//SortTest.AsyncMergeSort();

/*
=====================================================================================================================
									  NOTES:
=====================================================================================================================
Pragmas for C++ Compilers, Good resource
https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.cbclx01/prag_ishome.htm

 Open STD:
 http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3857.pdf


Physics Solver:
https://www.gdcvault.com/play/1013359/High-Performance-Physics-Solver-Design

=====================================================================================================================
*/






