#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

struct s_font_loading_task;

#pragma pack(push, 1)
struct s_font_loading_state
{
	char debug_filename[32];
	e_font_index font_index;
	bool file_open;
	s_file_reference file_reference;
	byte pad0[0x3];
	int32 task_id;
	c_synchronized_long started;
	c_synchronized_long failed;
	c_synchronized_long finished;
};
COMPILE_ASSERT(sizeof(s_font_loading_state) == 0x148);
#pragma pack(pop)

struct s_kerning_pair
{
	uns8 second_character;
	int8 offset;
};
COMPILE_ASSERT(sizeof(s_kerning_pair) == 0x2);

struct s_font_package_entry
{
	uns32 first_character_key;
	uns32 last_character_key;
};
COMPILE_ASSERT(sizeof(s_font_package_entry) == 0x8);

struct s_font_package_font
{
	int32 header_offset;
	int32 header_size;
	int16 package_table_index;
	int16 package_table_count;
};
COMPILE_ASSERT(sizeof(s_font_package_font) == 0xC);

struct s_font_package_file_header
{
	int32 version;
	int32 font_count;
	s_font_package_font fonts[k_font_package_maximum_fonts];
	int32 font_mapping[k_font_package_maximum_fonts];
	int32 header_data_offset;
	int32 header_data_size;
	int32 package_table_offset;
	int32 package_table_count;
};
COMPILE_ASSERT(sizeof(s_font_package_file_header) == 0x118);

struct s_font_character
{
	int16 character_width;
	uns16 packed_size;
	int16 bitmap_height;
	int16 bitmap_width;
	int16 initial_offset;
	int16 bitmap_origin_y;
};
COMPILE_ASSERT(sizeof(s_font_character) == 0xC);

struct s_font_header
{
	int32 version;
	char debug_name[32];
	int16 ascending_height;
	int16 descending_height;
	int16 leading_height;
	int16 leading_width;
	int32 kerning_pairs_offset;
	int32 kerning_pair_count;
	uns8 character_first_kerning_pair_index[k_font_maximum_number_of_kerning_pairs + 1];
	int32 location_table_offset;
	int32 location_table_count;
	int32 character_count;
	int32 character_data_offset;
	int32 character_data_size_bytes;
	uns32 no_such_character_data_location;
	int32 maximum_packed_pixel_size_bytes;
	int32 maximum_unpacked_pixel_size_bytes;
	int32 total_packed_pixel_size_bytes;
	int32 total_unpacked_pixel_size_bytes;
};
COMPILE_ASSERT(sizeof(s_font_header) == 0x15C);

struct s_font_package_file
{
	s_font_package_file_header header;
	s_font_header font_headers[k_font_package_maximum_fonts];
	byte data[0x6928];
};
COMPILE_ASSERT(sizeof(s_font_package_file) == k_font_package_size);

struct s_font_globals
{
	bool initialized;
	bool cached_to_hard_drive;
	bool emergency_mode;
	bool fonts_unavailable;
	c_enum<e_language, int32, _language_invalid, k_language_count> language;
	int32 failure_retry_count;
	c_synchronized_long async_error;
	s_font_loading_state package_loading_state;
	const s_font_package_file_header* font_package_header;

	//uns8 header_storage[k_font_package_size];
	s_font_package_file package_file;
};
COMPILE_ASSERT(sizeof(s_font_globals) == 0x815C);

struct s_font_package_cache_entry
{
	int32 package_index;
	int32 package_allocation_time;
	int32 package_loaded_time;
	int32 package_last_used_time;
	int32 async_task;
	c_synchronized_long async_task_bytes_read;
	c_synchronized_long async_task_complete;
	e_font_package_status status;
	byte package[k_font_package_size];
};
COMPILE_ASSERT(sizeof(s_font_package_cache_entry) == 0x8020);

struct s_font_package_cache
{
	c_synchronized_long initialized;
	int32 time;
	s_font_package_cache_entry entries[k_font_package_entry_count];
	//s_status_line status_lines[k_package_cache_status_line_count];
};
COMPILE_ASSERT(sizeof(s_font_package_cache) == 0x40108);

extern s_font_globals& g_font_globals;
extern s_font_package_cache& g_font_package_cache;

extern const char*& k_hard_drive_font_directory;
extern const char*& k_dvd_font_directory;
extern const char* const& k_font_package_base_name;
extern const char* const& k_font_package_suffix;

struct s_async_task;

extern void __cdecl font_block_until_load_completes(s_font_loading_state* loading_state);
extern void __cdecl font_close_loaded_file(s_font_loading_state* loading_state);
extern void __cdecl font_dispose();
extern const char* __cdecl font_get_debug_name(e_font_index internal_index);
extern e_font_index __cdecl font_get_font_index(e_font_id font);
extern const s_font_header* __cdecl font_get_header(e_font_id font);
extern const s_font_header* __cdecl font_get_header_internal(e_font_index internal_index);
extern bool __cdecl font_get_package_file_handle(s_file_handle* out_file_handle);
extern const s_font_package_file_header* __cdecl font_get_package_header_internal();
extern void __cdecl font_idle();
extern bool __cdecl font_in_emergency_mode();
extern void __cdecl font_initialize();
extern void __cdecl font_initialize_emergency();
extern void __cdecl font_load(s_font_loading_state* loading_state, e_font_index font_index, const char* filename, bool load_blocking);
extern e_async_completion __cdecl font_load_callback(s_font_loading_task* task);
extern void __cdecl font_load_idle(s_font_loading_state* loading_state, bool* out_failure_reported);
extern void __cdecl font_loading_idle();
extern void __cdecl font_reload();
extern int32 __cdecl font_table_get_font_file_references(const char* text, const s_file_reference* directory, s_file_reference* references, int32 max_references, int32* font_id_mapping, int32 max_font_ids);
extern bool __cdecl fonts_begin_loading(bool load_blocking);
extern void __cdecl fonts_close();
extern void __cdecl fonts_copy_to_hard_drive();
extern void __cdecl fonts_invalidate_cached_fonts();
extern void __cdecl fonts_select_language();
extern void __cdecl get_active_font_directory(s_file_reference* file);
extern void __cdecl get_dvd_font_directory(s_file_reference* file);
extern void __cdecl get_font_master_filename(e_language language, char* buffer, int32 buffer_size);
extern void __cdecl get_hard_drive_font_directory(s_file_reference* file);

