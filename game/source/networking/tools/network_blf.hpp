#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_options.hpp"
#include "memory/secure_signature.hpp"
#include "saved_games/saved_game_files.hpp"

#pragma pack(push, 1)
struct s_blf_header
{
public:
	void setup(int32 chunk_type, int32 chunk_size, int32 major_version, int32 minor_version);

	int32 chunk_type;
	int32 chunk_size;
	int16 major_version;
	int16 minor_version;
};
COMPILE_ASSERT(sizeof(s_blf_header) == 0xC);
#pragma pack(pop)

struct s_blf_chunk_start_of_file
{
public:
	static int32 const k_chunk_type = '_blf';
	static int32 const k_chunk_major_version = 1;
	static int32 const k_chunk_minor_version = 2;

	s_blf_chunk_start_of_file();

	void initialize();

	s_blf_header header;

	// BOM: https://en.wikipedia.org/wiki/Byte_order_mark
	// BOM: { UTF-16 (BE): FE FF }
	uns16 byte_order_mark;

	char file_type[32];

	char alignment_padding_unused[0x2];
};
COMPILE_ASSERT(sizeof(s_blf_chunk_start_of_file) == sizeof(s_blf_header) + 0x24);

#pragma pack(push, 1)

enum e_blf_file_authentication_type
{
	_blf_file_authentication_type_none = 0,
	_blf_file_authentication_type_crc,
	_blf_file_authentication_type_sha1,
	_blf_file_authentication_type_rsa,

	k_blf_file_authentication_type_count
};

struct s_blf_chunk_end_of_file
{
public:
	static int32 const k_chunk_type = '_eof';
	static int32 const k_chunk_major_version = 1;
	static int32 const k_chunk_minor_version = 1;
	static e_blf_file_authentication_type const k_authentication_type = _blf_file_authentication_type_none;

	s_blf_chunk_end_of_file();

	void initialize();

	s_blf_header header;

	int32 total_file_size;
	c_enum<e_blf_file_authentication_type, int8, _blf_file_authentication_type_none, k_blf_file_authentication_type_count> authentication_type;
};
COMPILE_ASSERT(sizeof(s_blf_chunk_end_of_file) == sizeof(s_blf_header) + 0x5);

struct s_blf_chunk_end_of_file_with_crc :
	s_blf_chunk_end_of_file
{
public:
	static e_blf_file_authentication_type const k_authentication_type = _blf_file_authentication_type_crc;

	s_blf_chunk_end_of_file_with_crc();

	void initialize();

	uns32 checksum;
};
COMPILE_ASSERT(sizeof(s_blf_chunk_end_of_file_with_crc) == 0x15);

struct s_blf_chunk_end_of_file_with_rsa :
	s_blf_chunk_end_of_file
{
	static e_blf_file_authentication_type const k_authentication_type = _blf_file_authentication_type_rsa;

	s_rsa_signature rsa_signature;
};
COMPILE_ASSERT(sizeof(s_blf_chunk_end_of_file_with_rsa) == 0x111);

struct s_blf_chunk_author
{
public:
	static int32 const k_chunk_type = 'athr';
	static int32 const k_chunk_major_version = 3;
	static int32 const k_chunk_minor_version = 1;

	s_blf_chunk_author();

	void initialize();

	s_blf_header header;

	char program_name[16];
	uns64 program_build_number;
	char program_build_string[28];
	char user[16];
};
COMPILE_ASSERT(sizeof(s_blf_chunk_author) == sizeof(s_blf_header) + 0x44);

#pragma pack(pop)

struct s_blf_chunk_content_header
{
public:
	static int32 const k_chunk_type = 'chdr';
	static int32 const k_chunk_major_version = 9;
	static int32 const k_chunk_minor_version = 1;

	s_blf_chunk_content_header();

	void initialize();
	void initialize_from_current_game_settings(e_controller_index controller_index, e_saved_game_file_type saved_game_file_type, wchar_t const* name, wchar_t const* description, uns64 size, int32 length_in_seconds);

	s_blf_header header;

	int16 build_number;
	int16 map_minor_version;

	s_saved_game_item_metadata metadata;
};
COMPILE_ASSERT(sizeof(s_blf_chunk_content_header) == sizeof(s_blf_header) + 0xFC);

struct s_blf_chunk_game_variant
{
public:
	static int32 const k_chunk_type = 'mpvr';
	static int32 const k_chunk_major_version = 3;
	static int32 const k_chunk_minor_version = 1;

	s_blf_chunk_game_variant();

	s_blf_header header;

	c_game_variant game_variant;
};
COMPILE_ASSERT(sizeof(s_blf_chunk_game_variant) == sizeof(s_blf_header) + 0x264);

struct s_blf_chunk_map_variant
{
public:
	static int32 const k_chunk_type = 'mapv';
	static int32 const k_chunk_major_version = 12;
	static int32 const k_chunk_minor_version = 1;

	s_blf_chunk_map_variant();

	s_blf_header header;
	byte pad[0x2];
	c_map_variant map_variant;
};
COMPILE_ASSERT(sizeof(s_blf_chunk_map_variant) == sizeof(s_blf_header) + 0xE094);

struct s_blffile_saved_game_file
{
public:
	s_blffile_saved_game_file();

	s_blf_chunk_start_of_file start_of_file;
	s_blf_chunk_content_header content_header;
};
COMPILE_ASSERT(sizeof(s_blffile_saved_game_file) == 0x138);

#pragma pack(push, 1)
struct s_blffile_game_variant :
	s_blffile_saved_game_file
{
public:
	s_blffile_game_variant();

	bool copy_to_and_validate(c_game_variant* game_variant, bool* out_was_valid) const;

	s_blf_chunk_game_variant variant;
	s_blf_chunk_end_of_file end_of_file;

	byte pad[0x3];
};
COMPILE_ASSERT(sizeof(s_blffile_game_variant) == 0x3BC);
#pragma pack(pop)

struct s_blffile_map_variant :
	public s_blffile_saved_game_file
{
public:
	s_blffile_map_variant();

	bool copy_to_and_validate(c_map_variant* map_variant, bool* out_was_valid) const;

	s_blf_chunk_map_variant variant;
	s_blf_chunk_end_of_file end_of_file;

	byte pad[0x7];
};
COMPILE_ASSERT(sizeof(s_blffile_map_variant) == 0xE1F0);

struct s_blf_saved_film :
	public s_blffile_saved_game_file
{
public:
	bool copy_to_and_validate_(game_options* options, int32* out_length_in_ticks, int32* out_start_tick, bool* out_was_valid);

#pragma pack(push, 4)
	struct s_blf_chunk_saved_film_header
	{
	public:
		static int32 const k_chunk_type = 'flmh';
		static int32 const k_chunk_major_version = 10;
		static int32 const k_chunk_minor_version = 2;

		s_blf_chunk_saved_film_header();

		struct s_saved_film_build_compatibility
		{
			s_saved_film_build_compatibility();

			char build_number[32];
			int32 executable_type;
			int32 network_executable_version;
			int32 network_compatible_version;
			e_language map_language;
			int32 map_minor_version;
			bool map_minor_version_is_tracked;
			byte pad1[0x3];
			int32 pad2[2];
			int32 map_signature_size;
			byte map_signature_bytes[0x3C];
		};
		COMPILE_ASSERT(sizeof(s_saved_film_build_compatibility) == 0x80);

		s_blf_header header;
		int8 pad0[4];
		s_saved_film_build_compatibility build_compatibility;
		bool is_host_film;
		bool contains_gamestate;
		bool is_snippet;
		int8 pad3[0x5];
		char session_id[128];
		game_options options;
		int32 recorded_time;
		int32 length_in_ticks;
		int32 snippet_start_tick;
		byte padding_to_align_for_utility_drive[0xD80];
	};
	COMPILE_ASSERT(sizeof(s_blf_chunk_saved_film_header) == sizeof(s_blf_header) + 0x259E0);
#pragma pack(pop)

	struct s_blf_chunk_saved_film_data
	{
	public:
		static int32 const k_chunk_type = 'flmd';
		static int32 const k_chunk_major_version = 1;
		static int32 const k_chunk_minor_version = 1;

		s_blf_chunk_saved_film_data();

		s_blf_header header;
	};
	COMPILE_ASSERT(sizeof(s_blf_chunk_saved_film_data) == sizeof(s_blf_header));

public:
	s_blf_saved_film();

	bool copy_to_and_validate(game_options* options, int32* out_length_in_ticks, int32* out_start_tick, bool* out_was_valid) const;

	s_blf_chunk_author author;
	s_blf_chunk_saved_film_header film_header;
	s_blf_chunk_saved_film_data film_data;
};
COMPILE_ASSERT(sizeof(s_blf_saved_film) == 0x25B80);

struct s_blf_chunk_campaign
{
public:
	static int32 const k_chunk_type = 'cmpn';
	static int32 const k_chunk_major_version = 1;
	static int32 const k_chunk_minor_version = 1;

	s_blf_chunk_campaign();

	s_blf_header header;

	e_campaign_id campaign_id;

	uns32 type_flags;

	wchar_t names[k_language_count][64];
	wchar_t descriptions[k_language_count][128];
	e_map_id map_ids[64];

	byte pad[0x4];
};
COMPILE_ASSERT(sizeof(s_blf_chunk_campaign) == sizeof(s_blf_header) + 0x130C);

enum e_scenario_insertion_flags
{
	_scenario_insertion_survival_bit = 0,
	_scenario_insertion_survival_always_unlocked_bit,
	_scenario_insertion_bit2,
	_scenario_insertion_return_from_map_bit,
};

struct s_scenario_insertion_point_halo3
{
	bool valid;

	uns8 flags;

	int16 zone_set;

	byte __pad4[0x4];

	wchar_t names[k_language_count][32];
	wchar_t descriptions[k_language_count][128];
};
COMPILE_ASSERT(sizeof(s_scenario_insertion_point_halo3) == 0xF08);

struct s_scenario_insertion_point_atlas
{
	bool valid;

	uns8 flags;

	int16 zone_set;

	// ODST
	e_map_id return_from_map_id;
	int32 survival_presence_context_id;

	byte __padC[0x4];

	wchar_t names[k_language_count][32];
	wchar_t descriptions[k_language_count][128];
};
COMPILE_ASSERT(sizeof(s_scenario_insertion_point_atlas) == 0xF10);

// Despite the fact that ODST added to this chunk neither version was updated
// Probably an oversight by Bungie?
struct s_blf_chunk_scenario
{
	static int32 const k_chunk_type = 'levl';
	static int32 const k_chunk_major_version = 3;
	static int32 const k_chunk_minor_version = 1;

	s_blf_chunk_scenario();
	
	s_blf_header header;

	e_map_id map_id;

	c_flags<e_level_flags, uns32, k_number_of_level_flags> flags;

	wchar_t name[k_language_count][32];
	wchar_t description[k_language_count][128];

	char image_file_base[k_tag_long_string_length];
	char scenario_file[k_tag_long_string_length];

	int32 presence_context_id;
	int32 sort_order;

	int8 mp_minimum_desired_players;
	int8 mp_maximum_desired_players;

	int8 maximum_teams[k_game_engine_type_count];

	bool allows_saved_films;

	byte __pad112A[0x6];
};
COMPILE_ASSERT(sizeof(s_blf_chunk_scenario) == sizeof(s_blf_header) + 0x1124);

template<typename insertion_struct, int32 insertion_count>
struct s_blf_chunk_scenario_minor_version :
	s_blf_chunk_scenario
{
public:
	s_blf_chunk_scenario_minor_version();

	insertion_struct insertions[insertion_count];
};

struct s_blf_chunk_scenario_halo3 :
	s_blf_chunk_scenario_minor_version<s_scenario_insertion_point_halo3, 4>
{
	s_blf_chunk_scenario_halo3();
};
COMPILE_ASSERT(sizeof(s_blf_chunk_scenario_halo3) == sizeof(s_blf_chunk_scenario) + (sizeof(s_scenario_insertion_point_halo3) * 4));

struct s_blf_chunk_scenario_atlas :
	s_blf_chunk_scenario_minor_version<s_scenario_insertion_point_atlas, 9>
{
	s_blf_chunk_scenario_atlas();
};
COMPILE_ASSERT(sizeof(s_blf_chunk_scenario_atlas) == sizeof(s_blf_chunk_scenario) + (sizeof(s_scenario_insertion_point_atlas) * 9));

struct s_map_image_data
{
	enum e_image_type
	{
		_image_type_jpeg = 0,
		_image_type_png,

		k_image_type_count,
	};

	e_image_type image_type;
	int32 image_data_bytes;
};
COMPILE_ASSERT(sizeof(s_map_image_data) == 0x8);

struct s_blf_chunk_map_image
{
public:
	static int32 const k_chunk_type = 'mapi';
	static int32 const k_chunk_major_version = 1;
	static int32 const k_chunk_minor_version = 1;

	s_blf_header header;
	s_map_image_data map_image_data;
};
COMPILE_ASSERT(sizeof(s_blf_chunk_map_image) == sizeof(s_blf_header) + sizeof(s_map_image_data));

extern bool __cdecl network_blf_verify_start_of_file(const char* buffer, int32 buffer_count, bool* out_byte_swap, int32* out_chunk_size);
extern bool __cdecl network_blf_find_chunk(const char* buffer, int32 buffer_count, bool must_byte_swap, int32 desired_chunk_type, int16 desired_version_major, int32* out_chunk_size, const char** out_found_chunk_data_size, int32* out_chunk_buffer_size, int16* out_version_minor, bool* out_eof_found);
extern bool __cdecl network_blf_read_for_known_chunk(const char* buffer, int32 buffer_count, bool byte_swap, int32 chunk_type, int16 major_version, int32* out_chunk_size, const char** out_chunk_buffer, int32* out_chunk_buffer_size, int16* out_minor_version, bool* out_eof_chunk);
extern bool __cdecl network_blf_verify_end_of_file(const char* buffer, int32 buffer_count, bool byte_swap, const char* eof_chunk_buffer, e_blf_file_authentication_type authentication_type);

template<typename t_blf_chunk_type>
void find_blf_chunk(s_file_reference* file, char* const file_buffer, const t_blf_chunk_type** out_blf_chunk_type, bool* must_byte_swap)
{
	*out_blf_chunk_type = nullptr;
	*must_byte_swap = false;

	bool file_added = false;
	uns32 error = 0;
	uns32 file_size = 0;
	int32 chunk_size = 0;
	const char* chunk_buffer = nullptr;
	bool eof_chunk = false;
	const t_blf_chunk_type* blf_chunk_type = nullptr;
	bool byte_swap = false;

	if (!file_open(file, FLAG(_file_open_flag_desired_access_read), &error))
	{
		//c_console::write_line("blf: failed to open file");
		file_close(file);
		goto function_end;
	}

	if (!file_get_size(file, &file_size))
	{
		//c_console::write_line("blf: failed to get file size of file");
		goto function_finish;
	}

	if (!file_read(file, file_size, false, file_buffer))
	{
		//c_console::write_line("blf: failed to read file");
		goto function_finish;
	}

	if (!network_blf_verify_start_of_file(file_buffer, file_size, &byte_swap, &chunk_size))
	{
		//c_console::write_line("blf: failed to verify start of file");
		goto function_finish;
	}

	if (!network_blf_find_chunk(file_buffer, file_size, byte_swap, t_blf_chunk_type::k_chunk_type, t_blf_chunk_type::k_chunk_major_version, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
	{
		//c_console::write_line("blf: failed to find chunk");
		goto function_finish;
	}

	if (chunk_buffer)
	{
		blf_chunk_type = (const t_blf_chunk_type*)(chunk_buffer - sizeof(s_blf_header));
		if (chunk_buffer != (const char*)sizeof(s_blf_header) && network_blf_find_chunk(file_buffer, file_size, byte_swap, s_blf_chunk_end_of_file::k_chunk_type, s_blf_chunk_end_of_file::k_chunk_major_version, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
		{
			if (chunk_size == sizeof(s_blf_chunk_end_of_file) && network_blf_verify_end_of_file(file_buffer, file_size, byte_swap, chunk_buffer - sizeof(s_blf_header), s_blf_chunk_end_of_file::k_authentication_type))
			{
				file_added = true;
			}
			else if (chunk_size == sizeof(s_blf_chunk_end_of_file_with_crc) && network_blf_verify_end_of_file(file_buffer, file_size, byte_swap, chunk_buffer - sizeof(s_blf_header), s_blf_chunk_end_of_file_with_crc::k_authentication_type))
			{
				file_added = true;
			}
			else if (chunk_size == sizeof(s_blf_chunk_end_of_file_with_rsa) && network_blf_verify_end_of_file(file_buffer, file_size, byte_swap, chunk_buffer - sizeof(s_blf_header), s_blf_chunk_end_of_file_with_rsa::k_authentication_type))
			{
				file_added = true;
			}

			if (file_added)
				goto function_finish;

			//c_console::write_line("blf: failed to verify end of file chunk");
		}
	}

function_finish:
	if (file_added)
	{
		*out_blf_chunk_type = blf_chunk_type;
		*must_byte_swap = byte_swap;
		file_close(file);
	}
function_end:

	if (!file_added)
	{
		// $TODO file_reference_get_name
		//c_console::write_line("blf: failed to add file '%s'", file->path);
	}
}

