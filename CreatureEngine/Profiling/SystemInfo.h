
namespace Profiling
{
	namespace SystemInfo
	{
	    /*
		Prints to the Console various System Information 
	    Also fills out global variables describing the state of our OS and Application 
		*/
		void Retrieve_SystemInfo();

	    /* Finds which types are most appropriate on the current architecture*/
		void get_OSTypeInformation();

	    /* Number of Hardware Threads available */
		unsigned int System_Cores_Count() noexcept;

		/* Number of Physical cores */
		unsigned int Physical_CPUCore_Count() noexcept;

		/* Return the number of Hyperthreaded cores avalible to the Application*/
		unsigned int get_HardwareThreadCount();

		extern unsigned int CPU_Core_Count;
		extern unsigned int Hardware_Thread_Count;
	}// End System Info NS
}// End Profiling NS