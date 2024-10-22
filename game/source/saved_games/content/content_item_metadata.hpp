#pragma once

#include "cseries/cseries.hpp"

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
	byte pad[0x1];
	qword game_id;

	void byteswap();
	bool is_valid() const;
	void decode(c_bitstream* packet);
	void encode(c_bitstream* packet) const;

	void decode_from_mcc(c_bitstream* packet);
	void encode_to_mcc(c_bitstream* packet) const;
};
static_assert(sizeof(s_content_item_metadata) == 0xF8);

