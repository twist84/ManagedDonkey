#include "text/font_package_cache.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "text/font_cache.hpp"
#include "text/font_loading.hpp"

HOOK_DECLARE(0x0065BB90, font_package_file_header_validate);
HOOK_DECLARE(0x0065C190, font_package_cache_delete);
HOOK_DECLARE(0x0065C4D0, font_package_cache_new);
HOOK_DECLARE(0x0065C590, font_package_clear);

//.text:0065B820 ; int32 __cdecl font_pack_character_pixels(int32, const uns16*, int32, void*)
//.text:0065BB00 ; void __cdecl font_package_character_byteswap(s_font_package_character*)
//.text:0065BB10 ; bool __cdecl font_package_character_validate(const s_font_package*, const s_font_package_character*)

void __cdecl font_package_entries_byteswap(s_font_package_entry* entry, int32 character_key)
{
	//INVOKE(0x0065BB70, font_package_entries_byteswap, entry, character_key);
}

void __cdecl font_package_file_header_byteswap(s_font_package_file_header* package_header)
{
	//INVOKE(0x0065BB80, font_package_file_header_byteswap, package_header);
}

bool __cdecl font_package_file_header_validate(const s_font_package_file_header* package_header)
{
	//return INVOKE(0x0065BB90, font_package_file_header_validate, package_header);

	ASSERT(package_header);

	bool valid = package_header->version == k_font_package_version;
	if (!valid)
	{
		event(_event_error, "fonts: package header version mismatch 0x%08X != 0x%08X, maybe you need to get new fonts?",
			package_header->version,
			k_font_package_version);
	}

	valid = valid && IN_RANGE_INCLUSIVE(package_header->font_count, 2, k_font_package_maximum_fonts);
	valid = valid && package_header->header_data_offset >= sizeof(s_font_package_file_header);
	valid = valid && package_header->header_data_offset + package_header->header_data_size <= package_header->package_table_offset;
	valid = valid && IN_RANGE_INCLUSIVE(package_header->package_table_count, 1, k_font_package_maximum_packages);
	valid = valid && package_header->package_table_offset >= package_header->header_data_offset + package_header->header_data_size;

	for (int32 font_index = 0; valid && font_index < package_header->font_count; font_index++)
	{
		const s_font_package_font* font = &package_header->fonts[font_index];

		valid = valid && font->header_size >= sizeof(s_font_header);
		valid = valid && font->header_offset >= package_header->header_data_offset;
		valid = valid && font->header_offset + font->header_size <= package_header->header_data_offset + package_header->header_data_size;
		valid = valid && font->package_table_index < k_font_package_size;
		valid = valid && font->package_table_index + font->package_table_count <= package_header->package_table_count;

		for (int32 test_font_index = 0; test_font_index < font_index; test_font_index++)
		{
			const s_font_package_font* test_font = &package_header->fonts[test_font_index];

			valid = valid && test_font->header_offset + test_font->header_size <= font->header_offset;
			valid = valid && test_font->package_table_index + test_font->package_table_count - 1 <= font->package_table_index;
		}
	}

	for (int32 font_mapping_index = 0; font_mapping_index < NUMBEROF(package_header->font_mapping); font_mapping_index++)
	{
		int32 font_index = package_header->font_mapping[font_mapping_index];
		valid = valid && (font_index == NONE || VALID_INDEX(font_index, package_header->font_count));
	}

	return valid;
}

//.text:0065BD20 ; const s_font_character* __cdecl font_package_get_character(const s_font_package* font_package, uns32)
//.text:0065BDA0 ; void __cdecl font_package_header_byteswap(s_font_package*)
//.text:0065BDB0 ; bool __cdecl font_package_header_validate(const s_font_package*)
//.text:0065BE40 ; int32 __cdecl font_package_table_find_character(const s_font_package_file_header* package_header, uns32)
//.text:0065BE70 ; int32 __cdecl font_unpack_character_pixels(int32, const void*, int32, uns16*)
//.text:0065C080 ; 
//.text:0065C0F0 ; int __cdecl package_table_search_function(const void*, const void*, const void*)
//.text:0065C110 ; public: __cdecl s_font_package_cache::s_font_package_cache()
//.text:0065C140 ; public: __cdecl s_font_package_cache_entry::s_font_package_cache_entry()
//.text:0065C160 ; public: __cdecl s_font_package_cache::~s_font_package_cache()
//.text:0065C180 ; public: __cdecl s_font_package_cache_entry::~s_font_package_cache_entry()

void __cdecl font_package_cache_delete()
{
	//INVOKE(0x0065C190, font_package_cache_delete);

	c_font_cache_scope_lock scope_lock;

	if (g_font_package_cache.initialized)
	{
		font_package_cache_flush();
		g_font_package_cache.initialized = false;
	}
}

void __cdecl font_package_cache_flush()
{
	INVOKE(0x0065C200, font_package_cache_flush);

#if 0
	c_font_cache_scope_lock scope_lock;
	for (int32 entry_index = 0; entry_index < k_font_package_entry_count; entry_index++)
	{
		s_font_package_cache_entry* entry = &g_font_package_cache.entries[entry_index];
		if (entry->async_task != INVALID_ASYNC_TASK_ID)
		{
			font_package_do_work(true, entry);
			ASSERT(entry->async_task == INVALID_ASYNC_TASK_ID);
		}
		entry->package_index = NONE;
		entry->status = _font_package_unavailable;
	}
#endif
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
	{
		font_package_clear(&entry);
	}
	g_font_package_cache.initialized = true;
}

//.text:0065C580 ; void __cdecl font_package_cache_update_status_lines()

void __cdecl font_package_clear(s_font_package_cache_entry* entry)
{
	//INVOKE(0x0065C590, font_package_clear, entry);

	entry->package_index = NONE;
	entry->package_last_used_time = 0L;
	entry->async_task = INVALID_ASYNC_TASK_ID;
	entry->async_task_bytes_read = 0L;
	entry->async_task_complete = false;
	entry->status = _font_package_unavailable;
}

bool __cdecl font_package_do_work(bool block, s_font_package_cache_entry* entry)
{
	return INVOKE(0x0065C5D0, font_package_do_work, block, entry);
}

//.text:0065C6F0 ; e_font_package_status __cdecl font_package_get(int32, c_flags<e_font_cache_flags, uns32, 3>, uns32, const s_font_package**)
//.text:0065C880 ; bool __cdecl font_package_make_ready(s_font_package*)

