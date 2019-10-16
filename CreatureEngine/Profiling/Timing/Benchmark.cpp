#include"Profiling\Timing\Benchmark.h"
/*
  =======================================================================================================================
                                         BENCHMARK TIMING MODULE                                                                            
  =======================================================================================================================
	Module for Timing sections of the Code base. As the Timer Object is created it utilizes RAII to end testing on
	Object deletion. Code is benchmarked by wrapping it in a scope. Data gathered via this means shall be output 
	to any number of Log methods designed to store a process the benchmarked data at a later time.
  =======================================================================================================================
*/

#include<chrono>
//
//
//namespace Profiling
//{
//	namespace Timing
//	{
//		template<size_t _SZ>
//		Profile_Timer<_SZ>::~Profile_Timer()
//		{}
//
//		void Start() {}
//		void Stop() {}
//
//			uint64_t Samples;
//			uint64_t Results;
//			uint64_t Accumulator;
//			static uint64_t static_SampleCounter;
//
//	}
//}
//