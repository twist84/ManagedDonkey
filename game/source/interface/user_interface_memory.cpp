#include "interface/user_interface_memory.hpp"

void* __cdecl user_interface_malloc_tracked(dword size, char const* file, long line)
{
	void* result = INVOKE(0x00AB4EF0, user_interface_malloc_tracked, size, file, line);

	return result;
}