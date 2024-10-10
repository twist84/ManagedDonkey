#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_effect_gpu_data;

enum e_global_effect_priority
{
	_global_effect_priority_normal = 0,
	_global_effect_priority_high,
	_global_effect_priority_essential,

	k_global_effect_priority_count
};

enum e_effect_environment
{
	_effect_environment_any_environment = 0,
	_effect_environment_air_only,
	_effect_environment_water_only,
	_effect_environment_space_only,

	k_effect_environment_count
};

enum e_effect_disposition
{
	_effect_disposition_either_mode = 0,
	_effect_disposition_violent_mode_only,
	_effect_disposition_nonviolent_mode_only,

	k_effect_disposition_count
};

struct effect_event_definition;
struct effect_definition
{
	dword_flags flags;

	// If this is non-zero, the effect will usually be exactly repeatable
	dword fixed_random_seed;

	// overlap threshold
	real restart_if_within; // world units

	real continue_if_within; // world units

	real death_delay;
	c_enum<e_global_effect_priority, char, _global_effect_priority_normal, k_global_effect_priority_count> priority;

	// pad
	byte ERQOGINED[0x3];

	short loop_start_event;
	short local_location0;
	real runtime_danger_radius;
	s_tag_block locations;
	c_typed_tag_block<effect_event_definition> events;

	// Looping Sound
	s_tag_reference looping_sound;
	char location;
	char bind_scale_to_event;
	short local_location1;
	real always_play_distance;
	real never_play_distance;

	// runtime
	real lightprobe_death_delay;
	real local_space_death_delay;

	s_tag_block conical_distribution;
};
static_assert(sizeof(effect_definition) == 0x68);

enum e_effect_part_flags
{
	// decals
	_effect_part_face_down_regardless_of_location_bit = 0,

	// lights
	_effect_part_offset_origin_away_from_geometry_bit,

	_effect_part_never_attached_to_object_bit,
	_effect_part_disabled_for_debugging_bit,
	_effect_part_draw_regardless_of_distance_bit,
	_effect_part_make_every_tick_bit,
	_effect_part_inherit_parent_variant_bit,
	_effect_part_batch_AOE_damage_bit,

	k_effect_part_flags
};

enum e_effect_camera_mode
{
	_effect_camera_mode_independent_of_camera_mode = 0,
	_effect_camera_mode_only_in_first_person,
	_effect_camera_mode_only_in_third_person,
	_effect_camera_mode_both_first_and_third,

	k_effect_camera_mode_count,
};

enum e_effect_part_scaleable_values
{
	_effect_part_scaleable_value_velocity_bit = 0,
	_effect_part_scaleable_value_velocity_delta_bit,
	_effect_part_scaleable_value_velocity_cone_angle_bit,
	_effect_part_scaleable_value_angular_velocity_bit,
	_effect_part_scaleable_value_angular_velocity_delta_bit,
	_effect_part_scaleable_value_type_specific_scale_bit,

	k_effect_part_scaleable_values
};

using effect_part_definition_type_group_tags = c_typed_tag_reference<BEAM_SYSTEM_TAG, RENDER_WATER_RIPPLE_TAG, DAMAGE_EFFECT_TAG, SOUND_TAG, DECAL_SYSTEM_TAG, CONTRAIL_SYSTEM_TAG, LENS_FLARE_TAG, LIGHT_TAG, LIGHT_VOLUME_SYSTEM_TAG, AREA_SCREEN_EFFECT_TAG, CHARACTER_TAG, OBJECT_TAG>;
struct effect_part_definition
{
	c_enum<e_effect_environment, short, _effect_environment_any_environment, k_effect_environment_count> create_in;
	c_enum<e_effect_disposition, short, _effect_disposition_either_mode, k_effect_disposition_count> violence_mode;
	short location;
	short secondary_location; // beams
	c_flags<e_effect_part_flags, word, k_effect_part_flags> flags;
	c_enum<e_global_effect_priority, char, _global_effect_priority_normal, k_global_effect_priority_count> priority;

	// Currently, the only supported use is to turn parts off in first-person view.
	c_enum<e_effect_camera_mode, char, _effect_camera_mode_independent_of_camera_mode, k_effect_camera_mode_count> camera_mode;

	tag base_group_tag;
	effect_part_definition_type_group_tags type;

	// initial velocity along the location's forward, for decals the distance at which decal is created (defaults to 0.5)
	real_bounds velocity_bounds; // world units per second

	euler_angles2d velocity_orientation; // yaw, pitch

	// initial velocity will be inside the cone defined by this angle.
	angle velocity_cone_angle; // degrees

	angle_bounds angular_velocity_bounds; // degrees per second
	real_bounds radius_modifier_bounds;
	real_point3d relative_offset;
	euler_angles2d relative_orientation; // yaw, pitch

	// SCALE MODIFIERS
	c_flags<e_effect_part_scaleable_values, dword, k_effect_part_scaleable_values> A_scale_value;
	c_flags<e_effect_part_scaleable_values, dword, k_effect_part_scaleable_values> B_scale_value;
};
static_assert(sizeof(effect_part_definition) == 0x60);

struct effect_event_definition
{
	c_string_id event_name;
	dword_flags flags;
	c_enum<e_global_effect_priority, char, _global_effect_priority_normal, k_global_effect_priority_count> priority;

	// pad
	byte EVUIQSNDS[0x3];

	// chance that this event will be skipped entirely
	real_fraction skip_fraction;

	// delay before this event takes place
	real_bounds delay_bounds; // seconds

	// duration of this event
	real_bounds duration_bounds; // seconds

	c_typed_tag_block<effect_part_definition> parts;
	s_tag_block accelerations;
	s_tag_block particle_systems;
};
static_assert(sizeof(effect_event_definition) == 0x44);

