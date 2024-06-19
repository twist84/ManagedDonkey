#pragma once

struct s_font_package_entry;
struct s_font_package_file_header;
struct s_font_package_cache_entry;

extern void __cdecl font_package_entries_byteswap(s_font_package_entry* entry, long character_key);
extern void __cdecl font_package_file_header_byteswap(s_font_package_file_header* package_header);
extern bool __cdecl font_package_file_header_validate(s_font_package_file_header const* package_header);
extern void __cdecl font_package_cache_delete();
extern void __cdecl font_package_cache_flush();
extern void __cdecl font_package_cache_idle();
extern void __cdecl font_package_cache_new();
extern void __cdecl font_package_clear(s_font_package_cache_entry* entry);
extern bool __cdecl font_package_do_work(bool blocking, s_font_package_cache_entry* entry);

