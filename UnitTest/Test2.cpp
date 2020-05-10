
#include "stdafx.h"
#include "CppUnitTest.h"

#include"../CreatureEngine/Core/Common.h"

#include "../CreatureEngine/Profiling/MemoryPerf/MemTracker.h"
#include "../CreatureEngine/Profiling/Timing/Benchmark.h"

#include "../CreatureEngine/Core/Threading/Threadpool.h"
#include"../CreatureEngine/Core/Threading/TestFunctions.h"

#include"../CreatureEngine/Profiling/RenderUtilities.h"

#include"../CreatureEngine/Renderer/LowLevel/Materials/Image/Texture.h"
#include <type_traits> 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")


namespace UnitTest
{
//#ifdef _PROFILE_MEMORY
	TEST_CLASS(MemoryTrackerTest)
	{
	public:
		TEST_METHOD(TestPass)
		{/* Wellness check*/
			Assert::AreEqual(!true, false);	
			Assert::AreNotEqual(true, false);
		}
		// TODO: Need to validate the Appropriate size of Memory is being allocated here
		// This is not complete as it sits
		TEST_METHOD(TestNewSize)
		{/* Check the pointer size */
			int *A[] = { 0 };
			*A = new int(100);
			Assert::AreEqual(sizeof(A), sizeof(int*));
		}

		TEST_METHOD(TestMemStamp)
		{/* Check that the proper header information is being added to the Memory block */
		///	Profiling::Memory::MemStamp *TestStamp = new Profiling::Memory::MemStamp(__FILE__, __LINE__);
		///	Assert::AreEqual(TestStamp->lineNum, __LINE__ - 1);
		///	Assert::AreEqual(TestStamp->filename, __FILE__);
		}

		TEST_METHOD(TestNewLocation)
		{/* Test Operator new to ensure it is creating the new block where it is suppose to */

			const int Count{ 1000 };
			int Sample{ 0 };

			int Arr[Count]{ 0 };

			for(uint32_t i{0}; i < Count; ++i)
			{/* Assign values to Arr */
				Arr[i] = i;
			}

			int *TestArray = new(Arr) int[Count];
		
			for (uint32_t i{ 0 }; i < 100; ++i)
			{/* Test 100 Random values to ensure alignment */
				int Diff = rand() % 100;
				Sample = rand() % (Count - Diff);
				Assert::AreEqual(TestArray[Sample] + Diff, TestArray[Sample + Diff]);
			}
			for (uint32_t i{ 0 }; i < 100; ++i)
			{/* Sample 100 random values ensuring Arr and TestArray are aligned at the same memory address*/
				Sample = rand() % Count;
				Assert::AreEqual(TestArray[Sample], Arr[Sample]);

				Arr[Sample] = rand() % 1000;
				Assert::AreEqual(TestArray[Sample], Arr[Sample]);

				Arr[Sample] = rand() % 1000;
				Assert::AreNotEqual(TestArray[Sample], Arr[Sample + 1]);
			}
			for (uint32_t i{ 0 }; i < 100; ++i)
			{/* Test if TestArray changes Arr */
				Sample = rand() % Count;
				Assert::AreEqual(TestArray[Sample], Arr[Sample]);

				TestArray[Sample] = rand() % 1000;
				Assert::AreEqual(TestArray[Sample], Arr[Sample]);

				TestArray[Sample] = rand() % 1000;
				Assert::AreNotEqual(TestArray[Sample], Arr[Sample + 1]);
			}
		}

		TEST_METHOD(TestAllocationHeader)
		{
			int *TestAllocation = new int();
		 
		}
	};
//#endif

	int TestTPpass(int _in)
	{
		std::cout << "TestTPass:" << _in << "\n";
		return 1;
	}
	int TestTPfail(int _in)
	{
		std::cout << "TestTfail:" << _in << "\n";
	    return 0;
	}
//	
//TEST_CLASS(ThreadPoolTest)
//{
//	/* Wellness check*/
//	TEST_METHOD(TestPass)
//		{
//			Assert::AreEqual(!true, false);
//			Assert::AreNotEqual(true, false);
//		}
//
//	TEST_METHOD(TestThreadCount)
//	{/* Wellness check*/
//		//Assert::AreEqual(!true, false);
//		//Assert::AreNotEqual(true, false);
//		
//		Core::Threading::ThreadPool::get();
//		auto A = Core::Threading::ThreadPool::get().Async([] {int ret = 10; return ret; });
//		//auto B = Core::Threading::ThreadPool::get().Async([] { return 10; });
//		//auto C = Core::Threading::ThreadPool::get().Async([] { return 10; });
//		//auto D = Core::Threading::ThreadPool::get().Async([] { return 10; });
//		//auto E = Core::Threading::ThreadPool::get().Async([] { return 10; });
//		//auto K = A.get() + B.get() + C.get() + D.get() + E.get();
//	 	//Assert::AreEqual(100,K);
//	}
//	int LOOP_COUNT = 100000;
//	TEST_METHOD(TestThreadPool)
//	{
//		int NUMBER_OF_THREADS = 100;
//	
//		using TP = Core::Threading::ThreadPool;
//
//		auto A = [&]()->bool// Return type
//		{
//			bool k{ false };
//			std::vector<int> i;
//			i.push_back(10);
//			std::cout << "Something" << i.size() << "\n";
//			i[-10000] = 22;
//			auto A = TP::get().Async(TestTPpass, std::move(19));
//			auto B = TP::get().Async(TestTPfail, std::move(42));
//			k = true;
//			return k;
//		};
//		bool B = A();
//		Assert::IsTrue(B); // MAYBE LOL
//
//	}
//};




	TEST_CLASS(BenchmarkRender)
	{
		TEST_METHOD(CreateMemoryBlock)
		{
			uint32_t
				PosX = 0,
				PosY = 0;
			uint32_t
				SizeX = 50,
				SizeY = 150;
            uint32_t
                RangeX = 50,
                RangeY = 150;

			Profiling::DisplayWindow MemoryBlock =
                Profiling::DisplayWindow({ PosX, PosY }, { SizeX , SizeY }, {RangeX, RangeY});

			Assert::AreEqual(MemoryBlock.size(), SizeX * SizeY * sizeof(uint32_t));
		}

		TEST_METHOD(ColorUnion)
		{
			uint32_t
				PosX = 0,
				PosY = 0;
			uint32_t
				SizeX = 1,
				SizeY = 50;
            uint32_t
                RangeX = 50,
                RangeY = 150;


			Profiling::DisplayWindow MemoryBlock =
                Profiling::DisplayWindow({ PosX, PosY }, { SizeX , SizeY }, {RangeX, RangeY} );

			MemoryBlock.setPixel(0, 0, 0xFFFFFFFF);

			MemoryBlock.Update(1);
			Assert::AreEqual(0xFFFFFFFF , (unsigned int)MemoryBlock.getPixel(0, 1));

			MemoryBlock.Update(1);
			Assert::AreEqual(0xFFFFFFFF , (unsigned int)MemoryBlock.getPixel(0, 2));

			MemoryBlock.Update(1);
			Assert::AreEqual(0xFFFFFFFF , (unsigned int)MemoryBlock.getPixel(0, 3));
		}
	};

}//End UnitTest NS










//for_loop(Index, 10)
//{
//	Function_Counter = 0;
//	Print("\n\n Loop Counter:  (X) FIX THIS iterations in the Worker Functions\n");
//	{
//		Timing::Profiling::Profile_Timer Bench("My Threadpool");
//		std::vector<std::vector<uint32_t>> Test;
//
//		auto A = TP::get().Async(TestFunctionE, std::move(LOOP_COUNT));
//		auto B = TP::get().Async(TestFunctionE, std::move(LOOP_COUNT));
//		auto C = TP::get().Async(TestFunctionB, 1431);
//		auto D = TP::get().Async(TestFunctionD, 123.321f, 10);
//		auto E = TP::get().Async(TestFunctionA);
//		auto F = TP::get().Async(TestFunctionC, 3.14159f, 123);
//		auto G = TP::get().Async(TestFunctionF, std::move(LOOP_COUNT));
//		auto H = TP::get().Async(TestFunctionG, std::move(LOOP_COUNT));
//		auto I = TP::get().Async(TestFunctionH, std::move(LOOP_COUNT));
//		auto J = TP::get().Async(TestFunctionI, std::move(LOOP_COUNT));
//		auto K = TP::get().Async(TestFunctionJ, std::move(LOOP_COUNT));
//
//		Print("Thread Pool Cluster");
//		std::vector<std::future<float>> Fut;
//		for_loop (i, (uint32_t)NUMBER_OF_THREADS)
//		{
//			auto F = TP::get().Async(TestFunctionC, 123.321f, std::move(rand() % NUMBER_OF_THREADS));
//			Fut.push_back(std::forward<std::future<float>>(F));
//		}
//		uint64_t result{ 0 };
//		uint64_t counter = Fut.size();
//		while (counter)
//		{
//			for (auto& F : Fut)
//			{
//				if (!is_ready(F))
//				{
//					continue;
//				}
//				result += (uint64_t)F.get();
//				--counter;
//			}
//		}
//		Print("End Thread Pool Cluster: " << result);
//		Print("Threadpool: " << result);
//	}
//}

// TODO: void *TrackMalloc(size_t size);
// TODO: void  TrackFree(void *p);
// TODO: void  TrackStamp(void *p, const MemStamp &stamp, char const *typeName);
// TODO: void  TrackDumpBlocks();
// TODO: void  TrackListMemoryUsage();

//TODO: void BlockHeader::Stamp(char const *filename, int lineNum, char const *typeName)
//{
//	myFilename = filename;
//	myLineNum = lineNum;
//	myTypeName = typeName;
//}





			//		Function_Counter = 0;
			//		{
			//
			//			Timing::Profiling::Profile_Timer ThreadBM("std::Async");
			//			auto  TPTest5T = std::async(std::launch::async | std::launch::deferred, TestFunctionE, LOOP_COUNT);
			//			auto  TPTest1T = std::async(std::launch::async | std::launch::deferred, TestFunctionB, 1431);
			//			auto  TPTest4T = std::async(std::launch::async | std::launch::deferred, TestFunctionD, 123.321f, 10);
			//			auto  TPTest3T = std::async(std::launch::async | std::launch::deferred, TestFunctionA);
			//			auto  TPTest2T = std::async(std::launch::async | std::launch::deferred, TestFunctionC, 3.14159f, 123);
			//			auto  TPTest6T = std::async(std::launch::async | std::launch::deferred, TestFunctionF, LOOP_COUNT);
			//			auto  TPTest7T = std::async(std::launch::async | std::launch::deferred, TestFunctionG, LOOP_COUNT);
			//			auto  TPTest8T = std::async(std::launch::async | std::launch::deferred, TestFunctionH, LOOP_COUNT);
			//			auto  TPTest9T = std::async(std::launch::async | std::launch::deferred, TestFunctionI, LOOP_COUNT);
			//			auto TPTest10T = std::async(std::launch::async | std::launch::deferred, TestFunctionJ, LOOP_COUNT);
			//
			//			Print("Async Cluster");
			//			std::vector<std::future<float>> Fut;
			//			for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
			//			{
			//				auto TPTest4loop = std::async(std::launch::async, TestFunctionC, 123.321f, rand() % NUMBER_OF_THREADS);
			//				Fut.push_back(std::move(TPTest4loop));
			//			}
			//
			//			uint64_t result{ 0 };
			//			uint64_t counter = Fut.size();
			//			while (counter)
			//			{
			//				for (auto& F : Fut)
			//				{
			//					if (!is_ready(F))
			//					{// if not ready yet, check the next  
			//						continue;
			//					}
			//					result += (uint64_t)F.get(); // it is ready 
			//					--counter;
			//				}
			//			}
			//
			//			Print("End Async Cluster");
			//			Print("Async :" << result);
			//			while (Function_Counter < 10) {}// SpinLock until every single function called returns as measured via the atomic int Function_Counter. 
			//
			//			std::vector<std::vector<uint32_t>> Test;
			//
			//			Test.push_back(TPTest5T.get());
			//			Test.push_back(TPTest6T.get());
			//			Test.push_back(TPTest7T.get());
			//			Test.push_back(TPTest8T.get());
			//			Test.push_back(TPTest9T.get());
			//			Test.push_back(TPTest10T.get());
			//
			//		}
			//
			//
			//		Function_Counter = 0;
			//		{
			//			Timing::Profiling::Profile_Timer LBench("Linear Benchmark");
			//			auto Test5 = TestFunctionE(std::move(LOOP_COUNT));// .5ms
			//			auto Test4 = TestFunctionD(123.321f, 10);
			//			auto Test1 = TestFunctionB(1431);
			//			auto Test3 = TestFunctionA();
			//			auto Test2 = TestFunctionC(3.14159f, 123);
			//			auto Test6 = TestFunctionF(std::move(LOOP_COUNT));//.3
			//			auto Test7 = TestFunctionG(std::move(LOOP_COUNT));//3.21
			//			auto Test8 = TestFunctionH(std::move(LOOP_COUNT));// .32
			//			auto Test9 = TestFunctionI(std::move(LOOP_COUNT));//2.8
			//			auto Test10 = TestFunctionJ(std::move(LOOP_COUNT));//2.6
			//
			//
			//			while (Function_Counter < 10) {}// SpinLock until every single function called returns as measured via the atomic int Function_Counter. 
			//
			//
			//			uint64_t result{ 0 };
			//			for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
			//			{
			//				result += (uint64_t)TestFunctionC(123.321f, rand() % NUMBER_OF_THREADS);
			//			}
			//			Print("Linear :" << result);
			//
			//			std::vector<std::vector<uint32_t>> Test;
			//			Test.push_back(Test5);
			//			Test.push_back(Test6);
			//			Test.push_back(Test7);
			//			Test.push_back(Test8);
			//			Test.push_back(Test9);
			//			Test.push_back(Test10);
			//
			//		}
			//		Sleep(2500);
			//	
