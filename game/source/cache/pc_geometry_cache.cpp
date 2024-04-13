#include "cache/pc_geometry_cache.hpp"

#include "cseries/cseries.hpp"

void __cdecl geometry_cache_close()
{
	INVOKE(0x00618CC0, geometry_cache_close);
}

long __cdecl geometry_cache_get_structure_block_pending_count()
{
	return INVOKE(0x00618D20, geometry_cache_get_structure_block_pending_count);
}

bool __cdecl geometry_cache_is_blocking()
{
	return INVOKE(0x00618D50, geometry_cache_is_blocking);
}

void __cdecl geometry_cache_open()
{
	INVOKE(0x00618D90, geometry_cache_open);
}

void __cdecl geometry_cache_update_for_render()
{
	INVOKE(0x00618DC0, geometry_cache_update_for_render);
}

void __cdecl geometry_cache_debug_render()
{
}

