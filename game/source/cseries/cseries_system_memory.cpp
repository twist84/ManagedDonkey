#include "cseries/cseries_system_memory.hpp"

#include "cseries/cseries.hpp"

void __cdecl system_free(void* memory)
{
	INVOKE(0x0052DB90, system_free, memory);
}

void* __cdecl system_malloc(unsigned int size)
{
	return INVOKE(0x0052DC70, system_malloc, size);
}

