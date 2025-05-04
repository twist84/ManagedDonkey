#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"

enum e_leaderboard_statistic
{
	_leaderboard_statistic_mu = 0,
	_leaderboard_statistic_sigma,
	_leaderboard_statistic_hopper_skill,
	_leaderboard_statistic_hopper_games_played,
	_leaderboard_statistic_hopper_games_completed,
	_leaderboard_statistic_hopper_games_won,
	_leaderboard_statistic_experience_base,
	_leaderboard_statistic_experience_penalty,
	_leaderboard_statistic_highest_skill,
	_leaderboard_statistic_ranked_games_played,
	_leaderboard_statistic_ranked_games_completed,
	_leaderboard_statistic_ranked_games_won,
	_leaderboard_statistic_unranked_games_played,
	_leaderboard_statistic_unranked_games_completed,
	_leaderboard_statistic_unranked_games_won,
	_leaderboard_statistic_first_game_played_date,
	_leaderboard_statistic_last_game_played_date,
	_leaderboard_statistic_custom_games_completed,
	_leaderboard_statistic_custom_games_won,

	k_leaderboard_statistic_count
};

enum e_online_property_id
{
	_online_property_id_gamer_language = 0,
	_online_property_id_gamer_zone,
	_online_property_id_gamer_hostname,
	_online_property_id_affiliate_score,
	_online_property_id_draw_probability,
	_online_property_id_beta,
	_online_property_id_tau,
	_online_property_id_relative_score,
	_online_property_id_session_team,
	_online_property_id_skill_hill_climbing_factor,
	_online_property_id_hopper_skill,
	_online_property_id_hopper_games_played,
	_online_property_id_hopper_games_completed,
	_online_property_id_hopper_games_won,
	_online_property_id_global_experience_base,
	_online_property_id_global_experience_penalty,
	_online_property_id_global_highest_skill_level_attained,
	_online_property_id_global_matchmade_ranked_games_played,
	_online_property_id_global_matchmade_ranked_games_completed,
	_online_property_id_global_matchmade_ranked_games_won,
	_online_property_id_global_matchmade_unranked_games_played,
	_online_property_id_global_matchmade_unranked_games_completed,
	_online_property_id_global_matchmade_unranked_games_won,
	_online_property_id_global_custom_games_played,
	_online_property_id_global_custom_games_completed,
	_online_property_id_global_custom_games_won,
	_online_property_id_global_first_game_played_date,
	_online_property_id_global_last_game_played_date,
	_online_property_id_hopper_identifier,
	_online_property_id_lowest_skill_level,
	_online_property_id_highest_skill_level,
	_online_property_id_average_skill_level,
	_online_property_id_average_experience_rating,
	_online_property_id_has_all_dlc_maps,
	_online_property_id_session_of_quitters,
	_online_property_id_language_override,
	_online_property_id_gamerzone_override,
	_online_property_id_country_code_override,
	_online_property_id_mixed_skill_override,
	_online_property_id_joinable_party_size_1,
	_online_property_id_joinable_party_size_2,
	_online_property_id_joinable_party_size_3,
	_online_property_id_joinable_party_size_4,
	_online_property_id_joinable_party_size_5,
	_online_property_id_joinable_party_size_6,
	_online_property_id_joinable_party_size_7,
	_online_property_id_joinable_party_size_8,
	_online_property_id_joinable_party_size_9,
	_online_property_id_nat_type,
	_online_property_id_average_mu,
	_online_property_id_min_average_skill_level,
	_online_property_id_max_average_skill_level,
	_online_property_id_min_average_experience_rating,
	_online_property_id_max_average_experience_rating,
	_online_property_id_min_average_mu,
	_online_property_id_max_average_mu,
	_online_property_id_min_skill_level,
	_online_property_id_max_skill_level,
	_online_property_id_unused,
	k_online_property_id_count,

	k_online_property_id_none = NONE,
};

struct s_online_data
{
	// e_online_leaderboard_id
	int32 leaderboard_id;

	// unsure
	struct s_unknown_struct
	{
		e_leaderboard_statistic statistic;

		//e_online_leaderboard_column_id
		int32 column;
	};
	s_unknown_struct* __unknown4;

	union
	{
		int32 data_as_long;
		real32 data_as_real;
	};
};
static_assert(sizeof(s_online_data) == 0xC);

struct s_online_property
{
	e_online_property_id id;
	byte __data4[0x4];
	s_online_data data;
	int32 __unknown14;
};
static_assert(sizeof(s_online_property) == 0x18);

struct s_online_stat_write
{
	int32 leaderboard_id;
	int32 property_count;
	s_online_property properties[16];
};
static_assert(sizeof(s_online_stat_write) == 0x188);

struct s_network_leaderboard_custom_stats_write
{
	s_online_stat_write stat_writes;
};
static_assert(sizeof(s_network_leaderboard_custom_stats_write) == sizeof(s_online_stat_write));

struct s_network_leaderboard_matchmade_stats_write
{
	s_online_stat_write stat_writes[3];
};
static_assert(sizeof(s_network_leaderboard_matchmade_stats_write) == sizeof(s_online_stat_write) * 3);

struct s_online_stat_query
{
	int32 leaderboard_id;
	int32 leaderboard_statistics_count;
	int32 leaderboard_statistics[32];
};
static_assert(sizeof(s_online_stat_query) == 0x88);

struct s_network_leaderboard_player_game_results
{
	s_player_identifier player_identifier;
	s_player_configuration player_configuration;
	int32 relative_score;

	bool __unknown162C;
	bool __unknown162D;
	bool __unknown162E;
	bool __unknown162F;
};
static_assert(sizeof(s_network_leaderboard_player_game_results) == 0x1630);

struct s_network_leaderboard_game_results
{
	uns16 hopper_identifier;
	byte __pad2[0x2];
	int32 draw_probability;
	real32 beta;
	real32 tau;
	int32 experience_base_increment;
	int32 experience_penalty_decrement;
	bool hopper_is_valid;
	bool is_ranked;
	bool team_game;
	byte __pad1B[0x1];
	int32 team_count;
	int32 player_count;
	uns32 start_time;
};
static_assert(sizeof(s_network_leaderboard_game_results) == 0x28);

struct s_network_leaderboard_query_user_statistics
{
	bool valid;
	int32 controller_index;
	uns64 xuid;
	wchar_t name[32];

	int32 __unknown50;
	bool __unknown54;
	int32 __time58;
	int32 __unknown5C;
	byte __data60[0x1C8];

	c_http_stored_buffer_downloader<128> update_user_player_stats_downloader;
	int32 highest_skill_set_state;
	int32 highest_skill;
};
static_assert(sizeof(s_network_leaderboard_query_user_statistics) == 0x948);

struct s_network_leaderboard_write_statistics
{
	int32 __unknown0;
	byte __data4[0x4];
};
static_assert(sizeof(s_network_leaderboard_write_statistics) == 0x8);

struct s_network_leaderboard_globals
{
	bool initialized;
	int32 global_arbitrated_leaderboard_id;
	int32 global_unarbitrated_leaderboard_id;
	bool refreshing;
	int32 __time10;
	uns16 hopper_id;
	int32 hopper_skill_leaderboard_id;
	int32 hopper_leaderboard_id;
	c_static_array<s_network_leaderboard_query_user_statistics, 4> query_users;
	int32 __unknown2540;
	int32 __time2540;
	c_static_array<s_network_leaderboard_write_statistics, 16> write_statistics;
};
static_assert(sizeof(s_network_leaderboard_globals) == 0x25C8);

extern s_network_leaderboard_globals& network_leaderboard_globals;

extern void __cdecl network_leaderboard_destory();
extern bool __cdecl network_leaderboard_initialize();
extern void __cdecl network_leaderboard_update();

