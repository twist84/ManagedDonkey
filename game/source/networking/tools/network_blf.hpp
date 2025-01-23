#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_options.hpp"
#include "memory/secure_signature.hpp"
#include "saved_games/saved_game_files.hpp"
#include "shell/shell.hpp"

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

struct s_blf_chunk_end_of_file_with_crc :
	s_blf_chunk_end_of_file
{
public:
	static e_blf_file_authentication_type const k_authentication_type = _blf_file_authentication_type_crc;

	s_blf_chunk_end_of_file_with_crc();

	void initialize();

	dword checksum;
};
static_assert(sizeof(s_blf_chunk_end_of_file_with_crc) == 0x15);

struct s_blf_chunk_end_of_file_with_rsa :
	s_blf_chunk_end_of_file
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

	s_saved_game_item_metadata metadata;
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
	byte pad[0x2];
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

#pragma pack(push, 1)
struct s_blffile_game_variant :
	s_blffile_saved_game_file
{
public:
	s_blffile_game_variant();

	s_blf_chunk_game_variant game_variant_chunk;
	s_blf_chunk_end_of_file end_of_file_chunk;

	byte pad[0x3];
};
static_assert(sizeof(s_blffile_game_variant) == 0x3BC);
#pragma pack(pop)

struct s_blffile_map_variant :
	s_blffile_saved_game_file
{
public:
	s_blffile_map_variant();

	bool copy_to_and_validate(c_map_variant* map_variant, bool* is_valid) const;

	s_blf_chunk_map_variant map_variant_chunk;
	s_blf_chunk_end_of_file end_of_file_chunk;

	byte pad[0x7];
};
static_assert(sizeof(s_blffile_map_variant) == 0xE1F0);

struct s_blf_saved_film :
	s_blffile_saved_game_file,
	s_blf_chunk_author
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

		struct s_saved_film_build_compatibility
		{
			s_saved_film_build_compatibility();

			c_static_string<k_tag_string_length> build_number;
			long executable_type;
			long network_executable_version;
			long network_compatible_version;
			e_language map_language;
			long map_minor_version;
			bool map_minor_version_is_tracked;
			byte pad1[0x3];
			dword pad2[2];
			long map_signature_size;
			byte map_signature_bytes[0x3C];
		};
		static_assert(sizeof(s_saved_film_build_compatibility) == 0x80);

		s_blf_header header;
		byte pad0[4];
		s_saved_film_build_compatibility build_compatibility;
		bool is_host_film;
		bool contains_gamestate;
		bool is_snippet;
		byte pad3[0x5];
		c_static_string<128> session_id;
		game_options options;
		long recorded_time;
		long length_in_ticks;
		long snippet_start_tick;
		byte padding_to_align_for_utility_drive[0xD80];
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

	wchar_t names[k_language_count][64];
	wchar_t descriptions[k_language_count][128];
	long map_ids[64];

	byte pad[0x4];
};
static_assert(sizeof(s_blf_chunk_campaign) == sizeof(s_blf_header) + 0x130C);

enum e_scenario_insertion_flags
{
	_scenario_insertion_survival_bit = 0,
	_scenario_insertion_survival_always_unlocked_bit,
	_scenario_insertion_bit2,
	_scenario_insertion_return_from_map_bit,
};

struct s_scenario_insertion_point_halo3
{
	bool visible;

	byte_flags flags;

	short zone_set;

	byte __pad4[0x4];

	wchar_t names[k_language_count][32];
	wchar_t descriptions[k_language_count][128];
};
static_assert(sizeof(s_scenario_insertion_point_halo3) == 0xF08);

struct s_scenario_insertion_point_atlas
{
	bool visible;

	byte_flags flags;

	short zone_set;

	// ODST
	long return_from_map_id;
	long survival_presence_context_id;

	byte __padC[0x4];

	wchar_t names[k_language_count][32];
	wchar_t descriptions[k_language_count][128];
};
static_assert(sizeof(s_scenario_insertion_point_atlas) == 0xF10);

// Despite the fact that ODST added to this chunk neither version was updated
// Probably an oversight by Bungie?
struct s_blf_chunk_scenario
{
	static long const k_chunk_type = 'levl';
	static long const k_version_major = 3;
	static long const k_version_minor = 1;

	s_blf_chunk_scenario();
	
	s_blf_header header;

	long map_id;

	c_flags<e_level_flags, dword, k_number_of_level_flags> flags;

	wchar_t name[k_language_count][32];
	wchar_t description[k_language_count][128];

	char image_file_base[k_tag_long_string_length];
	char scenario_file[k_tag_long_string_length];

	long presence_context_id;
	long sort_order;

	char mp_minimum_desired_players;
	char mp_maximum_desired_players;

	char maximum_teams[k_game_engine_type_count];

	bool allows_saved_films;

	byte __pad112A[0x6];
};
static_assert(sizeof(s_blf_chunk_scenario) == sizeof(s_blf_header) + 0x1124);

template<typename insertion_struct, long insertion_count>
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
static_assert(sizeof(s_blf_chunk_scenario_halo3) == sizeof(s_blf_chunk_scenario) + (sizeof(s_scenario_insertion_point_halo3) * 4));

struct s_blf_chunk_scenario_atlas :
	s_blf_chunk_scenario_minor_version<s_scenario_insertion_point_atlas, 9>
{
	s_blf_chunk_scenario_atlas();
};
static_assert(sizeof(s_blf_chunk_scenario_atlas) == sizeof(s_blf_chunk_scenario) + (sizeof(s_scenario_insertion_point_atlas) * 9));

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
	__pragma(warning(disable : 4200)) char buffer[];
};
static_assert(sizeof(s_blf_chunk_map_image) == sizeof(s_blf_header) + 0x8);

extern bool __cdecl network_blf_verify_start_of_file(char const* buffer, long buffer_count, bool* out_byte_swap, long* out_chunk_size);
extern bool __cdecl network_blf_find_chunk(char const* buffer, long buffer_count, bool must_byte_swap, long desired_chunk_type, short desired_version_major, long* out_chunk_size, char const** out_found_chunk_data_size, long* out_chunk_buffer_size, short* out_version_minor, bool* out_eof_found);
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
		// $TODO: file_reference_get_name
		//c_console::write_line("blf: failed to add file '%s'", file->path);
	}
}

