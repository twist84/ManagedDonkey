#include "text/font_package_cache.hpp"

//.text:0065BB70 ; void __cdecl font_package_entries_byteswap(s_font_package_entry* entry, long)
//.text:0065BB80 ; void __cdecl font_package_file_header_byteswap(s_font_package_file_header* header)
//.text:0065BB90 ; bool __cdecl font_package_file_header_validate(s_font_package_file_header const* header)
//.text:0065BD20 ; s_font_character const* __cdecl font_package_get_character(s_font_package const* font_package, dword)
//.text:0065BDA0 ; 
//.text:0065BDB0 ; 
//.text:0065BE40 ; long __cdecl font_package_table_find_character(s_font_package_file_header const* header, dword)
//.text:0065BE70 ; 
//.text:0065C080 ; 
//.text:0065C0F0 ; 
//.text:0065C110 ; 
//.text:0065C140 ; 
//.text:0065C160 ; 
//.text:0065C180 ; 

void __cdecl font_package_cache_delete()
{
	INVOKE(0x0065C190, font_package_cache_delete);
}

void __cdecl font_package_cache_flush()
{
	INVOKE(0x0065C200, font_package_cache_flush);
}

void __cdecl font_package_cache_idle()
{
	INVOKE(0x0065C370, font_package_cache_idle);
}

void __cdecl font_package_cache_new()
{
	INVOKE(0x0065C4D0, font_package_cache_new);
}

//.text:0065C580 ; 
//.text:0065C590 ; 
//.text:0065C5D0 ; bool __cdecl font_package_do_work(bool blocking, s_font_package_cache_entry* entry)
//.text:0065C6F0 ; e_font_package_status __cdecl font_package_get(long, c_flags<e_font_cache_flags, dword, 3>, dword, s_font_package const**)
//.text:0065C880 ; 

