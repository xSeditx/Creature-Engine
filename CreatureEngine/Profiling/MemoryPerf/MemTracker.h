#pragma once
/*
Copyright (c) 2002, 2008 Curtis Bartley
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

http://www.almostinfinite.com/memtrack.html
*/

#include <typeinfo>
#include <cassert>
#include <iostream>



#define MEMTRACK_NEW Profiling::Memory::MemStamp(__FILE__, __LINE__) * new
#define new MEMTRACK_NEW

namespace Profiling 
{
	namespace Memory
	{ /* ---------------------------------------- class MemStamp */
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
					std::cout << "Filename: " << _filename << " : Line:" << _lineNum <<"\n";
				}

				~MemStamp()
				{ }
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
*/