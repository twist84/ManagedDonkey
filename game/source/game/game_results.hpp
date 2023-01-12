#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"

struct s_game_results_incremental
{
	byte __data0[0x10];

	c_static_array<game_player_options, 16> players;

	dword __unknown16410[16];

	struct // statistics?
	{
		byte __unknown0[0x4380];
		byte __unknown4380[0x400];
		byte __unknown4780[0x660];
	} __unknown16450;

	byte __unknown1B230[17][32];
};

struct c_simulation_view;
struct c_game_results_replicator
{
	c_simulation_view* m_view;
	bool m_has_fatal_error;
	bool m_sending_updates;
	bool m_receiving_updates;
	long m_incremental_update_number;
	dword __unknownC;
	s_game_results_incremental m_incremental;
	dword __time1B460;
	byte __data1B464[4];
};
static_assert(sizeof(c_game_results_replicator) == 0x1B468);

// leaving enums here for now, will find them homes in the future

enum e_achievement
{
	_achievement_beat_sc100 = 0,
	_achievement_beat_sc110,
	_achievement_beat_sc120,
	_achievement_beat_sc130,
	_achievement_beat_sc140,
	_achievement_beat_sc150,
	_achievement_beat_l200,
	_achievement_beat_l300,
	_achievement_beat_campaign_normal,
	_achievement_beat_campaign_heroic,
	_achievement_beat_campaign_legendary,
	_achievement_wraith_killer,
	_achievement_naughty_naughty,
	_achievement_good_samaritan,
	_achievement_dome_inspector,
	_achievement_laser_blaster,
	_achievement_both_tubes,
	_achievement_i_like_fire,
	_achievement_my_clothes,
	_achievement_pink_and_deadly,
	_achievement_dark_times,
	_achievement_trading_down,
	_achievement_headcase,
	_achievement_boom_headshot,
	_achievement_ewww_sticky,
	_achievement_junior_detective,
	_achievement_gumshoe,
	_achievement_super_sleuth,
	_achievement_metagame_points_in_sc100,
	_achievement_metagame_points_in_sc110,
	_achievement_metagame_points_in_sc120,
	_achievement_metagame_points_in_sc130a,
	_achievement_metagame_points_in_sc130b,
	_achievement_metagame_points_in_sc140,
	_achievement_metagame_points_in_l200,
	_achievement_metagame_points_in_l300,
	_achievement_be_like_marty,
	_achievement_find_all_audio_logs,
	_achievement_find_01_audio_logs,
	_achievement_find_03_audio_logs,
	_achievement_find_15_audio_logs,
	_achievement_vidmaster_challenge_deja_vu,
	_achievement_vidmaster_challenge_endure,
	_achievement_vidmaster_challenge_classic,
	_achievement_heal_up,
	_achievement_stunning,
	_achievement_tourist,

	k_achievement_count
};

enum e_game_results_statistic
{
	_game_results_statistic_games_played = 0,
	_game_results_statistic_games_completed,
	_game_results_statistic_games_won,
	_game_results_statistic_games_tied,
	_game_results_statistic_rounds_completed,
	_game_results_statistic_rounds_won,
	_game_results_statistic_in_round_score,
	_game_results_statistic_in_game_total_score,
	_game_results_statistic_kills,
	_game_results_statistic_assists,
	_game_results_statistic_deaths,
	_game_results_statistic_betrayals,
	_game_results_statistic_suicides,
	_game_results_statistic_most_kills_in_a_row,
	_game_results_statistic_seconds_alive,
	_game_results_statistic_ctf_flag_scores,
	_game_results_statistic_ctf_flag_grabs,
	_game_results_statistic_ctf_flag_carrier_kills,
	_game_results_statistic_ctf_flag_returns,
	_game_results_statistic_assault_bomb_arms,
	_game_results_statistic_assault_bomb_grabs,
	_game_results_statistic_assault_bomb_disarms,
	_game_results_statistic_assault_bomb_detonations,
	_game_results_statistic_oddball_time_with_ball,
	_game_results_statistic_oddball_unused,
	_game_results_statistic_oddball_kills_as_carrier,
	_game_results_statistic_oddball_ball_carrier_kills,
	_game_results_statistic_king_time_on_hill,
	_game_results_statistic_king_total_control_time,
	_game_results_statistic_king_unused0,
	_game_results_statistic_king_unused1,
	_game_results_statistic_unused0,
	_game_results_statistic_unused1,
	_game_results_statistic_unused2,
	_game_results_statistic_vip_takedowns,
	_game_results_statistic_vip_kills_as_vip,
	_game_results_statistic_vip_guard_time,
	_game_results_statistic_vip_time_as_vip,
	_game_results_statistic_vip_lives_as_vip,
	_game_results_statistic_juggernaut_kills,
	_game_results_statistic_juggernaut_kills_as_juggernaut,
	_game_results_statistic_juggernaut_total_control_time,
	_game_results_statistic_total_wp,
	_game_results_statistic_juggernaut_unused,
	_game_results_statistic_territories_owned,
	_game_results_statistic_territories_captures,
	_game_results_statistic_territories_ousts,
	_game_results_statistic_territories_time_in_territory,
	_game_results_statistic_infection_zombie_kills,
	_game_results_statistic_infection_infections,
	_game_results_statistic_infection_time_as_human,

	k_game_results_statistic_count
};

enum e_game_results_damage_statistic
{
	_game_results_damage_statistic_damage_kills = 0,
	_game_results_damage_statistic_damage_deaths,
	_game_results_damage_statistic_damage_betrayals,
	_game_results_damage_statistic_damage_suicides,
	_game_results_damage_statistic_damage_headshots,

	k_game_results_damage_statistic_count
};

enum e_game_results_player_vs_player_statistic
{
	_game_results_player_vs_player_statistic_player_vs_player_kills = 0,
	_game_results_player_vs_player_statistic_player_vs_player_deaths,

	k_game_results_player_vs_player_statistic_count 
};

enum e_game_results_medal
{
	_game_results_medal_extermination = 0,
	_game_results_medal_perfection,
	_game_results_medal_multiple_kill_2,
	_game_results_medal_multiple_kill_3,
	_game_results_medal_multiple_kill_4,
	_game_results_medal_multiple_kill_5,
	_game_results_medal_multiple_kill_6,
	_game_results_medal_multiple_kill_7,
	_game_results_medal_multiple_kill_8,
	_game_results_medal_multiple_kill_9,
	_game_results_medal_multiple_kill_10,
	_game_results_medal_5_kills_in_a_row,
	_game_results_medal_10_kills_in_a_row,
	_game_results_medal_15_kills_in_a_row,
	_game_results_medal_20_kills_in_a_row,
	_game_results_medal_25_kills_in_a_row,
	_game_results_medal_30_kills_in_a_row,
	_game_results_medal_sniper_kill_5,
	_game_results_medal_sniper_kill_10,
	_game_results_medal_shotgun_kill_5,
	_game_results_medal_shotgun_kill_10,
	_game_results_medal_collision_kill_5,
	_game_results_medal_collision_kill_10,
	_game_results_medal_sword_kill_5,
	_game_results_medal_sword_kill_10,
	_game_results_medal_juggernaut_kill_5,
	_game_results_medal_juggernaut_kill_10,
	_game_results_medal_zombie_kill_5,
	_game_results_medal_zombie_kill_10,
	_game_results_medal_human_kill_5,
	_game_results_medal_human_kill_10,
	_game_results_medal_human_kill_15,
	_game_results_medal_koth_kill_5,
	_game_results_medal_shotgun_kill_sword,
	_game_results_medal_vehicle_impact_kill,
	_game_results_medal_vehicle_hijack,
	_game_results_medal_aircraft_hijack,
	_game_results_medal_deadplayer_kill,
	_game_results_medal_player_kill_spreeplayer,
	_game_results_medal_spartanlaser_kill,
	_game_results_medal_stickygrenade_kill,
	_game_results_medal_sniper_kill,
	_game_results_medal_bashbehind_kill,
	_game_results_medal_bash_kill,
	_game_results_medal_flame_kill,
	_game_results_medal_driver_assist_gunner,
	_game_results_medal_assault_bomb_planted,
	_game_results_medal_assault_player_kill_carrier,
	_game_results_medal_vip_player_kill_vip,
	_game_results_medal_juggernaut_player_kill_juggernaut,
	_game_results_medal_oddball_carrier_kill_player,
	_game_results_medal_ctf_flag_captured,
	_game_results_medal_ctf_flag_player_kill_carrier,
	_game_results_medal_ctf_flag_carrier_kill_player,
	_game_results_medal_infection_survive,
	_game_results_medal_unused1,
	_game_results_medal_unused2,
	_game_results_medal_unused3,

	k_game_results_medal_count
};

struct s_integer_statistic_update
{
	word statistic;
};

struct s_integer_statistic_definition
{
	const char* name;

	// type?
	dword __unknown4;

	word minimum_value;
	word maximum_value;
	dword encoding_bits;
};
static_assert(sizeof(s_integer_statistic_definition) == 0x10);

extern s_integer_statistic_definition(&g_game_results_achievement_statistic_definitions)[k_achievement_count];
extern s_integer_statistic_definition(&g_game_results_statistic_definitions)[k_game_results_statistic_count];
extern s_integer_statistic_definition(&g_game_results_damage_statistic_definitions)[k_game_results_damage_statistic_count];
extern s_integer_statistic_definition(&g_game_results_player_vs_player_statistic_definitions)[k_game_results_player_vs_player_statistic_count];
extern s_integer_statistic_definition(&g_game_results_medal_statistic_definitions)[k_game_results_medal_count];

struct c_bitstream;

extern void __cdecl game_results_statistic_increment(long player_absolute_index, long team_index, e_game_results_statistic statistic, long a4);
extern void __cdecl game_results_statistic_set(long player_absolute_index, long team_index, e_game_results_statistic statistic, long a4);
extern bool __cdecl game_results_statistics_decode(c_bitstream* packet, s_integer_statistic_update* statistics, long statistics_count, s_integer_statistic_definition const* statistic_definitions, long statistics_definitions_count);
extern void __cdecl game_results_statistics_encode(c_bitstream* packet, s_integer_statistic_update const* statistics, long statistics_count, s_integer_statistic_definition const* statistic_definitions, long statistics_definitions_count);

