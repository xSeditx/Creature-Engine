#pragma once

#include"../../Core/Common.h"
/*
Copyright (c) 2002, 2008 Curtis Bartley
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

http://www.almostinfinite.com/memtrack.html
*/

#if 0 /// There is a serious error,Possibly from multithreading possibly not. The Previous Nodes are not being properly Allocated or deleted
/// std::unique_lock Fixed the Issue however when I attempt to delete the memory that error still remains. 

#include <typeinfo>
#include <cassert>
#include <iostream>
#include <mutex>
#ifdef _PRINT_MEM
#    define Print_Memory_Info(x) std::cout << "Memory: " << x << "\n"
#else
#    define Print_Memory_Info(x)
#endif

#define MEMTRACK_NEW Profiling::Memory::MemStamp(__FILE__, __LINE__) * new
#define new MEMTRACK_NEW

namespace Profiling 
{
	namespace Memory
	{ /* ---------------------------------------- class MemStamp */


		extern std::mutex AllocationMutex;

			class MemStamp
			{
			public:        // member variables
				char const * const filename;
				int const lineNum;
			public:        // construction/destruction
				MemStamp(char const *_filename, int _lineNum)
					:
					filename(_filename), 
					lineNum(_lineNum) 
				{
					Print_Memory_Info("CREATED: " << "  Filename: " << _filename << " : Line:" << _lineNum << "\n");
				}

				~MemStamp()
				{ 
					Print_Memory_Info("DESTROYED: " << "  Filename: " << filename << " : Line:" << lineNum << "\n");
				}
			};

			/* ---------------------------------------- memory allocation and stamping prototypes */

			void *TrackMalloc(size_t size);
			void  TrackFree(void *p);
			void  TrackStamp(void *p, const MemStamp &stamp, char const *typeName);
			void  TrackDumpBlocks();
			void  TrackListMemoryUsage();

			/* ---------------------------------------- operator * (MemStamp, ptr) */

			template <class _Ty> inline _Ty *operator*(const MemStamp &stamp, _Ty *p)
			{
				TrackStamp(p, stamp, typeid(_Ty).name());
				return p;
			}

	}//ns Memory
}//ns Profiling





/* NOTES:
Region Allocators are only one to produce better results

Region per thread
Check local thread flag on Allocation(dynamic Scoping)!
Metadata Bitmap at 2MB granularity

c++ Memory Pool Allocator
https://www.youtube.com/watch?v=ObN_DUN2QU4
*/

#endif