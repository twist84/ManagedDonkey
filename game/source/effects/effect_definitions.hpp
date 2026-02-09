#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_effect_gpu_data;

enum e_effect_definition_flags
{
	_effect_deleted_when_inactive_bit = 0,
	_effect_parallel_events_bit,

	// This option will hurt performance on effects with very int16 duration.
	_effect_no_part_reuse_bit,

	_effect_age_creators_primary_weapon_bit,
	_effect_locations_hybrid_world_local_bit,
	_effect_can_penetrate_walls_bit,

	// Will help performance for parallel events of different lengths, if the effect is unlikely to be reused
	_effect_cannot_be_restarted_bit,

	// Do not adopt parent object's lightprobe, even when it's available
	_effect_force_use_own_lightprobe_bit,

	k_on_disk_effect_definition_flag_count,

	_effect_instant_bit = k_on_disk_effect_definition_flag_count,
	_effect_must_be_deterministic_bit,
	_effect_samples_lightmap_bit,
	_effect_samples_diffuse_texture_bit,
	_effect_uses_child_location_bit,
	_effect_xsynced_bit,
	_effect_depends_on_environment_bit,
	_effect_track_subframe_movements_bit,

	_effect_bit16,

	k_effect_definition_flag_count
};

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
typedef struct effect_definition
{
	//c_flags<e_effect_definition_flags, uns32, k_effect_definition_flag_count> flags;
	uns32 flags;

	// If this is non-zero, the effect will usually be exactly repeatable
	uns32 fixed_random_seed;

	// overlap threshold
	real32 restart_if_within; // world units

	real32 continue_if_within; // world units

	real32 death_delay;
	c_enum<e_global_effect_priority, int8, _global_effect_priority_normal, k_global_effect_priority_count> priority;

	byte pad[0x3];

	int16 loop_start_event;
	int16 local_location0;
	real32 runtime_danger_radius;
	s_tag_block locations;
	c_typed_tag_block<effect_event_definition> events;

	// Looping Sound
	s_tag_reference looping_sound;
	int8 location;
	int8 bind_scale_to_event;
	int16 local_location1;
	real32 always_play_distance;
	real32 never_play_distance;

	// runtime
	real32 lightprobe_death_delay;
	real32 local_space_death_delay;

	s_tag_block conical_distribution;
} s_effect_definition;
COMPILE_ASSERT(sizeof(effect_definition) == 0x68);

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

using effect_part_definition_type_group_tags = c_typed_tag_reference<BEAM_SYSTEM_TAG, RENDER_WATER_RIPPLE_TAG, DAMAGE_EFFECT_TAG, SOUND_TAG, DECAL_SYSTEM_TAG, CONTRAIL_SYSTEM_TAG, LENS_FLARE_TAG, LIGHT_TAG, LIGHT_VOLUME_SYSTEM_TAG, AREA_SCREEN_EFFECT_TAG, CHARACTER_TAG, OBJECT_TAG, INVALID_TAG>;
struct effect_part_definition
{
	c_enum<e_effect_environment, int16, _effect_environment_any_environment, k_effect_environment_count> create_in;
	c_enum<e_effect_disposition, int16, _effect_disposition_either_mode, k_effect_disposition_count> violence_mode;
	int16 location;
	int16 secondary_location; // beams
	c_flags<e_effect_part_flags, uns16, k_effect_part_flags> flags;
	c_enum<e_global_effect_priority, int8, _global_effect_priority_normal, k_global_effect_priority_count> priority;

	// Currently, the only supported use is to turn parts off in first-person view.
	c_enum<e_effect_camera_mode, int8, _effect_camera_mode_independent_of_camera_mode, k_effect_camera_mode_count> camera_mode;

	tag base_group_tag;
	effect_part_definition_type_group_tags type;

	// initial velocity along the location's forward, for decals the distance at which decal is created (defaults to 0.5)
	real_bounds velocity_bounds; // world units per second

	real_euler_angles2d velocity_orientation; // yaw, pitch

	// initial velocity will be inside the cone defined by this angle.
	real32 velocity_cone_angle; // degrees

	angle_bounds angular_velocity_bounds; // degrees per second
	real_bounds radius_modifier_bounds;
	real_point3d relative_offset;
	real_euler_angles2d relative_orientation; // yaw, pitch

	// SCALE MODIFIERS
	c_flags<e_effect_part_scaleable_values, uns32, k_effect_part_scaleable_values> A_scale_value;
	c_flags<e_effect_part_scaleable_values, uns32, k_effect_part_scaleable_values> B_scale_value;
};
COMPILE_ASSERT(sizeof(effect_part_definition) == 0x60);

struct effect_event_definition
{
	c_string_id event_name;
	uns32 flags;
	uns8 priority;

	byte pad[0x3];

	// chance that this event will be skipped entirely
	real32 skip_fraction;

	// delay before this event takes place
	real32 delay_lower_bound; // seconds
	real32 delay_upper_bound; // seconds

	// duration of this event
	real32 duration_lower_bound; // seconds
	real32 duration_upper_bound; // seconds

	c_typed_tag_block<effect_part_definition> parts;
	s_tag_block accelerations;
	s_tag_block particle_systems;
};
COMPILE_ASSERT(sizeof(effect_event_definition) == 0x44);

