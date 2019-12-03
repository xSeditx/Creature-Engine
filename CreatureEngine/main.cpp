#include<iostream>
#include<Windows.h>
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

int LOOP_COUNT = 100;
#include<string>

void  P0R0()
{
	std::cout << "P0R0" << "\n";
}
float P1R1(int _param)
{
	std::cout << "P1R1: " << _param << "\n";
	return static_cast<float>(_param * _param);
}
void  P1R0(int _param)
{
	std::cout << "P1R0:" << _param << "\n";
}
void  P2R0(int _param, int _param2)
{
	std::cout << "P1R0:" << _param << ":" << _param2 << "\n";
}
int   P0R1()
{
	std::cout << "P0R1" << "\n";
	return 42;
}

#define NUMBER_OF_THREADS 2000


int main()
{
	while (true)
	{
		//alloca()
		Function_Counter = 0;
		LOOP_COUNT += 1000;// 22100 is when Threadpool and Linear start to become one.
		Print("\n\n\n\n Loop Counter:" << LOOP_COUNT << " iterations in the Worker Functions\n");

		Profiling::Timing::Profile_Timer<100> Bench;
		{
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
				auto F = ThreadPool::get().Async(TestFunctionC, 123.321f, std::move(i));
				Fut.push_back(std::forward<std::future<float>>(F));
			}	

			uint64_t result{ 0 };
			int counter = Fut.size();
			while (counter)
			{
				for (auto& F : Fut)
				{
					if (!is_ready(F)) continue;
					result += F.get(); 
					--counter;
				}
			}

			Print("End Thread Pool Cluster: " << result);


			while (Function_Counter < 10) {/* SpinLock until every single function called returns as measured via the atomic int Function_Counter. */ }

		 	Print("Threadpool: " << result);
 			Bench.Stop();
		}
 
		Function_Counter = 0;
		Profiling::Timing::Profile_Timer<100> ThreadBM;
		{
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
			auto TPTest11T = new  asyncTask([](int)->int { Print("Lambda Function Call std::Async"); return 11; }, 10000);

			Print("Async Cluster");
			std::vector<std::future<float>> Fut;
	        for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
	        {
	        	auto TPTest4loop = std::async(std::launch::async, TestFunctionC, 123.321f, i);
				Fut.push_back(std::move(TPTest4loop));
	        }	
			Print("End Async Cluster");


			uint64_t result{ 0 };
			int counter = Fut.size();
			while (counter)
			{
				for (auto& F : Fut)
				{
					if (!is_ready(F))
					{// if not ready yet, check the next  
						continue;
					}
					result += F.get(); // it is ready 
					--counter;
				}
			}
			Print("Async :" << result);
			while (Function_Counter < 10) {// SpinLock until every single function called returns as measured via the atomic int Function_Counter. 
 }
			std::vector<std::vector<uint32_t>> Test;

			Test.push_back(TPTest5T.get());
			Test.push_back(TPTest6T.get());
			Test.push_back(TPTest7T.get());
			Test.push_back(TPTest8T.get());
			Test.push_back(TPTest9T.get());
			Test.push_back(TPTest10T.get());
			Print("Async: " << TPTest4T.get() << " : " << TestCompile(Test));

			ThreadBM.Stop();
		}


		Function_Counter = 0;
		Profiling::Timing::Profile_Timer<100> LBench;
		{
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
			auto Test11 = [](int)->int { Print("Lambda Function Linear call"); return 11; };


			while (Function_Counter > 10) {// SpinLock until every single function called returns as measured via the atomic int Function_Counter. 
			}

			uint64_t result{ 0 };
			for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
			{
				result += TestFunctionC( 123.321f, rand()% NUMBER_OF_THREADS);
			}
			Print("Linear :" << result);
			

			std::vector<std::vector<uint32_t>> Test;
			Test.push_back(Test5);
			Test.push_back(Test6);
			Test.push_back(Test7);
			Test.push_back(Test8);
			Test.push_back(Test9);
			Test.push_back(Test10);
			Print("Linear: " << Test4 << " : " << TestCompile(Test));

			LBench.Stop();
		}

		std::cout << " Straight Linear = " << LBench.Results / 1000.0f << " ms" << "\n";//0.020034
		std::cout << "     Thread Pool = " << Bench.Results / 1000.0f << " ms" << "\n";
		std::cout << "      std::async = " << ThreadBM.Results / 1000.0f << " ms" << "\n";//0.020034
		Sleep(1500);
	}

	//	Profiling::Memory::TrackDumpBlocks();
	//	Profiling::Memory::TrackListMemoryUsage();
	return 0;
}
OPTIMIZATION_ON()




/*    Apply tuple to a function: https://cpppatterns.com/patterns/apply-tuple-to-function.html    */
template<typename _FUNC, typename _TUPLE, size_t ..._SZ >
auto apply_tuple_impl(_FUNC&& _function, _TUPLE&& _arguments, std::index_sequence<_SZ...>)
->decltype((std::forward<_FUNC>(_function)(std::get<_SZ>(std::forward<_TUPLE>(_arguments))...)))/// Return was Decltype(auto) just auto and trailing return added to create zero/low cost abstraction
{
	return std::forward<_FUNC>(_function)(std::get<_SZ>(std::forward<_TUPLE>(_arguments))...);
}

template<typename _FUNC, typename _TUPLE>
decltype(auto) apply_from_tuple(_FUNC&& _function, _TUPLE&& _tupleArguments)
{
	std::size_t constexpr tSize
	{// Initialize tSize instead of using =
		 std::tuple_size<typename std::remove_reference<_TUPLE>::type>::value
	};

	return
		apply_tuple_impl(std::forward<_FUNC>(_function),
		std::forward<_TUPLE>(_tupleArguments),
		std::make_index_sequence<tSize>());
}

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






