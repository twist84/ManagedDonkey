#pragma once

#include "cseries/cseries.hpp"
#include "math/function_definitions.hpp"
#include "physics/character_physics_definitions.hpp"
#include "units/unit_definition.hpp"

struct s_biped_camera_height
{
	c_string_id weapon_class;
	real32 standing_height;
	real32 crouching_height;
};
COMPILE_ASSERT(sizeof(s_biped_camera_height) == 0xC);

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
	c_flags<e_biped_lock_on_flags, uns32, k_biped_lock_on_flags> flags;
	real32 lock_on_distance;
};
COMPILE_ASSERT(sizeof(s_biped_lock_on_data) == 0x8);

struct s_biped_movement_gate
{
	real32 period;
	real32 z_offset;
	real32 y_offset;
	real32 speed_threshold;

	// default function		
	mapping_function default_function;
};
COMPILE_ASSERT(sizeof(s_biped_movement_gate) == 0x24);

struct biped_contact_point
{
	c_string_id marker_name; // old string id
};
COMPILE_ASSERT(sizeof(biped_contact_point) == sizeof(string_id));

enum e_biped_leap_flags
{
	_biped_leap_flag_force_early_roll_bit = 0,

	k_biped_leap_flags
};

struct s_biped_leaping_data
{
	// wall-leaping fields

	c_flags<e_biped_leap_flags, uns32, k_biped_leap_flags> leap_flags;
	real32 dampening_scale;            // [0,1] 1= very slow changes
	real32 roll_delay;                 // [0,1] 1= roll fast and late
	real32 cannonball_off_axis_scale;  // [0,1] weight
	real32 cannonball_off_track_scale; // [0,1] weight
	angle_bounds cannonball_roll_bounds;      // degrees per second
	real_bounds anticipation_ratio_bounds;    // current velocity/leap velocity
	real_bounds reaction_force_bounds;        // units per second
	real32 lobbing_desire;             // 1= heavy arc, 0= no arc
};
COMPILE_ASSERT(sizeof(s_biped_leaping_data) == 48);

struct s_biped_ground_fitting_data
{
	// ground fitting data

	real32 ground_normal_dampening; // react to slope changes (0=slow, 1= fast)
	real32 root_offset_max_scale;            // vertical drop to ground allowed (0=none, 1=full)
	real32 root_offset_dampening;   // react to root changes (0=slow, 1= fast)
	real32 following_cam_scale;     // root offset effect on following cam (0=none, 1=full)
	real32 root_leaning_scale;      // lean into slopes (0=none, 1=full)
	real32 foot_roll_max;                   // orient to ground slope (degrees)
	real32 foot_pitch_max;                  // orient to ground slope (degrees)


	// pivot-on-foot data

	real32 pivot_on_foot_scale; // (0=none, 1= full)
	real32 pivot_min_foot_delta;         // vert world units to find lowest foot

	// leg length * this = stride length
	real32 pivot_stride_length_scale;

	real32 pivot_throttle_scale;   // pivoting slows throttle (0=none, 1= full)
	real32 pivot_offset_dampening; // react to pivot changes (0=slow, 1= fast)
};
COMPILE_ASSERT(sizeof(s_biped_ground_fitting_data) == 0x30);

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

	real32 moving_turning_speed; // degrees per second
	c_flags<e_biped_definition_flags, uns32, k_biped_definition_flags> flags;
	real32 stationary_turning_threshold;

	// if the biped dies while moving faster than this velocity, immediately transition to ragdoll.  Use 0 for 'never'
	real32 ragdoll_threshold_velocity; // wu/s

	// when the biped transitions to ragdoll, this region will change to the destroyed state
	c_string_id ragdoll_region_name;


	// jumping and landing

	real32 jump_velocity; // world units per second

	// the longest amount of time the biped can take to recover from a soft landing
	real32 maximum_soft_landing_time; // seconds

	// the longest amount of time the biped can take to recover from a hard landing
	real32 maximum_hard_landing_time; // seconds

	// below this velocity the biped does not react when landing
	real32 minimum_soft_landing_velocity; // world units per second

	// below this velocity the biped will not do a soft landing when returning to the ground
	real32 minimum_hard_landing_velocity; // world units per second

	// the velocity corresponding to the maximum landing time
	real32 maximum_hard_landing_velocity; // world units per second

	// the maximum velocity with which a character can strike the ground and live
	real32 death_hard_landing_velocity; // world units per second

	// 0 is the default.
	// Bipeds are stuned when damaged by vehicle collisions, also some are when they take emp damage
	real32 stun_duration;


	// camera, collision, and autoaim

	real32 standing_camera_height; // world units
	real32 running_camera_height; // world units
	real32 crouching_camera_height; // world units
	real32 crouch_walking_camera_height; // world units
	real32 crouch_transition_time; // seconds

	c_function_definition camera_height_velocity_function;
	c_typed_tag_block<s_biped_camera_height> camera_heights;

	// looking-downward angle that starts camera interpolation to fp position
	real32 camera_interpolation_start; // degrees

	// looking-downward angle at which camera interpolation to fp position is complete
	real32 camera_interpolation_end; // degrees

	// amount of fp camera movement forward and back (1.0 is full)
	real32 camera_forward_movement_scale;

	// amount of fp camera movement side-to-side (1.0 is full)
	real32 camera_side_movement_scale;

	// amount of fp camera movement vertically (1.0 is full)
	real32 camera_vertical_movement_scale;

	// fp camera must always be at least this far out from root node
	real32 camera_exclusion_distance; // world units

	real32 autoaim_width; // world units

	s_biped_lock_on_data lock_on_data;
	int16 runtime_physics_control_node_index;
	int16 unused2;
	real32 runtime_cosine_stationary_turning_threshold;
	real32 runtime_crouch_transition_velocity;
	real32 runtime_camera_height_velocity;
	int16 runtime_pelvis_node_index;
	int16 runtime_head_node_index;
	real32 fp_crouch_moving_animation_playback_speed_multiplier;

	// when the biped ragdolls from a head shot it acceleartes based on this value.
	// 0 defaults to the standard acceleration scale
	real32 head_shot_acc_scale;

	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> area_damage_effect;
	c_typed_tag_block<s_biped_movement_gate> movement_gates;
	c_typed_tag_block<s_biped_movement_gate> movement_gates_crouching;
	real32 jump_aim_offset_distance; // world units
	real32 jump_aim_offset_duration; // seconds
	real32 land_aim_offset_distance; // world units
	real32 land_aim_offset_duration; // seconds
	real32 aim_compensate_minimum_distance; // world units
	real32 aim_compensate_maximum_distance; // world units
	s_character_physics_definition physics;

	// these are the points where the biped touches the ground
	c_typed_tag_block<biped_contact_point> contact_points;

	// when the flood reanimate this guy, he turns into a ...
	c_typed_tag_reference<CHARACTER_TAG, INVALID_TAG> reanimation_character;

	// the kind of muffins I create to cover my horrible transformation
	c_typed_tag_reference<MUFFIN_TAG, INVALID_TAG> reanimation_morph_muffins;

	// when I die, out of the ashes of my death crawls a ...
	c_typed_tag_reference<CHARACTER_TAG, INVALID_TAG> death_spawn_character;

	int16 death_spawn_count;
	int16 death_spawn_unused;
	s_biped_leaping_data leaping_data;
	s_biped_ground_fitting_data ground_fitting_data;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(_biped_definition) == 0x23C);

typedef struct biped_definition
{
	static tag const k_group_tag = BIPED_TAG;

	_object_definition object;
	_unit_definition unit;
	_biped_definition biped;

	void update_reference_names();
} s_biped_definition;
COMPILE_ASSERT(sizeof(biped_definition) == sizeof(_object_definition) + sizeof(_unit_definition) + sizeof(_biped_definition));

