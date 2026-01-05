#pragma once

#include "cseries/cseries.hpp"

class c_bitstream;
struct s_saved_game_item_metadata
{
	uns64 unique_id;
	wchar_t display_name[16];
	utf8 description[128];
	char author[16];
	e_saved_game_file_type file_type;
	bool author_is_xuid_online;
	byte pad0[3];
	uns64 author_xuid;
	uns64 size_in_bytes;
	uns64 date;
	int32 length_seconds;
	e_campaign_id campaign_id;
	e_map_id map_id;
	e_game_engine_type game_engine_index;
	e_campaign_difficulty_level campaign_difficulty;

	union
	{
		struct
		{
			uns8 campaign_insertion_point;
			bool campaign_survival_enabled;
			byte pad[0x1];
		};
		//struct
		//{
		//	uns16 hopper_id;
		//	uns16 pad;
		//};
	};

	uns64 game_id;

	void byteswap();
	void decode(c_bitstream* packet);
	void encode(c_bitstream* packet) const;
	static uns64 __cdecl generate_unique_id();
	e_gui_game_mode get_gui_game_mode() const;
	e_metadata_file_type get_metadata_file_type() const;
	void initialize_from_current_game_settings(e_controller_index controller_index, e_saved_game_file_type file_type, const wchar_t* in_name, const wchar_t* in_description, uns64 size);
	bool is_valid() const;

	void decode_from_mcc(c_bitstream* packet);
	void encode_to_mcc(c_bitstream* packet) const;
};
static_assert(sizeof(s_saved_game_item_metadata) == 0xF8);

