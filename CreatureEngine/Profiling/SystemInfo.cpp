#include"SystemInfo.h"
#include<typeinfo>
#include<iostream>

#include<thread>
#include<cassert>
#include<Windows.h>

namespace Profiling
{
	namespace SystemInfo
	{
		unsigned int CPU_Core_Count{ 0 };
		unsigned int Hardware_Thread_Count{ 0 };

		void Retrieve_SystemInfo()
		{
			CPU_Core_Count = Physical_CPUCore_Count();
			Hardware_Thread_Count = get_HardwareThreadCount();

			std::cout << "Number of Physical CPU Cores avalible: " << CPU_Core_Count << "\n";
			std::cout << "Number of Hardware threads avalible: " << System_Cores_Count() << "\n";

			get_OSTypeInformation();

			SYSTEM_INFO system_info;
			ZeroMemory(&system_info, sizeof(system_info));
			GetSystemInfo(&system_info);
			std::cout << "Processor Type: " << system_info.dwProcessorType << "\n";
			std::cout << "Processor Architecture: " << system_info.wProcessorArchitecture << "\n";
			std::cout << "Processor Type: " << system_info.dwProcessorType << "\n";
			std::cout << "Allocation Granularity: " << system_info.dwAllocationGranularity << "\n";
			std::cout << "Active Processor Mask: " << system_info.dwActiveProcessorMask << "\n";
			std::cout << "Oem Id: " << system_info.dwOemId << "\n";
			std::cout << "Minimum Application Address: " << system_info.lpMinimumApplicationAddress << "\n";
			std::cout << "Maximum Application  Address: " << system_info.lpMaximumApplicationAddress << "\n";
			std::cout << "ProcessorLevel: " << system_info.wProcessorLevel << "\n";
			std::cout << "Processor Revision: " << system_info.wProcessorRevision << "\n";
			std::cout << "Reserved: " << system_info.wReserved << "\n";
		}

		unsigned int get_HardwareThreadCount()
		{
			return std::thread::hardware_concurrency();
		}

		void get_OSTypeInformation()
		{
			std::cout << 
				/* Signed integers */
				typeid(int8_t).name()  << " int8_t " << "\n" <<
				typeid(int16_t).name() << " int16_t " << "\n" <<
				typeid(int32_t).name() << " int32_t " << "\n" <<
				typeid(int64_t).name() << " int64_t " << "\n" <<

				/* Signed Fast Integers*/
				typeid(int_fast8_t).name() << " int_fast8_t " << "\n" <<
				typeid(int_fast16_t).name() << " int_fast16_t " << "\n" <<
				typeid(int_fast32_t).name() << " int_fast32_t " << "\n" <<
				typeid(int_fast64_t).name() << " int_fast64_t " << "\n" <<

				/* Signed Promised integers*/
				typeid(int_least8_t).name() << " int_least8_t " << "\n" <<
				typeid(int_least16_t).name() << " int_least16_t " << "\n" <<
				typeid(int_least32_t).name() << " int_least32_t " << "\n" <<
				typeid(int_least64_t).name() << " int_least64_t " << "\n" <<

				/* Unsigned integers */
				typeid(uint8_t).name() << " uint8_t " << "\n" <<
				typeid(uint16_t).name() << " uint16_t " << "\n" <<
				typeid(uint32_t).name() << " uint32_t " << "\n" <<
				typeid(uint64_t).name() << " uint64_t " << "\n" <<

				/* Unsigned Fast Integers*/
				typeid(uint_fast8_t).name() << " uint_fast8_t " << "\n" <<
				typeid(uint_fast16_t).name() << " uint_fast16_t " << "\n" <<
				typeid(uint_fast32_t).name() << " uint_fast32_t " << "\n" <<
				typeid(uint_fast64_t).name() << " uint_fast64_t " << "\n" <<

				/* Unsigned Promised Integers*/
				typeid(uint_least8_t).name() << " uint_least8_t " << "\n" <<
				typeid(uint_least16_t).name() << " uint_least16_t " << "\n" <<
				typeid(uint_least32_t).name() << " uint_least32_t " << "\n" <<
				typeid(uint_least64_t).name() << " uint_least64_t " << "\n";
		}


		unsigned int get_MemoryPage_Size() noexcept
		{
			SYSTEM_INFO system_info;
			ZeroMemory(&system_info, sizeof(system_info));
			GetSystemInfo(&system_info);
			return static_cast<unsigned int>(system_info.dwPageSize);
		}

		unsigned int System_Cores_Count() noexcept
		{
			SYSTEM_INFO system_info;
			ZeroMemory(&system_info, sizeof(system_info));
			GetSystemInfo(&system_info);
			return static_cast<unsigned int>(system_info.dwNumberOfProcessors);
		}


		unsigned int Physical_CPUCore_Count() noexcept
		{
			DWORD length = 0;
			const bool result_first = GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &length);
			assert(GetLastError() == ERROR_INSUFFICIENT_BUFFER);

			std::unique_ptr< uint8_t[] > buffer(new uint8_t[length]);
			const PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info =
				reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX> (buffer.get());

			const bool result_second = GetLogicalProcessorInformationEx(RelationProcessorCore, info, &length);
			assert(result_second != false);

			unsigned int  nb_physical_cores = 0;
			unsigned int offset = 0;
			do {
				const PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX current_info =
					reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.get() + offset);
				offset += current_info->Size;
				++nb_physical_cores;
			} while (offset < length);

			return nb_physical_cores;
		}
	}// End System Info NS
}// End Profiling NS





