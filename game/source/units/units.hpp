#pragma once

#include "ai/sector.hpp"
#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "motor/mover.hpp"
#include "objects/target_tracking.hpp"
#include "units/unit_definition.hpp"

long const k_seat_acceleration_memory_length = 6;

enum
{
	_unit_estimate_none = 0,
	_unit_estimate_head_standing,
	_unit_estimate_head_crouching,
	_unit_estimate_gun_position,
	_unit_estimate_aiming_standing,
	_unit_estimate_aiming_crouching,

	k_unit_estimate_position_modes_count
};

enum e_unit_drop_type
{
	_unit_drop_type_drop = 0,
	_unit_drop_type_delete,
	_unit_drop_type_drop_right,
	_unit_drop_type_drop_left,
	_unit_drop_type_response_to_deletion,

	k_unit_drop_type_count,

	k_unit_drop_type_none = -1
};

enum e_weapon_set
{
	_weapon_set_primary = 0,
	_weapon_set_secondary,

	k_weapon_set_count
};

struct s_unit_weapon_set
{
	word set_identifier;
	c_static_array<char, k_weapon_set_count> weapon_indices;
};

struct unit_control_data
{
	string_id desired_mode;
	short aiming_speed;
	s_unit_weapon_set weapon_set;
	short grenade_index;
	short zoom_level;
	dword_flags control_flags;
	real_vector3d throttle;
	real primary_trigger;
	real secondary_trigger;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d looking_vector;
	real_point3d gaze_position;
	s_aim_assist_targeting_result aim_assist_data;
};
static_assert(sizeof(unit_control_data) == 0x80);

struct s_unit_predicted_weapon_state
{
	short rounds_loaded;
	short rounds_inventory;
	long definition_index;
};
static_assert(sizeof(s_unit_predicted_weapon_state) == 0x8);

struct s_unknown_unit_struct_sizeof_14
{
	long __unknown0;
	long __unknown4;
	long __unknown8;
	long __unknownC;
	long __time10;
};

struct unit_attacker
{
	long game_time;
	real score;
	long team_index;
	long player_index;
};
static_assert(sizeof(unit_attacker) == 0x10);

// #TODO: are there more than 32 flags?
enum e_unit_control_flags
{
	// _button_action_crouch
	// _button_action_vehicle_dive
	_unit_control_crouch_modifier_bit = 0,

	// _button_action_jump
	// _button_action_vehicle_raise
	_unit_control_jump_bit,

	// _button_action_flashlight
	_unit_control_vision_mode_bit,

	_unit_control_exact_facing_bit,

	// _button_action_use_consumable_1
	_unit_control_use_equipment_bit,

	// _button_action_melee
	_unit_control_melee_attack_bit,

	_unit_control_melee_lunge_bit,

	// _button_action_use_consumable_2
	_unit_control_equipment_2_bit,

	// _button_action_lean_right
	_unit_control_sprint_bit8,

	// _button_action_use_consumable_3
	_unit_control_equipment_3_bit,

	// _button_action_use_consumable_4
	_unit_control_equipment_4_bit,

	// _button_action_vehicle_boost
	_unit_control_sprint_bit11,

	_unit_control_bit12,
	_unit_control_bit13,

	// _button_action_vehicle_boost
	_unit_control_bit14,

	_unit_control_jetpack_bit,

	_unit_control_bit16,
	_unit_control_bit17,

	_unit_control_primary_weapon_primary_trigger_bit,
	_unit_control_primary_weapon_secondary_trigger_bit,
	_unit_control_primary_weapon_predicted_primary_trigger_bit,
	_unit_control_primary_weapon_predicted_secondary_trigger_bit,

	// _button_action_flashlight
	_unit_control_primary_weapon_power_toggle_bit,

	_unit_control_secondary_weapon_primary_trigger_bit,
	_unit_control_secondary_weapon_secondary_trigger_bit,
	_unit_control_secondary_weapon_predicted_primary_trigger_bit,
	_unit_control_secondary_weapon_predicted_secondary_trigger_bit,

	// _button_action_flashlight
	_unit_control_secondary_weapon_power_toggle_bit,

	// _button_action_throw_grenade
	_unit_control_throw_grenade_bit,

	_unit_control_reload_primary_bit,
	_unit_control_reload_secondary_bit,

	// _button_action_melee_fire
	_unit_control_tertiary_weapon_bit,

	k_unit_control_flags_count
};

struct _unit_datum
{
	long awake_tick_count;
	long actor_index;
	long simulation_actor_index;
	dword_flags flags;
	long team;
	long player_index;
	long last_weak_player_index;
	long game_time_at_last_unit_effect;
	dword_flags unit_control_flags;
	long desired_mode;
	real_vector3d facing_vector;
	real_vector3d desired_aiming_vector;
	real_vector3d aiming_vector;
	real_vector3d aiming_velocity;
	real_vector3d desired_looking_vector;
	real_vector3d looking_vector;
	real_vector3d looking_velocity;
	real_point3d gaze_position;
	real_vector3d throttle;
	real_vector3d control_throttle;
	byte control_context_identifier;
	char aiming_speed;
	s_damage_reporting_info special_death_damage_reporting_info;
	char special_death_type;
	short special_death_node_index;

	bool __unknownAA;
	byte __padAB[0x1];

	real_vector3d special_death_node_acceleration;
	real primary_trigger;
	real secondary_trigger;
	s_aim_assist_targeting_result aim_assist_data;
	c_target_tracking_system target_tracking;
	dword_flags persistent_control_flags;
	char weapon_drop_delay_ticks;
	char left_eye_node;
	char right_eye_node;
	byte horizontal_aiming_change;
	byte primary_fire_timer;
	byte secondary_fire_timer;
	short parent_seat_index;
	real ambient_illumination;
	real self_illumination;
	real mouth_aperture;
	real mouth_time;
	s_unit_weapon_set current_weapon_set;
	s_unit_weapon_set desired_weapon_set;
	c_static_array<long, 4> weapon_object_indices;
	c_static_array<long, 4> weapon_last_used_at_game_time;
	c_static_array<long, 4> equipment_object_indices;
	c_static_array<long, 4> active_equipment_object_indices;
	long equipment_pickup_time;
	long consumable_energy_level;
	long consumable_energy_restored_game_time;
	short weapon_firing_time;
	char current_grenade_index;
	char desired_grenade_index;
	c_static_array<byte, 4> grenade_counts;
	char current_zoom_level;
	char desired_zoom_level;
	char gunner_inactive_ticks;
	char aiming_change;
	long weapon_control_last_active_time;
	long driver_object_index;
	long gunner_object_index;
	long last_vehicle_index;
	long game_time_at_last_vehicle_exit;
	short fake_squad_index;
	char impulse_vehicle_from_seat_ticks;
	char seat_power_valid_flags;
	c_static_array<real, 1> seat_power;
	real integrated_light_power;
	real integrated_light_battery;
	real integrated_night_vision_power;
	real open_state;
	real_vector3d seat_acceleration;
	c_static_array<real_point3d, k_seat_acceleration_memory_length> seat_acceleration_memory;
	short seat_acceleration_memory_index;
	long last_seat_acceleration_reset_from_warping_time;

	short predicted_seat_index;
	long predicted_vehicle_index;

	long __unknown234; // predicted?
	long __unknown238; // predicted?

	long predicted_player_index;
	long predicted_simulation_actor_index;
	long predicted_simulation_actor_squad_index;
	long predicted_simulation_actor_fire_team_index;
	long predicted_simulation_actor_starting_location_index;
	c_static_array<s_unit_predicted_weapon_state, 4> predicted_weapon_state;

	real active_camouflage;
	real recent_active_camouflage;
	real active_camouflage_regrowth;
	long active_camouflage_end_time;

	// health pack equipment values
	real last_used_healthpack_time;
	real healthpack_vitality;
	real healthpack_shield;

	byte_flags map_editor_helper_flags;

	short emp_timer;
	short emp_campaign_metagame_timer;

	real crouch;

	short delayed_damage_category;
	short delayed_damage_timer;
	real delayed_damage_peak;
	long delayed_damage_owner_object_index;
	long flaming_death_attacker_object_index;

	real run_blindly_angle;
	real run_blindly_angle_delta;

	long hologram_creator_weak_unit_index;
	long hologram_creation_time;
	long hologram_ticks_left;
	long hologram_definition_index;
	real hologram_shimmer_value;
	real_point3d hologram_destination;

	long sync_action_type;
	real_point3d sync_action_origin;
	real_vector3d sync_action_forward;
	real_vector3d sync_action_up;
	bool sync_action_critical_participant;
	byte __pad2F9[0x3];

	// pass to `sub_B4BCB0`, is this an array?
	s_unknown_unit_struct_sizeof_14 __unknown2FC;
	s_unknown_unit_struct_sizeof_14 __unknown310;

	long time_of_death;
	real movement_stun;
	short movement_stun_ticks;

	short killing_spree_count;
	long killing_spree_last_time;

	short melee_inhibit_ticks;

	byte __data336[0x2];

	// updated in `unit_record_damage`
	c_static_array<unit_attacker const, 4> attackers;
	c_static_array<long, 4> attacker_weapon_unit_indices;

	// used in `unit_compute_boost_fraction`
	real boost_power_meter;
	real boost_recharge_delay;

	long last_enemies_moving_nearby_tick;
	long last_damaged_tick;
	long last_in_soft_kill_volume_time;
	long last_in_phantom_volume_time;

	// updated in `unit_update_team_index`
	long __unknown3A0_team_index_update_time; // time value
	long __unknown3A4_team_index;

	// updated in `sub_B4BD70`
	long __unknown3A8_object_index;

	// only 2 calls to `object_header_block_allocate` within `unit_new`
	object_header_block_reference debug_unit_input_storage;
	object_header_block_reference seat_storage;

	dword_flags ai_unit_flags;
	c_sector_ref ai_sector;
	long debug_unit_input[16];

	// _equipment_type_armor_lock, e_unit_flags bit 17
	bool __unknown3FC;
	byte __pad3FD[0x3];

	// saber related, used is `unit_delete`, `unit_dispose_from_old_structure_bsp`, `sub_B486D0`
	long __unknown400[2];

	byte __data408[0x4];
};
static_assert(sizeof(_unit_datum) == 0x40C);
static_assert(0x0AA == OFFSETOF(_unit_datum, __unknownAA));
static_assert(0x0AB == OFFSETOF(_unit_datum, __padAB));
static_assert(0x234 == OFFSETOF(_unit_datum, __unknown234));
static_assert(0x238 == OFFSETOF(_unit_datum, __unknown238));
static_assert(0x2F9 == OFFSETOF(_unit_datum, __pad2F9));
static_assert(0x2FC == OFFSETOF(_unit_datum, __unknown2FC));
static_assert(0x310 == OFFSETOF(_unit_datum, __unknown310));
static_assert(0x336 == OFFSETOF(_unit_datum, __data336));
static_assert(0x3A0 == OFFSETOF(_unit_datum, __unknown3A0_team_index_update_time));
static_assert(0x3A4 == OFFSETOF(_unit_datum, __unknown3A4_team_index));
static_assert(0x3A8 == OFFSETOF(_unit_datum, __unknown3A8_object_index));
static_assert(0x3FC == OFFSETOF(_unit_datum, __unknown3FC));
static_assert(0x3FD == OFFSETOF(_unit_datum, __pad3FD));
static_assert(0x400 == OFFSETOF(_unit_datum, __unknown400));
static_assert(0x408 == OFFSETOF(_unit_datum, __data408));

struct unit_datum
{
	long definition_index;
	_object_datum object;
	_mover_datum mover;
	_unit_datum unit;
};
static_assert(sizeof(unit_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_mover_datum) + sizeof(_unit_datum));

struct unit_seat_source
{
	long vehicle_index;
	short seat_index;
	c_flags<e_unit_seat_flags, dword, k_unit_seat_flags>* flags;
};
static_assert(sizeof(unit_seat_source) == 0xC);

extern bool debug_objects_unit_vectors;
extern bool debug_objects_unit_seats;
extern bool debug_objects_unit_mouth_apeture;
extern bool debug_objects_unit_firing;
extern bool debug_objects_unit_acceleration;
extern bool debug_objects_unit_camera;

extern long __cdecl unit_get_current_primary_weapon(long unit_index);
extern bool __cdecl any_unit_is_dangerous(long* out_unit_index);
extern bool __cdecl unit_active_camouflage_is_active(long unit_index);
extern void __cdecl unit_active_camouflage_ding(long unit_index, real active_camo_ding, real active_camo_regrowth_rate);
extern void __cdecl unit_active_camouflage_disable(long unit_index, real interpolation_time);
extern void __cdecl unit_active_camouflage_enable(long unit_index, real interpolation_time, long end_time);
extern void __cdecl unit_active_camouflage_strength(long unit_index, real active_camouflage);
extern void __cdecl unit_add_equipment_to_inventory(long unit_index, long slot_index, long object_index);
extern bool __cdecl unit_add_grenade_to_inventory(long unit_index, long object_index);
extern short __cdecl unit_add_grenade_type_to_inventory(long unit_index, short grenade_type, short grenade_count);
extern void __cdecl unit_add_starting_profile_equipment(long unit_index, short profile_index, bool clear_player, bool create_new);
extern bool __cdecl unit_add_weapon_to_inventory(long unit_index, long object_index, long weapon_addition_method);
extern void __cdecl unit_control(long unit_index, unit_control_data const* control_data);
extern void __cdecl unit_debug_ninja_rope(long unit_index);
extern bool __cdecl unit_get_acceleration_origin_and_data(long unit_index, real_matrix4x3* origin, unit_seat_acceleration* acceleration_data);
extern long __cdecl unit_get_active_primary_weapon(long unit_index, long* parent_unit_index);
extern long __cdecl unit_get_aim_assist_dash_target(long unit_index);
extern bool __cdecl unit_get_aim_position(long unit_index, real_point3d* aim_position);
extern long __cdecl unit_get_aiming_unit_index(long unit_index);
extern void __cdecl unit_get_aiming_vector(long unit_index, real_vector3d* aiming_vector);
extern short __cdecl unit_get_all_seats(long unit_index, unit_seat_source* sources, short maximum_source_count, bool a4);
extern void __cdecl unit_get_camera_position(long unit_index, real_point3d* position);
extern real __cdecl unit_get_field_of_view(long unit_index, real fov_radians, short zoom_level);
extern void __cdecl unit_get_head_position(long unit_index, real_point3d* position);
extern short __cdecl unit_get_zoom_level(long unit_index);
extern bool __cdecl unit_has_weapon_definition_index(long unit_index, long weapon_definition_index);
extern long __cdecl unit_inventory_get_weapon(long unit_index, short inventory_index);
extern void __cdecl unit_render_debug(long unit_index);
extern bool __cdecl unit_try_to_drop_weapon(long unit_index, bool drop_secondary);
extern bool __cdecl unit_unsuspecting(long unit_index, real_point3d const* attacker_position, real_vector3d const* attacker_direction);
extern void __cdecl unit_unzoom(long unit_index, bool a2);
extern bool __cdecl unit_update(long unit_index);
extern void __cdecl unit_update_active_camouflage(long unit_index);
extern bool __cdecl unit_update_aiming(long unit_index);
extern bool __cdecl unit_update_armor_lock(long unit_index);
extern void __cdecl unit_update_consumable_energy(long unit_index);
extern bool __cdecl unit_update_control(long unit_index);
extern bool __cdecl unit_update_damage(long unit_index);
extern void __cdecl unit_update_driver_and_gunner(long unit_index);
extern bool __cdecl unit_update_equipment(long unit_index, long slot_index);
extern void __cdecl unit_update_health(long unit_index);
extern void __cdecl unit_update_hologram(long unit_index);
extern void __cdecl unit_update_illumination(long unit_index);
extern void __cdecl sub_B4BCB0(s_unknown_unit_struct_sizeof_14* a1);
extern bool __cdecl sub_B4BD70(long unit_index);
extern void __cdecl unit_update_predicted_controller(long unit_index);
extern void __cdecl unit_sync_with_predicted_vehicle(long unit_index);
extern bool __cdecl unit_update_seats(long unit_index);
extern void __cdecl unit_update_target_tracking(long unit_index);
extern void __cdecl unit_update_team_index(long unit_index);
extern void __cdecl unit_update_vision_mode(long unit_index);
extern bool __cdecl unit_update_weapons(long unit_index);
extern bool __cdecl unit_use_current_equipment(long unit_index, long slot_index, bool a3);
extern void __cdecl sub_B4CF60(long unit_index, long equipment_definition_index);
extern bool __cdecl sub_B4D160(long unit_index);
extern double __cdecl sub_B4D1E0(long unit_index);
extern short __cdecl unit_weapon_next_index(long unit_index, long a2, short a3, short a4);
extern bool __cdecl units_debug_can_select_unit(long unit_index);
extern long __cdecl units_debug_get_closest_unit(long unit_index);
extern long __cdecl units_debug_get_next_unit(long unit_index);

extern unit_datum* unit_get(long unit_index);

