#pragma once

#include "cseries/cseries.hpp"
#include "math/function_definitions.hpp"
#include "physics/character_physics_definitions.hpp"
#include "units/unit_definition.hpp"

struct s_biped_camera_height
{
	c_string_id weapon_class;
	real standing_height;
	real crouching_height;
};
static_assert(sizeof(s_biped_camera_height) == 0xC);

enum e_biped_lock_on_flags
{
	_biped_lock_on_flag_locked_by_human_targeting_bit = 0,
	_biped_lock_on_flag_locked_by_plasma_targeting_bit,
	_biped_lock_on_flag_always_locked_by_plasma_targeting_bit,

	k_biped_lock_on_flags
};

struct s_biped_lock_on_data
{
	// lock-on fields
	c_flags<e_biped_lock_on_flags, dword_flags, k_biped_lock_on_flags> flags;
	real lock_on_distance;
};
static_assert(sizeof(s_biped_lock_on_data) == 0x8);

struct s_biped_movement_gate
{
	real period;
	real z_offset;
	real y_offset;
	real speed_threshold;

	// default function		
	mapping_function default_function;
};
static_assert(sizeof(s_biped_movement_gate) == 0x24);

struct biped_contact_point
{
	c_string_id marker_name; // old string id
};
static_assert(sizeof(biped_contact_point) == sizeof(string_id));

enum e_biped_leap_flags
{
	_biped_leap_flag_force_early_roll_bit = 0,

	k_biped_leap_flags
};

struct s_biped_leaping_data
{
	// wall-leaping fields

	c_flags<e_biped_leap_flags, dword_flags, k_biped_leap_flags> leap_flags;
	real_fraction dampening_scale;            // [0,1] 1= very slow changes
	real_fraction roll_delay;                 // [0,1] 1= roll fast and late
	real_fraction cannonball_off_axis_scale;  // [0,1] weight
	real_fraction cannonball_off_track_scale; // [0,1] weight
	angle_bounds cannonball_roll_bounds;      // degrees per second
	real_bounds anticipation_ratio_bounds;    // current velocity/leap velocity
	real_bounds reaction_force_bounds;        // units per second
	real_fraction lobbing_desire;             // 1= heavy arc, 0= no arc
};
static_assert(sizeof(s_biped_leaping_data) == 48);

struct s_biped_ground_fitting_data
{
	// ground fitting data

	real_fraction ground_normal_dampening; // react to slope changes (0=slow, 1= fast)
	real root_offset_max_scale;            // vertical drop to ground allowed (0=none, 1=full)
	real_fraction root_offset_dampening;   // react to root changes (0=slow, 1= fast)
	real_fraction following_cam_scale;     // root offset effect on following cam (0=none, 1=full)
	real_fraction root_leaning_scale;      // lean into slopes (0=none, 1=full)
	angle foot_roll_max;                   // orient to ground slope (degrees)
	angle foot_pitch_max;                  // orient to ground slope (degrees)


	// pivot-on-foot data

	real_fraction pivot_on_foot_scale; // (0=none, 1= full)
	real pivot_min_foot_delta;         // vert world units to find lowest foot

	// leg length * this = stride length
	real pivot_stride_length_scale;

	real_fraction pivot_throttle_scale;   // pivoting slows throttle (0=none, 1= full)
	real_fraction pivot_offset_dampening; // react to pivot changes (0=slow, 1= fast)
};
static_assert(sizeof(s_biped_ground_fitting_data) == 0x30);

enum e_biped_definition_flags
{
	_biped_definition_flags_turns_without_animating = 0,
	_biped_definition_flags_has_physical_rigid_bodies_when_alive,
	_biped_definition_flags_immune_to_falling_damage,
	_biped_definition_flags_unused0,
	_biped_definition_flags_unused1,
	_biped_definition_flags_unused2,
	_biped_definition_flags_random_speed_increase,
	_biped_definition_flags_unused3,
	_biped_definition_flags_spawn_death_children_on_destroy,
	_biped_definition_flags_stunned_by_emp_damage,
	_biped_definition_flags_dead_physics_when_stunned,
	_biped_definition_flags_always_ragdoll_when_dead,
	_biped_definition_flags_snaps_turns,

	k_biped_definition_flags
};

struct _biped_definition
{
	// $$$ BIPED $$$

	angle moving_turning_speed; // degrees per second
	c_flags<e_biped_definition_flags, dword_flags, k_biped_definition_flags> flags;
	angle stationary_turning_threshold;

	// if the biped dies while moving faster than this velocity, immediately transition to ragdoll.  Use 0 for 'never'
	real ragdoll_threshold_velocity; // wu/s

	// when the biped transitions to ragdoll, this region will change to the destroyed state
	c_string_id ragdoll_region_name;


	// jumping and landing

	real jump_velocity; // world units per second

	// the longest amount of time the biped can take to recover from a soft landing
	real maximum_soft_landing_time; // seconds

	// the longest amount of time the biped can take to recover from a hard landing
	real maximum_hard_landing_time; // seconds

	// below this velocity the biped does not react when landing
	real minimum_soft_landing_velocity; // world units per second

	// below this velocity the biped will not do a soft landing when returning to the ground
	real minimum_hard_landing_velocity; // world units per second

	// the velocity corresponding to the maximum landing time
	real maximum_hard_landing_velocity; // world units per second

	// the maximum velocity with which a character can strike the ground and live
	real death_hard_landing_velocity; // world units per second

	// 0 is the default.
	// Bipeds are stuned when damaged by vehicle collisions, also some are when they take emp damage
	real stun_duration;


	// camera, collision, and autoaim

	real standing_camera_height; // world units
	real running_camera_height; // world units
	real crouching_camera_height; // world units
	real crouch_walking_camera_height; // world units
	real crouch_transition_time; // seconds

	mapping_function camera_height_velocity_function;
	c_typed_tag_block<s_biped_camera_height> camera_heights;

	// looking-downward angle that starts camera interpolation to fp position
	angle camera_interpolation_start; // degrees

	// looking-downward angle at which camera interpolation to fp position is complete
	angle camera_interpolation_end; // degrees

	// amount of fp camera movement forward and back (1.0 is full)
	real camera_forward_movement_scale;

	// amount of fp camera movement side-to-side (1.0 is full)
	real camera_side_movement_scale;

	// amount of fp camera movement vertically (1.0 is full)
	real camera_vertical_movement_scale;

	// fp camera must always be at least this far out from root node
	real camera_exclusion_distance; // world units

	real autoaim_width; // world units

	s_biped_lock_on_data lock_on_data;
	short runtime_physics_control_node_index;
	byte JWSSY[0x2]; // pad
	real runtime_cosine_stationary_turning_threshold;
	real runtime_crouch_transition_velocity;
	real runtime_camera_height_velocity;
	short runtime_pelvis_node_index;
	short runtime_head_node_index;
	real fp_crouch_moving_animation_playback_speed_multiplier;

	// when the biped ragdolls from a head shot it acceleartes based on this value.
	// 0 defaults to the standard acceleration scale
	real head_shot_acc_scale;

	c_typed_tag_reference<EFFECT_TAG> area_damage_effect;
	c_typed_tag_block<s_biped_movement_gate> movement_gates;
	c_typed_tag_block<s_biped_movement_gate> movement_gates_crouching;
	real jump_aim_offset_distance; // world units
	real jump_aim_offset_duration; // seconds
	real land_aim_offset_distance; // world units
	real land_aim_offset_duration; // seconds
	real aim_compensate_minimum_distance; // world units
	real aim_compensate_maximum_distance; // world units
	s_character_physics_definition physics;

	// these are the points where the biped touches the ground
	c_typed_tag_block<biped_contact_point> contact_points;

	// when the flood reanimate this guy, he turns into a ...
	c_typed_tag_reference<CHARACTER_TAG> reanimation_character;

	// the kind of muffins I create to cover my horrible transformation
	c_typed_tag_reference<MUFFIN_TAG> reanimation_morph_muffins;

	// when I die, out of the ashes of my death crawls a ...
	c_typed_tag_reference<CHARACTER_TAG> death_spawn_character;

	short death_spawn_count;
	byte TQYQEWG[2]; // pad
	s_biped_leaping_data leaping_data;
	s_biped_ground_fitting_data ground_fitting_data;

	void update_reference_names();
};
static_assert(sizeof(_biped_definition) == 0x23C);

struct biped_definition
{
	static tag const k_group_tag = BIPED_TAG;

	_object_definition object;
	_unit_definition unit;
	_biped_definition biped;

	void update_reference_names();
};
static_assert(sizeof(biped_definition) == sizeof(_object_definition) + sizeof(_unit_definition) + sizeof(_biped_definition));

