#pragma once

struct s_font_package_file_header;

extern bool __cdecl font_package_file_header_validate(s_font_package_file_header const* package_header);
extern void __cdecl font_package_cache_delete();
extern void __cdecl font_package_cache_flush();
extern void __cdecl font_package_cache_idle();
extern void __cdecl font_package_cache_new();

