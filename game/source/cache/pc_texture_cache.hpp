#pragma once

extern void __cdecl texture_cache_close();
extern void __cdecl texture_cache_close_pregame();
extern bool __cdecl texture_cache_is_blocking();
extern void __cdecl texture_cache_open();
extern void __cdecl texture_cache_open_pregame();
extern void __cdecl texture_cache_update_for_render();
extern void __cdecl texture_cache_debug_render();

