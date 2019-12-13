
#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CreatureEngine/Profiling/MemoryPerf/MemTracker.h"
#include "../CreatureEngine/Profiling/Timing/Benchmark.h"

#include "../CreatureEngine/Core/Threading/Threadpool.h"

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
		 	Assert::AreEqual(100,K);
		}
	};
}//End UnitTest NS