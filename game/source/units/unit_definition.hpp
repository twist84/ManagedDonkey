#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "game/campaign_metagame_definitions.hpp"
#include "objects/object_definitions.hpp"
#include "shell/shell.hpp"

struct s_unit_camera_track
{
	c_typed_tag_reference<CAMERA_TRACK_TAG> track;

	void update_reference_names();
};
static_assert(sizeof(s_unit_camera_track) == sizeof(s_tag_reference));

struct s_unit_camera_acceleration_displacment_function
{
	real k;
	real scale;
	real power;
	real maximum;

	// scale factor used when this acceleration component is along the axis of the forward vector of the camera
	real camera_scale_axial;

	// scale factor used when this acceleration component is perpendicular to the camera
	real camera_scale_perpendicular;
};
static_assert(sizeof(s_unit_camera_acceleration_displacment_function) == 0x18);

struct s_unit_camera_acceleration
{
	// camera scale x MIN(scale x POWER((K x X), power), maximum)
	s_unit_camera_acceleration_displacment_function velocity_i;
	s_unit_camera_acceleration_displacment_function velocity_j;
	s_unit_camera_acceleration_displacment_function velocity_k;

	// maximum offset velocity
	real maximum_displacment_velocity;
};
static_assert(sizeof(s_unit_camera_acceleration) == 0x4C);

enum e_unit_camera_flags
{
	_unit_camera_flag_pitch_bounds_absolute_space_bit = 0,

	k_unit_camera_flags
};

struct s_unit_camera
{
	c_flags<e_unit_camera_flags, short, k_unit_camera_flags> flags;
	byte doh[0x2]; // pad
	c_old_string_id camera_marker_name;
	c_old_string_id camera_submerged_marker_name;
	angle pitch_auto_level;
	angle_bounds pitch_range;
	c_typed_tag_block<s_unit_camera_track> camera_tracks;
	angle pitch_minimum_spring;
	angle pitch_mmaximum_spring;
	angle spring_velocity;
	c_typed_tag_block<s_unit_camera_acceleration> camera_acceleration;
};
static_assert(sizeof(s_unit_camera) == 0x3C);

struct unit_seat_acceleration
{
	vector3d acceleration_range; // world units per second squared

	// actions fail
	real accel_action_scale; // [0,1+]

	// detach unit
	real accel_attach_scale; // [0,1+]
};
static_assert(sizeof(unit_seat_acceleration) == 0x14);

struct s_unit_additional_node_names
{
	c_string_id preferred_gun_node;
};
static_assert(sizeof(s_unit_additional_node_names) == sizeof(string_id));

struct s_unit_boarding_melee
{
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> boarding_melee_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> boarding_melee_response;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> eviction_melee_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> eviction_melee_response;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> landing_melee_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> flurry_melee_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> obstacle_smash_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> assassination_primary_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> assassination_ragdoll_damage;

	void update_reference_names();
};
static_assert(sizeof(s_unit_boarding_melee) == sizeof(s_tag_reference) * 9);

struct s_unit_boost
{
	c_typed_tag_reference<COLLISION_DAMAGE_TAG> boost_collision_damage;
	real boost_peak_power;
	real boost_rise_power;
	real boost_peak_time;
	real boost_fall_power;
	real dead_time;

	void update_reference_names();
};
static_assert(sizeof(s_unit_boost) == sizeof(real) * 5 + sizeof(s_tag_reference));

struct s_unit_lipsync_scales
{
	real_fraction attack_weight;
	real_fraction decay_weight;
};
static_assert(sizeof(s_unit_lipsync_scales) == 0x8);

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

	c_flags<e_unit_definition_flags, dword_flags, k_unit_definition_flags> flags;
	c_enum<e_unit_default_teams, short, _unit_default_teams_default, k_unit_default_teams_count> default_team;
	c_enum<e_ai_sound_volume, short, _ai_sound_volume_silent, k_ai_sound_volume_count> constant_sound_volume;
	c_typed_tag_reference<BIPED_TAG, VEHICLE_TAG> hologram_unit_reference;
	c_typed_tag_block<s_campaign_metagame_bucket> campaign_metagame_bucket;
	c_typed_tag_reference<EFFECT_TAG> integrated_light_toggle;
	angle camera_field_of_view; // degrees
	real camera_stiffness;
	s_unit_camera unit_camera;

	// sync action camera fields
	s_unit_camera sync_action_camera;

	c_typed_tag_reference<DAMAGE_RESPONSE_DEFINITION_TAG> assasination_start_damage_response;
	c_typed_tag_reference<WEAPON_TAG> assassination_weapon;

	// the anchor we attach the knife to when we stow it
	c_string_id assassination_weapon_stow_marker;

	// the anchor we attach the knife to when we pull it out
	c_string_id assassination_weapon_out_marker;

	// the marker on the knife that we anchor to the biped
	c_string_id assassination_weapon_anchor_marker;

	unit_seat_acceleration acceleration;

	real soft_ping_threshold; // [0,1]
	real soft_ping_interrupt_time; // seconds
	real hard_ping_threshold; // [0,1]
	real hard_ping_interrupt_time; // seconds
	real hard_death_threshold; // [0,1]
	real feign_death_threshold; // [0,1]
	real feign_death_time; // seconds

	// this must be set to tell the AI how far it should expect our dive animation to move us
	real distance_of_dive_anim; // world units

	// if we take this much damage in a short space of time we will play our 'stunned movement' animations
	real stunned_movement_threshold; // [0,1]
	real feign_death_chance; // [0,1]
	real feign_repeat_chance; // [0,1]

	// automatically created character when this unit is driven
	c_typed_tag_reference<CHARACTER_TAG> spawned_turret_character;

	// number of actors which we spawn
	int16_bounds spawned_actor_count;

	// velocity at which we throw spawned actors
	real spawned_velocity;

	angle aiming_velocity_maximum; // degrees per second
	angle aiming_acceleration_maximum; // degrees per second squared

	real_fraction casual_aiming_modifier; // [0,1]

	angle looking_velocity_maximum; // degrees per second
	angle looking_acceleration_maximum; // degrees per second squared

	// where the primary weapon is attached
	c_string_id right_hand_node;

	// where the seconday weapon is attached (for dual-pistol modes)
	c_string_id left_hand_node;

	s_unit_additional_node_names more_damn_nodes;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> melee_damage;
	s_unit_boarding_melee your_momma;
	c_enum<e_global_chud_blip_type, short, _global_chud_blip_type_medium, k_global_chud_blip_type_count> motion_sensor_blip_size;
	c_enum<e_unit_item_owner_size, short, _unit_item_owner_size_small, k_unit_item_owner_size_count> item_owner_size;
	c_typed_tag_block<s_posture_definition> postures;
	c_typed_tag_block<unit_hud_reference> new_hud_interfaces;
	c_typed_tag_block<dialogue_variant_definition> dialogue_variants;

	real grenade_maximum_autoaim_distance; // world units
	angle grenade_angle; // degrees
	angle grenade_angle_max_elevation; // degrees
	angle grenade_angle_min_elevation; // degrees
	real grenade_velocity; // world units per second
	c_enum<e_grenade_type, short, _grenade_type_human_fragmentation, k_grenade_type_count> grenade_type;
	short grenade_count;
	c_typed_tag_block<powered_seat_definition> powered_seats;
	c_typed_tag_block<unit_initial_weapon> weapons;
	c_typed_tag_block<s_target_tracking_parameters> target_trackingk;
	c_typed_tag_block<unit_seat> seats_block;

	// EMP Disabling
	real emp_disabled_time; // seconds
	c_typed_tag_reference<EFFECT_TAG> emp_disabled_effect;

	// Boost
	s_unit_boost boost;

	// Lipsync
	s_unit_lipsync_scales lipsync;

	// Exit and Detach
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> exit_and_detach_damage;
	c_typed_tag_reference<WEAPON_TAG> exit_and_detach_weapon;

	void update_reference_names();
};
static_assert(sizeof(_unit_definition) == 0x2C8);

struct unit_definition
{
	static tag const k_group_tag = UNIT_TAG;

	_object_definition object;
	_unit_definition unit;

	void update_reference_names();
};
static_assert(sizeof(unit_definition) == sizeof(_object_definition) + sizeof(_unit_definition));

struct s_posture_definition
{
	c_string_id name;
	vector3d pill_offset;
};
static_assert(sizeof(s_posture_definition) == 0x10);

struct unit_hud_reference
{
	c_typed_tag_reference<CHUD_DEFINITION_TAG> chud_interface;

	void update_reference_names();
};
static_assert(sizeof(unit_hud_reference) == 0x10);

struct dialogue_variant_definition
{
	short variant_number;
	byte BQCVEMF[2]; // pad
	c_typed_tag_reference<DIALOGUE_TAG> dialogue;

	void update_reference_names();
};
static_assert(sizeof(dialogue_variant_definition) == 0x14);

struct powered_seat_definition
{
	real driver_powerup_time;
	real driver_powerdown_time;
};
static_assert(sizeof(powered_seat_definition) == 0x8);

struct unit_initial_weapon
{
	c_typed_tag_reference<WEAPON_TAG> weapon;

	void update_reference_names();
};
static_assert(sizeof(unit_initial_weapon) == sizeof(s_tag_reference));

struct s_tracking_type
{
	c_string_id tracking_type;
};
static_assert(sizeof(s_tracking_type) == sizeof(string_id));

struct s_target_tracking_parameters
{
	c_typed_tag_block<s_tracking_type> tracking_types;
	real acquire_time;
	real grace_time;
	real decay_time;
	c_typed_tag_reference<SOUND_TAG, SOUND_LOOPING_TAG> tracking_sound;
	c_typed_tag_reference<SOUND_TAG, SOUND_LOOPING_TAG> locked_sound;

	void update_reference_names();
};
static_assert(sizeof(s_target_tracking_parameters) == 0x38);

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
	c_flags<e_unit_seat_flags, dword_flags, k_unit_seat_flags> flags;
	c_old_string_id label;
	c_old_string_id marker_name;
	c_string_id entry_markers_name;
	c_string_id boarding_grenade_marker;
	c_string_id boarding_grenade_string;
	c_string_id boarding_melee_string;
	c_string_id in_seat_string;

	// nathan is too lazy to make pings for each seat.
	real_fraction ping_scale;

	// how much time it takes to evict a rider from a flipped vehicle
	real turnover_time; // seconds

	unit_seat_acceleration acceleration;
	real ai_scariness;
	c_enum<e_global_ai_seat_type, short, _global_ai_seat_type_none, k_global_ai_seat_type_count> ai_seat_type;
	short boarding_seat;

	// how far to interpolate listener position from camera to occupant's head
	real_fraction listener_interpolation_factor;

	// speed dependant turn rates
	// when the unit velocity is 0, the yaw/pitch rates are the left values
	// at [max speed reference], the yaw/pitch rates are the right values.
	// the max speed reference is what the code uses to generate a clamped speed from 0..1
	// the exponent controls how midrange speeds are interpreted.
	real_bounds yaw_rate_bounds; // degrees per second
	real_bounds pitch_rate_bounds; // degrees per second

	// 0 means use default 17
	real pitch_interpolation_time; // seconds to interpolate
	real min_speed_reference;
	real max_speed_reference;
	real speed_exponent;

	// camera fields
	s_unit_camera unit_camera;
	c_typed_tag_block<unit_hud_reference> unit_hud_interface;
	c_string_id enter_seat_string;

	angle yaw_minimum;
	angle yaw_maximum;
	c_typed_tag_reference<CHARACTER_TAG> built_in_gunner;

	// entry fields
	// note: the entry radius shouldn't exceed 3 world units,
	// as that is as far as the player will search for a vehicle
	// to enter.

	// how close to the entry marker a unit must be
	real entry_radius;

	// angle from marker forward the unit must be
	angle entry_marker_cone_angle;

	// angle from unit facing the marker must be
	angle entry_marker_facing_angle;
	real maximum_relative_velocity;

	c_string_id invisible_seat_region;
	long runtime_invisible_seat_region_index;

	void update_reference_names();
};
static_assert(sizeof(unit_seat) == 0xE4);

