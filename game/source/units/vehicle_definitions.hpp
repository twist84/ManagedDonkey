#pragma once

#include "cseries/cseries.hpp"
#include "physics/havok_vehicle_physics_definitions.hpp"
#include "tag_files/tag_groups.hpp"
#include "units/unit_definition.hpp"

enum e_vehicle_definition_flags
{
	_vehicle_definition_flag_no_friction_w_driver_bit = 0,
	_vehicle_definition_flag_can_trigger_automatic_opening_doors_bit,
	_vehicle_definition_flag_autoaim_when_teamless_bit,
	_vehicle_definition_flag_ai_weapon_cannot_rotate_bit,
	_vehicle_definition_flag_ai_does_not_require_driver_bit,
	_vehicle_definition_flag_ai_driver_enable_bit,
	_vehicle_definition_flag_ai_driver_flying_bit,
	_vehicle_definition_flag_ai_driver_can_sidestep_bit,
	_vehicle_definition_flag_ai_driver_hovering_bit,
	_vehicle_definition_flag_noncombat_vehicle_bit,
	_vehicle_definition_flag_causes_collision_damage_bit,
	_vehicle_definition_flag_huge_vehicle_physics_group_bit,
	_vehicle_definition_flag_enable_wheelie_popping_hack_bit,
	_vehicle_definition_flag_ai_auto_turret_bit,

	k_vehicle_definition_flags
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

	k_vehicle_type_count
};

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
	//c_static_array<s_tag_block, k_vehicle_type_count> type;

	c_typed_tag_block<s_vehicle_human_tank_definition> type_human_tank;
	c_typed_tag_block<s_vehicle_human_jeep_definition> type_human_jeep;
	c_typed_tag_block<s_vehicle_human_plane_definition> type_human_plane;
	c_typed_tag_block<s_vehicle_alien_scout_definition> type_alien_scout;
	c_typed_tag_block<s_vehicle_alien_fighter_definition> type_alien_fighter;
	c_typed_tag_block<s_vehicle_turret_definition> type_turret;
	c_typed_tag_block<s_vehicle_mantis_definition> type_mantis;
	c_typed_tag_block<s_vehicle_vtol_definition> type_vtol;
	c_typed_tag_block<s_vehicle_chopper_definition> type_chopper;
	c_typed_tag_block<s_vehicle_guardian_definition> type_guardian;

	void update_reference_names();
};
static_assert(sizeof(s_vehicle_physics_types) == 0x78);

struct s_toruqe_curve_definition
{
	real min_torque;
	real max_torque;
	real peak_torque_scale;
	real past_peak_torque_exponent;

	// generally 0 for loading torque and something less than max torque for cruising torque
	real torque_at_max_angular_velocity;
	real torque_at_2x_max_angular_velocity;
};
static_assert(sizeof(s_toruqe_curve_definition) == 0x18);

struct s_vehicle_gear_definition
{
	// loaded torque
	s_toruqe_curve_definition loaded_torque_curve;

	// cruising torque
	s_toruqe_curve_definition cruising_torque_curve;

	// gearing
	real min_time_to_upshift;
	real engine_up_shift_scale;
	real gear_ratio;
	real min_time_to_downshift;
	real engine_down_shift_scale;
};
static_assert(sizeof(s_vehicle_gear_definition) == 0x44);

struct s_vehicle_engine_definition
{
	// engine

	real engine_moment;
	real engine_max_angular_velocity;
	c_typed_tag_block<s_vehicle_gear_definition> gears;
	c_typed_tag_reference<SOUND_TAG> gear_shift_sound;

	void update_reference_names();
};
static_assert(sizeof(s_vehicle_engine_definition) == 0x24);

struct s_vehicle_human_tank_definition
{
	angle forward_arc;
	real forward_turn_scale;
	real reverse_turn_scale;


	// forward differential
	// how quickly the differential reaches it's target and what the ratio is

	real maximum_left_differential;
	real maximum_right_differential;
	real differential_acceleration;
	real differential_deceleration;


	// reverse differential
	// how quickly the differential reaches it's target and what the ratio is

	real maximum_left_reverse_differential;
	real maximum_right_reverse_differential;
	real differential_reverse_acceleration;
	real differential_reverse_deceleration;
	s_vehicle_engine_definition engine;

	// wheel circumferance
	real wheel_circumferance;
	real gravity_adjust;

	void update_reference_names();
};
static_assert(sizeof(s_vehicle_human_tank_definition) == 0x58);

struct s_vehicle_steering_control_definition
{
	// steering overdampening
	// when the steering is off by more than the cusp angle
	// the steering will overcompensate more and more.  when it
	// is less, it overcompensates less and less.  the exponent
	// should be something in the neighborhood of 2.0

	real overdampen_cusp_angle; // degrees
	real overdampen_exponent;
};
static_assert(sizeof(s_vehicle_steering_control_definition) == 0x8);

struct s_vehicle_turning_control_definition
{
	// turning

	real maximum_left_turn;
	real maximum_right_turn_negative;
	real turn_rate;
};
static_assert(sizeof(s_vehicle_turning_control_definition) == 0xC);

struct s_vehicle_human_jeep_definition
{
	s_vehicle_steering_control_definition steering_control;
	s_vehicle_turning_control_definition turning_control;
	s_vehicle_engine_definition engine;

	// wheel circumferance
	real wheel_circumferance;
	real gravity_adjust;
};
static_assert(sizeof(s_vehicle_human_jeep_definition) == 0x40);

struct s_vehicle_steering_animation_definition
{
	// steering animation and interpolation

	// This data controls the depiction of steering
	// through animation - not the vehicles ability to turn
	// interpolation scale= how much we interpolate between steering poses.
	// max angle= max animated steering angle change allowed per tick.

	// 1= heavy interp. of steering animations
	real interpolation_scale;

	// non-zero= max angle delta per frame
	angle max_angle;
};
static_assert(sizeof(s_vehicle_steering_animation_definition) == 0x8);

struct s_vehicle_human_plane_definition
{
	// velocity control variables

	real maximum_forward_speed;
	real maximum_reverse_speed;
	real speed_acceleration;
	real speed_deceleration;
	real maximum_left_slide;
	real maximum_right_slide;
	real slide_acceleration;
	real slide_deceleration;
	real maximum_up_rise;
	real maximum_down_rise;
	real rise_acceleration;
	real rise_deceleration;


	// human plane tuning variables

	real flying_torque_scale;
	real air_friction_deceleration;
	real thrust_scale;
	real turn_rate_scale_when_boosting;
	real maximum_roll;
	s_vehicle_steering_animation_definition steering_animation;
};
static_assert(sizeof(s_vehicle_human_plane_definition) == 0x4C);

struct s_engine_function_definition
{
	// this is the name of the region by which we gauge the overall damage of the vehicle
	c_string_id object_function_damage_region;

	// speed at which engine position funciton  moves.
	// value of 1 means goes from 0-1 in 1 second
	real min_anti_gravity_engine_speed;

	// speed at which engine position funciton  moves.
	// value of 1 means goes from 0-1 in 1 second
	real max_anti_gravity_engine_speed;

	// strictly used for object funtion. in 0-1 space
	real engine_speed_acceleration;

	// function is capped by speed of the vehicle.
	// So when we slow down for any reason we see the function go down
	real maximum_vehicle_speed;
};
static_assert(sizeof(s_engine_function_definition) == 0x14);

enum e_alien_scout_specific_type
{
	_alien_scout_specific_type_none = 0,
	_alien_scout_specific_type_ghost,
	_alien_scout_specific_type_spectre,
	_alien_scout_specific_type_wraith,
	_alien_scout_specific_type_hover_craft,

	k_alien_scout_specific_type_count
};

enum e_alien_scout_flags
{
	_alien_scout_flag_locked_camera_bit = 0,

	k_alien_scout_flags
};

struct s_vehicle_alien_scout_definition
{
	s_vehicle_steering_control_definition steering_control;


	// velocity control variables

	real maximum_forward_speed;
	real maximum_reverse_speed;
	real speed_acceleration;
	real speed_deceleration;
	real maximum_left_slide;
	real maximum_right_slide;
	real slide_acceleration;
	real slide_deceleration;

	// specific types
	// different types are treated differently alien scout controller
	c_enum<e_alien_scout_specific_type, char, _alien_scout_specific_type_none, k_alien_scout_specific_type_count> specific_type;

	c_flags<e_alien_scout_flags, char, k_alien_scout_flags> flags;
	byte NPBE[0x2]; // pad

	real drag_coeficient;
	real constant_deceleration;

	// 0 defaults to 1
	real torque_scale;

	// engine object function
	s_engine_function_definition engine_gravity_function;

	// contrail object function
	s_engine_function_definition contrail_gravity_function;

	// engine rotation function
	real_bounds gear_rotation_speed;

	s_vehicle_steering_animation_definition steering_animation;
};
static_assert(sizeof(s_vehicle_alien_scout_definition) == 0x70);

struct s_vehicle_alien_fighter_definition
{
	s_vehicle_steering_control_definition steering_control;
	s_vehicle_turning_control_definition turning_control;


	// velocity control variables

	real maximum_forward_speed;
	real maximum_reverse_speed;
	real speed_acceleration;
	real speed_deceleration;
	real maximum_left_slide;
	real maximum_right_slide;
	real slide_acceleration;
	real slide_deceleration;


	// torque scale
	// how hard the vehicle trys to rotate to it's desired rotation

	real flying_torque_scale;


	// fixed gun offset
	// this offset will be aligned to the units aiming vector instead of the vehicle forward vector

	real fixed_gun_yaw;
	real fixed_gun_pitch;


	// alien fighter trick variables

	real maximum_trick_frequency;
	real loop_trick_duration;
	real roll_trick_duration;


	// alien fighter fake flight control

	real zero_gravity_speed;
	real full_gravity_speed;
	real strafe_boost_scale;
	real off_stick_deceleration_scale;
	real cruising_throttle;
	real dive_speed_scale;
};
static_assert(sizeof(s_vehicle_alien_fighter_definition) == 0x64);

struct s_vehicle_turret_definition
{
	byte ASEXF[0x4]; // pad
};
static_assert(sizeof(s_vehicle_turret_definition) == 0x4);

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

struct s_walker_physics_leg_block
{
	// walker physics

	c_enum<e_walker_physics_leg_group, char, _walker_physics_leg_group_primary, k_walker_physics_leg_group_count> leg_group;
	c_enum<e_walker_physics_leg_side, char, _walker_physics_leg_side_left, k_walker_physics_leg_side_count> leg_side;
	char leg_side_order;
	bool valid;
	c_string_id hip_node_a_name;
	c_string_id hip_node_b_name;
	c_string_id knee_node_a_name;
	c_string_id knee_node_b_name;
	c_string_id foot_marker_name;
	byte RLTGT[0x3C]; // pad
	c_flags<e_walker_physics_leg_flags, long, k_walker_physics_leg_flags> flags;
	vector3d runtime_initial_origin_to_hip_offset;
	vector3d runtime_pivot_center_to_hip_offset;
	real runtime_upper_leg_length;
	real runtime_lower_leg_length;
	short runtime_hip_node_a_index;
	short runtime_hip_node_b_index;
	short runtime_knee_node_a_index;
	short runtime_knee_node_b_index;
	short runtime_foot_marker_group_index;
	short runtime_foot_node_index;
	short runtime_hip_node_index;
	short runtime_knee_node_index;
	vector3d plant_constraint_position;
	byte GKX[0xC]; // pad
};
static_assert(sizeof(s_walker_physics_leg_block) == 0xA0);

struct s_walker_physics_definition
{
	vector3d maximum_leg_motion;
	real maximum_turn;
	c_typed_tag_block<s_walker_physics_leg_block> legs;
	real leg_apex_fraction;
	real lift_exponent;
	real drop_exponent;
	vector3d object_space_pivot_position;
};
static_assert(sizeof(s_walker_physics_definition) == 0x34);

struct s_vehicle_mantis_definition
{
	s_vehicle_steering_control_definition steering_control;
	s_vehicle_turning_control_definition turning_control;


	// velocity control variables

	real maximum_forward_speed;
	real maximum_reverse_speed;
	real speed_acceleration;
	real speed_deceleration;
	real maximum_left_slide;
	real maximum_right_slide;
	real slide_acceleration;
	real slide_deceleration;
	s_walker_physics_definition walker_physics;

	// fraction of walkcycle at end for pause
	real walk_cycle_pause;

	// number of legs mantis needs planted to be considered stable.
	short stable_planted_legs;
	byte pad[0x2]; // pad

	real time_without_plant_buffer; // seconds
	real not_along_up_gravity_scale; // 0-1
	real speed_acceleration_limit;
	real speed_acceleration_match_scale;
	real slide_acceleration_limit;
	real slide_acceleration_match_scale;
	real turn_acceleration_limit;
	real turn_acceleration_match_scale;


	// jumping

	real jump_set_time; // seconds

	// 0-1, portion of set time spent interpolating into neutral stance
	real jump_set_interpolation_fraction;

	real jump_leap_time; // seconds
	real jump_recovery_time; // seconds

	// 0-1, portion of recovery time spent interpolating into neutral stance
	real jump_recovery_fraction;

	// WU, amount foot moves up to get ready to jump
	real jump_leg_set_distance;

	// WU, amount foot moves down when jumping
	real jump_leg_distance;
};
static_assert(sizeof(s_vehicle_mantis_definition) == 0xAC);

struct s_vehicle_vtol_definition
{
	s_vehicle_turning_control_definition turning_control;
	c_string_id left_lift_marker;
	c_string_id right_lift_marker;
	c_string_id thrust_marker;


	// rise angles

	real full_lift_angle_up;
	real lift_dead_angle_up;
	real full_lift_angle_down;
	real lift_dead_angle_down;


	// elevation lag
	// minimum is how much you can lag with with out acceleration and maximum is lag with acceleration

	real minimum_rise_target_lag;
	real maximum_rise_target_lag;


	// minimum and maximum up acceleration

	real minimum_up_acceleration;
	real maximum_up_acceleration;


	// turn, left and forward accelerations

	real maximum_turn_acceleration;
	real turn_acceleration_gain;
	real rotor_damping;
	real maximum_left_acceleration;
	real maximum_forward_acceleration;


	// lift arm pivot
	// this simulates the engines being on a pivot like on an osprey.  This helps better cradle the center of mass.

	real lift_arm_pivot_length;
	real drag_coeficient;
	real constant_deceleration;
	real magic_angular_acc_exp;
	real magic_angular_acc_scale;
	real magic_angular_acc_k;
	angle lift_angles_acc;
	angle render_lift_angles_acc;


	// prop rotation
	// propeller speed data to drive propeller animations

	real_bounds prop_rotation_speed;
};
static_assert(sizeof(s_vehicle_vtol_definition) == 0x74);

struct s_vehicle_chopper_definition
{
	s_vehicle_steering_control_definition steering_control;
	s_vehicle_turning_control_definition turning_control;
	s_vehicle_engine_definition engine;

	// wheel circumferance

	real wheel_circumferance;
	c_string_id rotation_marker;
	real magic_turning_scale;
	real magic_turning_acc;
	real magic_turning_max_vel;
	real magic_turning_exponent;
	real bank_to_slide_ratio;
	real bank_slide_exponent;
	real bank_to_turn_ratio;
	real bank_turn_exponent;
	real bank_fraction;
	real bank_rate;
	real wheel_accel;
	real gyroscopic_damping;
};
static_assert(sizeof(s_vehicle_chopper_definition) == 0x70);

struct s_vehicle_guardian_definition
{
	s_vehicle_steering_control_definition steering_control;
	real maximum_forward_speed;
	real maximum_reverse_speed;
	real speed_acceleration;
	real speed_deceleration;
	real maximum_left_slide;
	real maximum_right_slide;
	real slide_acceleration;
	real slide_deceleration;
	real torque_scale;
	real anti_gravity_force_z_offset;
};
static_assert(sizeof(s_vehicle_guardian_definition) == 0x30);

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
	static tag const k_group_tag = VEHICLE_TAG;

	_unit_definition unit;

	// $$$ VEHICLE $$$

	c_flags<e_vehicle_definition_flags, dword_flags, k_vehicle_definition_flags> flags;

	// physics type
	// define one of the following blocks for the type of physics you wish this vehicle to have.
	s_vehicle_physics_types physics_types;

	// friction and antigravity points
	s_havok_vehicle_physics_definition havok_vehicle_physics;

	c_enum<e_player_training_vehicle_type, char, _player_training_vehicle_type_none, k_player_training_vehicle_type_count> player_training_vehicle_type;

	// The size determine what kind of seats in larger vehicles it may occupy (i.e. small or large cargo seats)
	c_enum<e_vehicle_size, char, _vehicle_size_small, k_vehicle_size_count> vehicle_size;

	byte VQWHV[0x2]; // pad
	real minimum_flipping_angular_velocity;
	real maximum_flipping_angular_velocity;
	real crouch_transition_time; // seconds
	real HOOJYTSU;

	// how much do we scale the force the vehicle the applies down on the seat when he enters. 0 == no acceleration
	real seat_enterance_acceleration_scale;

	// how much do we scale the force the vehicle the applies down on the seat when he exits. 0 == no acceleration
	real seat_exit_accelersation_scale;

	real blur_speed;
	c_string_id flip_message;

	// sounds and effects

	c_typed_tag_reference<SOUND_TAG> suspension_sound;
	c_typed_tag_reference<EFFECT_TAG> special_effect;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, DAMAGE_RESPONSE_DEFINITION_TAG> driver_boost_damage_effect_or_response;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, DAMAGE_RESPONSE_DEFINITION_TAG> rider_boost_damage_effect_or_response;

	void update_reference_names();
};
static_assert(sizeof(_vehicle_definition) == 0x528);

