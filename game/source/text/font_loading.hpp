#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "multithreading/synchronized_value.hpp"
#include "shell/shell.hpp"
#include "tag_files/files.hpp"

enum
{
	k_latest_font_package_header_version = 0xC0000003,
	
	k_font_package_file_size = 0x8000,
	k_font_package_entry_count = 8,
	k_font_count = 16,

	k_latest_font_header_version = 0xF0000005,
	k_font_header_kerning_pair_index_count = 256,
};

enum e_font_index
{
	_font_index_none = -1,
	_font_index_fallback = -2,

	k_maximum_font_index_count = 10
};

enum
{
	_package_cache_status_line_package_status = 0,
	_package_cache_status_line_load_stats,

	k_package_cache_status_line_count
};

enum e_font_package_status
{
	_font_package_unavailable = 0,
	_font_package_delayed_loading,
	_font_package_loading,
	_font_package_ready,

	k_font_package_status_count
};

#pragma pack(push, 1)
struct s_font_loading_state
{
	char debug_filename[32];
	e_font_index font_index;
	bool file_open;
	s_file_reference file_reference;
	byte pad0[0x3];
	long task_id;
	c_synchronized_long started;
	c_synchronized_long failed;
	c_synchronized_long finished;
};
static_assert(sizeof(s_font_loading_state) == 0x148);
#pragma pack(pop)

struct s_kerning_pair
{
	byte second_character;
	char offset;
};
static_assert(sizeof(s_kerning_pair) == 0x2);

struct s_font_package_entry
{
	dword first_character_key;
	dword last_character_key;
};
static_assert(sizeof(s_font_package_entry) == 0x8);

struct s_font_package_font
{
	long header_offset;
	long header_size;
	short package_table_index;
	short package_table_count;
};
static_assert(sizeof(s_font_package_font) == 0xC);

struct s_font_package_file_header
{
	long version;
	long font_count;
	s_font_package_font fonts[k_font_count];
	long font_mapping[k_font_count];
	long header_data_offset;
	long header_data_size;
	long package_table_offset;
	long package_table_count;
};
static_assert(sizeof(s_font_package_file_header) == 0x118);

struct s_font_character
{
	short character_width;
	word packed_size;
	short bitmap_height;
	short bitmap_width;
	short initial_offset;
	short bitmap_origin_y;
};
static_assert(sizeof(s_font_character) == 0xC);

struct s_font_header
{
	long version;
	char debug_name[32];
	short ascending_height;
	short descending_height;
	short leading_height;
	short leading_width;
	long kerning_pairs_offset;
	long kerning_pair_count;
	byte character_first_kerning_pair_index[k_font_header_kerning_pair_index_count];
	long location_table_offset;
	long location_table_count;
	long character_count;
	long character_data_offset;
	long character_data_size_bytes;
	dword no_such_character_data_location;
	long maximum_packed_pixel_size_bytes;
	long maximum_unpacked_pixel_size_bytes;
	long total_packed_pixel_size_bytes;
	long total_unpacked_pixel_size_bytes;
};
static_assert(sizeof(s_font_header) == 0x15C);

struct s_font_package_file
{
	s_font_package_file_header header;
	s_font_header font_headers[k_font_count];
	byte data[0x6928];
};
static_assert(sizeof(s_font_package_file) == k_font_package_file_size);

struct s_font_globals
{
	bool initialized;
	bool cached_to_hard_drive;
	bool emergency_mode;
	bool fonts_unavailable;
	c_enum<e_language, long, _language_invalid, k_language_count> language;
	long failure_retry_count;
	c_synchronized_long async_error;
	s_font_loading_state package_loading_state;
	s_font_package_file_header const* font_package_header;

	//byte header_storage[k_font_package_file_size];
	s_font_package_file package_file;
};
static_assert(sizeof(s_font_globals) == 0x815C);

struct s_font_package_cache_entry
{
	long package_index;
	long package_allocation_time;
	long package_loaded_time;
	long package_last_used_time;
	long async_task;
	c_synchronized_long async_task_bytes_read;
	c_synchronized_long async_task_complete;
	e_font_package_status status;
	byte package[k_font_package_file_size];
};
static_assert(sizeof(s_font_package_cache_entry) == 0x8020);

struct s_font_package_cache
{
	c_synchronized_long initialized;
	long time;
	s_font_package_cache_entry entries[k_font_package_entry_count];
	//s_status_line status_lines[k_package_cache_status_line_count];
};
static_assert(sizeof(s_font_package_cache) == 0x40108);

extern s_font_globals& g_font_globals;
extern s_font_package_cache& g_font_package_cache;

extern char const*& k_hard_drive_font_directory;
extern char const*& k_dvd_font_directory;
extern char const* const& k_font_package_base_name;
extern char const* const& k_font_package_suffix;

enum e_async_completion;
struct s_async_task;

enum e_language;

extern void __cdecl font_block_until_load_completes(s_font_loading_state* loading_state);
extern void __cdecl font_close_loaded_file(s_font_loading_state* loading_state);
extern void __cdecl font_dispose();
extern char const* __cdecl font_get_debug_name(e_font_index internal_index);
extern e_font_index __cdecl font_get_font_index(e_font_id font);
extern s_font_header const* __cdecl font_get_header(e_font_id font);
extern s_font_header const* __cdecl font_get_header_internal(e_font_index internal_index);
extern bool __cdecl font_get_package_file_handle(s_file_handle* out_file_handle);
extern s_font_package_file_header const* __cdecl font_get_package_header_internal();
extern void __cdecl font_idle();
extern bool __cdecl font_in_emergency_mode();
extern void __cdecl font_initialize();
extern void __cdecl font_initialize_emergency();
extern void __cdecl font_load(s_font_loading_state* loading_state, e_font_index font_index, char const* filename, bool load_blocking);
extern e_async_completion __cdecl font_load_callback(s_async_task* task);
extern void __cdecl font_load_idle(s_font_loading_state* loading_state, bool* out_failure_reported);
extern void __cdecl font_loading_idle();
extern void __cdecl font_reload();
extern long __cdecl font_table_get_font_file_references(char const* text, s_file_reference const* directory, s_file_reference* references, long max_references, long* font_id_mapping, long max_font_ids);
extern bool __cdecl fonts_begin_loading(bool load_blocking);
extern void __cdecl fonts_close();
extern void __cdecl fonts_copy_to_hard_drive();
extern void __cdecl fonts_invalidate_cached_fonts();
extern void __cdecl fonts_select_language();
extern void __cdecl get_active_font_directory(s_file_reference* file);
extern void __cdecl get_dvd_font_directory(s_file_reference* file);
extern void __cdecl get_font_master_filename(e_language language, char* buffer, long buffer_size);
extern void __cdecl get_hard_drive_font_directory(s_file_reference* file);

