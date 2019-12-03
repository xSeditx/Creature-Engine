
___
# System Information
___

System Information and Global Identifiers for various OS and Hardware specific information
___
namespace Profiling
namespace SystemInfo
___



___

 Function Name | Description
-------------------|----------------
void Retrieve_SystemInfo();	|	Prints to the Console various System Information. Also fills out global variables describing the state of our OS and Application 
void get_OSTypeInformation(); |  Finds which types are most appropriate on the current architecture
unsigned int System_Cores_Count() noexcept; | Number of Hardware Threads available  
unsigned int Physical_CPUCore_Count() noexcept;|  Number of Physical cores 
unsigned int get_HardwareThreadCount(); |  Return the number of Hyperthreaded cores avalible to the Application

___
 
  Data Members | Description
 --------------|----------------------
unsigned int CPU_Core_Count;| Number of Physical CPU Cores
unsigned int Hardware_Thread_Count;| Number of usable hardware threads

___