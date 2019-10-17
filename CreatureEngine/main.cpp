#include<iostream>
//#include"Common.h"

#include"Profiling\SystemInfo.h"
#include"Profiling\MemoryPerf\MemTracker.h"

int main()
{
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