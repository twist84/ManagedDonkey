#include "memory/base/base_memory_windows.hpp"

#include "cseries/cseries.hpp"

void __cdecl base_memory_information_get(s_base_memory_information* out_information)
{
	INVOKE(0x004EC0E0, base_memory_information_get, out_information);
}

