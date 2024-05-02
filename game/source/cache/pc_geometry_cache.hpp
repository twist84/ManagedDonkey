#pragma once

enum e_geometry_block_type
{
	_geometry_block_type_unknown0,
	_geometry_block_type_unknown1,

	k_geometry_block_type_count
};

extern void __cdecl geometry_cache_block_for_one_frame(e_geometry_block_type block_type);
extern void __cdecl geometry_cache_close();
extern long __cdecl geometry_cache_get_structure_block_pending_count();
extern bool __cdecl geometry_cache_is_blocking();
extern void __cdecl geometry_cache_open();
extern void __cdecl geometry_cache_update_for_render();
extern void __cdecl geometry_cache_debug_render();

