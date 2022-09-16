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

	dword font_index;

	bool font_file_loaded;
	s_file_reference font_file;
	byte : 8;
	byte : 8;
	byte : 8;

	dword async_task;

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
		dword offset;

		// 0x15C
		dword size;

		word __unknown8;
		word __unknownA;
	} fonts[16];

	byte __dataC8[0x40];

	dword package_file_font_offset;
	dword package_file_font_size;

	s_font_package_entry first_package_entry;
};
static_assert(sizeof(s_font_package_file_header) == 0x118);

struct s_font_character
{
	// 256
	word __unknown0;

	// 8192
	word packed_size;

	// 256
	word height;

	// 64
	word width;

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
	dword kerning_pairs_offset;
	dword kerning_pair_count;
	byte kerning_pairs[256];

	dword location_table_offset;
	dword location_table_count;

	dword __unknown13C;
	dword __unknown140;

	dword character_data_size_bytes;

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
	c_enum<e_language, dword, k_language_count> language;
	dword reload_retry_count;

	dword __unknownC;

	s_font_loading_state loading_state;
	s_font_package_file_header* font_package_header;
	s_font_package_file font_package;
};
static_assert(sizeof(s_font_globals) == 0x815C);

extern s_font_globals& g_font_globals;