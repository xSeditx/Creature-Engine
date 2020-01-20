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
#include"../CreatureEngine/Renderer/LowLevel/OpenGL/OpenGL.h"
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

#define _TEST_THREADPOOL_SPEED    1
//
//#include <intrin.h>

// Getting the Stack frame caller
// https://social.msdn.microsoft.com/Forums/vstudio/en-US/da4e85c5-407e-4a57-87e7-14bf994504d9/error-asm-keyword-not-supported-on-this-architecture?forum=vcgeneral
// void * memoryAllocator = _ReturnAddress();

//#pragma message (__FILE__ "[" STRING(__LINE__) "]: test")
//https://docs.microsoft.com/en-us/cpp/preprocessor/pragma-directives-and-the-pragma-keyword?view=vs-2019

int TestRecursion(int _param)
{
	_param--;
	Print("Recursion " << _param);
	if(_param > 0)
	{
		auto E = ThreadPool::get().Async(TestRecursion, (int)_param );
		E.get();
	}
	Print("Exit " << _param);

	return 65;
}

#include<stack>
std::stack<std::string> CS;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
std::mutex DEBUGMutex;


bool TEST_PROFILE_WINDOW();

#include<utility>
#include"Profiling/RenderUtilities.h"
#include"FrontEnd/Window.h"
#include"Core/Application.h"


class App
	: public Application
{

	Vec2 Vertices[3] =
	{
		{-1.0,-1.0 },
		{ 1.0,-1.0 },
		{ 0.0, 1.0 }
	};

	Vec2 UVcoords[3] =
	{
		{ 0.0, 0.0 },
		{ 1.0, 0.0 },
		{ 1.0, 1.0 }
	};

	GLuint Indices[3] =
	{
		0,1,2
	};
	GLuint VAO{ 0 }, VBO{ 0 }, IBO{ 0 };




	Vec2 ProfilerVerts[4] =
	{
		{-1.0,-1.0 },
		{ 1.0,-1.0 },
		{ 0.0, 1.0 },
		{ 1.0, 1.0 }
	};

	Vec2 ProfilerUV[4] =
	{
		{ 0.0, 0.0 },
		{ 1.0, 0.0 },
		{ 1.0, 1.0 },
		{ 0.0, 1.0 }
	};

	GLuint ProfilerIndices[3] =
	{
		0,1,2,
	};




	virtual void OnCreate()
	{


		glUseProgram(getWindow().defaultShader());
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		GLuint Location = 4;

		Location = glGetAttribLocation(getWindow().defaultShader(), "aPos");
		glEnableVertexAttribArray(Location);
		glVertexAttribPointer(Location, 3, GL_FLOAT, GL_FALSE, 0, (char *)NULL);

		uint32_t ERR = 0;
		if ((ERR = glGetError()))
		{
			Print("Error" << ERR);
			__debugbreak();
		}

		ERR = 0;
		if ((ERR = glGetError()))
		{
			Print("Error" << ERR);
			__debugbreak();
		}
 	}
	virtual void OnRender()
	{
		glUseProgram(getWindow().defaultShader());
		{
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			{
				glDrawArrays(GL_TRIANGLES, 0, 3);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		glUseProgram(0);
	}

};


//https://en.cppreference.com/w/cpp/compiler_support
int main()
{
	TODO("Setup the Bitmap object for the Profiler so that I can dynamically update it and get the HUD for the profiler Operation. Do NOTHING else first!");
	App MyApp;
	MyApp.Init();
	iVec2 r = OpenGL::get_MaximumViewportDimensions();
	Print("Max Viewport Dimensions: " << r.x << " : " << r.y);
	iVec4 vp = OpenGL::get_Viewport();
 	Print("Viewport: " << vp.x << " : " << vp.y << " : " << vp.z << " : " << vp.w );

	MyApp.Run();
	MyApp.End();

  	TEST_UNIT(TEST_PROFILE_WINDOW());
	DEBUG_CODE(_Trace("Testing Trace Macro", 100000));
	DEBUGPrint(CON_Red, "Testing Print");
 

	while (true)
	{
        TestAsyncSort SortTest( 1024 ); // 4096); // 262144); //

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


	//	Profiling::Memory::TrackDumpBlocks();
	//	Profiling::Memory::TrackListMemoryUsage();
	return 0;
}
  


bool TEST_PROFILE_WINDOW()
{
	{
		Profiling::DisplayWindow Test({ 0,0 }, { 10, 150 });
		{	// TEST SET PIXEL

			Test.setPixel(1, 1, 0xffffffff);
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




/*
=====================================================================================================================
									  NOTES:
=====================================================================================================================
 Pragmas for C++ Compilers, Good resource
 https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.cbclx01/prag_ishome.htm

 Open STD:
 http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3857.pdf

 Actor based system
 http://www.actor-framework.org/pdf/manual.pdf

 Physics Solver:
 https://www.gdcvault.com/play/1013359/High-Performance-Physics-Solver-Design



 Advanced Programming with Microsoft Quick C
 https://books.google.com/books?id=IjCjBQAAQBAJ&pg=PA41&lpg=PA41&dq=%23pragma+check_stack(+%5B%7B+on+%7C+off+%7D%5D+)+what+does+it+do&source=bl&ots=44JLKNArHT&sig=ACfU3U3eSXG1JpiNDv9IgKbrUv3ByJrJqw&hl=en&sa=X&ved=2ahUKEwj3rrKygeTmAhVxkuAKHbx8C1YQ6AEwAXoECAkQAQ#v=onepage&q=%23pragma%20check_stack(%20%5B%7B%20on%20%7C%20off%20%7D%5D%20)%20what%20does%20it%20do&f=false
=====================================================================================================================
*/

/*
             Great List of Compiler Directives.
    Likely can eliminate verything else in this Comment because of this
    https://assets.ctfassets.net/9pcn2syx7zns/61QggiXm0CGYmsJEUfwjld/682ad1c09e795402d3c5f10c009985ad/preprocessor.pdf
    
    
    int _inp(ushort _port);                          Reads a Port
    int _outp(unsigned short port, int databyte)    Writes to a Port
    
    #pragma auto_inline( [ { on | off } ] )
    
    #pragma loop( hint_parallel( n ) )
    #pragma loop( no_vector )
    #pragma loop( ivdep )
    
    #pragma omp [directive]
    			 parallel      Defines a parallel region, which is code that will be executed by multiple threads in parallel.
    			 for           Causes the work done in a for loop inside a parallel region to be divided among threads.
    			 sections      Identifies code sections to be divided among all threads.
    			 single        Lets you specify that a section of code should be executed on a single thread, not necessarily the master thread.
    
    			 For master and synchronization:
    			 Directive     Description
    
    			 master        Specifies that only the master thread should execute a section of the program.
    			 critical      Specifies that code is only executed on one thread at a time.
    			 barrier       Synchronizes all threads in a team; all threads pause at the barrier, until all threads execute the barrier.
    			 atomic        Specifies that a memory location that will be updated atomically.
    			 flush         Specifies that all threads have the same view of memory for all shared objects.
    			 ordered       Specifies that code under a parallelized for loop should be executed like a sequential loop.
    
    			 threadprivate Specifies that a variable is private to a thread.
*/












//glVertexAttribPointer(
//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//	3,                  // size
//	GL_FLOAT,           // type
//	GL_FALSE,           // normalized?
//	0,                  // stride
//	(void*)0            // array buffer offset
//);

