#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"

struct s_network_session_player;

typedef c_enum<e_online_data_type, uns8, _online_data_context, _online_data_null> c_online_data_type;

struct s_online_data
{
	c_online_data_type type;

	union
	{
		int32 data_as_long;
		uns32 data_as_qword;
		real64 data_as_double;

		struct
		{
			int32 size;
			wchar_t* ustring;
		} string;

		real32 data_as_float;

		struct
		{
			int32 size;
			byte* data;
		} binary;

		struct //_FILETIME
		{
			unsigned int dwLowDateTime;
			unsigned int dwHighDateTime;
		} day_time;
	};

};
COMPILE_ASSERT(sizeof(s_online_data) == 0x10);

struct s_online_property
{
	e_online_property_id id;
	s_online_data value;
};
COMPILE_ASSERT(sizeof(s_online_property) == 0x18);
COMPILE_ASSERT(0x0 == OFFSETOF(s_online_property, id));
//COMPILE_ASSERT(0x4 == OFFSETOF(s_online_property, value));

struct s_online_stat_write
{
	e_online_leaderboard_id leaderboard_id;
	int32 property_count;
	s_online_property properties[16];
};
COMPILE_ASSERT(sizeof(s_online_stat_write) == 0x188);
COMPILE_ASSERT(0x0 == OFFSETOF(s_online_stat_write, leaderboard_id));
COMPILE_ASSERT(0x4 == OFFSETOF(s_online_stat_write, property_count));
COMPILE_ASSERT(0x8 == OFFSETOF(s_online_stat_write, properties));

struct s_network_leaderboard_custom_stats_write
{
	s_online_stat_write stat_writes;
};
COMPILE_ASSERT(sizeof(s_network_leaderboard_custom_stats_write) == sizeof(s_online_stat_write));

struct s_network_leaderboard_matchmade_stats_write
{
	s_online_stat_write stat_writes[3];
};
COMPILE_ASSERT(sizeof(s_network_leaderboard_matchmade_stats_write) == sizeof(s_online_stat_write) * 3);

struct s_online_stat_query
{
	e_online_leaderboard_id leaderboard_id;
	int32 column_count;
	e_online_leaderboard_column_id columns[32];
};
COMPILE_ASSERT(sizeof(s_online_stat_query) == 0x88);
COMPILE_ASSERT(0x0 == OFFSETOF(s_online_stat_query, leaderboard_id));
COMPILE_ASSERT(0x4 == OFFSETOF(s_online_stat_query, column_count));
COMPILE_ASSERT(0x8 == OFFSETOF(s_online_stat_query, columns));

struct s_network_leaderboard_player_game_results
{
	uns64 identifier;
	s_player_configuration configuration;
	int32 relative_score;
	bool played;
	bool completed;
	bool won;
	bool valid;
};
COMPILE_ASSERT(sizeof(s_network_leaderboard_player_game_results) == 0x1630);
COMPILE_ASSERT(0x0000 == OFFSETOF(s_network_leaderboard_player_game_results, identifier));
COMPILE_ASSERT(0x0008 == OFFSETOF(s_network_leaderboard_player_game_results, configuration));
COMPILE_ASSERT(0x1628 == OFFSETOF(s_network_leaderboard_player_game_results, relative_score));
COMPILE_ASSERT(0x162C == OFFSETOF(s_network_leaderboard_player_game_results, played));
COMPILE_ASSERT(0x162D == OFFSETOF(s_network_leaderboard_player_game_results, completed));
COMPILE_ASSERT(0x162E == OFFSETOF(s_network_leaderboard_player_game_results, won));
COMPILE_ASSERT(0x162F == OFFSETOF(s_network_leaderboard_player_game_results, valid));

struct s_network_leaderboard_game_results
{
	uns16 hopper_identifier;
	int32 draw_probability;
	real32 beta;
	real32 tau;
	int32 experience_base_increment;
	int32 experience_penalty_decrement;
	bool matchmade;
	bool ranked;
	bool team_game;
	int32 team_count;
	int32 player_count;
	uns32 game_time;
};
COMPILE_ASSERT(sizeof(s_network_leaderboard_game_results) == 0x28);
COMPILE_ASSERT(0x00 == OFFSETOF(s_network_leaderboard_game_results, hopper_identifier));
COMPILE_ASSERT(0x04 == OFFSETOF(s_network_leaderboard_game_results, draw_probability));
COMPILE_ASSERT(0x08 == OFFSETOF(s_network_leaderboard_game_results, beta));
COMPILE_ASSERT(0x0C == OFFSETOF(s_network_leaderboard_game_results, tau));
COMPILE_ASSERT(0x10 == OFFSETOF(s_network_leaderboard_game_results, experience_base_increment));
COMPILE_ASSERT(0x14 == OFFSETOF(s_network_leaderboard_game_results, experience_penalty_decrement));
COMPILE_ASSERT(0x18 == OFFSETOF(s_network_leaderboard_game_results, matchmade));
COMPILE_ASSERT(0x19 == OFFSETOF(s_network_leaderboard_game_results, ranked));
COMPILE_ASSERT(0x1A == OFFSETOF(s_network_leaderboard_game_results, team_game));
COMPILE_ASSERT(0x1C == OFFSETOF(s_network_leaderboard_game_results, team_count));
COMPILE_ASSERT(0x20 == OFFSETOF(s_network_leaderboard_game_results, player_count));
COMPILE_ASSERT(0x24 == OFFSETOF(s_network_leaderboard_game_results, game_time));

struct s_network_leaderboard_statistic
{
	bool valid;
	s_online_data stat_data;
};

struct s_network_leaderboard_query_statistics
{
	int32 state;
	bool query_blocked;
	uns32 timestamp;
	int failure_count;
	s_network_leaderboard_statistic stats[19];
};

struct s_network_leaderboard_query_user_statistics
{
  bool valid;
  e_controller_index controller_index;
  uns64 xuid;
  c_static_wchar_string<32> gamertag;
  s_network_leaderboard_query_statistics queried_stats;
  c_http_stored_buffer_downloader<128> highest_skill_downloader;
  e_network_leaderboard_user_highest_skill_update_state highest_skill_update_state;
  int32 highest_skill;
};
COMPILE_ASSERT(sizeof(s_network_leaderboard_query_user_statistics) == 0x948);

struct s_network_leaderboard_write_statistics
{
	e_network_leaderboard_statistics_write_state state;
	uns32 timestamp;
};
COMPILE_ASSERT(sizeof(s_network_leaderboard_write_statistics) == 0x8);

struct s_network_leaderboard_globals
{
	bool initialized;
	e_online_leaderboard_id global_arbitrated_leaderboard_id;
	e_online_leaderboard_id global_unarbitrated_leaderboard_id;
	bool refresh_requested;
	uns32 last_refresh_time;
	uns16 hopper_identifier;
	e_online_leaderboard_id hopper_skill_leaderboard_id;
	e_online_leaderboard_id hopper_leaderboard_id;
	c_static_array<s_network_leaderboard_query_user_statistics, 4> query_user_stats;
	s_network_leaderboard_write_statistics global_write;
	c_static_array<s_network_leaderboard_write_statistics, 16> write_player_stats;
};
COMPILE_ASSERT(sizeof(s_network_leaderboard_globals) == 0x25C8);

extern s_network_leaderboard_globals& network_leaderboard_globals;

extern void __cdecl network_leaderboard_destory();
extern e_network_leaderboard_write_status __cdecl network_leaderboard_get_write_status();
extern bool __cdecl network_leaderboard_initialize();
extern bool __cdecl network_leaderboard_player_stats_valid(const s_network_session_player* player);
extern void __cdecl network_leaderboard_update();

