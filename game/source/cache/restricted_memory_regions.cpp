#include "cache/restricted_memory_regions.hpp"

#include "cseries/cseries.hpp"

void __cdecl restricted_region_unlock_primary(long index)
{
	INVOKE(0x005A05B0, restricted_region_unlock_primary, index);
}

