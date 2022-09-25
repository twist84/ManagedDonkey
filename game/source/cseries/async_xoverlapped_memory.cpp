#include "cseries/async_xoverlapped_memory.hpp"

void* __cdecl overlapped_malloc_tracked(dword size, char const* file, long line)
{
	return INVOKE(0x0066D570, overlapped_malloc_tracked, size, file, line);
}