#include "text/font_package_cache.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "text/font_cache.hpp"
#include "text/font_loading.hpp"

HOOK_DECLARE(0x0065BB90, font_package_file_header_validate);
HOOK_DECLARE(0x0065C190, font_package_cache_delete);
HOOK_DECLARE(0x0065C4D0, font_package_cache_new);
HOOK_DECLARE(0x0065C590, font_package_clear);

void __cdecl font_package_entries_byteswap(s_font_package_entry* entry, long character_key)
{
	INVOKE(0x0065BB70, font_package_entries_byteswap, entry, character_key);
}

void __cdecl font_package_file_header_byteswap(s_font_package_file_header* package_header)
{
	INVOKE(0x0065BB80, font_package_file_header_byteswap, package_header);
}

bool __cdecl font_package_file_header_validate(s_font_package_file_header const* package_header)
{
	//return INVOKE(0x0065BB90, font_package_file_header_validate, package_header);

	ASSERT(package_header);

	bool valid = package_header->version == k_latest_font_package_header_version;
	if (!valid)
		GENERATE_EVENT(_event_error, "fonts: package header version mismatch 0x%08X != 0x%08X, maybe you need to get new fonts?",
			package_header->version,
			k_latest_font_package_header_version);

	valid &= package_header->font_count > 1 && package_header->font_count <= k_font_count && package_header->header_data_offset >= sizeof(s_font_package_file_header);
	valid &= package_header->header_data_offset + package_header->header_data_size <= package_header->package_table_offset;
	valid &= (package_header->package_table_count > 0 && package_header->package_table_count <= 65536)
		&& package_header->package_table_offset >= package_header->header_data_offset + package_header->header_data_size;

	for (long i = 0; valid && i < package_header->font_count; i++)
	{
		s_font_package_font const* font = &package_header->fonts[i];

		valid &= font->header_size >= sizeof(s_font_header);
		valid &= font->header_offset >= package_header->header_data_offset
			&& font->header_offset + font->header_size <= package_header->header_data_offset + package_header->header_data_size;
		valid &= font->package_table_index < sizeof(s_font_package_file)
			&& font->package_table_index + font->package_table_count <= package_header->package_table_count;

		for (long j = 0; j < i; j++)
		{
			valid &= package_header->fonts[j].header_offset + package_header->fonts[j].header_size <= font->header_offset
				&& package_header->fonts[j].package_table_index + package_header->fonts[j].package_table_count - 1 <= font->package_table_index;
		}
	}

	for (long k = 0; k < NUMBEROF(package_header->font_mapping); k++)
	{
		valid &= package_header->font_mapping[k] == NONE
			|| package_header->font_mapping[k] >= 0
			&& package_header->font_mapping[k] < package_header->font_count;
	}

	return valid;
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
	//INVOKE(0x0065C190, font_package_cache_delete);

	c_font_cache_scope_lock scope_lock;

	if (g_font_package_cache.initialized)
	{
		font_package_cache_flush();
		g_font_package_cache.initialized = 0;
	}
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
	//INVOKE(0x0065C4D0, font_package_cache_new);

	c_font_cache_scope_lock scope_lock;

	g_font_package_cache.time = 0;
	for (s_font_package_cache_entry& entry : g_font_package_cache.entries)
		font_package_clear(&entry);
	g_font_package_cache.initialized = 1;
}

//.text:0065C580 ; 

void __cdecl font_package_clear(s_font_package_cache_entry* entry)
{
	//INVOKE(0x0065C590, font_package_clear, entry);

	entry->package_index = NONE;
	entry->package_last_used_time = 0;
	entry->async_task = NONE;
	entry->async_task_bytes_read = 0;
	entry->async_task_complete = 0;
	entry->status = 0;
}

bool __cdecl font_package_do_work(bool blocking, s_font_package_cache_entry* entry)
{
	return INVOKE(0x0065C5D0, font_package_do_work, blocking, entry);
}

//.text:0065C6F0 ; e_font_package_status __cdecl font_package_get(long, c_flags<e_font_cache_flags, dword, 3>, dword, s_font_package const**)
//.text:0065C880 ; 

