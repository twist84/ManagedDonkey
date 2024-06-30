#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_candy_monitor.hpp"
#include "game/game_engine_scoring.hpp"
#include "game/game_engine_simulation.hpp"
#include "game/game_engine_spawn_influencer.hpp"
#include "game/game_engine_teleporters.hpp"
#include "game/game_engine_util.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_statborg.hpp"
#include "shell/shell.hpp"
#include "saved_games/scenario_map_variant.hpp"
#include "text/unicode.hpp"

enum e_game_engine_end_condition
{
	_game_engine_end_condition_end_default = 0,
	_game_engine_end_condition_game_end_scripting,
	_game_engine_end_condition_should_end_not_enough_living,
	_game_engine_end_condition_should_end_all_dead,
	_game_engine_end_condition_should_end_current_engine_override,
	_game_engine_end_condition_round_end_team_scoring,
	_game_engine_end_condition_round_end_scoring,
	_game_engine_end_condition_game_end_rounds,
	_game_engine_end_condition_game_end_rounds_team_early_victory,
	_game_engine_end_condition_game_end_rounds_early_victory,
	_game_engine_end_condition_game_end_rounds_exceeded,
	_game_engine_end_condition_round_end_time_ran_out,
	_game_engine_end_condition_game_end_external,
	_game_engine_end_condition_game_end_invalid_team_mapping,
	_game_engine_end_condition_game_end_due_to_automation,
	_game_engine_end_condition_game_end_exceeded_maximum_rounds,
	_game_engine_end_condition_round_end_juggernaut_left,
	_game_engine_end_condition_round_end_juggernaut_unknown1,
	_game_engine_end_condition_round_end_juggernaut_unknown2,
	_game_engine_end_condition_round_end_infection,
	_game_engine_end_condition_round_end_ctf_sides,
	_game_engine_end_condition_round_end_editor_reset,
	_game_engine_end_condition_round_end_editor_change_mode,
	_game_engine_end_condition_round_end_vip_vip_killed,

	k_game_engine_end_condition_count
};

extern char const* (&k_game_engine_end_conditions)[k_game_engine_end_condition_count];

// game_engine_hud_get_state_message
// e_global_string_id
enum e_game_engine_state
{
	_game_engine_state_game_over = 0,
	_game_engine_state_round_progress,
	_game_engine_state_round_over,
	_game_engine_state_waiting_for_next_round,

	k_game_engine_state_count
};

struct s_player_waypoint_data
{
	byte __data0[0x4];
	real_point3d head_position;
	word respawn_timer10;
	word respawn_timer12;
	byte __data[0x8];
};
static_assert(sizeof(s_player_waypoint_data) == 0x1C);

struct s_simulation_player_netdebug_data
{
	bool is_host;

	short host_estimated_bps;
	short host_transmission_rate;
	short host_transmission_bps;

	short client_rtt_msec;
	short client_packet_rate;
	short client_bandwidth_bps;
	short client_packet_loss;
};
static_assert(sizeof(s_simulation_player_netdebug_data) == 0x10);

struct s_multiplayer_weapon_tracker
{
	dword weapon_index;
	word multiplayer_weapon_identifier;
	dword owner_unit_index;
	dword owner_player_index;
};
static_assert(sizeof(s_multiplayer_weapon_tracker) == 0x10);

struct s_game_engine_globals
{
	dword_flags flags;
	word_flags valid_team_mask;
	word_flags initial_teams;
	word_flags valid_designators;
	word_flags valid_teams;
	word_flags active_teams;
	word game_simulation;
	c_static_array<short, 9> team_designator_to_team_index;
	c_static_array<char, 8> team_lives_per_round;
	short __unknown2A;
	dword gamestate_index;
	dword statborg_gamestate_index;
	c_static_array<long, 16> player_simulation_object_glue_indices;
	byte __data74[0x4];
	c_map_variant map_variant;
	c_enum<e_game_engine_state, short, _game_engine_state_game_over, k_game_engine_state_count> current_state;
	short round_index;
	long round_timer;
	c_flags<long, byte, 8> round_condition_flags;
	s_game_engine_score_list score_list;

	union
	{
		s_ctf_globals ctf_globals;
		//s_slayer_globals slayer_globals;
		s_oddball_globals oddball_globals;
		s_king_globals king_globals;
		s_sandbox_globals sandbox_globals;
		s_vip_globals vip_globals;
		s_juggernaut_globals juggernaut_globals;
		s_territories_globals territories_globals;
		s_assault_globals assault_globals;
		s_infection_globals infection_globals;

		// probably contains more bytes than it should
		byte globals_storage[0x1800];
	};

	word timer;
	word __unknownF992;
	dword game_variant_round_time_limit_ticks_per_second;
	real user_fade_to_black_amounts[4];
	byte_flags user_fade_to_black_flags;
	byte __dataF9A9;
	short __unknownF9AA;
	long shot_id;
	c_static_array<s_dead_player_info, 64> spawn_influencers;
	c_game_statborg statborg;

	// are these related?
	long __unknown102D4;
	c_static_array<s_player_waypoint_data, 16> player_waypoints;

	// are these related?
	long __unknown10498;
	c_static_array<s_simulation_player_netdebug_data, 16> player_netdebugs;

	c_multiplayer_candy_monitor_manager candy_monitor_manager;
	dword round_end_ticks;
	c_enum<e_game_engine_state, long, _game_engine_state_game_over, k_game_engine_state_count> desired_state;
	bool game_finished;
	long garbage_collect_speed; // e_garbage_collect_speed
	dword __unknown13DAC;
	c_enum<e_game_engine_type, long, _game_engine_type_none, k_game_engine_type_count> game_engine_index;
	long multiplayer_weapon_count;
	c_static_array<s_multiplayer_weapon_tracker, 8> multiplayer_weapons;
	c_area_set<c_teleporter_area, 32> teleporters;
	long current_event_identifier;
	c_static_array<s_game_engine_queued_event, 64> event_queue;
	byte __data1584C[0xC];
};
static_assert(sizeof(s_game_engine_globals) == 0x15858);

struct s_player_state_data
{
	c_enum<e_game_engine_status, long, _game_engine_status_waiting_for_space_to_clear, k_game_engine_status_count> state_index;
	long __unknown4;
	long state_response_index;
};
static_assert(sizeof(s_player_state_data) == 0xC);

struct s_user_talker_data
{
	c_static_array<short, 16> __unknown0;
};
static_assert(sizeof(s_user_talker_data) == 0x20);

struct s_local_game_engine_globals
{
	long __time0;

	byte __data4[0x10];

	c_static_array<s_player_state_data, 4> player_states;
	c_static_array<s_user_talker_data, 4> user_talkers;
};
static_assert(sizeof(s_local_game_engine_globals) == 0xC4);

extern void __cdecl game_engine_end_round_with_winner(long team, bool a1, e_game_engine_end_condition condition);
extern void __cdecl game_engine_game_starting();
extern void __cdecl game_engine_get_multiplayer_string(string_id id, c_static_wchar_string<1024>* out_multiplayer_string);
extern long __cdecl game_engine_get_player_place(long absolute_player_index);
extern long __cdecl game_engine_get_player_score_for_display(long absolute_player_index, bool final_score);
extern e_game_engine_status __cdecl game_engine_get_player_state_index(long player_index);
extern c_game_statborg* __cdecl game_engine_get_statborg();
extern long __cdecl game_engine_get_team_place(long team);
extern long __cdecl game_engine_get_team_score_for_display(long team, bool final_score);
extern void __cdecl game_engine_get_universal_string(long a1, c_static_wchar_string<256>* formatted_string);
extern bool __cdecl game_engine_in_round();
extern void __cdecl game_engine_interface_update(float world_seconds_elapsed);
extern bool __cdecl game_engine_player_is_dead_and_betrayed_by_griefer(long player_index, long* griefer_player_index);
extern bool __cdecl game_engine_player_is_out_of_lives(long player_index);
extern bool __cdecl game_engine_player_is_playing(long player_index);
extern void __cdecl game_engine_player_set_spawn_timer(long player_index, long spawn_time);
extern long __cdecl game_engine_get_pre_round_ticks();
extern long __cdecl game_engine_get_post_round_ticks();
extern long __cdecl game_engine_round_time_get();
extern bool __cdecl game_engine_running();
extern void __cdecl game_engine_update();
extern void __cdecl game_engine_update_after_game();
extern void __cdecl game_engine_update_global_fade_timers(real world_seconds_elapsed);
extern void __cdecl game_engine_update_round_conditions();
extern void __cdecl game_engine_dump_variant_settings(char const* filename);
extern void __cdecl post_game_engine_globals_message(long message_type, char a2, short a3);
extern void __cdecl process_game_engine_globals_messages();

