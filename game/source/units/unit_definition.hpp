#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "game/campaign_metagame_definitions.hpp"
#include "objects/object_definitions.hpp"

struct s_unit_camera_track
{
	c_typed_tag_reference<CAMERA_TRACK_TAG, INVALID_TAG> track;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_unit_camera_track) == sizeof(s_tag_reference));

struct s_unit_camera_acceleration_displacment_function
{
	real32 k;
	real32 scale;
	real32 power;
	real32 maximum;

	// scale factor used when this acceleration component is along the axis of the forward vector of the camera
	real32 camera_scale_axial;

	// scale factor used when this acceleration component is perpendicular to the camera
	real32 camera_scale_perpendicular;
};
COMPILE_ASSERT(sizeof(s_unit_camera_acceleration_displacment_function) == 0x18);

struct s_unit_camera_acceleration
{
	// camera scale x MIN(scale x POWER((K x X), power), maximum)
	s_unit_camera_acceleration_displacment_function velocity_i;
	s_unit_camera_acceleration_displacment_function velocity_j;
	s_unit_camera_acceleration_displacment_function velocity_k;

	// maximum offset velocity
	real32 maximum_displacment_velocity;
};
COMPILE_ASSERT(sizeof(s_unit_camera_acceleration) == 0x4C);

enum e_unit_camera_flags
{
	_unit_camera_flag_pitch_bounds_absolute_space_bit = 0,

	k_unit_camera_flags
};

struct s_unit_camera
{
	c_flags<e_unit_camera_flags, int16, k_unit_camera_flags> flags;
	uns16 pad;
	c_old_string_id camera_marker_name;
	c_old_string_id camera_submerged_marker_name;
	real32 pitch_auto_level;
	angle_bounds pitch_range;
	c_typed_tag_block<s_unit_camera_track> camera_tracks;
	real32 pitch_minimum_spring;
	real32 pitch_mmaximum_spring;
	real32 spring_velocity;
	c_typed_tag_block<s_unit_camera_acceleration> camera_acceleration;
};
COMPILE_ASSERT(sizeof(s_unit_camera) == 0x3C);

struct unit_seat_acceleration
{
	real_vector3d scale; // world units per second squared

	// actions fail
	real32 action_limit; // [0,1+]

	// detach unit
	real32 attachment_limit; // [0,1+]
};
COMPILE_ASSERT(sizeof(unit_seat_acceleration) == 0x14);

struct s_unit_additional_node_names
{
	c_string_id preferred_gun_node;
};
COMPILE_ASSERT(sizeof(s_unit_additional_node_names) == sizeof(string_id));

struct s_unit_boarding_melee
{
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> boarding_melee_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> boarding_melee_response;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> eviction_melee_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> eviction_melee_response;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> landing_melee_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> flurry_melee_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> obstacle_smash_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> assassination_primary_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> assassination_ragdoll_damage;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_unit_boarding_melee) == sizeof(s_tag_reference) * 9);

struct s_unit_boost
{
	c_typed_tag_reference<COLLISION_DAMAGE_TAG, INVALID_TAG> boost_collision_damage;
	real32 boost_peak_power;
	real32 boost_rise_power;
	real32 boost_peak_time;
	real32 boost_fall_power;
	real32 dead_time;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_unit_boost) == sizeof(real32) * 5 + sizeof(s_tag_reference));

struct s_unit_lipsync_scales
{
	real32 attack_weight;
	real32 decay_weight;
};
COMPILE_ASSERT(sizeof(s_unit_lipsync_scales) == 0x8);

enum e_unit_definition_flags
{
	_unit_definition_flag_circular_aiming_bit = 0,
	_unit_definition_flag_destroyed_after_dying_bit,
	_unit_definition_flag_half_speed_interpolation_bit,
	_unit_definition_flag_fires_from_camera_bit,
	_unit_definition_flag_entrance_inside_bounding_sphere_bit,
	_unit_definition_flag_doesnt_show_readied_weapon_bit,
	_unit_definition_flag_causes_passenger_dialogue_bit,
	_unit_definition_flag_resists_pings_bit,
	_unit_definition_flag_melee_attack_is_fatal_bit,
	_unit_definition_flag_dont_reface_during_pings_bit,
	_unit_definition_flag_has_no_aiming_bit,
	_unit_definition_flag_simple_creature_bit,
	_unit_definition_flag_impact_melee_attaches_to_unit_bit,
	_unit_definition_flag_impact_melee_dies_on_shields_bit,
	_unit_definition_flag_cannot_open_doors_automatically_bit,
	_unit_definition_flag_melee_attackers_cannot_attach_bit,
	_unit_definition_flag_not_instantly_killed_by_melee_bit,
	_unit_definition_flag_flashlight_power_doesnt_transfer_to_weapon_bit,
	_unit_definition_flag_runs_around_flaming_bit,
	_unit_definition_flag_top_level_for_aoe_damage_bit,
	_unit_definition_flag_special_cinematic_unit_bit,
	_unit_definition_flag_ignored_by_autoaiming_bit,
	_unit_definition_flag_shields_fry_infection_forms_bit,
	_unit_definition_flag_use_velocity_as_acceleration_bit,
	_unit_definition_flag_unused1_bit,
	_unit_definition_flag_acts_as_gunner_for_parent_bit,
	_unit_definition_flag_controlled_by_parent_gunner_bit,
	_unit_definition_flag_parents_primary_weapon_bit,
	_unit_definition_flag_unit_has_boost_bit,
	_unit_definition_flag_allow_aim_while_opening_or_closing_bit,
	_unit_definition_flag_use_aim_still_xx_for_airborne_bit,

	k_unit_definition_flags
};

enum e_unit_default_teams
{
	_unit_default_teams_default = 0,
	_unit_default_teams_player,
	_unit_default_teams_human,
	_unit_default_teams_covenant,
	_unit_default_teams_flood,
	_unit_default_teams_sentinel,
	_unit_default_teams_heretic,
	_unit_default_teams_prophet,
	_unit_default_teams_guilty,
	_unit_default_teams_unused9,
	_unit_default_teams_unused10,
	_unit_default_teams_unused11,
	_unit_default_teams_unused12,
	_unit_default_teams_unused13,
	_unit_default_teams_unused14,
	_unit_default_teams_unused15,

	k_unit_default_teams_count
};

enum e_global_chud_blip_type
{
	_global_chud_blip_type_medium = 0,
	_global_chud_blip_type_small,
	_global_chud_blip_type_large,

	k_global_chud_blip_type_count
};

enum e_unit_item_owner_size
{
	_unit_item_owner_size_small = 0,
	_unit_item_owner_size_medium,
	_unit_item_owner_size_player,
	_unit_item_owner_size_large,

	k_unit_item_owner_size_count
};

struct s_posture_definition;
struct unit_hud_reference;
struct dialogue_variant_definition;
struct powered_seat_definition;
struct unit_initial_weapon;
struct s_target_tracking_parameters;
struct unit_seat;
struct _unit_definition
{
	// $$$ UNIT $$$

	c_flags<e_unit_definition_flags, uns32, k_unit_definition_flags> flags;
	c_enum<e_unit_default_teams, int16, _unit_default_teams_default, k_unit_default_teams_count> default_team;
	c_enum<e_ai_sound_volume, int16, _ai_sound_volume_silent, k_ai_sound_volume_count> constant_sound_volume;
	c_typed_tag_reference<BIPED_TAG, VEHICLE_TAG, INVALID_TAG> hologram_unit_reference;
	c_typed_tag_block<s_campaign_metagame_bucket> campaign_metagame_bucket;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> integrated_light_toggle;
	real32 camera_field_of_view; // degrees
	real32 camera_stiffness;
	s_unit_camera unit_camera;

	// sync action camera fields
	s_unit_camera sync_action_camera;

	c_typed_tag_reference<DAMAGE_RESPONSE_DEFINITION_TAG, INVALID_TAG> assasination_start_damage_response;
	c_typed_tag_reference<WEAPON_TAG, INVALID_TAG> assassination_weapon;

	// the anchor we attach the knife to when we stow it
	c_string_id assassination_weapon_stow_marker;

	// the anchor we attach the knife to when we pull it out
	c_string_id assassination_weapon_out_marker;

	// the marker on the knife that we anchor to the biped
	c_string_id assassination_weapon_anchor_marker;

	unit_seat_acceleration acceleration;

	real32 soft_ping_threshold; // [0,1]
	real32 soft_ping_interrupt_time; // seconds
	real32 hard_ping_threshold; // [0,1]
	real32 hard_ping_interrupt_time; // seconds
	real32 hard_death_threshold; // [0,1]
	real32 feign_death_threshold; // [0,1]
	real32 feign_death_time; // seconds

	// this must be set to tell the AI how far it should expect our dive animation to move us
	real32 distance_of_dive_anim; // world units

	// if we take this much damage in a short space of time we will play our 'stunned movement' animations
	real32 stunned_movement_threshold; // [0,1]
	real32 feign_death_chance; // [0,1]
	real32 feign_repeat_chance; // [0,1]

	// automatically created character when this unit is driven
	c_typed_tag_reference<CHARACTER_TAG, INVALID_TAG> spawned_turret_character;

	// number of actors which we spawn
	int16_bounds spawned_actor_count;

	// velocity at which we throw spawned actors
	real32 spawned_velocity;

	real32 aiming_velocity_maximum; // degrees per second
	real32 aiming_acceleration_maximum; // degrees per second squared

	real32 casual_aiming_modifier; // [0,1]

	real32 looking_velocity_maximum; // degrees per second
	real32 looking_acceleration_maximum; // degrees per second squared

	// where the primary weapon is attached
	c_string_id right_hand_node;

	// where the seconday weapon is attached (for dual-pistol modes)
	c_string_id left_hand_node;

	s_unit_additional_node_names more_damn_nodes;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> melee_damage;
	s_unit_boarding_melee your_momma;
	c_enum<e_global_chud_blip_type, int16, _global_chud_blip_type_medium, k_global_chud_blip_type_count> motion_sensor_blip_size;
	c_enum<e_unit_item_owner_size, int16, _unit_item_owner_size_small, k_unit_item_owner_size_count> item_owner_size;
	c_typed_tag_block<s_posture_definition> postures;
	c_typed_tag_block<unit_hud_reference> new_hud_interfaces;
	c_typed_tag_block<dialogue_variant_definition> dialogue_variants;

	real32 grenade_maximum_autoaim_distance; // world units
	real32 grenade_angle; // degrees
	real32 grenade_angle_max_elevation; // degrees
	real32 grenade_angle_min_elevation; // degrees
	real32 grenade_velocity; // world units per second
	c_enum<e_grenade_type, int16, _grenade_type_human_fragmentation, k_grenade_type_count> grenade_type;
	int16 grenade_count;
	c_typed_tag_block<powered_seat_definition> powered_seats;
	c_typed_tag_block<unit_initial_weapon> weapons;
	c_typed_tag_block<s_target_tracking_parameters> target_trackingk;
	c_typed_tag_block<unit_seat> seats_block;

	// EMP Disabling
	real32 emp_disabled_time; // seconds
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> emp_disabled_effect;

	// Boost
	s_unit_boost boost;

	// Lipsync
	s_unit_lipsync_scales lipsync;

	// Exit and Detach
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> exit_and_detach_damage;
	c_typed_tag_reference<WEAPON_TAG, INVALID_TAG> exit_and_detach_weapon;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(_unit_definition) == 0x2C8);

struct unit_definition
{
	static tag const k_group_tag = UNIT_TAG;

	_object_definition object;
	_unit_definition unit;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(unit_definition) == sizeof(_object_definition) + sizeof(_unit_definition));

struct s_posture_definition
{
	c_string_id name;
	real_vector3d pill_offset;
};
COMPILE_ASSERT(sizeof(s_posture_definition) == 0x10);

struct unit_hud_reference
{
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> chud_interface;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(unit_hud_reference) == 0x10);

struct dialogue_variant_definition
{
	int16 variant_number;
	uns16 pad;
	c_typed_tag_reference<DIALOGUE_TAG, INVALID_TAG> dialogue;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(dialogue_variant_definition) == 0x14);

struct powered_seat_definition
{
	real32 driver_powerup_time;
	real32 driver_powerdown_time;
};
COMPILE_ASSERT(sizeof(powered_seat_definition) == 0x8);

struct unit_initial_weapon
{
	c_typed_tag_reference<WEAPON_TAG, INVALID_TAG> weapon;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(unit_initial_weapon) == sizeof(s_tag_reference));

struct s_tracking_type
{
	c_string_id tracking_type;
};
COMPILE_ASSERT(sizeof(s_tracking_type) == sizeof(string_id));

struct s_target_tracking_parameters
{
	c_typed_tag_block<s_tracking_type> tracking_types;
	real32 acquire_time;
	real32 grace_time;
	real32 decay_time;
	c_typed_tag_reference<SOUND_TAG, SOUND_LOOPING_TAG, INVALID_TAG> tracking_sound;
	c_typed_tag_reference<SOUND_TAG, SOUND_LOOPING_TAG, INVALID_TAG> locked_sound;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_target_tracking_parameters) == 0x38);

enum e_unit_seat_flags
{
	_unit_seat_invisible_bit = 0,
	_unit_seat_locked_bit,
	_unit_seat_driver_bit,
	_unit_seat_gunner_bit,
	_unit_seat_third_person_camera_bit,
	_unit_seat_allows_weapons_bit,
	_unit_seat_third_person_on_enter_bit,
	_unit_seat_first_person_camera_slaved_to_gun_bit,
	_unit_seat_allow_vehicle_communication_animations_bit,
	_unit_seat_not_valid_without_driver_bit,
	_unit_seat_allow_ai_noncombatants_bit,
	_unit_seat_boarding_seat_bit,
	_unit_seat_ai_firing_disabled_by_max_acceleration_bit,
	_unit_seat_boarding_enters_seat_bit,
	_unit_seat_boarding_need_any_passenger_bit,
	_unit_seat_controls_open_and_close_bit,
	_unit_seat_invalid_for_player_bit,
	_unit_seat_invalid_for_non_player_bit,
	_unit_seat_gunner_player_only_bit,
	_unit_seat_invisible_under_major_damage_bit,
	_unit_seat_melee_instant_killable_bit,
	_unit_seat_leader_preference_bit,
	_unit_seat_allows_exit_and_detach_bit,
	_unit_seat_blocks_headshots_bit,
	_unit_seat_exits_to_ground_bit,
	_unit_seat_closes_early_opens_late_bit,
	_unit_seat_forward_from_attachment_bit,
	_unit_seat_disallow_ai_shooting_bit,
	_unit_seat_closes_early_opens_early_bit,
	_unit_seat_closes_late_opens_late_bit,
	_unit_seat_prevents_weapon_stowing_bit,

	k_unit_seat_flags
};

enum e_global_ai_seat_type
{
	_global_ai_seat_type_none = 0,
	_global_ai_seat_type_passenger,
	_global_ai_seat_type_gunner,
	_global_ai_seat_type_small_cargo,
	_global_ai_seat_type_large_cargo,
	_global_ai_seat_type_driver,

	k_global_ai_seat_type_count
};

struct unit_seat
{
	c_flags<e_unit_seat_flags, uns32, k_unit_seat_flags> flags;
	c_old_string_id label;
	c_old_string_id marker_name;
	c_string_id entry_markers_name;
	c_string_id boarding_grenade_marker;
	c_string_id boarding_grenade_string;
	c_string_id boarding_melee_string;
	c_string_id in_seat_string;

	// nathan is too lazy to make pings for each seat.
	real32 ping_scale;

	// how much time it takes to evict a rider from a flipped vehicle
	real32 turnover_time; // seconds

	unit_seat_acceleration acceleration;
	real32 ai_scariness;
	c_enum<e_global_ai_seat_type, int16, _global_ai_seat_type_none, k_global_ai_seat_type_count> ai_seat_type;
	int16 boarding_seat;

	// how far to interpolate listener position from camera to occupant's head
	real32 listener_interpolation_factor;

	// speed dependant turn rates
	// when the unit velocity is 0, the yaw/pitch rates are the left values
	// at [max speed reference], the yaw/pitch rates are the right values.
	// the max speed reference is what the code uses to generate a clamped speed from 0..1
	// the exponent controls how midrange speeds are interpreted.
	real_bounds yaw_rate_bounds; // degrees per second
	real_bounds pitch_rate_bounds; // degrees per second

	// 0 means use default 17
	real32 pitch_interpolation_time; // seconds to interpolate
	real32 min_speed_reference;
	real32 max_speed_reference;
	real32 speed_exponent;

	// camera fields
	s_unit_camera unit_camera;
	c_typed_tag_block<unit_hud_reference> unit_hud_interface;
	c_string_id enter_seat_string;

	real32 yaw_minimum;
	real32 yaw_maximum;
	c_typed_tag_reference<CHARACTER_TAG, INVALID_TAG> built_in_gunner;

	// entry fields
	// note: the entry radius shouldn't exceed 3 world units,
	// as that is as far as the player will search for a vehicle
	// to enter.

	// how close to the entry marker a unit must be
	real32 entry_radius;

	// real32 from marker forward the unit must be
	real32 entry_marker_cone_angle;

	// real32 from unit facing the marker must be
	real32 entry_marker_facing_angle;
	real32 maximum_relative_velocity;

	c_string_id invisible_seat_region;
	int32 runtime_invisible_seat_region_index;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(unit_seat) == 0xE4);

