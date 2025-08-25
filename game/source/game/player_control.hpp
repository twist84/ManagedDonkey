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

	// $TODO find any more, for now count is 64
	k_number_of_player_control_action_test_bits = 64
};

enum e_player_control_bit
{
	_player_control_camera_off_bit = 0,

	k_number_of_player_control_bits,
};

struct s_player_control_non_deterministic_input_user_state
{
	// c_flags<e_player_control_action_test_bit, uns64, k_number_of_player_control_action_test_bits> action_test_flags
	uns32 __flags0;
	byte __data4[0x2];
	bool player_input_locked;
	byte __data7[0x1];

	c_flags<e_player_control_action_test_bit, uns64, k_number_of_player_control_action_test_bits> testing_for_action_flags;
	c_flags<e_player_control_action_test_bit, uns64, k_number_of_player_control_action_test_bits> inhibit_button_flags;
	c_flags<e_player_control_bit, uns32, k_number_of_player_control_bits> control_flags;

	// Locks the accept button until the player presses accept
	bool lock_accept_button_until_pressed;

	// Locks the cancel button until the player presses cancel
	bool lock_cancel_button_until_pressed;

	// Locks the y button until the player presses y
	bool lock_y_button_until_pressed;

	// scale input to x strength over y seconds
	bool player_input_scale_in_progress;
	real32 current_input_scale;
	real32 current_input_scale_target;
	real32 current_input_scale_time_remaining;

	byte __pad2C[0x4];
};
static_assert(sizeof(s_player_control_non_deterministic_input_user_state) == 0x30);

struct s_player_interaction
{
	int16 type;

	union
	{
		struct
		{
			int16 seat_index;
		} enter_vehicle;

		struct
		{
			uns8 pick_up_flags;
		} pick_up_weapon;
	} data;

	int32 object_index;
};
static_assert(sizeof(s_player_interaction) == 0x8);

struct s_player_action_context
{
	s_player_interaction interaction;
	int32 melee_target_unit_index;
};
static_assert(sizeof(s_player_action_context) == 0xC);

struct s_player_control_input
{
	real_point2d throttle;
	real32 primary_trigger;
	real32 secondary_trigger;
	real_euler_angles2d facing_delta;
	uns32 unit_control_flags;
	uns32 player_control_flags;
	uns16 player_action_flags;
	bool controller_look_inverted;
	real32 lookstick_pitch;
	s_aim_assist_targeting_result aim_assist_targeting;
};
static_assert(sizeof(s_player_control_input) == 0x50);

struct s_player_control_state
{
	uns32 control_flags;
	uns16 action_flags;
	uns16 pad;
	real_euler_angles2d desired_angles;
	real_point2d throttle;
	real32 primary_trigger;
	real32 secondary_trigger;
	s_unit_weapon_set desired_weapon_set;
	int16 desired_grenade_index;
	int16 desired_zoom_level;
	s_player_action_context action_context;
	s_aim_assist_targeting_result aim_assist_targeting;

	struct
	{
		bool rotation_valid;
		bool player_locked_for_manipulation;
		real_euler_angles2d map_editor_rotation;
		uns16 map_editor_flags;
	} map_editor_data;
};
static_assert(sizeof(s_player_control_state) == 0x6C);

struct s_player_control_output_state
{
	int32 unit_index;
	s_player_control_state output;
};
static_assert(sizeof(s_player_control_output_state) == 0x70);

struct s_player_control_input_state
{
	s_player_control_output_state output;
	bool use_autolevel;
	real32 autolevel_time;
	real_matrix4x3 last_local_physics_transform;
	real32 camera_offset_y;
	real32 camera_offset_z;
	int32 last_local_physics_object_index;
	bool magnetism_active;
	real32 look_yaw_acceleration_time;
	real32 look_pitch_acceleration_time;
	real32 pitch_minimum;
	real32 pitch_maximum;
	bool crouching;
	int16 tracking_crouch_ticks;
	bool tracking_temporary_zoom;
	uns16 saved_zoom_level;
	int16 tracking_temporary_zoom_ticks;
	bool rotate_weapons_in_progress;
	bool rotate_weapons_suppress;
	bool switch_grenade_prev_button_was_down;
	bool switch_grenade_next_button_was_down;
	real32 scripted_set_pitch_velocity;
	int32 scripted_set_pitch_ticks;
	bool gaze_at_point;
	real_point3d gaze_target;
	real32 gaze_max_velocity;
};
static_assert(sizeof(s_player_control_input_state) == 0xF8);

struct player_action
{
	uns32 control_context;
	uns8 control_context_identifier;
	uns32 control_flags;
	real_euler_angles2d angles;
	real_point2d throttle;
	struct
	{
		bool rotation_valid;
		bool player_locked_for_manipulation;
		real_euler_angles2d rotation_yaw_pitch;
		uns16 flags;
	};
	real32 trigger;
	real32 secondary_trigger;
	uns16 action_flags;
	s_unit_weapon_set weapon_set;
	uns16 grenade_index;
	uns16 zoom_level;
	s_player_action_context action_context;
	s_aim_assist_targeting_result targeting;
	bool velocity_exceeds_motion_tracker_threshold;
	uns64 action_test_flags;
};
static_assert(sizeof(player_action) == 0x80);

struct s_player_control_globals
{
	s_player_control_non_deterministic_input_user_state input_user_states[16];
	s_player_control_input_state input_states[4];
	s_player_control_output_state output_state[4];
	real32 maximum_down_pitch_allowed;
	real32 maximum_up_pitch_allowed;
	bool machinima_camera_enabled;
	bool machinima_camera_old_controls;
	bool machinima_camera_debug;
	bool initialized;
};
static_assert(sizeof(s_player_control_globals) == 0x8B0);

struct s_game_input_state;

extern int32 first_input_user();
extern int32 next_input_user(int32 input_user_index);
extern int32 first_output_user();
extern int32 next_output_user(int32 user_index);
extern s_player_control_input_state* player_control_input_get(int32 input_user_index);
extern s_player_control_output_state* player_control_output_get(int32 user_index);
extern void player_control_update_debug_render();
//extern  void __cdecl __tls_set_g_player_control_globals_allocator(void* address)
//extern  void __cdecl __tls_set_g_player_control_globals_deterministic_allocator(void* address);
//extern  void __cdecl clear_player_control_input(s_player_control_input* input);
extern real32 __cdecl evaluate_piecewise_linear_function(int16 count, real32* const function, real32 a3);
//extern void __cdecl player0_controller_set_look_invert(bool);
//extern real32 __cdecl player0_get_looking_pitch();
//extern bool __cdecl player0_looking_down();
//extern bool __cdecl player0_looking_up();
//extern void __cdecl player0_set_pitch(real32, int32);
//extern void __cdecl player1_set_pitch(real32, int32);
//extern void __cdecl player2_set_pitch(real32, int32);
//extern void __cdecl player3_set_pitch(real32, int32);
//extern void __cdecl player_action_test_inhibit_button_flags_set_all_players(e_player_control_action_test_bit, bool);
//extern void __cdecl player_action_test_testing_for_action_flags_set_all_players(e_player_control_action_test_bit, bool);
//extern bool __cdecl player_control_action_test_accept();
//extern bool __cdecl player_control_action_test_action();
//extern bool __cdecl player_control_action_test_any_players(e_player_control_action_test_bit);
//extern bool __cdecl player_control_action_test_any_players_range(e_player_control_action_test_bit, e_player_control_action_test_bit);
//extern bool __cdecl player_control_action_test_back();
//extern bool __cdecl player_control_action_test_cancel();
//extern bool __cdecl player_control_action_test_dpad_down();
//extern bool __cdecl player_control_action_test_dpad_left();
//extern bool __cdecl player_control_action_test_dpad_right();
//extern bool __cdecl player_control_action_test_dpad_up();
//extern bool __cdecl player_control_action_test_grenade_trigger();
//extern void __cdecl player_control_action_test_input(int32, s_game_input_state*, s_player_control_input*);
//extern bool __cdecl player_control_action_test_jump();
//extern bool __cdecl player_control_action_test_left_shoulder();
//extern bool __cdecl player_control_action_test_look_invert();
//extern bool __cdecl player_control_action_test_look_relative_all_directions();
//extern bool __cdecl player_control_action_test_look_relative_down();
//extern bool __cdecl player_control_action_test_look_relative_left();
//extern bool __cdecl player_control_action_test_look_relative_right();
//extern bool __cdecl player_control_action_test_look_relative_up();
//extern bool __cdecl player_control_action_test_lookstick_backward();
//extern bool __cdecl player_control_action_test_lookstick_forward();
//extern bool __cdecl player_control_action_test_melee();
//extern bool __cdecl player_control_action_test_move_relative_all_directions();
//extern bool __cdecl player_control_action_test_primary_trigger();
//extern void __cdecl player_control_action_test_reset();
//extern bool __cdecl player_control_action_test_right_shoulder();
//extern bool __cdecl player_control_action_test_rotate_grenades();
//extern bool __cdecl player_control_action_test_rotate_weapons();
//extern bool __cdecl player_control_action_test_x();
//extern bool __cdecl player_control_action_test_y();
//extern bool __cdecl player_control_action_test_zoom();
extern void __cdecl player_control_build_action(int32 player_index, int32 input_user_index, player_action* action);
//extern bool __cdecl player_control_camera_control_is_active();
//extern bool __cdecl player_control_compute_input_inhibition(int32, e_controller_index, const s_game_input_state*, s_player_control_input*);
//extern void __cdecl player_control_copy_state_from_action(const player_action*, s_player_control_state*);
//extern void __cdecl player_control_copy_state_from_unit(int32, s_player_control_state*);
extern void __cdecl player_control_dispose();
extern void __cdecl player_control_dispose_from_old_map();
//extern void __cdecl player_control_fade_in_all_input(real32);
//extern void __cdecl player_control_fade_in_all_input_for_player(int32, real32);
//extern void __cdecl player_control_fade_out_all_input(real32);
//extern void __cdecl player_control_fade_out_all_input_for_player(int32, real32);
//extern bool __cdecl player_control_flags_test_all_players(e_player_control_bit);
extern const s_player_action_context* __cdecl player_control_get_action_context(int32 user_index);
//extern int32 __cdecl player_control_get_aiming_unit_index(int32);
extern void __cdecl player_control_get_aiming_vector(int32 user_index, real_vector3d* aiming_vector);
extern void __cdecl player_control_get_controller_input(int32 input_user_index, e_controller_index controller_index, real32 world_seconds_elapsed, real32 game_seconds_elapsed, s_game_input_state** input_states, s_player_control_input* input);
//extern const real_euler_angles2d* __cdecl player_control_get_facing_angles(int32);
//extern real32 __cdecl player_control_get_field_of_view(int32);
//extern real32 __cdecl player_control_get_field_of_view_change_time(int32);
extern bool __cdecl player_control_get_machinima_camera_debug();
extern bool __cdecl player_control_get_machinima_camera_enabled();
extern bool __cdecl player_control_get_machinima_camera_use_old_controls();
//extern real32 __cdecl player_control_get_target_autoaim_level(int32);
//extern int32 __cdecl player_control_get_target_player_index(int32);
//extern const s_aim_assist_targeting_result* __cdecl player_control_get_targeting(int32);
//extern void __cdecl player_control_get_unit_camera_info(int32, s_unit_camera_info*);
extern int16 __cdecl player_control_get_zoom_level(int32 user_index);
extern void __cdecl player_control_suppress_rotate_weapons(int32 user_index);
//extern void __cdecl player_control_handle_weapon_put_away(int32, int16);
extern void __cdecl player_control_initialize();
extern void __cdecl player_control_initialize_for_new_map();
//extern void __cdecl player_control_input_set_unit_index(int32, int32);
//extern void __cdecl player_control_lock_gaze(int32, int32, real32);
extern bool __cdecl player_control_machinima_available();
//extern void __cdecl player_control_modify_desired_angles_default(int32, real32, real32, real32, real32);
//extern s_player_control_non_deterministic_input_user_state* __cdecl player_control_non_deterministic_input_user_state_get(int32);
//extern void __cdecl player_control_output_set_unit_index(int32, int32);
//extern void __cdecl player_control_permanent_impulse(int32, const real_euler_angles2d*);
extern void __cdecl player_control_propagate_output(int32 input_user_index);
//extern void __cdecl player_control_scale_all_input(real32, real32);
//extern void __cdecl player_control_scale_all_input_for_player(int32, real32, real32);
//extern void __cdecl player_control_set_deterministic_action_test_flags(int32, uns64);
//extern void __cdecl player_control_set_external_action(int32, const player_action*);
extern void __cdecl player_control_set_facing(int32 input_user_index, const real_vector3d* facing);
//extern void __cdecl player_control_state_build_action(const s_player_control_state*, player_action*);
//extern void __cdecl player_control_state_clear(s_player_control_state*);
//extern void __cdecl player_control_suppress_rotate_weapons(int32);
//extern void __cdecl player_control_unlock_gaze(int32);
//extern void __cdecl player_control_unzoom(int32);
//extern void __cdecl player_control_unzoom_all();
extern void __cdecl player_control_update(real32 world_seconds_elapsed, real32 game_seconds_elapsed);
extern void __cdecl player_control_update_machinima();
extern void __cdecl player_control_update_player(int32 player_index, int32 input_user_index, e_controller_index controller_index, real32 world_seconds_elapsed, real32 game_seconds_elapsed);
//extern void __cdecl player_set_pitch_internal(int32, real32, int32);
//extern bool __cdecl sub_5D5670(e_controller_index, s_player_control_input* input);
//extern bool __cdecl scripted_player_control_set_camera_control(bool);
//extern real32 __cdecl signed_angular_difference(real32, real32);
//extern void __cdecl player_action_test_inhibit_button_flags_set(int32, e_player_control_action_test_bit, bool);
//extern void __cdecl player_action_test_testing_for_action_flags_set(int32, e_player_control_action_test_bit, bool);
//extern bool __cdecl player_control_action_test(int32, e_player_control_action_test_bit);
//extern bool __cdecl player_control_action_test_accept(int32);
//extern bool __cdecl player_control_action_test_action(int32);
//extern bool __cdecl player_control_action_test_back(int32);
//extern bool __cdecl player_control_action_test_cancel(int32);
//extern bool __cdecl player_control_action_test_dpad_down(int32);
//extern bool __cdecl player_control_action_test_dpad_left(int32);
//extern bool __cdecl player_control_action_test_dpad_right(int32);
//extern bool __cdecl player_control_action_test_dpad_up(int32);
//extern bool __cdecl player_control_action_test_grenade_trigger(int32);
//extern bool __cdecl player_control_action_test_left_shoulder(int32);
//extern bool __cdecl player_control_action_test_look_invert(int32);
//extern bool __cdecl player_control_action_test_look_relative_all_directions(int32);
//extern bool __cdecl player_control_action_test_look_relative_down(int32);
//extern bool __cdecl player_control_action_test_look_relative_left(int32);
//extern bool __cdecl player_control_action_test_look_relative_right(int32);
//extern bool __cdecl player_control_action_test_look_relative_up(int32);
//extern bool __cdecl player_control_action_test_lookstick_backward(int32);
//extern bool __cdecl player_control_action_test_lookstick_forward(int32);
//extern bool __cdecl player_control_action_test_melee(int32);
//extern bool __cdecl player_control_action_test_move_relative_all_directions(int32);
//extern bool __cdecl player_control_action_test_range(int32, e_player_control_action_test_bit, e_player_control_action_test_bit);
//extern void __cdecl player_control_action_test_reset(int32);
//extern bool __cdecl player_control_action_right_shoulder(int32);
//extern bool __cdecl player_control_action_test_rotate_grenades(int32);
//extern bool __cdecl player_control_action_test_rotate_weapons(int32);
//extern bool __cdecl player_control_action_test_vision_trigger(int32);
//extern bool __cdecl player_control_action_x(int32);
//extern bool __cdecl player_control_action_y(int32);
//extern void __cdecl player_control_lock_accept_button_until_pressed(int32);
//extern void __cdecl player_control_lock_cancel_button_until_pressed(int32);
//extern void __cdecl player_control_lock_y_button_until_pressed(int32);
//extern bool __cdecl user_currently_piloting_aircraft(int32);

