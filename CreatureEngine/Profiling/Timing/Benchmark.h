#include<chrono>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<std::chrono::system_clock> SystemClock;
typedef std::chrono::time_point<std::chrono::steady_clock> SteadyClock;
typedef std::chrono::duration<float> fDuration;

namespace Profiling
{
	namespace Timing
	{
		template<size_t _SZ>
		struct Profile_Timer
		{
			/*We Define all in the Header as Inlining is a must for accurate results
			*/
			Profile_Timer()
			{

			}
			~Profile_Timer()
			{
				Clock CurrentTime = Clock::now();
				End_time = CurrentTime - Start_time;
			}

			void Start()
			{
			}
			void Stop()
			{}

			Clock Start_time{ 0 };
			Clock End_time{ 0 };
			uint64_t Samples;
			uint64_t Results;
			uint64_t Accumulator;
			static uint64_t SampleCounter;
		};

		template<size_t _SZ>  uint64_t Profile_Timer<_SZ>::SampleCounter = _SZ;
	}
}
