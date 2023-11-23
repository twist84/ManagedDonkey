#include "cache/physical_memory_map.hpp"

#include "cseries/cseries.hpp"

void* __cdecl _physical_memory_malloc_fixed(long memory_stage, char const* name, long size, unsigned long flags)
{
	return INVOKE(0x0051D180, _physical_memory_malloc_fixed, memory_stage, name, size, flags);
}

unsigned long align_up(unsigned long value, long alignment_bits)
{
	return (value + (1 << alignment_bits) - 1) & (0xFFFFFFFF << alignment_bits);
}

