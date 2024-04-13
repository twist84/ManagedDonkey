#pragma once

extern void __cdecl geometry_cache_close();
extern long __cdecl geometry_cache_get_structure_block_pending_count();
extern bool __cdecl geometry_cache_is_blocking();
extern void __cdecl geometry_cache_open();
extern void __cdecl geometry_cache_update_for_render();
extern void __cdecl geometry_cache_debug_render();

