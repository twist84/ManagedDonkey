#include "cache/pc_texture_cache.hpp"

#include "cseries/cseries.hpp"

//.text:00618B30 ; bool __cdecl texture_cache_bitmap_new(long, bitmap_data*)
//.text:00618B70 ; void __cdecl texture_cache_block_for_one_frame(e_texture_cache_block_type)

void __cdecl texture_cache_close()
{
	INVOKE(0x00618B80, texture_cache_close);
}

void __cdecl texture_cache_close_pregame()
{
	INVOKE(0x00618B90, texture_cache_close_pregame);
}

bool __cdecl texture_cache_is_blocking()
{
	return INVOKE(0x00618BF0, texture_cache_is_blocking);
}

void __cdecl texture_cache_open()
{
	INVOKE(0x00618C20, texture_cache_open);
}

void __cdecl texture_cache_open_pregame()
{
	INVOKE(0x00618C30, texture_cache_open_pregame);
}

void __cdecl texture_cache_update_for_render()
{
	INVOKE(0x00618C60, texture_cache_update_for_render);
}

void __cdecl texture_cache_debug_render()
{
}

