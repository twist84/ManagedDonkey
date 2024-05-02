#pragma once

enum e_texture_cache_block_type
{
	_texture_cache_block_type_unknown0,
	_texture_cache_block_type_unknown1,

	k_texture_cache_block_type_count
};

extern void __cdecl texture_cache_block_for_one_frame(e_texture_cache_block_type block_type);
extern void __cdecl texture_cache_close();
extern void __cdecl texture_cache_close_pregame();
extern bool __cdecl texture_cache_is_blocking();
extern void __cdecl texture_cache_open();
extern void __cdecl texture_cache_open_pregame();
extern void __cdecl texture_cache_update_for_render();
extern void __cdecl texture_cache_debug_render();

