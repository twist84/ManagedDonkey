#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"

struct s_network_leaderboard_query_user_statistics
{
	bool valid;
	long controller_index;
	qword xuid;
	wchar_t name[32];

	long __unknown50;
	bool __unknown54;
	long __time58;
	long __unknown5C;
	byte __data60[0x1C8];

	c_http_stored_buffer_downloader<128> update_user_player_stats_downloader;
	long highest_skill_set_state;
	long highest_skill;
};
static_assert(sizeof(s_network_leaderboard_query_user_statistics) == 0x948);

struct s_network_leaderboard_write_statistics
{
	long __unknown0;
	byte __data4[0x4];
};
static_assert(sizeof(s_network_leaderboard_write_statistics) == 0x8);

struct s_network_leaderboard_globals
{
	bool initialized;
	long global_arbitrated_leaderboard_id;
	long global_unarbitrated_leaderboard_id;
	bool refreshing;
	long __time10;
	word hopper_id;
	long hopper_skill_leaderboard_id;
	long hopper_leaderboard_id;
	c_static_array<s_network_leaderboard_query_user_statistics, 4> query_users;
	long __unknown2540;
	long __time2540;
	c_static_array<s_network_leaderboard_write_statistics, 16> write_statistics;
};
static_assert(sizeof(s_network_leaderboard_globals) == 0x25C8);

