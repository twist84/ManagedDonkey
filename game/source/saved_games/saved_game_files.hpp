#pragma once

#include "cseries/cseries.hpp"
#include "interface/user_interface_text_parser.hpp"

enum e_saved_game_file_type
{
	_saved_game_file_type_none = -1,
	_saved_game_file_type_personal,
	_saved_game_file_type_ctf,
	_saved_game_file_type_slayer,
	_saved_game_file_type_oddball,
	_saved_game_file_type_king,
	_saved_game_file_type_juggernaut,
	_saved_game_file_type_territories,
	_saved_game_file_type_assault,
	_saved_game_file_type_infection,
	_saved_game_file_type_vip,
	_saved_game_file_type_usermap,
	_saved_game_file_type_film,
	_saved_game_file_type_clip,
	_saved_game_file_type_screenshot,

	k_saved_game_file_type_count
};

struct c_bitstream;
struct s_content_item_metadata
{
	qword unique_id;
	wchar_t name[16];
	char description[128];
	char author[16];
	c_enum<e_saved_game_file_type, long, _saved_game_file_type_none, k_saved_game_file_type_count> file_type;
	bool author_is_xuid_online;
	byte pad0[3];
	qword author_id;
	qword size_in_bytes;
	qword date;
	long length_seconds;
	long campaign_id;
	long map_id;
	long game_engine_type;
	long campaign_difficulty;
	byte campaign_insertion_point;
	bool campaign_survival_enabled;
	byte pad[1];
	qword game_id;

	void byteswap();
	bool is_valid() const;
	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);
};
static_assert(sizeof(s_content_item_metadata) == 0xF8);

struct s_saved_game_file_text_parser_input
{
	long __unknown0;
	long __unknown4;
	long __unknown8;
	long __unknownC;
	long __unknown10;
};
static_assert(sizeof(s_saved_game_file_text_parser_input) == 0x14);

struct c_saved_game_files_globals
{
	byte_flags flags;
	byte __data1[3];
	s_saved_game_file_text_parser_input text_parser_input;
	c_magic_string_game_tag_parser map_variant_name_parser;
	c_magic_string_game_tag_parser game_variant_name_parser;
	c_magic_string_game_tag_parser campaign_map_name_parser;
	c_magic_string_game_tag_parser difficulty_parser;
};
static_assert(sizeof(c_saved_game_files_globals) == 0x68);

