#pragma once

#include "cseries/cseries.hpp"

struct s_saved_game_item_metadata
{
	qword unique_id;
	wchar_t name[16];
	char description[128];
	char author[16];
	dword file_type;
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
};
static_assert(sizeof(s_saved_game_item_metadata) == 0xF8);

