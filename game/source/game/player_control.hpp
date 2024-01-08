#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "units/units.hpp"

struct s_player_control_globals_deterministic
{
	byte __data[0x80];
};
static_assert(sizeof(s_player_control_globals_deterministic) == 0x80);

struct s_player_control_non_deterministic_input_user_state
{
	byte __data0[0x6];
	bool player_input_locked;
	byte __data7[0x11];
	dword player_control_flags;
	byte __data1C[0x14];
};
static_assert(sizeof(s_player_control_non_deterministic_input_user_state) == 0x30);

struct s_player_interaction
{
	word interaction_type;
	word interaction_seat_index;
	dword interaction_object;
};
static_assert(sizeof(s_player_interaction) == 0x8);

struct s_player_action_context
{
	s_player_interaction interaction;
	dword melee_target_unit;
};
static_assert(sizeof(s_player_action_context) == 0xC);

struct s_player_control_internal_state
{
	dword control_flags;
	dword action_flags;
	euler_angles2d desired_angles;
	real_point2d throttle;
	real primary_trigger;
	real secondary_trigger;
	s_unit_weapon_set weapon_set;
	word grenade_index;
	word zoom_level;
	s_player_action_context action_context;
	s_aim_assist_targeting_result targeting;
	bool map_editor_rotation_valid;
	bool map_editor_player_locked_for_manipulation;
	byte : 8;
	byte : 8;
	euler_angles2d map_editor_rotation;
	word_flags map_editor_flags;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_player_control_internal_state) == 0x6C);

struct s_player_control_state
{
	dword unit_index;
	s_player_control_internal_state internal_state;
};
static_assert(sizeof(s_player_control_state) == 0x70);

struct s_player_control
{
	s_player_control_state state;
	byte __data70[0x68];
	byte __unknownD8;
	bool alert_carry;
	byte __unknownDA;
	byte __unknownDB;
	real __unknownDC;
	long __unknownE0;
	bool gaze_locked;
	real_point3d __positionE8;
	angle gaze_fov; // degrees
};
static_assert(sizeof(s_player_control) == 0xF8);

#pragma pack(push, 4)
struct player_action
{
	dword control_context;
	byte control_context_identifier;
	byte : 8;
	byte : 8;
	byte : 8;
	dword_flags control_flags;
	euler_angles2d angles;
	real_point2d throttle;
	bool map_editor_rotation_valid;
	bool map_editor_player_locked_for_manipulation;
	byte : 8;
	byte : 8;
	euler_angles2d map_editor_rotation;
	word_flags map_editor_flags;
	byte : 8;
	byte : 8;
	real trigger;
	real secondary_trigger;
	word_flags action_flags;
	s_unit_weapon_set weapon_set;
	word grenade_index;
	word zoom_level;
	byte : 8;
	byte : 8;
	s_player_action_context action_context;
	s_aim_assist_targeting_result targeting;
	bool velocity_exceeds_motion_tracker_threshold;
	byte : 8;
	byte : 8;
	byte : 8;
	qword action_test_flags;
};
static_assert(sizeof(player_action) == 0x80);
#pragma pack(pop)

struct s_player_control_globals
{
	s_player_control_non_deterministic_input_user_state input_user_states[16];
	s_player_control controls[4];
	s_player_control_state control_states[4];
	dword __unknown8A0;
	dword __unknown8A4;
	bool machinima_camera_enabled;
	bool machinima_camera_use_old_controls;
	bool machinima_camera_debug;
	bool __unknown8AB;
	byte __data8AC[0x4];
};
static_assert(sizeof(s_player_control_globals) == 0x8B0);

extern s_player_control_globals* __cdecl player_control_globals_get();
extern bool __cdecl player_control_get_machinima_camera_enabled();
extern bool __cdecl player_control_get_machinima_camera_use_old_controls();
extern bool __cdecl player_control_get_machinima_camera_debug();
extern void __cdecl player_control_set_machinima_camera(bool enabled, bool debug, bool use_old_controls);
extern void __cdecl player_control_toggle_machinima_camera_enabled();
extern void __cdecl player_control_toggle_machinima_camera_use_old_controls();
extern void __cdecl player_control_toggle_machinima_camera_debug();
extern void __cdecl player_control_toggle_player_input(long user_index);
extern long __cdecl player_mapping_get_input_user(long user_index);
extern short __cdecl player_control_get_zoom_level(long user_index);
extern void __cdecl player_control_suppress_rotate_weapons(long user_index);
extern void __cdecl player_control_update(real world_seconds_elapsed, real game_seconds_elapsed);

