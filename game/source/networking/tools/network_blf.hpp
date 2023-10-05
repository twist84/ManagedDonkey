#pragma once

#include "cseries/cseries.hpp"
#include "game/game_options.hpp"
#include "memory/secure_signature.hpp"
#include "saved_games/saved_game_files.hpp"

#pragma pack(push, 1)
struct s_blf_header
{
public:
	void setup(long chunk_type, long chunk_size, long major_version, long minor_version);

	long chunk_type;
	long chunk_size;
	short major_version;
	short minor_version;
};
static_assert(sizeof(s_blf_header) == 0xC);
#pragma pack(pop)

struct s_blf_chunk_start_of_file
{
public:
	static long const k_chunk_type = '_blf';
	static long const k_version_major = 1;
	static long const k_version_minor = 2;

	s_blf_chunk_start_of_file();

	void initialize();

	s_blf_header header;

	// BOM: https://en.wikipedia.org/wiki/Byte_order_mark
	// BOM: { UTF-16 (BE): FE FF }
	word byte_order_mark;

	c_static_string<k_tag_string_length> name;

	byte pad[2];
};
static_assert(sizeof(s_blf_chunk_start_of_file) == sizeof(s_blf_header) + 0x24);

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
	static long const k_chunk_type = '_eof';
	static long const k_version_major = 1;
	static long const k_version_minor = 1;
	static e_blf_file_authentication_type const k_authentication_type = _blf_file_authentication_type_none;

	s_blf_chunk_end_of_file();

	void initialize();

	s_blf_header header;

	long file_size;
	c_enum<e_blf_file_authentication_type, char, _blf_file_authentication_type_none, k_blf_file_authentication_type_count> authentication_type;
};
static_assert(sizeof(s_blf_chunk_end_of_file) == sizeof(s_blf_header) + 0x5);

struct s_blf_chunk_end_of_file_with_crc : s_blf_chunk_end_of_file
{
public:
	static e_blf_file_authentication_type const k_authentication_type = _blf_file_authentication_type_crc;

	s_blf_chunk_end_of_file_with_crc();

	void initialize();

	dword checksum;
};
static_assert(sizeof(s_blf_chunk_end_of_file_with_crc) == 0x15);

struct s_blf_chunk_end_of_file_with_rsa : s_blf_chunk_end_of_file
{
	static e_blf_file_authentication_type const k_authentication_type = _blf_file_authentication_type_rsa;

	s_rsa_signature rsa_signature;
};
static_assert(sizeof(s_blf_chunk_end_of_file_with_rsa) == 0x111);

struct s_blf_chunk_author
{
public:
	static long const k_chunk_type = 'athr';
	static long const k_version_major = 3;
	static long const k_version_minor = 1;

	s_blf_chunk_author();

	void initialize();

	s_blf_header header;

	c_static_string<16> build_name;
	qword build_identifier;
	c_static_string<28> build_string;
	c_static_string<16> author_name;
};
static_assert(sizeof(s_blf_chunk_author) == sizeof(s_blf_header) + 0x44);

#pragma pack(pop)

struct s_blf_chunk_content_header
{
public:
	static long const k_chunk_type = 'chdr';
	static long const k_version_major = 9;
	static long const k_version_minor = 1;

	s_blf_chunk_content_header();

	void initialize();

	s_blf_header header;

	short build_number;
	short map_minor_version;

	s_content_item_metadata metadata;
};
static_assert(sizeof(s_blf_chunk_content_header) == sizeof(s_blf_header) + 0xFC);

struct s_blf_chunk_game_variant
{
public:
	static long const k_chunk_type = 'mpvr';
	static long const k_version_major = 3;
	static long const k_version_minor = 1;

	s_blf_chunk_game_variant();

	s_blf_header header;

	c_game_variant game_variant;
};
static_assert(sizeof(s_blf_chunk_game_variant) == sizeof(s_blf_header) + 0x264);

struct s_blf_chunk_map_variant
{
public:
	static long const k_chunk_type = 'mapv';
	static long const k_version_major = 12;
	static long const k_version_minor = 1;

	s_blf_chunk_map_variant();

	s_blf_header header;
	short : 16;
	c_map_variant map_variant;
};
static_assert(sizeof(s_blf_chunk_map_variant) == sizeof(s_blf_header) + 0xE094);

struct s_blffile_saved_game_file
{
public:
	s_blffile_saved_game_file();

	s_blf_chunk_start_of_file start_of_file_chunk;
	s_blf_chunk_content_header content_header_chunk;
};
static_assert(sizeof(s_blffile_saved_game_file) == 0x138);

struct s_blffile_game_variant : s_blffile_saved_game_file
{
public:
	s_blffile_game_variant();

	s_blf_chunk_game_variant game_variant_chunk;
	s_blf_chunk_end_of_file end_of_file_chunk;

	byte pad[3];
};
static_assert(sizeof(s_blffile_game_variant) == 0x3BC);

struct s_blffile_map_variant : s_blffile_saved_game_file
{
public:
	s_blffile_map_variant();

	bool copy_to_and_validate(c_map_variant* map_variant, bool* is_valid) const;

	s_blf_chunk_map_variant map_variant_chunk;
	s_blf_chunk_end_of_file end_of_file_chunk;

	byte pad[7];
};
static_assert(sizeof(s_blffile_map_variant) == 0xE1F0);

struct s_blf_saved_film : s_blffile_saved_game_file, s_blf_chunk_author
{
#pragma pack(push, 4)
	struct s_blf_chunk_saved_film_header_delta // 08172.07.03.08.2240.delta
	{
	public:
		static long const k_chunk_type = 'flmh';
		static long const k_version_major = 2;
		static long const k_version_minor = 1;

		s_blf_header header;
		dword __unknownC; // alignment

		// build_string.set(version_get_build_string());
		c_static_string<k_tag_string_length> build_string;

		// network_get_build_identifiers(&executable_type, &executable_version, &compatible_version)
		long executable_type;
		long executable_version;
		long compatible_version;

		// map_language = get_map_language();
		long map_language;

		// map_minor_version = get_map_minor_version();
		long map_minor_version;

		// tracked = get_map_minor_version_is_tracked();
		dword tracked;                 // bool

		// valid_and_authoritative = game_options_valid() && game_is_authoritative();
		dword valid_and_authoritative; // bool

		dword __unknown4C; // padding

		// sessionid.set(netdebug_get_sessionid());
		c_static_string<128> sessionid;

		// sizeof(game_options) == 0xD300
		byte options[0xD300];

		// start_ticks = system_seconds();
		long start_ticks;

		// length_in_ticks = 0;
		long length_in_ticks;
	};
	static_assert(sizeof(s_blf_chunk_saved_film_header_delta) == sizeof(s_blf_header) + 0xD3CC);

	struct s_blf_chunk_saved_film_header // same as delta but more
	{
	public:
		static long const k_chunk_type = 'flmh';
		static long const k_version_major = 10;
		static long const k_version_minor = 2;

		s_blf_chunk_saved_film_header();

		s_blf_header header;

		byte __dataC[0x10C];
		game_options options;
		byte __data[0xD8C];
	};
	static_assert(sizeof(s_blf_chunk_saved_film_header) == sizeof(s_blf_header) + 0x259E0);
#pragma pack(pop)

	struct s_blf_chunk_saved_film_data
	{
	public:
		static long const k_chunk_type = 'flmd';
		static long const k_version_major = 1;
		static long const k_version_minor = 1;

		s_blf_chunk_saved_film_data();

		s_blf_header header;
	};
	static_assert(sizeof(s_blf_chunk_saved_film_data) == sizeof(s_blf_header));

public:
	s_blf_saved_film();

	bool copy_to_and_validate(c_game_variant* game_variant, c_map_variant* map_variant, bool* is_valid) const;

	s_blf_chunk_saved_film_header film_header;
	s_blf_chunk_saved_film_data film_data;
};
static_assert(sizeof(s_blf_saved_film) == 0x25B80);

struct s_blf_chunk_campaign
{
public:
	static long const k_chunk_type = 'cmpn';
	static long const k_version_major = 1;
	static long const k_version_minor = 1;

	s_blf_chunk_campaign();

	s_blf_header header;

	long campaign_id;

	dword_flags type_flags;

	c_static_wchar_string<64> names[12];
	c_static_wchar_string<128> descriptions[12];
	c_static_array<long, 64> map_ids;

	long : 32;
};
static_assert(sizeof(s_blf_chunk_campaign) == sizeof(s_blf_header) + 0x130C);

struct s_blf_chunk_scenario_insertion
{
	bool visible;

	byte __unknown1;

	short zone_set_index;

	dword __unknown4;

	c_static_array<c_static_wchar_string<32>, 12> names;
	c_static_array<c_static_wchar_string<128>, 12> descriptions;

	dword __unknownF08;
	dword __unknownF0C;
};
static_assert(sizeof(s_blf_chunk_scenario_insertion) == 0xF10);

enum e_scenario_type_flags
{
	_scenario_type_flag_ui_bit = 4,
	_scenario_type_flag_atlas_bit,
	_scenario_type_flag_multi_bit,
	_scenario_type_flag_dlc_bit,
	_scenario_type_flag_test_bit,
	_scenario_type_flag_temp_bit,
};

struct s_blf_chunk_scenario
{
public:
	static long const k_chunk_type = 'levl';
	static long const k_version_major = 3;
	static long const k_version_minor = 2;

	s_blf_chunk_scenario();

	s_blf_header header;

	long map_id;

	c_flags<e_scenario_type_flags, dword, _scenario_type_flag_temp_bit + 1> type_flags;

	c_static_wchar_string<32> names[12];
	c_static_wchar_string<128> descriptions[12];

	c_static_string<k_tag_long_string_length> image_file_base;
	c_static_string<k_tag_long_string_length> scenario_path;

	long precense_context_id;
	long sort_order;

	char multiplayer_minimum_desired_players;
	char multiplayer_maximum_desired_players;

	char engine_maximum_teams[11];

	bool allows_saved_films;

	short : 16;
	long : 32;

	s_blf_chunk_scenario_insertion insertions[9];
};
static_assert(sizeof(s_blf_chunk_scenario) == sizeof(s_blf_header) + 0x98B4);

enum e_map_image_type
{
	_map_image_type_jpg = 0,
	_map_image_type_png,

	k_map_image_type_count
};

struct s_blf_chunk_map_image
{
public:
	static long const k_chunk_type = 'mapi';
	static long const k_version_major = 1;
	static long const k_version_minor = 1;

	s_blf_header header;

	byte type;
	long buffer_size;

#pragma warning(push)
#pragma warning(disable : 4200)
	char buffer[];
#pragma warning(pop)
};
static_assert(sizeof(s_blf_chunk_map_image) == sizeof(s_blf_header) + 0x8);

extern bool __cdecl network_blf_verify_start_of_file(char const* buffer, long buffer_count, bool* out_byte_swap, long* out_chunk_size);
extern bool __cdecl network_blf_find_chunk(char const* buffer, long buffer_count, bool byte_swap, long chunk_type, short major_version, long* out_chunk_size, char const** out_chunk_buffer, long* chunk_buffer_size, short* out_minor_version, bool* out_eof_chunk);
extern bool __cdecl network_blf_read_for_known_chunk(char const* buffer, long buffer_count, bool byte_swap, long chunk_type, short major_version, long* out_chunk_size, char const** out_chunk_buffer, long* out_chunk_buffer_size, short* out_minor_version, bool* out_eof_chunk);
extern bool __cdecl network_blf_verify_end_of_file(char const* buffer, long buffer_count, bool byte_swap, char const* eof_chunk_buffer, e_blf_file_authentication_type authentication_type);

template<typename t_blf_chunk_type>
void find_blf_chunk(s_file_reference* file, char* const file_buffer, t_blf_chunk_type const** out_blf_chunk_type, bool* must_byte_swap)
{
	*out_blf_chunk_type = nullptr;
	*must_byte_swap = false;

	bool file_added = false;
	dword error = 0;
	dword file_size = 0;
	long chunk_size = 0;
	char const* chunk_buffer = nullptr;
	bool eof_chunk = false;
	t_blf_chunk_type const* blf_chunk_type = nullptr;
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

	if (!network_blf_find_chunk(file_buffer, file_size, byte_swap, t_blf_chunk_type::k_chunk_type, t_blf_chunk_type::k_version_major, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
	{
		//c_console::write_line("blf: failed to find chunk");
		goto function_finish;
	}

	if (chunk_buffer)
	{
		blf_chunk_type = reinterpret_cast<t_blf_chunk_type const*>(chunk_buffer - sizeof(s_blf_header));
		if (chunk_buffer != (char const*)sizeof(s_blf_header) && network_blf_find_chunk(file_buffer, file_size, byte_swap, s_blf_chunk_end_of_file::k_chunk_type, s_blf_chunk_end_of_file::k_version_major, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
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
		// #TODO: file_reference_get_name
		//c_console::write_line("blf: failed to add file '%s'", file->path);
	}
}

