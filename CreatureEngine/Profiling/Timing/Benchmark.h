#include<chrono>
#include"Core/Common.h"
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<std::chrono::system_clock> SystemClock;
typedef std::chrono::time_point<std::chrono::steady_clock> SteadyClock;
typedef std::chrono::duration<float> fDuration;



using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;
using Nanoseconds  = std::chrono::nanoseconds;

namespace Profiling
{
	namespace Timing
	{

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



		template < typename _Res  >
		struct scoped_Timer
			:
			 Timer<>
		{
			using Resolution = _Res;
			scoped_Timer(uint64_t* _storage)
				:
				Timer<_Res>(),
				Storage(_storage)
			{
				Start_Time = Clock::now();
			}

			~scoped_Timer()
			{
				*Storage = (std::chrono::duration_cast <Resolution> (Clock::now() - Start_Time).count());
			}

			uint64_t* Storage;
		};


        template < typename _Res = std::chrono::microseconds >
        struct functional_Timer
        	: Timer< std::chrono::microseconds>
        {
        	using Resolution = std::chrono::microseconds;// = _Res...
        
        	functional_Timer(uint64_t _trigger)
        		: 
        		Timer<_Res>(),
        		Trigger(_trigger)
        	{}
        
        	template < typename _Func, typename ... ARGS >
        	auto Update(_Func&& _function, ARGS&& ... _args)
        	{
        		if ((std::chrono::duration_cast <Resolution> (Clock::now() - Start_Time).count()) > Trigger)
        		{
        			Start_Time = Clock::now();
        			return _function(_args...);
        		}
        		return false;
        	}
        
        	uint64_t Trigger;
        };
        



	// CREATURE_API
		struct  Profile_Timer
			:
			scoped_Timer<Microseconds>
		{
			/*We Define all in the Header as Inlining is a must for accurate results
			*/
			Profile_Timer()
				:
				scoped_Timer<Microseconds>(&Results)
			{}
			~Profile_Timer()
			{}

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
			uint64_t Samples;
			uint64_t Results;
			uint64_t Accumulator;
			//static uint64_t SampleCounter;
		};

		//template<size_t _SZ>  uint64_t Profile_Timer<_SZ>::SampleCounter = _SZ;
	}
}
