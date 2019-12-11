#pragma once

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
#include"Core/Common.h"
#include"Timer.h"
#include<unordered_map>

namespace Timing
{
	namespace Profiling
	{

		struct CREATURE_API Profile_Timer
			:
			Timer<Nanoseconds>
		{
			Profile_Timer(std::string _name)
				:
				Name(_name)
			{
				Start();
			}

			~Profile_Timer()
			{
				uint64_t Results = Stop();

				Print("==========================================");
				Print("Name: " << Name.c_str());
				Print("------------------------------------------");
				Print("Duration: " << Results * 0.0000001 << "ms");
				Print("Average: " << ((Accumulator[Name] += Results) / (++Samples[Name])) * 0.0000001 << "ms");
				Print("========================================== \n ");
			}


			SteadyClock Start_Time;
			std::string Name;

			static std::unordered_map<std::string, uint64_t> Samples;
			static std::unordered_map<std::string, uint64_t> Accumulator;
		};

	}
}
	_static std::unordered_map<std::string, uint64_t> Timing::Profiling::Profile_Timer::Samples;
	_static std::unordered_map<std::string, uint64_t> Timing::Profiling::Profile_Timer::Accumulator;









/*
========================================================================================================================================================================
                                                          TRASH                                                
========================================================================================================================================================================
*/

//0.0000001
//template<size_t _SZ>  uint64_t Profile_Timer<_SZ>::SampleCounter = _SZ;
///void Stop()
///{
///	Result = (Clock::now().time_since_epoch() - Start_time.);
///}
///void Start()
///{
///	Start_Time = Clock::now();
///}
///uint64_t Sample()
///{
///	return std::chrono::duration_cast <Resolution> (Clock::now() - Start_Time).count();
///}
///uint64_t Stop()
///{
///	Duration = Clock::now();
///	return (Duration - Start_Time).count();
///}
///void Reset()
///{
///	Start_Time = Clock::now();
///	Duration = Clock::now();
///}


//SystemClock End_time{ 0 };
/*

template<typename _Res = std::chrono::microseconds>
struct CREATURE_API Timer
{
	using Resolution = std::chrono::microseconds;

	SteadyClock Start_Time;
	SteadyClock Duration;

	Timer() = default;	~Timer() = default;

	void Start()
	{
		Start_Time = Clock::now();
	}
	uint64_t Sample()
	{

		return std::chrono::duration_cast <Resolution> (Clock::now() - Start_Time).count();
	}
	uint64_t Stop()
	{
		Duration = Clock::now();
		return (Duration - Start_Time).count();
	}
	void Reset()
	{
		Start_Time = Clock::now();
		Duration = Clock::now();
	}
};

*/