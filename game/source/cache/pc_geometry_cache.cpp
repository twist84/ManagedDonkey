#include "cache/pc_geometry_cache.hpp"

#include "cseries/cseries.hpp"

bool __cdecl geometry_cache_is_blocking()
{
	return INVOKE(0x00618D50, geometry_cache_is_blocking);

	return false;
}

void __cdecl geometry_cache_debug_render()
{
}

