#include "stdafx.h"
#include "CppUnitTest.h"
#include "Profiling\MemoryPerf\MemTracker.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(MemoryTrackerTest)
	{
	public:

		TEST_METHOD(TestMemStamp)
		{
			Profiling::Memory::MemStamp *TestStamp = new Profiling::Memory::MemStamp(__FILE__, __LINE__);
			Assert::AreEqual(TestStamp->lineNum, __LINE__ - 1);
			Assert::AreEqual(TestStamp->filename, __FILE__);
		}
		TEST_METHOD(TestPass)
		{
			Assert::AreEqual(!true, false);
		}
		TEST_METHOD(TestAllocationHeader)
		{
			int *TestAllocation = new int();
		}
	};
}