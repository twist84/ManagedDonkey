#pragma once

#include "cseries/cseries.hpp"
#include "game/game_options.hpp"
#include "saved_games/saved_game_files.hpp"

struct s_blf_header
{
public:
	void setup(long signature, long chunk_size, long major_version, long minor_version);

protected:
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

protected:
	word byte_order_marker;
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

protected:
	long file_size;
	c_enum<e_blf_file_authentication_type, char, k_blf_file_authentication_type_count> authentication_type;
};
static_assert(sizeof(s_blf_chunk_end_of_file) == 0x11);

struct s_blf_chunk_end_of_file_with_crc : s_blf_chunk_end_of_file
{
public:
	s_blf_chunk_end_of_file_with_crc();

	void initialize();

protected:
	dword checksum;
};
static_assert(sizeof(s_blf_chunk_end_of_file_with_crc) == 0x15);

struct s_blf_chunk_author : s_blf_header
{
public:
	s_blf_chunk_author();

	void initialize();

protected:
	char build_name[16];
	qword build_identifier;
	char build_string[28];
	char author_name[16];
};
static_assert(sizeof(s_blf_chunk_author) == 0x50);

#pragma pack(pop)

struct s_blf_chunk_content_header : s_blf_header
{
public:
	s_blf_chunk_content_header();

	void initialize();

protected:

	short build_number;
	short map_minor_version;

	s_saved_game_item_metadata metadata;
};
static_assert(sizeof(s_blf_chunk_content_header) == 0x108);

struct s_blf_chunk_game_variant : s_blf_header
{
public:
	s_blf_chunk_game_variant();

protected:
	c_game_variant game_variant;
};
static_assert(sizeof(s_blf_chunk_game_variant) == 0x270);

struct s_blf_chunk_map_variant : s_blf_header
{
public:
	s_blf_chunk_map_variant();

protected:
	c_map_variant map_variant;
	byte pad[4];
};
static_assert(sizeof(s_blf_chunk_map_variant) == 0xE0A0);

struct s_blffile_saved_game_file
{
public:
	s_blffile_saved_game_file();

protected:
	s_blf_chunk_start_of_file start_of_file_chunk;
	s_blf_chunk_content_header content_header_chunk;
};
static_assert(sizeof(s_blffile_saved_game_file) == 0x138);

struct s_blffile_game_variant : s_blffile_saved_game_file
{
public:
	s_blffile_game_variant();

protected:
	s_blf_chunk_game_variant game_variant_chunk;
	s_blf_chunk_end_of_file end_of_file_chunk;

	byte pad[3];
};
static_assert(sizeof(s_blffile_game_variant) == 0x3BC);

struct s_blffile_map_variant : s_blffile_saved_game_file
{
public:
	s_blffile_map_variant();

protected:
	s_blf_chunk_map_variant map_variant_chunk;
	s_blf_chunk_end_of_file end_of_file_chunk;

	byte pad[7];
};
static_assert(sizeof(s_blffile_map_variant) == 0xE1F0);

struct s_blf_chunk_campaign : s_blf_header
{
	long campaign_id;

	dword_flags type_flags;

	wchar_t names[12][64];
	wchar_t descriptions[12][128];

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

	wchar_t names[12][32];
	wchar_t descriptions[12][128];

	dword __unknownF08;
	dword __unknownF0C;
};
static_assert(sizeof(s_blf_chunk_scenario_insertion) == 0xF10);

struct s_blf_chunk_scenario : s_blf_header
{
public:
	s_blf_chunk_scenario();

protected:
	long map_id;

	dword_flags type_flags;

	wchar_t names[12][32];
	wchar_t descriptions[12][128];

	char image_file_base[256];
	char scenario_path[256];

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