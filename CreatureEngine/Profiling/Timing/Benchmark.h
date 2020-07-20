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
#include<unordered_map>
#include"../../Core/Common.h"

#include"Timer.h"




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


//_static std::unordered_map<std::string, uint64_t> Timing::Profiling::Profile_Timer::Samples;
//_static std::unordered_map<std::string, uint64_t> Timing::Profiling::Profile_Timer::Accumulator;
//
//
//void Write_Profile(const ProfileResult & _result)
//{
//    if (ProfileCount++ > 0)
//    {
//        OutputStream << ",";
//    }
//    std::string Name = _result.Name;
//    std::replace(Name.begin(), Name.end(), '"', '\'');
//
//    OutputStream << "{";
//    OutputStream << "\"cat\":\"function\",";
//        OutputStream << "\"dur\":"\ <<
//        OutputStream
//        OutputStream
//        OutputStream
//        OutputStream
//        OutputStream
//
//}
//




#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>
struct ProfileResult
{
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};

struct InstrumentationSession
{
    std::string Name;
};

class Instrumentor
{
private:
    InstrumentationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
    int m_ProfileCount;

public:
    Instrumentor()
        :
        m_CurrentSession(nullptr),
        m_ProfileCount(0)
    {}

    void BeginSession(const std::string& name, const std::string& filepath = "results.json")
    {
        m_OutputStream.open(filepath);
        WriteHeader();
        m_CurrentSession = new InstrumentationSession{ name };
    }

    void EndSession()
    {
        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void WriteProfile(const ProfileResult& _result)
    {
        if (m_ProfileCount++ > 0)
        {
            m_OutputStream << ",";
        }

        std::string name = _result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (_result.End - _result.Start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << _result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << _result.Start;
        m_OutputStream << "}";
        m_OutputStream.flush();
    }

    void WriteHeader()
    {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        m_OutputStream.flush();
    }

    void WriteFooter()
    {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    static Instrumentor& get()
    {
        static Instrumentor instance;
        return instance;
    }
};

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name)
        : m_Name(name), m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::get().WriteProfile({ m_Name, start, end, threadID });

        m_Stopped = true;
    }
private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};





#define _STACK_TRACE_

/* Trace calls in and our of specified functions

This will likely become so much more in which ALL of our function calls will be wrapped in a method that traces their creation and destruction so when desired
we will have more functionality for tracking the stack and functions while easily being capable of turning it off at will
*/
// std::cout << "IN: "<< x << typeid(*this).name() << "\n"
// std::cout <<"OUT: "<< x << typeid(*this).name() << "\n"
#ifdef _STACK_TRACE_
#    define trace_IN(x)   PROFILE_FUNCTION()
#    define trace_OUT(x) 
#    define trace(x)      { trace_IN(x);
#    define Return(x)       return x; } trace_OUT(0);

#    define trace_scope(_name) PROFILE_SCOPE(_name)

#    define PROFILE_BEGIN_SESSION(_name, _filepath) Instrumentor::get().BeginSession(_name, _filepath)
#    define PROFILE_END_SESSION()   Instrumentor::get().EndSession();
#    define PROFILE_SCOPE(_name) InstrumentationTimer timer##__LINE__(_name);
#    define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__) void __cdecl Stop()

#else
#    define trace_IN(x)   
#    define trace_OUT(x)  
#    define trace(x)
#    define Return(x)       return x;


#    define PROFILE_BEGIN_SESSION(_name, _filepath) 
#    define PROFILE_END_SESSION()   
#    define PROFILE_SCOPE(_name) 
#    define PROFILE_FUNCTION() 
#endif












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