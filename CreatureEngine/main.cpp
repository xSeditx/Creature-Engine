#include<iostream>
//#include"Common.h"
//#include"Profiling\MemTracker.h"
#include"Profiling\SystemInfo.h"
int main()
{
	Profiling::SystemInfo::Retrieve_SystemInfo();
	{
		int *A = new int();
		delete(A);
	}

//	Profiling::Memory::TrackListMemoryUsage();
	return 0;
}

///$(VCInstallDir)Auxiliary\VS\UnitTest\lib
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