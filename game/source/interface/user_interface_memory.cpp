#include "interface/user_interface_memory.hpp"

void __cdecl user_interface_free(void const* pointer)
{
	INVOKE(0x00AB4E50, user_interface_free, pointer);
}

void* __cdecl user_interface_malloc_tracked(dword size, char const* file, long line)
{
	return INVOKE(0x00AB4EF0, user_interface_malloc_tracked, size, file, line);
}

c_allocation_base* __cdecl user_interface_memory_allocation()
{
	return INVOKE(0x00AB4FE0, user_interface_memory_allocation);
}

