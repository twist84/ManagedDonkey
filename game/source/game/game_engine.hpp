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
#include "saved_games/scenario_map_variant.hpp"
#include "text/unicode.hpp"

enum e_game_engine_end_condition
{
	_game_engine_end_default = 0,
	_game_engine_game_end_scripting,
	_game_engine_should_end_not_enough_living,
	_game_engine_should_end_all_dead,
	_game_engine_should_end_current_engine_override,
	_game_engine_round_end_team_scoring,
	_game_engine_round_end_scoring,
	_game_engine_game_end_rounds,
	_game_engine_game_end_rounds_team_early_victory,
	_game_engine_game_end_rounds_early_victory,
	_game_engine_game_end_rounds_exceeded,
	_game_engine_round_end_time_ran_out,
	_game_engine_game_end_external,
	_game_engine_game_end_invalid_team_mapping,
	_game_engine_game_end_due_to_automation,
	_game_engine_game_end_exceeded_maximum_rounds,
	_game_engine_round_end_juggernaut_left,
	_game_engine_round_end_juggernaut_unknown1,
	_game_engine_round_end_juggernaut_unknown2,
	_game_engine_round_end_infection,
	_game_engine_round_end_ctf_sides,
	_game_engine_round_end_editor_reset,
	_game_engine_round_end_editor_change_mode,
	_game_engine_round_end_vip_killed,

	k_game_engine_game_end_condition_count
};

extern char const* (&k_game_engine_end_conditions)[k_game_engine_game_end_condition_count];

// game_engine_hud_get_state_message
// e_string_id_global
enum e_game_engine_state
{
	_game_engine_state_game_over = 0,
	_game_engine_state_round_progress,
	_game_engine_state_round_over,
	_game_engine_state_waiting_for_next_round,

	k_game_engine_state_count
};

enum e_garbage_collect_speed
{
	_garbage_collect_speed_normal = 0,
	_garbage_collect_speed_fast,

	k_garbage_collect_speed_count
};

enum e_game_engine_performance_flags
{
	_unused = 0,

	k_game_engine_performance_flags_count
};
using c_game_engine_performance_flags = c_flags<e_game_engine_performance_flags, uint16, k_game_engine_performance_flags_count>;

struct s_player_navpoint_data
{
	bool last_living_location_valid;
	real_point3d last_living_location;
	uint16 total_time_to_respawn_in_ticks;
	uint16 current_time_to_respawn_in_ticks;
	
	//long dead_unit_index;
	uint8 __data14[0x4];

	char current_navpoint_action;
	char current_navpoint_action_timer;
	char next_navpoint_action;
};
static_assert(sizeof(s_player_navpoint_data) == 0x1C);

struct s_simulation_player_netdebug_data
{
	bool is_host;
	uint8 pad1;

	uint16 host_estimated_bps;
	uint16 host_transmission_rate;
	uint16 host_transmission_bps;

	uint16 client_rtt_msec;
	uint16 client_packet_rate;
	uint16 client_bandwidth_bps;
	uint16 client_packet_loss;
};
static_assert(sizeof(s_simulation_player_netdebug_data) == 0x10);

struct s_multiplayer_weapon_tracker
{
	long weapon_index;
	short multiplayer_weapon_identifier;
	uint16 pad;
	long owner_unit_index;
	long owner_player_index;
};
static_assert(sizeof(s_multiplayer_weapon_tracker) == 0x10);

struct s_game_engine_globals
{
	long flags;
	uint16 allowable_team_designators;
	uint16 initial_teams;
	uint16 valid_team_designators;
	uint16 valid_teams;
	uint16 active_teams;
	uint16 ever_active_teams;
	c_static_array<short, 9> team_designator_to_team_index;
	c_static_array<char, 8> team_lives_per_round;
	uint32 game_engine_gamestate_index;
	uint32 statborg_gamestate_index;
	c_static_array<long, 16> player_gamestate_indices;
	c_map_variant runtime_map_variant;
	c_enum<e_game_engine_state, short, _game_engine_state_game_over, k_game_engine_state_count> current_state;
	short round_index;
	long round_start_time;
	c_flags<long, uint8, 8> round_condition_flags;
	s_game_engine_score_list round_scoring_list;

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
		uint8 multiplayer_globals_storage[0x1800];
	};

	short round_timer_in_seconds;
	uint32 game_variant_round_time_limit_ticks_per_second;
	real32 fade_to_black_amount[4];
	uint8 fade_to_black_cache_latch;
	short out_of_round_timer;
	long global_shot_id;
	c_static_array<s_dead_player_info, 64> dead_player_records;
	c_game_statborg stats;

	// are these related?
	long __unknown102D4;
	c_static_array<s_player_navpoint_data, 16> player_navpoint_data;

	long last_netdebug_update_time;
	c_static_array<s_simulation_player_netdebug_data, 16> player_netdebug_data;

	c_multiplayer_candy_monitor_manager candy_monitor_manager;
	uint32 game_engine_state_timer;
	c_enum<e_game_engine_state, long, _game_engine_state_game_over, k_game_engine_state_count> desired_state;
	bool game_engine_has_handled_game_end;
	e_garbage_collect_speed garbage_collect_speed;
	c_game_engine_performance_flags performance_flags;
	c_enum<e_game_engine_type, long, _game_engine_type_none, k_game_engine_type_count> game_engine_index;

	long multiplayer_weapon_count;
	c_static_array<s_multiplayer_weapon_tracker, 8> multiplayer_weapons;

	c_area_set<c_teleporter_area, 32> teleporters;

	long game_engine_event_identifier;
	c_static_array<s_game_engine_queued_event, 64> event_queue;

	long game_time_at_last_respawn;
	long respawn_count_current_tick;

	uint8 __data15854[0x4];
};
static_assert(sizeof(s_game_engine_globals) == 0x15858);

struct s_player_state_data
{
	long current_state_index;
	long current_state_timer;
	long current_state_response_index;
};
static_assert(sizeof(s_player_state_data) == 0xC);

struct s_user_talker_data
{
	c_static_array<short, 16> talking_ticks;
};
static_assert(sizeof(s_user_talker_data) == 0x20);

struct s_local_game_engine_globals
{
	long round_or_game_over_timer;
	c_static_array<real32, 4> score_offsets;
	c_static_array<s_player_state_data, 4> user_player_state_data;
	c_static_array<s_user_talker_data, 4> user_talker_data;
};
static_assert(sizeof(s_local_game_engine_globals) == 0xC4);

extern void __cdecl game_engine_dispose();
extern void __cdecl game_engine_dispose_from_old_map();
extern void __cdecl game_engine_end_round_with_winner(e_game_team winner_index, bool end_game_instead_of_round, e_game_engine_end_condition end_condition);
extern void __cdecl game_engine_game_ending();
extern void __cdecl game_engine_game_starting();
extern long __cdecl game_engine_get_finalized_player_place(long player_index);
extern long __cdecl game_engine_get_finalized_team_place(e_game_team team_index);
extern void __cdecl game_engine_get_multiplayer_string(string_id id, c_static_wchar_string<1024>* out_multiplayer_string);
extern s_simulation_player_netdebug_data const* __cdecl game_engine_get_player_netdebug_data(long player_index);
extern long __cdecl game_engine_get_player_place(long absolute_player_index);
extern long __cdecl game_engine_get_player_score_for_display(long absolute_player_index, bool final_score);
extern e_game_engine_status __cdecl game_engine_get_player_state_index(long player_index);
extern c_game_statborg* __cdecl game_engine_get_statborg();
extern long __cdecl game_engine_get_team_place(long team);
extern long __cdecl game_engine_get_team_score_for_display(long team, bool final_score);
extern void __cdecl game_engine_get_universal_string(long a1, c_static_wchar_string<256>* formatted_string);
extern bool __cdecl game_engine_in_round();
extern void __cdecl game_engine_initialize();
extern void __cdecl game_engine_initialize_for_new_game();
extern void __cdecl game_engine_initialize_for_new_map();
extern void __cdecl game_engine_initialize_for_new_round();
extern void __cdecl game_engine_interface_update(real32 world_seconds_elapsed);
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
extern void __cdecl game_engine_update_global_fade_timers(real32 world_seconds_elapsed);
extern void __cdecl game_engine_update_round_conditions();
extern void __cdecl game_engine_dump_variant_settings(char const* filename);
extern void __cdecl post_game_engine_globals_message(long message_type, char a2, short a3);
extern void __cdecl process_game_engine_globals_messages();

