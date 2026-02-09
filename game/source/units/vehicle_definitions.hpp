#pragma once

#include "cseries/cseries.hpp"
#include "physics/havok_vehicle_physics_definitions.hpp"
#include "tag_files/tag_groups.hpp"
#include "units/unit_definition.hpp"

enum e_vehicle_definition_flags
{
	_vehicle_frictionless_with_driver_bit = 0,
	_vehicle_can_trigger_automaticly_opening_doors_bit,
	_vehicle_autoaim_when_teamless_bit,
	_vehicle_ai_weapon_cannot_rotate_bit,
	_vehicle_ai_does_not_require_driver_bit,
	_vehicle_ai_driver_enable_bit,
	_vehicle_ai_driver_flying_bit,
	_vehicle_ai_driver_nondirectional_bit,
	_vehicle_ai_driver_hovering_bit,
	_vehicle_noncombat_bit,
	_vehicle_does_not_cause_collision_damage_bit,
	_vehicle_in_huge_vehicle_physics_group_bit,
	_vehicle_allows_wheelie_popping_bit,
	_vehicle_ai_auto_turret_bit,

	k_vehicle_definition_flags_count
};

enum e_vehicle_type
{
	_vehicle_type_human_tank = 0,
	_vehicle_type_human_jeep,
	_vehicle_type_human_plane,
	_vehicle_type_alien_scout,
	_vehicle_type_alien_fighter,
	_vehicle_type_turret,
	_vehicle_type_mantis,
	_vehicle_type_vtol,
	_vehicle_type_chopper,
	_vehicle_type_guardian,

	k_vehicle_type_count,

	_vehicle_mask_ground = FLAG(_vehicle_type_human_tank) | FLAG(_vehicle_type_human_jeep) | FLAG(_vehicle_type_alien_scout) | FLAG(_vehicle_type_chopper),
	_vehicle_mask_flying = FLAG(_vehicle_type_human_plane) | FLAG(_vehicle_type_alien_fighter) | FLAG(_vehicle_type_vtol)
};
COMPILE_ASSERT(0b0000000100001011 == _vehicle_mask_ground);
COMPILE_ASSERT(0b0000000010010100 == _vehicle_mask_flying);

struct s_vehicle_human_tank_definition;
struct s_vehicle_human_jeep_definition;
struct s_vehicle_human_plane_definition;
struct s_vehicle_alien_scout_definition;
struct s_vehicle_alien_fighter_definition;
struct s_vehicle_turret_definition;
struct s_vehicle_mantis_definition;
struct s_vehicle_vtol_definition;
struct s_vehicle_chopper_definition;
struct s_vehicle_guardian_definition;
struct s_vehicle_physics_types
{
	c_static_array<s_tag_block, k_vehicle_type_count> tag_blocks;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_vehicle_physics_types) == 0x78);

struct s_toruqe_curve_definition
{
	real32 min_torque;
	real32 max_torque;
	real32 peak_torque_scale;
	real32 past_peak_torque_exponent;

	// generally 0 for loading torque and something less than max torque for cruising torque
	real32 torque_at_max_angular_velocity;
	real32 torque_at_2x_max_angular_velocity;
};
COMPILE_ASSERT(sizeof(s_toruqe_curve_definition) == 0x18);

struct s_vehicle_gear_definition
{
	// loaded torque
	s_toruqe_curve_definition loaded_torque_curve;

	// cruising torque
	s_toruqe_curve_definition cruising_torque_curve;

	// gearing
	real32 min_time_to_upshift;
	real32 engine_up_shift_scale;
	real32 gear_ratio;
	real32 min_time_to_downshift;
	real32 engine_down_shift_scale;
};
COMPILE_ASSERT(sizeof(s_vehicle_gear_definition) == 0x44);

struct s_vehicle_engine_definition
{
	// engine

	real32 engine_moment;
	real32 engine_max_angular_velocity;
	c_typed_tag_block<s_vehicle_gear_definition> gears;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> gear_shift_sound;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_vehicle_engine_definition) == 0x24);

struct s_vehicle_human_tank_definition
{
	real32 forward_arc;
	real32 forward_turn_scale;
	real32 reverse_turn_scale;


	// forward differential
	// how quickly the differential reaches it's target and what the ratio is

	real32 maximum_left_differential;
	real32 maximum_right_differential;
	real32 differential_acceleration;
	real32 differential_deceleration;


	// reverse differential
	// how quickly the differential reaches it's target and what the ratio is

	real32 maximum_left_reverse_differential;
	real32 maximum_right_reverse_differential;
	real32 differential_reverse_acceleration;
	real32 differential_reverse_deceleration;
	s_vehicle_engine_definition engine;

	// wheel circumferance
	real32 wheel_circumferance;
	real32 gravity_adjust;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_vehicle_human_tank_definition) == 0x58);

struct s_vehicle_steering_control_definition
{
	// steering overdampening
	// when the steering is off by more than the cusp angle
	// the steering will overcompensate more and more.  when it
	// is less, it overcompensates less and less.  the exponent
	// should be something in the neighborhood of 2.0

	real32 overdampen_cusp_angle; // degrees
	real32 overdampen_exponent;
};
COMPILE_ASSERT(sizeof(s_vehicle_steering_control_definition) == 0x8);

struct s_vehicle_turning_control_definition
{
	// turning

	real32 maximum_left_turn;
	real32 maximum_right_turn_negative;
	real32 turn_rate;
};
COMPILE_ASSERT(sizeof(s_vehicle_turning_control_definition) == 0xC);

struct s_vehicle_human_jeep_definition
{
	s_vehicle_steering_control_definition steering_control;
	s_vehicle_turning_control_definition turning_control;
	s_vehicle_engine_definition engine;

	// wheel circumferance
	real32 wheel_circumferance;
	real32 gravity_adjust;
};
COMPILE_ASSERT(sizeof(s_vehicle_human_jeep_definition) == 0x40);

struct s_vehicle_steering_animation_definition
{
	// steering animation and interpolation

	// This data controls the depiction of steering
	// through animation - not the vehicles ability to turn
	// interpolation scale= how much we interpolate between steering poses.
	// max angle= max animated steering angle change allowed per tick.

	// 1= heavy interp. of steering animations
	real32 interpolation_scale;

	// non-zero= max angle delta per frame
	real32 max_angle;
};
COMPILE_ASSERT(sizeof(s_vehicle_steering_animation_definition) == 0x8);

struct s_vehicle_human_plane_definition
{
	// velocity control variables

	real32 maximum_forward_speed;
	real32 maximum_reverse_speed;
	real32 speed_acceleration;
	real32 speed_deceleration;
	real32 maximum_left_slide;
	real32 maximum_right_slide;
	real32 slide_acceleration;
	real32 slide_deceleration;
	real32 maximum_up_rise;
	real32 maximum_down_rise;
	real32 rise_acceleration;
	real32 rise_deceleration;


	// human plane tuning variables

	real32 flying_torque_scale;
	real32 air_friction_deceleration;
	real32 thrust_scale;
	real32 turn_rate_scale_when_boosting;
	real32 maximum_roll;
	s_vehicle_steering_animation_definition steering_animation;
};
COMPILE_ASSERT(sizeof(s_vehicle_human_plane_definition) == 0x4C);

struct s_engine_function_definition
{
	// this is the name of the region by which we gauge the overall damage of the vehicle
	c_string_id object_function_damage_region;

	// speed at which engine position funciton  moves.
	// value of 1 means goes from 0-1 in 1 second
	real32 min_anti_gravity_engine_speed;

	// speed at which engine position funciton  moves.
	// value of 1 means goes from 0-1 in 1 second
	real32 max_anti_gravity_engine_speed;

	// strictly used for object funtion. in 0-1 space
	real32 engine_speed_acceleration;

	// function is capped by speed of the vehicle.
	// So when we slow down for any reason we see the function go down
	real32 maximum_vehicle_speed;
};
COMPILE_ASSERT(sizeof(s_engine_function_definition) == 0x14);

struct physics_variable_speed
{
	real32 maximum_forward_speed;
	real32 maximum_reverse_speed;
	real32 acceleration;
	real32 deceleration;
};
COMPILE_ASSERT(sizeof(physics_variable_speed) == 0x10);

struct s_vehicle_alien_scout_definition
{
	enum e_flags
	{
		_locked_camera_bit = 0,

		k_flags_count
	};

	enum e_specific_type
	{
		_specific_type_none = 0,
		_specific_type_ghost,
		_specific_type_spectre,
		_specific_type_wraith,
		_specific_type_hover_craft,

		k_specific_type_count
	};

	s_vehicle_steering_control_definition steering;

	// velocity control variables
	physics_variable_speed speed;
	physics_variable_speed slide;

	// specific types
	// different types are treated differently alien scout controller
	c_enum<e_specific_type, int8, _specific_type_none, k_specific_type_count> specific_type;

	c_flags<e_flags, uns8, k_flags_count> flags;
	int8 pad[0x2];

	real32 drag_coeficient;
	real32 constant_deceleration;

	// 0 defaults to 1
	real32 torque_scale;

	// engine object function
	s_engine_function_definition engine_function_definition;

	// contrail object function
	s_engine_function_definition contrail_function_definition;

	// engine rotation function
	real32 gear_cycle_rate_min;
	real32 gear_cycle_rate_max;

	s_vehicle_steering_animation_definition steering_animation;
};
COMPILE_ASSERT(sizeof(s_vehicle_alien_scout_definition) == 0x70);

struct s_vehicle_alien_fighter_definition
{
	s_vehicle_steering_control_definition steering_control;
	s_vehicle_turning_control_definition turning_control;


	// velocity control variables

	real32 maximum_forward_speed;
	real32 maximum_reverse_speed;
	real32 speed_acceleration;
	real32 speed_deceleration;
	real32 maximum_left_slide;
	real32 maximum_right_slide;
	real32 slide_acceleration;
	real32 slide_deceleration;


	// torque scale
	// how hard the vehicle trys to rotate to it's desired rotation

	real32 flying_torque_scale;


	// fixed gun offset
	// this offset will be aligned to the units aiming vector instead of the vehicle forward vector

	real32 fixed_gun_yaw;
	real32 fixed_gun_pitch;


	// alien fighter trick variables

	real32 maximum_trick_frequency;
	real32 loop_trick_duration;
	real32 roll_trick_duration;


	// alien fighter fake flight control

	real32 zero_gravity_speed;
	real32 full_gravity_speed;
	real32 strafe_boost_scale;
	real32 off_stick_deceleration_scale;
	real32 cruising_throttle;
	real32 dive_speed_scale;
};
COMPILE_ASSERT(sizeof(s_vehicle_alien_fighter_definition) == 0x64);

struct s_vehicle_turret_definition
{
	int32 pad[0x1];
};
COMPILE_ASSERT(sizeof(s_vehicle_turret_definition) == 0x4);

enum e_walker_physics_leg_group
{
	_walker_physics_leg_group_primary = 0,
	_walker_physics_leg_group_secondary,

	k_walker_physics_leg_group_count
};

enum e_walker_physics_leg_side
{
	_walker_physics_leg_side_left = 0,
	_walker_physics_leg_side_right,

	k_walker_physics_leg_side_count
};

enum e_walker_physics_leg_flags
{
	_walker_physics_leg_flag_constrained_plant_bit = 0,

	k_walker_physics_leg_flags
};

struct s_walker_physics_leg
{
	// walker physics

	c_enum<e_walker_physics_leg_group, int8, _walker_physics_leg_group_primary, k_walker_physics_leg_group_count> leg_group;
	c_enum<e_walker_physics_leg_side, int8, _walker_physics_leg_side_left, k_walker_physics_leg_side_count> leg_side;
	int8 leg_side_order;
	bool valid;
	c_string_id hip_node_a_name;
	c_string_id hip_node_b_name;
	c_string_id knee_node_a_name;
	c_string_id knee_node_b_name;
	c_string_id foot_marker_group_name;
	int32 pad2[0xF];
	c_flags<e_walker_physics_leg_flags, int32, k_walker_physics_leg_flags> flags;
	real_vector3d initial_hip_to_foot_offset;
	real_vector3d initial_origin_to_hip_offset;
	real32 upper_leg_length;
	real32 lower_leg_length;
	int16 hip_node_a_index;
	int16 hip_node_b_index;
	int16 knee_node_a_index;
	int16 knee_node_b_index;
	int16 runtime_foot_marker_group_index;
	int16 runtime_foot_node_index;
	int16 runtime_hip_node_index;
	int16 runtime_knee_node_index;
	real_point3d plant_constraint_position;
	int32 pad3[0x3];
};
COMPILE_ASSERT(sizeof(s_walker_physics_leg) == 0xA0);

struct s_walker_physics_definition
{
	real_vector3d maximum_leg_motion;
	real32 maximum_turn;
	c_typed_tag_block<s_walker_physics_leg> legs;
	real32 leg_apex_fraction;
	real32 lift_exponent;
	real32 drop_exponent;
	real_vector3d object_space_pivot_position;
};
COMPILE_ASSERT(sizeof(s_walker_physics_definition) == 0x34);

struct s_vehicle_mantis_definition
{
	s_vehicle_steering_control_definition steering_control;
	s_vehicle_turning_control_definition turning_control;


	// velocity control variables

	real32 maximum_forward_speed;
	real32 maximum_reverse_speed;
	real32 speed_acceleration;
	real32 speed_deceleration;
	real32 maximum_left_slide;
	real32 maximum_right_slide;
	real32 slide_acceleration;
	real32 slide_deceleration;
	s_walker_physics_definition walker_physics;

	// fraction of walkcycle at end for pause
	real32 walk_cycle_pause;

	// number of legs mantis needs planted to be considered stable.
	int16 stable_planted_legs;
	int16 pad;

	real32 time_without_plant_buffer; // seconds
	real32 not_along_up_gravity_scale; // 0-1
	real32 speed_acceleration_limit;
	real32 speed_acceleration_match_scale;
	real32 slide_acceleration_limit;
	real32 slide_acceleration_match_scale;
	real32 turn_acceleration_limit;
	real32 turn_acceleration_match_scale;


	// jumping

	real32 jump_set_time; // seconds

	// 0-1, portion of set time spent interpolating into neutral stance
	real32 jump_set_interpolation_fraction;

	real32 jump_leap_time; // seconds
	real32 jump_recovery_time; // seconds

	// 0-1, portion of recovery time spent interpolating into neutral stance
	real32 jump_recovery_fraction;

	// WU, amount foot moves up to get ready to jump
	real32 jump_leg_set_distance;

	// WU, amount foot moves down when jumping
	real32 jump_leg_distance;
};
COMPILE_ASSERT(sizeof(s_vehicle_mantis_definition) == 0xAC);

struct s_vehicle_vtol_definition
{
	s_vehicle_turning_control_definition turning_control;
	c_string_id left_lift_marker;
	c_string_id right_lift_marker;
	c_string_id thrust_marker;


	// rise angles

	real32 full_lift_angle_up;
	real32 lift_dead_angle_up;
	real32 full_lift_angle_down;
	real32 lift_dead_angle_down;


	// elevation lag
	// minimum is how much you can lag with with out acceleration and maximum is lag with acceleration

	real32 minimum_rise_target_lag;
	real32 maximum_rise_target_lag;


	// minimum and maximum up acceleration

	real32 minimum_up_acceleration;
	real32 maximum_up_acceleration;


	// turn, left and forward accelerations

	real32 maximum_turn_acceleration;
	real32 turn_acceleration_gain;
	real32 rotor_damping;
	real32 maximum_left_acceleration;
	real32 maximum_forward_acceleration;


	// lift arm pivot
	// this simulates the engines being on a pivot like on an osprey.  This helps better cradle the center of mass.

	real32 lift_arm_pivot_length;
	real32 drag_coeficient;
	real32 constant_deceleration;
	real32 magic_angular_acc_exp;
	real32 magic_angular_acc_scale;
	real32 magic_angular_acc_k;
	real32 lift_angles_acc;
	real32 render_lift_angles_acc;


	// prop rotation
	// propeller speed data to drive propeller animations

	real_bounds prop_rotation_speed;
};
COMPILE_ASSERT(sizeof(s_vehicle_vtol_definition) == 0x74);

struct s_vehicle_chopper_definition
{
	s_vehicle_steering_control_definition steering_control;
	s_vehicle_turning_control_definition turning_control;
	s_vehicle_engine_definition engine;

	// wheel circumferance

	real32 wheel_circumferance;
	c_string_id rotation_marker;
	real32 magic_turning_scale;
	real32 magic_turning_acc;
	real32 magic_turning_max_vel;
	real32 magic_turning_exponent;
	real32 bank_to_slide_ratio;
	real32 bank_slide_exponent;
	real32 bank_to_turn_ratio;
	real32 bank_turn_exponent;
	real32 bank_fraction;
	real32 bank_rate;
	real32 wheel_accel;
	real32 gyroscopic_damping;
};
COMPILE_ASSERT(sizeof(s_vehicle_chopper_definition) == 0x70);

struct s_vehicle_guardian_definition
{
	s_vehicle_steering_control_definition steering_control;
	real32 maximum_forward_speed;
	real32 maximum_reverse_speed;
	real32 speed_acceleration;
	real32 speed_deceleration;
	real32 maximum_left_slide;
	real32 maximum_right_slide;
	real32 slide_acceleration;
	real32 slide_deceleration;
	real32 torque_scale;
	real32 anti_gravity_force_z_offset;
};
COMPILE_ASSERT(sizeof(s_vehicle_guardian_definition) == 0x30);

enum e_player_training_vehicle_type
{
	_player_training_vehicle_type_none = 0,
	_player_training_vehicle_type_warthog,
	_player_training_vehicle_type_warthog_turret,
	_player_training_vehicle_type_ghost,
	_player_training_vehicle_type_banshee,
	_player_training_vehicle_type_tank,
	_player_training_vehicle_type_wraith,

	k_player_training_vehicle_type_count
};

enum e_vehicle_size
{
	_vehicle_size_small = 0,
	_vehicle_size_large,

	k_vehicle_size_count
};

struct _vehicle_definition
{
	// $$$ VEHICLE $$$

	c_flags<e_vehicle_definition_flags, uns32, k_vehicle_definition_flags_count> flags;

	// physics type
	// define one of the following blocks for the type of physics you wish this vehicle to have.
	s_vehicle_physics_types physics_types;

	// friction and antigravity points
	s_havok_vehicle_physics_definition physics;

	c_enum<e_player_training_vehicle_type, int8, _player_training_vehicle_type_none, k_player_training_vehicle_type_count> training_type;

	// The size determine what kind of seats in larger vehicles it may occupy (i.e. small or large cargo seats)
	c_enum<e_vehicle_size, int8, _vehicle_size_small, k_vehicle_size_count> vehicle_size;
	int16 pad;

	real32 minimum_flipping_angular_velocity;
	real32 maximum_flipping_angular_velocity;
	real32 crouch_transition_time; // seconds
	real32 runtime_crouch_transition_velocity;

	// how much do we scale the force the vehicle the applies down on the seat when he enters. 0 == no acceleration
	real32 seat_enterance_acceleration_scale;

	// how much do we scale the force the vehicle the applies down on the seat when he exits. 0 == no acceleration
	real32 seat_exit_accelersation_scale;

	real32 blur_speed;
	c_string_id flip_message;

	// sounds and effects

	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> suspension_sound;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> special_effect;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, DAMAGE_RESPONSE_DEFINITION_TAG, INVALID_TAG> driver_boost_damage_effect_or_response;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, DAMAGE_RESPONSE_DEFINITION_TAG, INVALID_TAG> rider_boost_damage_effect_or_response;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(_vehicle_definition) == 0x140);

typedef struct vehicle_definition
{
	static tag const k_group_tag = VEHICLE_TAG;

	_object_definition object;
	_unit_definition unit;
	_vehicle_definition vehicle;

	void update_reference_names();
} s_vehicle_definition;
COMPILE_ASSERT(sizeof(vehicle_definition) == sizeof(_object_definition) + sizeof(_unit_definition) + sizeof(_vehicle_definition));

extern void __cdecl vehicle_definition_fixup(int32 vehicle_definition_index);
extern e_vehicle_type __cdecl vehicle_definition_get_default_type(int32 vehicle_definition_index);
extern void __cdecl vehicle_definition_teardown(int32 vehicle_definition_index);
extern const s_vehicle_alien_fighter_definition* __cdecl vehicle_get_alien_fighter_definition(int32 vehicle_definition_index);
extern const s_vehicle_alien_scout_definition* __cdecl vehicle_get_alien_scout_definition(int32 vehicle_definition_index);
extern const s_vehicle_chopper_definition* __cdecl vehicle_get_chopper_definition(int32 vehicle_definition_index);
extern const s_vehicle_guardian_definition* __cdecl vehicle_get_guardian_definition(int32 vehicle_definition_index);
extern const s_vehicle_human_jeep_definition* __cdecl vehicle_get_human_jeep_definition(int32 vehicle_definition_index);
extern const s_vehicle_human_plane_definition* __cdecl vehicle_get_human_plane_definition(int32 vehicle_definition_index);
extern const s_vehicle_human_tank_definition* __cdecl vehicle_get_human_tank_definition(int32 vehicle_definition_index);
extern const s_vehicle_mantis_definition* __cdecl vehicle_get_mantis_definition(int32 vehicle_definition_index);
extern const s_vehicle_turret_definition* __cdecl vehicle_get_turret_definition(int32 vehicle_definition_index);
extern const s_vehicle_vtol_definition* __cdecl vehicle_get_vtol_definition(int32 vehicle_definition_index);

