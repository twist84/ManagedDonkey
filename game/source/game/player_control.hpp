#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "units/units.hpp"

struct s_player_control_globals_deterministic
{
	byte __data[0x80];
};
static_assert(sizeof(s_player_control_globals_deterministic) == 0x80);

enum e_player_control_action_test_bit
{
	// true if any player has hit the action key since the last call to (player_action_test_reset).
	_player_control_action_test_action_bit = 0,

	// true if any player has jumped since the last call to (player_action_test_reset).
	_player_control_action_test_jump_bit,

	// true if any player has hit accept since the last call to (player_action_test_reset).
	_player_control_action_test_accept_bit,

	// true if any player has hit cancel key since the last call to (player_action_test_reset).
	_player_control_action_test_cancel_bit,

	// true if any player has used primary trigger since the last call to (player_action_test_reset).
	_player_control_action_test_primary_trigger_bit,

	// true if any player has used grenade trigger since the last call to (player_action_test_reset).
	_player_control_action_test_grenade_trigger_bit,

	// true if any player has hit the melee button since the last call to (player_action_test_reset).
	_player_control_action_test_melee_bit,

	// true if any player has hit the rotate-weapon button since the last call to (player_action_test_reset).
	_player_control_action_test_rotate_weapons_bit,

	// true if any player has hit the rotate-grenades button since the last call to (player_action_test_reset).
	_player_control_action_test_rotate_grenades_bit,

	// true if any player has hit the zoom button since the last call to (player_action_test_reset).
	_player_control_action_test_zoom_bit,

	// true if any player has looked up since the last call to (player_action_test_reset).
	_player_control_action_test_look_relative_up_bit,

	// true if any player has looked down since the last call to (player_action_test_reset).
	_player_control_action_test_look_relative_down_bit,

	// true if any player has looked left since the last call to (player_action_test_reset).
	_player_control_action_test_look_relative_left_bit,

	// true if any player has looked right since the last call to (player_action_test_reset).
	_player_control_action_test_look_relative_right_bit,

	// true if any player has moved forward since the last call to (player_action_test_reset).
	_player_control_action_test_move_relative_forward_bit,

	// true if any player has moved backward since the last call to (player_action_test_reset).
	_player_control_action_test_move_relative_backward_bit,

	// true if any player has moved left since the last call to (player_action_test_reset).
	_player_control_action_test_move_relative_left_bit,

	// true if any player has moved right since the last call to (player_action_test_reset).
	_player_control_action_test_move_relative_right_bit,

	// true if any player has pressed the back button since the last call to (player_action_test_reset).
	_player_control_action_test_back_bit,

	// true if any player has used vision trigger since the last call to (player_action_test_reset).
	_player_control_action_test_vision_trigger_bit,

	// true if the first player pushed forward on lookstick
	_player_control_action_test_lookstick_forward_bit,

	// true if the first player pushed backward on lookstick
	_player_control_action_test_lookstick_backward_bit,

	// true if any player has pressed the dpad left button since the last call to (player_action_test_reset).
	_player_control_action_test_dpad_left_bit,

	// true if any player has pressed the dpad right button since the last call to (player_action_test_reset).
	_player_control_action_test_dpad_right_bit,

	// true if any player has pressed the dpad up button since the last call to (player_action_test_reset).
	_player_control_action_test_dpad_up_bit,

	// true if any player has pressed the dpad down button since the last call to (player_action_test_reset).
	_player_control_action_test_dpad_down_bit,

	// true if player0's look pitch is inverted
	_player_control_action_test_look_invert_bit,

	// true if any player has pressed the x button since the last call to (player_action_test_reset).
	_player_control_action_test_x_bit,

	// true if any player has pressed the y button since the last call to (player_action_test_reset).
	_player_control_action_test_y_bit,

	// true if any player has pressed the left shoulder button since the last call to (player_action_test_reset).
	_player_control_action_test_left_shoulder_bit,

	// true if any player has pressed the right shoulder button since the last call to (player_action_test_reset).
	_player_control_action_test_right_shoulder_bit,

	// true if any player has pressed the button action to activate the waypoint since the last call to (player_action_test_reset).
	_player_control_action_test_unkown31_bit, // is this waypoint_activate?

	// are there more than 32?
	_player_control_action_test_unkown32_bit,

	// #TODO: find any more, for now count is 64
	k_player_control_action_test_bits = 64
};

enum e_player_control_bit
{
	_player_control_camera_control_enabled_bit = 0,

	k_player_control_bits,
};

struct s_player_control_non_deterministic_input_user_state
{
	// c_flags<e_player_control_action_test_bit, qword, k_player_control_action_test_bits> flags
	dword_flags __flags0;
	byte __data4[0x2];
	bool player_input_locked;
	byte __data7[0x1];

	c_flags<e_player_control_action_test_bit, qword, k_player_control_action_test_bits> testing_for_action_flags;
	c_flags<e_player_control_action_test_bit, qword, k_player_control_action_test_bits> inhibit_button_flags;
	c_flags<e_player_control_bit, dword, k_player_control_bits> player_control_flags;

	// Locks the accept button until the player presses accept
	bool lock_accept_button_until_pressed;

	// Locks the cancel button until the player presses cancel
	bool lock_cancel_button_until_pressed;

	// Locks the y button until the player presses y
	bool lock_y_button_until_pressed;

	// scale input to x strength over y seconds
	bool input_scale_enabled;
	real input_scale_strength0;
	real input_scale_strength1;
	real input_scale_time_seconds;

	byte __pad2C[0x4];
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

struct s_player_control_input
{
	real_point2d throttle;
	real primary_trigger;
	real secondary_trigger;
	euler_angles2d desired_angles;
	dword_flags control_flags;
	dword_flags __flags1C;
	word_flags action_flags;
	bool controller_look_inverted;
	real __unknown24;
	s_aim_assist_targeting_result targeting;
};
static_assert(sizeof(s_player_control_input) == 0x50);

struct s_player_control_state
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
static_assert(sizeof(s_player_control_state) == 0x6C);

struct s_player_control_input_state
{
	dword unit_index;
	s_player_control_state state;
};
static_assert(sizeof(s_player_control_input_state) == 0x70);

struct s_player_control
{
	s_player_control_input_state input_state;
	bool __unknown70;
	real __unknown74;
	real_matrix4x3 matrix;
	real __unknownAC;
	real __unknownB0;
	int object_index;
	bool __unknownB8[4];
	real __unknown8C;
	real __unknown90;
	euler_angles2d __anglesC4;
	byte __dataCC[4];
	bool __unknownD0;
	byte __dataD1[5];
	bool __unknownD6;
	bool __unknownD7;
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
	s_player_control_input_state control_states[4];
	dword __unknown8A0;
	dword __unknown8A4;
	bool machinima_camera_enabled;
	bool machinima_camera_use_old_controls;
	bool machinima_camera_debug;
	bool __unknown8AB;
	byte __data8AC[0x4];
};
static_assert(sizeof(s_player_control_globals) == 0x8B0);

extern bool __cdecl player_control_get_machinima_camera_enabled();
extern bool __cdecl player_control_get_machinima_camera_use_old_controls();
extern bool __cdecl player_control_get_machinima_camera_debug();
extern void __cdecl player_control_set_machinima_camera(bool enabled, bool debug, bool use_old_controls);
extern void __cdecl player_control_toggle_machinima_camera_enabled();
extern void __cdecl player_control_toggle_machinima_camera_use_old_controls();
extern void __cdecl player_control_toggle_machinima_camera_debug();
extern void __cdecl player_control_toggle_player_input(long user_index);
extern long __cdecl player_mapping_get_input_user(long player_index);
extern short __cdecl player_control_get_zoom_level(long user_index);
extern void __cdecl player_control_suppress_rotate_weapons(long user_index);
extern void __cdecl player_control_update(real world_seconds_elapsed, real game_seconds_elapsed);

