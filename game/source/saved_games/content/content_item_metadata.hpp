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

enum e_campaign_id;
enum e_map_id;
struct c_bitstream;
struct s_saved_game_item_metadata
{
	uint64 unique_id;
	wchar_t name[16];
	char description[128];
	char author[16];
	c_enum<e_saved_game_file_type, long, _saved_game_file_type_none, k_saved_game_file_type_count> file_type;
	bool author_is_xuid_online;
	uint8 pad0[3];
	uint64 author_id;
	uint64 size_in_bytes;
	uint64 date;
	long length_seconds;
	e_campaign_id campaign_id;
	e_map_id map_id;
	long game_engine_type;
	long campaign_difficulty;
	uint8 campaign_insertion_point;
	bool campaign_survival_enabled;
	uint8 pad[0x1];
	uint64 game_id;

	void byteswap();
	bool is_valid() const;
	void decode(c_bitstream* packet);
	void encode(c_bitstream* packet) const;

	void decode_from_mcc(c_bitstream* packet);
	void encode_to_mcc(c_bitstream* packet) const;
};
static_assert(sizeof(s_saved_game_item_metadata) == 0xF8);

