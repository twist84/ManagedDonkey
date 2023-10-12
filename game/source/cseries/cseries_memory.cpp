#include "cseries/cseries_memory.hpp"

#include "cseries/cseries.hpp"

#include <string.h>

enum
{
	k_maximum_memmove_size = 0x20000000
};

void memmove_guarded(void* write_start, void const* source, unsigned int size, void const* bounds_lower, unsigned int bounds_size)
{
	ASSERT(size >= 0 && size <= k_maximum_memmove_size);

	if (size)
	{
		char* bounds_upper = (char*)bounds_lower + bounds_size - 1;
		char* write_end = (char*)write_start + size - 1;

		ASSERT(bounds_upper >= bounds_lower);
		ASSERT(bounds_size > 0);
		ASSERT(write_start >= bounds_lower && write_start <= bounds_upper);
		ASSERT(write_end >= bounds_lower && write_end <= bounds_upper);

		memmove(write_start, source, size);
	}
}
