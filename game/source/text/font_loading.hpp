#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

enum e_font_index
{
	_font_index_none = -1
};

#pragma pack(push, 1)
struct s_font_loading_state
{
	char filename[32];

	long font_index;

	bool font_file_loaded;
	s_file_reference font_file;
	byte : 8;
	byte : 8;
	byte : 8;

	long async_task;

	c_synchronized_long started;
	c_synchronized_long failed;
	c_synchronized_long finished;
};
static_assert(sizeof(s_font_loading_state) == 0x148);
#pragma pack(pop)

struct s_font_package_entry
{
	long first_character_key;
	long last_character_key;
};
static_assert(sizeof(s_font_package_entry) == 0x8);

struct s_font_package_file_header
{
	// 0xC0000003
	dword version;

	// 16
	long font_count;

	struct
	{
		long offset;

		// 0x15C
		long size;

		word __unknown8;
		word __unknownA;
	} fonts[16];

	long font_index_mapping[16];

	long package_file_font_offset;
	long package_file_font_size;

	s_font_package_entry first_package_entry;
};
static_assert(sizeof(s_font_package_file_header) == 0x118);

struct s_font_character
{
	// 256
	word __unknown0;

	// 8192
	short packed_size;

	// 256
	short height;

	// 64
	short width;

	// 256
	word __unknown8;

	// 64
	word __unknownA;
};
static_assert(sizeof(s_font_character) == 0xC);

struct s_font_header
{
	// 0xF0000005
	dword version;

	string name;

	word __unknown24;
	word __unknown26;
	word __unknown28;
	word __unknown2A;

	// sizeof(s_font_header)
	long kerning_pairs_offset;
	long kerning_pair_count;
	byte kerning_pairs[256];

	long location_table_offset;
	long location_table_count;

	dword __unknown13C;
	dword __unknown140;

	long character_data_size_bytes;

	dword __unknown148;
	dword __unknown14C;
	dword __unknown150;
	dword __unknown154;
	dword __unknown158;
};
static_assert(sizeof(s_font_header) == 0x15C);

struct s_font_package_file
{
	s_font_package_file_header header;
	s_font_header font_headers[16];
	byte __data16D8[0x6928];
};
static_assert(sizeof(s_font_package_file) == 0x8000);

struct s_font_globals
{
	bool initialized;
	bool load_font_from_hard_drive;
	bool emergency_mode;
	bool permanently_unavailable;
	dword language;
	long reload_retry_count;

	dword __unknownC;

	s_font_loading_state loading_state;
	s_font_package_file_header* font_package_header;
	s_font_package_file font_package;
};
static_assert(sizeof(s_font_globals) == 0x815C);

extern s_font_globals& g_font_globals;

extern char const* const& k_hard_drive_font_directory;
extern char const* const& k_dvd_font_directory;
extern char const* const& k_font_package_base_name;
extern char const* const& k_font_package_suffix;


extern void __cdecl font_block_until_load_completes(s_font_loading_state* loading_state);
extern void __cdecl fonts_close_internal(s_font_loading_state* loading_state);
extern void __cdecl font_dispose();
extern char const* __cdecl font_get_debug_name(long font_index);
extern long __cdecl font_get_font_index(long font_id);
extern s_font_header const* __cdecl font_get_header(long font_id);
extern s_font_header const* __cdecl font_get_loaded_header(long font_index);
extern bool __cdecl font_get_package_file_handle(s_file_handle* out_file_handle);
extern s_font_package_file_header const* __cdecl font_get_package_header_internal();
extern void __cdecl font_idle();
extern bool __cdecl font_in_emergency_mode();
extern void __cdecl font_initialize();
extern void __cdecl font_initialize_emergency();
extern void __cdecl font_load(s_font_loading_state* loading_state, long font_index, char const* filename, bool load_blocking);
extern long __cdecl font_load_callback(void* userdata);
extern void __cdecl font_loading_idle();
extern void __cdecl font_reload();
extern void __cdecl font_load_wrapper(bool load_blocking);
extern void __cdecl fonts_close();
extern void __cdecl fonts_copy_to_hard_drive();
extern void __cdecl font_invalidate_cached_fonts();
extern void __cdecl fonts_select_language();
extern void __cdecl get_font_master_filename(dword language, char* buffer, long buffer_size);
