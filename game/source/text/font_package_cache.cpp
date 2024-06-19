#include "text/font_package_cache.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "text/font_loading.hpp"

HOOK_DECLARE(0x0065BB90, font_package_file_header_validate);

//.text:0065BB70 ; void __cdecl font_package_entries_byteswap(s_font_package_entry* entry, long)
//.text:0065BB80 ; void __cdecl font_package_file_header_byteswap(s_font_package_file_header* package_header)

bool __cdecl font_package_file_header_validate(s_font_package_file_header const* package_header)
{
	//return INVOKE(0x0065BB90, font_package_file_header_validate, package_header);

	ASSERT(package_header);

	bool valid = package_header->version == 0xC0000003;
	if (!valid)
		generate_event(_event_level_error, "fonts: package header version mismatch 0x%08X != 0x%08X, maybe you need to get new fonts?",
			package_header->version,
			0xC0000003);
	
	valid &= package_header->font_count > 1 && package_header->font_count <= NUMBEROF(package_header->fonts) && package_header->package_file_font_offset >= sizeof(s_font_package_file_header);
	valid &= package_header->package_file_font_offset + package_header->package_file_font_size <= package_header->first_package_entry.first_character_key;
	valid &= (package_header->first_package_entry.last_character_key > 0 && package_header->first_package_entry.last_character_key <= 0x10000)
		&& package_header->first_package_entry.first_character_key >= package_header->package_file_font_offset + package_header->package_file_font_size;
	
	for (long i = 0; valid && i < package_header->font_count; i++)
	{
		s_font_package_file_header::s_font const* font = &package_header->fonts[i];
	
		valid &= font->size >= sizeof(s_font_header);
		valid &= font->offset >= package_header->package_file_font_offset
			&& font->offset + font->size <= package_header->package_file_font_offset + package_header->package_file_font_size;
		valid &= font->__unknown8 < 0x8000
			&& font->__unknown8 + font->__unknownA <= package_header->first_package_entry.last_character_key;
	
		for (long j = 0; j < i; j++)
		{
			valid &= package_header->fonts[j].offset + package_header->fonts[j].size <= font->offset
				&& package_header->fonts[j].__unknown8 + package_header->fonts[j].__unknownA - 1 <= font->__unknown8;
		}
	}
	
	for (long k = 0; k < NUMBEROF(package_header->font_index_mapping); k++)
	{
		valid &= package_header->font_index_mapping[k] == NONE
			|| package_header->font_index_mapping[k] >= 0
			&& package_header->font_index_mapping[k] < package_header->font_count;
	}
	
	return valid;

	//s_font_package_file_header_mcc const* package_header_mcc = reinterpret_cast<s_font_package_file_header_mcc const*>(package_header);
	//
	//bool valid = package_header_mcc->version == 0xC0000004;
	//if (!valid)
	//	generate_event(_event_level_error, "fonts: package header version mismatch 0x%08X != 0x%08X, maybe you need to get new fonts?",
	//		package_header_mcc->version,
	//		0xC0000004);
	//
	//valid &= package_header_mcc->font_count > 1 && package_header_mcc->font_count <= NUMBEROF(package_header_mcc->fonts) && package_header_mcc->package_file_font_offset >= sizeof(s_font_package_file_header_mcc);
	//valid &= package_header_mcc->package_file_font_offset + package_header_mcc->package_file_font_size <= package_header_mcc->first_package_entry.first_character_key;
	//valid &= (package_header_mcc->first_package_entry.last_character_key > 0 && package_header_mcc->first_package_entry.last_character_key <= 0x10000)
	//	&& package_header_mcc->first_package_entry.first_character_key >= package_header_mcc->package_file_font_offset + package_header_mcc->package_file_font_size;
	//
	//for (long i = 0; valid && i < package_header_mcc->font_count; i++)
	//{
	//	s_font_package_file_header_mcc::s_font const* font = &package_header_mcc->fonts[i];
	//
	//	valid &= font->size >= sizeof(s_font_header);
	//	valid &= font->offset >= package_header_mcc->package_file_font_offset
	//		&& font->offset + font->size <= package_header_mcc->package_file_font_offset + package_header_mcc->package_file_font_size;
	//	valid &= font->__unknown8 < 0x8000
	//		&& font->__unknown8 + font->__unknownA <= package_header_mcc->first_package_entry.last_character_key;
	//
	//	for (long j = 0; j < i; j++)
	//	{
	//		valid &= package_header_mcc->fonts[j].offset + package_header_mcc->fonts[j].size <= font->offset
	//			&& package_header_mcc->fonts[j].__unknown8 + package_header_mcc->fonts[j].__unknownA - 1 <= font->__unknown8;
	//	}
	//}
	//
	//for (long k = 0; k < NUMBEROF(package_header_mcc->font_index_mapping); k++)
	//{
	//	valid &= package_header_mcc->font_index_mapping[k] == NONE
	//		|| package_header_mcc->font_index_mapping[k] >= 0
	//		&& package_header_mcc->font_index_mapping[k] < package_header_mcc->font_count;
	//}
	//
	//return valid;
}

//.text:0065BD20 ; s_font_character const* __cdecl font_package_get_character(s_font_package const* font_package, dword)
//.text:0065BDA0 ; 
//.text:0065BDB0 ; 
//.text:0065BE40 ; long __cdecl font_package_table_find_character(s_font_package_file_header const* package_header, dword)
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

