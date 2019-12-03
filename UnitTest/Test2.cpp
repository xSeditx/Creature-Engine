#include "stdafx.h"
#include "CppUnitTest.h"
#include "Profiling\MemoryPerf\MemTracker.h"
#include "Profiling\Timing\Benchmark.h"

#include "Core\Threading\Threadpool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


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
			Profiling::Memory::MemStamp *TestStamp = new Profiling::Memory::MemStamp(__FILE__, __LINE__);
			Assert::AreEqual(TestStamp->lineNum, __LINE__ - 1);
			Assert::AreEqual(TestStamp->filename, __FILE__);
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

	};
//#endif
	TEST_CLASS(ThreadPoolTest)
	{

		TEST_METHOD(TestPass)
		{/* Wellness check*/
			Assert::AreEqual(!true, false);
			Assert::AreNotEqual(true, false);
		}

		TEST_METHOD(TestThreadCount)
		{/* Wellness check*/
			//Assert::AreEqual(!true, false);
			//Assert::AreNotEqual(true, false);
			Core::Threading::ThreadPool::get();
			auto A = Core::Threading::ThreadPool::get().Async([] {int ret = 10; return ret; });
			auto B = Core::Threading::ThreadPool::get().Async([] { return 10; });
			auto C = Core::Threading::ThreadPool::get().Async([] { return 10; });
			auto D = Core::Threading::ThreadPool::get().Async([] { return 10; });
			auto E = Core::Threading::ThreadPool::get().Async([] { return 10; });
			auto K = A.get() + B.get() + C.get() + D.get() + E.get();
		//	Assert::AreEqual(100,K);
		}



		TEST_METHOD(ThreadPoolSpeed)
		{
			int Samples{ 10 };
			int LOOP_COUNT = 20000;
			int NUMBER_OF_THREADS = 2000;
			int Linear{ 0 }, Threaded{ 0 }, Async{ 0 };
			for (int i{ 0 }; i < Samples; ++i)
			{
				//alloca()
				Function_Counter = 0;
				LOOP_COUNT += 1000;// 22100 is when Threadpool and Linear start to become one.
				Print("\n\n\n\n Loop Counter:" << LOOP_COUNT << " iterations in the Worker Functions\n");

				Profiling::Timing::Profile_Timer<100> Bench;
				{
					std::vector<std::vector<uint32_t>> Test;

					auto A = Core::Threading::ThreadPool::get().Async(TestFunctionE, std::move(LOOP_COUNT));
					auto B = Core::Threading::ThreadPool::get().Async(TestFunctionE, std::move(LOOP_COUNT));
					auto C = Core::Threading::ThreadPool::get().Async(TestFunctionB, 1431);
					auto D = Core::Threading::ThreadPool::get().Async(TestFunctionD, 123.321f, 10);
					auto E = Core::Threading::ThreadPool::get().Async(TestFunctionA);
					auto F = Core::Threading::ThreadPool::get().Async(TestFunctionC, 3.14159f, 123);
					auto G = Core::Threading::ThreadPool::get().Async(TestFunctionF, std::move(LOOP_COUNT));
					auto H = Core::Threading::ThreadPool::get().Async(TestFunctionG, std::move(LOOP_COUNT));
					auto I = Core::Threading::ThreadPool::get().Async(TestFunctionH, std::move(LOOP_COUNT));
					auto J = Core::Threading::ThreadPool::get().Async(TestFunctionI, std::move(LOOP_COUNT));
					auto K = Core::Threading::ThreadPool::get().Async(TestFunctionJ, std::move(LOOP_COUNT));

					Print("Thread Pool Cluster");

					std::vector<std::future<float>> Fut;
					for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
					{
						auto F = Core::Threading::ThreadPool::get().Async(TestFunctionC, 123.321f, std::move(i));
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


				uint64_t  results;
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
						result += TestFunctionC(123.321f, rand() % NUMBER_OF_THREADS);
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

				Linear += LBench.Results;
				Threaded += Bench.Results;
				Async += ThreadBM.Results;
			}
			Assert::IsTrue((Threaded > Linear) && (Threaded > Async));
		}
	};
}//End UnitTest NS