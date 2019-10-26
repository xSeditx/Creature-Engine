#include<iostream>
//#include"Common.h"

#include"Profiling\SystemInfo.h"
#include"Profiling\MemoryPerf\MemTracker.h"
#include"Core\Threading\Threadpool.h"

using namespace Core;
using namespace Threading;

#include<string>

int TestFunctionA()
{
	std::cout << "FunctionA" << "\n";
	return 11;
}
int TestFunctionB(int _param)
{
	 int result;
	std::cout << "FunctionB: " << _param << "\n";
	//result.set_value(10);
	result = _param * 2;
	return result;
}
float TestFunctionC(float _paramA, int _paramB)
{
	std::cout << "FunctionC: " << _paramA << " : " << _paramB << "\n";
	return _paramA * _paramB;
}

std::string TestFunctionD(float _paramA, int _paramB)
{
	std::cout << "FunctionD " << _paramA << " : " << _paramB << "\n";
	return "String: " + std::to_string(_paramA) + " : " + std::to_string(_paramB);
}


int main()
{
	auto Test4 = ThreadPool::get().Async(TestFunctionD,123.321f, 10);
	auto Test1 = ThreadPool::get().Async(TestFunctionB, 1431);
	auto Test3 = ThreadPool::get().Async(TestFunctionA);
	auto Test2 = ThreadPool::get().Async(TestFunctionC, 3.14159f, 123);

	std::cout << "Return Value A:" << Test1.get() << "\n";
	std::cout << "Return Value B:" << Test2.get() << "\n";
	std::cout << "Return Value C:" << Test3.get() << "\n";
	std::cout << "Return Value D:" << Test4.get() << "\n";
	Profiling::SystemInfo::Retrieve_SystemInfo();
	{
		int *A = new int();
		delete(A);
	}
	return 0;
}


/*
Googles Class Structure:
---
Typedefs, Using, and Enums,
Constants,
Constructors,
Destructor, 
Methods(including static methods),
Data Members( including static data members)

*/