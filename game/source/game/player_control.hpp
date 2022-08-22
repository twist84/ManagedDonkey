#pragma once

#include "cseries/cseries.hpp"

struct s_player_control_non_deterministic_input_user_state
{
	char __data0[6];
	bool player_input_locked;
	char __data7[0x30 - 7];
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

struct s_aim_assist_targeting_result
{
	bool __unknown0;
	dword target_player_index;
	dword target_object_index;
	dword model_target;
	real primary_autoaim_level;
	real secondary_autoaim_level;
	real_vector3d lead_vector;
	dword_flags flags;
};
static_assert(sizeof(s_aim_assist_targeting_result) == 0x28);

struct s_player_control_internal_state
{
	dword control_flags;
	dword action_flags;
	real_euler_angles2d desired_angles;
	real_point2d throttle;
	real primary_trigger;
	real secondary_trigger;
	word weapon_set_identifier;
	byte primary_weapon_index;
	byte secondary_weapon_index;
	word grenade_index;
	word zoom_level;
	s_player_action_context action_context;
	s_aim_assist_targeting_result targeting;
	bool map_editor_rotation_valid;
	bool map_editor_player_locked_for_manipulation;
	char __data66[2];
	real_euler_angles2d map_editor_rotation;
	word_flags map_editor_flags;
	char __data6A[2];
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
	char __data70[120];
	real_point3d __positionE8;
	char __dataF4[4];
};
static_assert(sizeof(s_player_control) == 0xF8);

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
	char __data8AC[4];
};
static_assert(sizeof(s_player_control_globals) == 0x8B0);

extern s_player_control_globals* player_control_globals_get();
extern inline bool player_control_get_machinima_camera_enabled();
extern inline bool player_control_get_machinima_camera_use_old_controls();
extern inline bool player_control_get_machinima_camera_debug();
extern inline void player_control_set_machinima_camera(bool enabled, bool debug, bool use_old_controls);
extern inline void player_control_toggle_machinima_camera_enabled();
extern inline void player_control_toggle_machinima_camera_use_old_controls();
extern inline void player_control_toggle_machinima_camera_debug();
extern inline void player_control_toggle_player_input(long user_index);