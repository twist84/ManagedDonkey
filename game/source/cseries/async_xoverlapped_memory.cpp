#include "cseries/async_xoverlapped_memory.hpp"

void __cdecl overlapped_free(void const* ptr)
{
	INVOKE(0x0066D4D0, overlapped_free, ptr);
}

void* __cdecl overlapped_malloc(uns32 size)
{
	//return INVOKE(0x0066D550, overlapped_malloc, size);

	return overlapped_malloc_tracked(size, nullptr, 0);
}

void* __cdecl overlapped_malloc_tracked(uns32 size, const char* file, int32 line)
{
	return INVOKE(0x0066D570, overlapped_malloc_tracked, size, file, line);
}

//.text:0066D620 ; c_allocation_base* __cdecl get_overlapped_allocation()

void __cdecl overlapped_memory_dispose()
{
	INVOKE(0x0066D630, overlapped_memory_dispose);
}

//.text:0066D650 ; void __cdecl overlapped_memory_dump(const char*)

void __cdecl overlapped_memory_initialize()
{
	INVOKE(0x0066D670, overlapped_memory_initialize);
}

