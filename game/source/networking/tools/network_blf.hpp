#pragma once

#include "cseries/cseries.hpp"
#include "game/game_options.hpp"
#include "memory/secure_signature.hpp"
#include "saved_games/saved_game_files.hpp"

struct s_blf_header
{
public:
	void setup(long signature, long chunk_size, long major_version, long minor_version);

	tag signature;
	dword chunk_size;
	word major_version;
	word minor_version;
};
static_assert(sizeof(s_blf_header) == 0xC);

struct s_blf_chunk_start_of_file : s_blf_header
{
public:
	s_blf_chunk_start_of_file();

	void initialize();

	// BOM: https://en.wikipedia.org/wiki/Byte_order_mark
	// BOM: { UTF-16 (BE): FE FF }
	word byte_order_mark;

	string name;

	byte pad[2];
};
static_assert(sizeof(s_blf_chunk_start_of_file) == 0x30);

#pragma pack(push, 1)

enum e_blf_file_authentication_type
{
	_blf_file_authentication_type_none = 0,
	_blf_file_authentication_type_crc,
	_blf_file_authentication_type_sha1,
	_blf_file_authentication_type_rsa,

	k_blf_file_authentication_type_count
};

struct s_blf_chunk_end_of_file : s_blf_header
{
public:
	s_blf_chunk_end_of_file();

	void initialize();

	long file_size;
	c_enum<e_blf_file_authentication_type, char, k_blf_file_authentication_type_count> authentication_type;
};
static_assert(sizeof(s_blf_chunk_end_of_file) == 0x11);

struct s_blf_chunk_end_of_file_with_crc : s_blf_chunk_end_of_file
{
public:
	s_blf_chunk_end_of_file_with_crc();

	void initialize();

	dword checksum;
};
static_assert(sizeof(s_blf_chunk_end_of_file_with_crc) == 0x15);

struct s_blf_chunk_end_of_file_with_rsa : s_blf_chunk_end_of_file
{
	s_rsa_signature rsa_signature;
};
static_assert(sizeof(s_blf_chunk_end_of_file_with_rsa) == 0x111);

struct s_blf_chunk_author : s_blf_header
{
public:
	s_blf_chunk_author();

	void initialize();

	c_static_string<16> build_name;
	qword build_identifier;
	c_static_string<28> build_string;
	c_static_string<16> author_name;
};
static_assert(sizeof(s_blf_chunk_author) == 0x50);

#pragma pack(pop)

struct s_blf_chunk_content_header : s_blf_header
{
public:
	s_blf_chunk_content_header();

	void initialize();

	short build_number;
	short map_minor_version;

	s_saved_game_item_metadata metadata;
};
static_assert(sizeof(s_blf_chunk_content_header) == 0x108);

struct s_blf_chunk_game_variant : s_blf_header
{
public:
	s_blf_chunk_game_variant();

	c_game_variant game_variant;
};
static_assert(sizeof(s_blf_chunk_game_variant) == 0x270);

struct s_blf_chunk_map_variant : s_blf_header
{
public:
	s_blf_chunk_map_variant();

	c_map_variant map_variant;
	byte pad[4];
};
static_assert(sizeof(s_blf_chunk_map_variant) == 0xE0A0);

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

	s_blf_chunk_map_variant map_variant_chunk;
	s_blf_chunk_end_of_file end_of_file_chunk;

	byte pad[7];
};
static_assert(sizeof(s_blffile_map_variant) == 0xE1F0);

struct s_blf_chunk_campaign : s_blf_header
{
public:
	s_blf_chunk_campaign();

	long campaign_id;

	dword_flags type_flags;

	c_static_wchar_string<64> names[12];
	c_static_wchar_string<128> descriptions[12];

	long map_ids[64];

	long : 32;
};
static_assert(sizeof(s_blf_chunk_campaign) == 0x1318);

struct s_blf_chunk_scenario_insertion
{
	bool visible;

	byte __unknown1;

	short zone_set_index;

	dword __unknown4;

	c_static_wchar_string<32> names[12];
	c_static_wchar_string<128> descriptions[12];

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

struct s_blf_chunk_scenario : s_blf_header
{
public:
	s_blf_chunk_scenario();

	long map_id;

	c_flags<e_scenario_type_flags, dword, _scenario_type_flag_temp_bit + 1> type_flags;

	c_static_wchar_string<32> names[12];
	c_static_wchar_string<128> descriptions[12];

	c_static_string<256> image_file_base;
	c_static_string<256> scenario_path;

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
static_assert(sizeof(s_blf_chunk_scenario) == 0x98C0);

extern bool network_blf_verify_start_of_file(char const* buffer, long buffer_size, bool* out_byte_swap, long* out_chunk_size);
extern bool network_blf_find_chunk(char const* buffer, long buffer_size, bool byte_swap, long signature, short major_version, long* out_chunk_size, char const** out_chunk_buffer, long* chunk_buffer_size, short* out_minor_version, bool* out_eof_chunk);
extern bool network_blf_verify_end_of_file(char const* buffer, long buffer_size, bool byte_swap, char const* eof_chunk_buffer, e_blf_file_authentication_type authentication_type);