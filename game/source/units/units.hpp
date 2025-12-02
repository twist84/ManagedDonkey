#pragma once

#include "ai/sector.hpp"
#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "motor/mover.hpp"
#include "objects/target_tracking.hpp"
#include "units/unit_definition.hpp"

int32 const k_seat_acceleration_memory_length = 6;

enum e_weapon_addition_method
{
	_unit_add_weapon_unknown_method = NONE,
	_unit_add_weapon_normally,
	_unit_add_weapon_silently,
	_unit_add_weapon_as_only_weapon,
	_unit_add_weapon_as_primary_weapon,
	_unit_add_weapon_swap_for_primary_weapon,
	_unit_add_weapon_as_secondary_weapon,
	_unit_add_weapon_swap_for_secondary_weapon,
	_unit_add_weapon_as_secondary_weapon_silently,
	_unit_add_weapon_unknown8,

	k_unit_add_weapon_modes_count,
	k_unit_add_weapon_modes_bits = 4,
};

struct unit_weapon_pickup_result
{
	bool can_pick_up[2];
	bool can_swap_for_current[2];
};
static_assert(sizeof(unit_weapon_pickup_result) == 0x4);

struct s_unit_weapon_set
{
	uns16 set_identifier;
	c_static_array<int8, k_unit_weapon_set_count> weapon_indices;
};

struct unit_control_data
{
	string_id desired_mode;
	int16 aiming_speed;
	s_unit_weapon_set weapon_set;
	int16 grenade_index;
	int16 zoom_level;
	uns32 control_flags;
	real_vector3d throttle;
	real32 primary_trigger;
	real32 secondary_trigger;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d looking_vector;
	real_point3d gaze_position;
	s_aim_assist_targeting_result aim_assist_data;
};
static_assert(sizeof(unit_control_data) == 0x80);

struct s_unit_predicted_weapon_state
{
	int16 rounds_loaded;
	int16 rounds_inventory;
	int32 definition_index;
};
static_assert(sizeof(s_unit_predicted_weapon_state) == 0x8);

struct s_unknown_unit_struct_sizeof_14
{
	int32 __unknown0;
	int32 __unknown4;
	int32 __unknown8;
	int32 __unknownC;
	int32 __time10;
};

struct unit_attacker
{
	int32 game_time;
	real32 score;
	int32 team_index;
	int32 player_index;
};
static_assert(sizeof(unit_attacker) == 0x10);

// $TODO are there more than 32 flags?
enum e_unit_control_flags
{
	// _button_crouch
	// _button_vehicle_dive
	_unit_control_crouch_modifier_bit = 0,

	// _button_jump
	// _button_vehicle_raise
	_unit_control_jump_bit,

	// _button_flashlight
	_unit_control_vision_mode_bit,

	_unit_control_exact_facing_bit,

	// _button_use_consumable_1
	_unit_control_use_equipment_bit,

	// _button_melee_attack
	_unit_control_melee_attack_bit,

	_unit_control_melee_lunge_bit,

	// _button_use_consumable_2
	_unit_control_equipment_2_bit,

	// _button_lean_right
	_unit_control_sprint_bit8,

	// _button_use_consumable_3
	_unit_control_equipment_3_bit,

	// _button_use_consumable_4
	_unit_control_equipment_4_bit,

	// _button_vehicle_boost
	_unit_control_sprint_bit11,

	_unit_control_bit12,
	_unit_control_bit13,

	// _button_vehicle_boost
	_unit_control_bit14,

	_unit_control_jetpack_bit,

	_unit_control_bit16,
	_unit_control_bit17,

	_unit_control_primary_weapon_primary_trigger_bit,
	_unit_control_primary_weapon_secondary_trigger_bit,
	_unit_control_primary_weapon_predicted_primary_trigger_bit,
	_unit_control_primary_weapon_predicted_secondary_trigger_bit,

	// _button_flashlight
	_unit_control_primary_weapon_power_toggle_bit,

	_unit_control_secondary_weapon_primary_trigger_bit,
	_unit_control_secondary_weapon_secondary_trigger_bit,
	_unit_control_secondary_weapon_predicted_primary_trigger_bit,
	_unit_control_secondary_weapon_predicted_secondary_trigger_bit,

	// _button_flashlight
	_unit_control_secondary_weapon_power_toggle_bit,

	// _button_throw_grenade
	_unit_control_throw_grenade_bit,

	_unit_control_reload_primary_bit,
	_unit_control_reload_secondary_bit,

	// _button_fire_tertiary
	_unit_control_tertiary_weapon_bit,

	k_unit_control_flags_count
};

struct _unit_datum
{
	int32 awake_tick_count;
	int32 actor_index;
	int32 simulation_actor_index;
	uns32 flags;
	int32 team;
	int32 player_index;
	int32 last_weak_player_index;
	int32 game_time_at_last_unit_effect;
	uns32 unit_control_flags;
	int32 desired_mode;
	real_vector3d desired_facing_vector;
	real_vector3d desired_aiming_vector;
	real_vector3d aiming_vector;
	real_vector3d aiming_velocity;
	real_vector3d desired_looking_vector;
	real_vector3d looking_vector;
	real_vector3d looking_velocity;
	real_point3d gaze_position;
	real_vector3d throttle;
	real_vector3d control_throttle;
	uns8 control_context_identifier;
	int8 aiming_speed;
	s_damage_reporting_info special_death_damage_reporting_info;
	int8 special_death_type;
	int16 special_death_node_index;

	bool __unknownAA;
	byte __padAB[0x1];

	real_vector3d special_death_node_acceleration;
	real32 primary_trigger;
	real32 secondary_trigger;
	s_aim_assist_targeting_result aim_assist_data;
	c_target_tracking_system target_tracking;
	uns32 persistent_control_flags;
	int8 weapon_drop_delay_ticks;
	int8 left_eye_node;
	int8 right_eye_node;
	uns8 horizontal_aiming_change;
	uns8 primary_fire_timer;
	uns8 secondary_fire_timer;
	int16 parent_seat_index;
	real32 ambient_illumination;
	real32 self_illumination;
	real32 mouth_aperture;
	real32 mouth_time;
	s_unit_weapon_set current_weapon_set;
	s_unit_weapon_set desired_weapon_set;
	c_static_array<int32, 4> weapon_object_indices;
	c_static_array<int32, 4> weapon_last_used_at_game_time;
	c_static_array<int32, 4> equipment_object_indices;
	c_static_array<int32, 4> active_equipment_object_indices;
	int32 equipment_pickup_time;
	int32 consumable_energy_level;
	int32 consumable_energy_restored_game_time;
	int16 weapon_firing_time;
	int8 current_grenade_index;
	int8 desired_grenade_index;
	c_static_array<uns8, 4> grenade_counts;
	int8 current_zoom_level;
	int8 desired_zoom_level;
	int8 gunner_inactive_ticks;
	int8 aiming_change;
	int32 weapon_control_last_active_time;
	int32 driver_object_index;
	int32 gunner_object_index;
	int32 last_vehicle_index;
	int32 game_time_at_last_vehicle_exit;
	int16 fake_squad_index;
	int8 impulse_vehicle_from_seat_ticks;
	int8 seat_power_valid_flags;
	c_static_array<real32, 1> seat_power;
	real32 integrated_light_power;
	real32 integrated_light_battery;
	real32 integrated_night_vision_power;
	real32 open_state;
	real_vector3d seat_acceleration;
	c_static_array<real_point3d, k_seat_acceleration_memory_length> seat_acceleration_memory;
	int16 seat_acceleration_memory_index;
	int32 last_seat_acceleration_reset_from_warping_time;

	int16 predicted_seat_index;
	int32 predicted_vehicle_index;

	int32 __unknown234; // predicted?
	int32 __unknown238; // predicted?

	int32 predicted_player_index;
	int32 predicted_simulation_actor_index;
	int32 predicted_simulation_actor_squad_index;
	int32 predicted_simulation_actor_fire_team_index;
	int32 predicted_simulation_actor_starting_location_index;
	c_static_array<s_unit_predicted_weapon_state, 4> predicted_weapon_state;

	real32 active_camouflage;
	real32 recent_active_camouflage;
	real32 active_camouflage_regrowth;
	int32 active_camouflage_end_time;

	// health pack equipment values
	real32 last_used_healthpack_time;
	real32 healthpack_vitality;
	real32 healthpack_shield;

	uns8 map_editor_helper_flags;

	int16 emp_timer;
	int16 emp_campaign_metagame_timer;

	real32 crouch;

	int16 delayed_damage_category;
	int16 delayed_damage_timer;
	real32 delayed_damage_peak;
	int32 delayed_damage_owner_object_index;
	int32 flaming_death_attacker_object_index;

	real32 run_blindly_angle;
	real32 run_blindly_angle_delta;

	int32 hologram_creator_weak_unit_index;
	int32 hologram_creation_time;
	int32 hologram_ticks_left;
	int32 hologram_definition_index;
	real32 hologram_shimmer_value;
	real_point3d hologram_destination;

	int32 sync_action_type;
	real_point3d sync_action_origin;
	real_vector3d sync_action_forward;
	real_vector3d sync_action_up;
	bool sync_action_critical_participant;
	byte __pad2F9[0x3];

	// pass to `sub_B4BCB0`, is this an array?
	s_unknown_unit_struct_sizeof_14 __unknown2FC;
	s_unknown_unit_struct_sizeof_14 __unknown310;

	int32 time_of_death;
	real32 movement_stun;
	int16 movement_stun_ticks;

	int16 killing_spree_count;
	int32 killing_spree_last_time;

	int16 melee_inhibit_ticks;

	byte __data336[0x2];

	// updated in `unit_record_damage`
	c_static_array<unit_attacker const, 4> attackers;
	c_static_array<int32, 4> attacker_weapon_unit_indices;

	// used in `unit_compute_boost_fraction`
	real32 boost_power_meter;
	real32 boost_recharge_delay;

	int32 last_enemies_moving_nearby_tick;
	int32 last_damaged_tick;
	int32 last_in_soft_kill_volume_time;
	int32 last_in_phantom_volume_time;

	// updated in `unit_update_team_index`
	int32 __unknown3A0_team_index_update_time; // time value
	int32 __unknown3A4_team_index;

	// updated in `sub_B4BD70`
	int32 __unknown3A8_object_index;

	// only 2 calls to `object_header_block_allocate` within `unit_new`
	object_header_block_reference debug_unit_input_storage;
	object_header_block_reference seat_storage;

	uns32 ai_unit_flags;
	c_sector_ref ai_sector;
	int32 debug_unit_input[16];

	// _equipment_type_armor_lock, e_unit_flags bit 17
	bool __unknown3FC;
	byte __pad3FD[0x3];

	// saber related, used is `unit_delete`, `unit_dispose_from_old_structure_bsp`, `sub_B486D0`
	int32 emblem_player_shoulder;
	int32 emblem_clan_chest;

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
static_assert(0x408 == OFFSETOF(_unit_datum, __data408));

struct unit_datum
{
	int32 definition_index;
	_object_datum object;
	_mover_datum mover;
	_unit_datum unit;
};
static_assert(sizeof(unit_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_mover_datum) + sizeof(_unit_datum));

struct unit_seat_source
{
	int32 vehicle_index;
	int16 seat_index;
	c_flags<e_unit_seat_flags, uns32, k_unit_seat_flags>* flags;
};
static_assert(sizeof(unit_seat_source) == 0xC);

extern bool debug_objects_unit_vectors;
extern bool debug_objects_unit_seats;
extern bool debug_objects_unit_mouth_apeture;
extern bool debug_objects_unit_firing;
extern bool debug_objects_unit_acceleration;
extern bool debug_objects_unit_camera;

extern int32 __cdecl unit_get_current_primary_weapon(int32 unit_index);
extern bool __cdecl any_unit_is_dangerous(int32* out_unit_index);
extern bool __cdecl unit_active_camouflage_is_active(int32 unit_index);
extern void __cdecl unit_active_camouflage_ding(int32 unit_index, real32 ding_amount, real32 transition_time);
extern void __cdecl unit_active_camouflage_disable(int32 unit_index, real32 transition_time);
extern void __cdecl unit_active_camouflage_enable(int32 unit_index, real32 transition_time, int32 end_time);
extern void __cdecl unit_active_camouflage_set_maximum(int32 unit_index, real32 maximum);
extern void __cdecl unit_add_equipment_to_inventory(int32 unit_index, int32 slot_index, int32 equipment_index);
extern bool __cdecl unit_add_grenade_to_inventory(int32 unit_index, int32 equipment_index);
extern int16 __cdecl unit_add_grenade_type_to_inventory(int32 unit_index, int16 grenade_type, int16 grenade_count);
extern void __cdecl unit_add_starting_profile_equipment(int32 unit_index, int16 starting_profile_index, bool reset, bool infinite_supply);
extern bool __cdecl unit_add_weapon_to_inventory(int32 unit_index, int32 object_index, e_weapon_addition_method mode);
extern bool __cdecl unit_can_access_object(int32 unit_index, int32 object_index);
extern bool __cdecl unit_can_pickup_weapon(int32 unit_index, int32 weapon_index, e_weapon_addition_method mode, unit_weapon_pickup_result* result);
extern bool __cdecl unit_can_pickup_equipment(int32 unit_index, int32 equipment_index);
extern bool __cdecl unit_can_see_point(int32 unit_index, const real_point3d* point, real32 theta);
extern bool __cdecl unit_clip_vector_to_aiming_screen_bounds(int32 unit_index, real_vector3d* vector, bool use_looking_screen);
extern void __cdecl unit_control(int32 unit_index, const unit_control_data* control_data);
extern void __cdecl unit_debug_ninja_rope(int32 unit_index);
extern bool __cdecl unit_get_acceleration_origin_and_data(int32 unit_index, real_matrix4x3* origin, unit_seat_acceleration* acceleration_data);
extern int32 __cdecl unit_get_active_primary_weapon(int32 unit_index, int32* gunner_unit_index_ref);
extern int32 __cdecl unit_get_aim_assist_dash_target(int32 unit_index);
extern bool __cdecl unit_get_aim_position(int32 unit_index, real_point3d* aim_position);
extern int32 __cdecl unit_get_aiming_unit_index(int32 unit_index);
extern void __cdecl unit_get_aiming_vector(int32 unit_index, real_vector3d* aiming_vector);
extern int16 __cdecl unit_get_all_seats(int32 unit_index, unit_seat_source* seat_array, int16 max_number_of_seats, bool allow_dead_vehicles);
extern void __cdecl unit_get_camera_position(int32 unit_index, real_point3d* camera_position);
extern int32 __cdecl unit_get_current_equipment(int32 unit_index, int32 slot_index);
extern real32 __cdecl unit_get_field_of_view(int32 unit_index, real32 default_field_of_view, int16 zoom_level);
extern void __cdecl unit_get_head_position(int32 unit_index, real_point3d* head_position);
extern int16 __cdecl unit_get_zoom_level(int32 unit_index);
extern bool __cdecl unit_has_weapon_definition_index(int32 unit_index, int32 weapon_definition_index);
extern int32 __cdecl unit_inventory_get_weapon(int32 unit_index, int16 inventory_index);
extern void __cdecl unit_render_debug(int32 unit_index);
extern void __cdecl unit_render_debug_indicator(int32 unit_index, const real_argb_color* color);
extern void __cdecl unit_set_aiming_vectors(int32 unit_index, const real_vector3d* aiming_vector, const real_vector3d* looking_vector);
extern bool __cdecl unit_try_to_drop_weapon(int32 unit_index, bool drop_secondary);
extern bool __cdecl unit_unsuspecting(int32 unit_index, const real_point3d* attacker_position, const real_vector3d* attacker_direction);
extern void __cdecl unit_unzoom(int32 unit_index, bool unzoom_binoculars_only);
extern bool __cdecl unit_update(int32 unit_index);
extern void __cdecl unit_update_active_camouflage(int32 unit_index);
extern bool __cdecl unit_update_aiming(int32 unit_index);
extern bool __cdecl unit_update_armor_lock(int32 unit_index);
extern void __cdecl unit_update_consumable_energy(int32 unit_index);
extern bool __cdecl unit_update_control(int32 unit_index);
extern bool __cdecl unit_update_damage(int32 unit_index);
extern void __cdecl unit_update_driver_and_gunner(int32 unit_index);
extern bool __cdecl unit_update_equipment(int32 unit_index, int32 slot_index);
extern void __cdecl unit_update_health(int32 unit_index);
extern void __cdecl unit_update_hologram(int32 unit_index);
extern void __cdecl unit_update_illumination(int32 unit_index);
extern void __cdecl sub_B4BCB0(s_unknown_unit_struct_sizeof_14* a1);
extern bool __cdecl sub_B4BD70(int32 unit_index);
extern void __cdecl unit_update_predicted_controller(int32 unit_index);
extern void __cdecl unit_sync_with_predicted_vehicle(int32 unit_index);
extern bool __cdecl unit_update_seats(int32 unit_index);
extern void __cdecl unit_update_target_tracking(int32 unit_index);
extern void __cdecl unit_update_team_index(int32 unit_index);
extern void __cdecl unit_update_vision_mode(int32 unit_index);
extern bool __cdecl unit_update_weapons(int32 unit_index);
extern bool __cdecl unit_use_current_equipment(int32 unit_index, int32 slot_index, bool network_predicted);
extern void __cdecl sub_B4CF60(int32 unit_index, int32 equipment_definition_index);
extern bool __cdecl sub_B4D160(int32 unit_index);
extern real64 __cdecl sub_B4D1E0(int32 unit_index);
extern int16 __cdecl unit_weapon_next_index(int32 unit_index, int32 desired_seat, int16 current_index, int16 delta);
extern bool __cdecl units_debug_can_select_unit(int32 unit_index);
extern int32 __cdecl units_debug_get_closest_unit(int32 unit_index);
extern int32 __cdecl units_debug_get_next_unit(int32 unit_index);

