

#include "stdafx.h"  
#include "CppUnitTest.h" 

// NOTE:
// URL Microsoft Unit Testing Documents
// https://docs.microsoft.com/en-us/visualstudio/test/how-to-use-microsoft-test-framework-for-cpp?view=vs-2019


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTest
{
	TEST_CLASS(MemoryTrackerInVivoTest)
	{
	public:
		TEST_METHOD(TestMemStampInVivo)
		{
		}
	};
}


//#include"MemTracker.h"

//#include "MyProjectUnderTest.h"  

//namespace UnitTest
//{
////	[TestClass]
//	class MemoryTraceTest
//	{
////		[TestMethod]
//		void Test_MemStamp()
//		{
//			//Profiling::Memory::MemStamp TestStamp = Profiling::Memory::MemStamp();
//		}
//	};
//
//}
//
		//	Profiling::Memory::MemStamp TestStamp =  Profiling::Memory::MemStamp(__FILE__, __LINE__);
		//	Assert::AreEqual(TestStamp.lineNum, __LINE__ - 1);
		//	Assert::AreEqual(TestStamp.filename, __FILE__);