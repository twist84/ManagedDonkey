#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "multithreading/synchronized_value.hpp"
#include "shell/shell.hpp"
#include "tag_files/files.hpp"

enum e_font_index
{
	_font_index_none = -1
};

#pragma pack(push, 1)
struct s_font_loading_state
{
	c_static_string<k_tag_string_length> filename;

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
	//  ho: 0xC0000003
	// mcc: 0xC0000004
	dword version;

	//  ho: 16
	// mcc: 64
	long font_count;

	struct s_font // is this the actual name
	{
		long offset;

		// sizeof(s_font_header)
		long size;

		word __unknown8;
		word __unknownA;
	};
	static_assert(sizeof(s_font) == 0xC);

	c_static_array<s_font, 16> fonts;
	//c_static_array<s_font, 64> fonts; // mcc

	c_static_array<long, 16> font_index_mapping;
	//c_static_array<long, 64> font_index_mapping; // mcc

	long package_file_font_offset;
	long package_file_font_size;

	s_font_package_entry first_package_entry;
};
static_assert(sizeof(s_font_package_file_header) == 0x118); // 0x418 in mcc

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
static_assert(sizeof(s_font_character) == 0xC); // 0x10 in mcc
//static_assert(sizeof(s_font_character) == 0x10); // mcc

struct s_font_header
{
	//  ho: 0xF0000005
	// mcc: 0xF0000006
	dword version;

	c_static_string<k_tag_string_length> name;

	//  ho: 64
	// mcc: 512
	word __unknown24;

	//  ho: 64
	// mcc: 512
	word __unknown26;

	//  ho: 64
	// mcc: 512
	word __unknown28;

	//  ho: 256
	// mcc: 768
	word __unknown2A;

	// sizeof(s_font_header)
	long kerning_pairs_offset;

	// NUMBEROF(kerning_pairs)
	long kerning_pair_count;
	byte kerning_pairs[256];

	long location_table_offset;

	//  ho: 0x10000
	// mcc: 0x10000
	long location_table_count;

	//  ho: 0x10000
	// mcc: 0x10000
	dword __unknown13C;
	dword __unknown140;

	long character_data_size_bytes;

	//  ho: 0x402
	// mcc: 0x1802
	dword __unknown148;
	//qword __unknown148; // mcc

	//  ho: 0x2000
	// mcc: 0xC000
	dword __unknown14C;

	// sizeof(s_font_package_file)
	dword __unknown150;

	//  ho: 0x8000000
	// mcc: 0x8000000
	dword __unknown154;
	dword __unknown158;
};
static_assert(sizeof(s_font_header) == 0x15C);
//static_assert(sizeof(s_font_header) == 0x168); // in mcc

struct s_font_package_file
{
	s_font_package_file_header header;
	s_font_header font_headers[16];
	//s_font_header font_headers[64]; // mcc
	byte __data16D8[0x6928];
};
static_assert(sizeof(s_font_package_file) == 0x8000);
//static_assert(sizeof(s_font_package_file) == 0xC000); // mcc

struct s_font_globals
{
	bool initialized;
	bool load_font_from_hard_drive;
	bool emergency_mode;
	bool permanently_unavailable;
	c_enum<e_language, long, _language_invalid, k_language_count> language;
	long reload_retry_count;

	long __unknownC;

	//// mcc
	//long __unknown10;
	//long __unknown14;

	s_font_loading_state loading_state;
	s_font_package_file_header* font_package_header;
	s_font_package_file font_package;
};
static_assert(sizeof(s_font_globals) == 0x815C);
//static_assert(sizeof(s_font_globals) == 0xC170); // mcc

struct s_font_package_cache_entry
{
	long load_package_index;
	long __unknown4;
	long __unknown8;
	long __unknownC;
	long async_task;
	c_synchronized_long __unknown14;
	c_synchronized_long __unknown18;
	long status;

	s_font_package_file font_package_file;
};
static_assert(sizeof(s_font_package_cache_entry) == 0x8020);
//static_assert(sizeof(s_font_package_cache_entry) == 0xC020); // mcc

struct s_font_package_cache
{
	c_synchronized_long initialized;
	long __unknown4;

	c_static_array<s_font_package_cache_entry, 8> entries;

	//long __unknown60108; // mcc
};
static_assert(sizeof(s_font_package_cache) == 0x40108);
//static_assert(sizeof(s_font_package_cache) == 0xC0110); // mcc

extern s_font_globals& g_font_globals;
extern s_font_package_cache& g_font_package_cache;

extern char const*& k_hard_drive_font_directory;
extern char const*& k_dvd_font_directory;
extern char const* const& k_font_package_base_name;
extern char const* const& k_font_package_suffix;

enum e_language;

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
extern long __cdecl font_load_callback(void* callback_data);
extern void __cdecl font_loading_idle();
extern void __cdecl font_reload();
extern void __cdecl font_load_wrapper(bool load_blocking);
extern void __cdecl fonts_close();
extern void __cdecl fonts_copy_to_hard_drive();
extern void __cdecl font_invalidate_cached_fonts();
extern void __cdecl fonts_select_language();
extern void __cdecl get_font_master_filename(e_language language, char* buffer, long buffer_size);

