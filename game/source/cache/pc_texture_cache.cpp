#include "cache/pc_texture_cache.hpp"

#include "cseries/cseries.hpp"

bool __cdecl texture_cache_is_blocking()
{
	//return INVOKE(0x00618BF0, texture_cache_is_blocking);

	return false;
}

void __cdecl texture_cache_open()
{
	INVOKE(0x00618C20, texture_cache_open);
}

void __cdecl texture_cache_debug_render()
{
}

