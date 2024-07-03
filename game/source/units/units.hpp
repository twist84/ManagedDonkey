#pragma once

#include "ai/sector.hpp"
#include "cseries/cseries.hpp"
#include "game/aim_assist.hpp"
#include "motor/mover.hpp"
#include "objects/target_tracking.hpp"
#include "units/unit_definition.hpp"

long const k_seat_acceleration_memory_length = 6;

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
	word grenade_index;
	word zoom_level;
	dword_flags control_flags;
	vector3d throttle;
	real primary_trigger;
	real secondary_trigger;
	vector3d facing_vector;
	vector3d aiming_vector;
	vector3d looking_vector;
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
	long unit_control_flags;
	long desired_mode;
	vector3d facing_vector;
	vector3d desired_aiming_vector;
	vector3d aiming_vector;
	vector3d aiming_velocity;
	vector3d desired_looking_vector;
	vector3d looking_vector;
	vector3d looking_velocity;
	real_point3d gaze_position;
	vector3d throttle;
	vector3d control_throttle;
	byte control_context_identifier;
	char aiming_speed;
	s_damage_reporting_info special_death_damage_reporting_info;
	char special_death_type;
	short special_death_node_index;

	bool __unknownAA;
	byte __padAB[0x1];

	vector3d special_death_node_acceleration;
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
	real consumable_energy_level;
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
	vector3d seat_acceleration;
	c_static_array<real_point3d, k_seat_acceleration_memory_length> seat_acceleration_memory;
	short seat_acceleration_memory_index;
	byte seat_acceleration__data226[0x2];
	long seat_acceleration_reset_time;

	short predicted_seat_index;

	byte __data22E[0x2];

	long predicted_vehicle_index;

	long __unknown234; // predicted?
	long __unknown238; // predicted?

	long predicted_player_index;
	long predicted_simulation_actor_index;
	long predicted_simulation_actor_squad_index;
	long predicted_simulation_actor_cell_index;
	long predicted_simulation_actor_spawn_point_index;
	c_static_array<s_unit_predicted_weapon_state, 4> predicted_weapon_state;

	real active_camouflage;
	real active_camouflage_super_amount;
	real active_camouflage_regrowth;
	long active_camouflage_end_time;

	// health pack equipment values
	real last_used_healthpack_game_time; // health pack use end time
	real healthpack_vitality;            // health pack heath amount  / health pack duration
	real healthpack_shield;              // health pack shield amount / health pack duration;

	byte_flags map_editor_helper_flags;
	byte __data28D[0x1];

	short emp_timer;
	short emp_campaign_metagame_timer;
	byte __data292[0x2];

	real crouch;

	short last_damage_category;
	short delayed_damage_timer;
	real delayed_damage_peak;
	long delayed_damage_attacker_object_index;
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
	vector3d sync_action_forward;
	vector3d sync_action_up;
	bool sync_action_critical_participant;
	byte __pad2F9[0x3];

	// pass to `sub_B4BCB0`, is this an array?
	s_unknown_unit_struct_sizeof_14 __unknown2FC;
	s_unknown_unit_struct_sizeof_14 __unknown310;

	long time_of_death;
	real movement_stun;
	short movement_stun_ticks;

	short __unknown32E;
	long __unknown330; // time value

	// updated in `unit_update_damage`, unit_damage_aftermath_apply
	short __unknown334;

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
	c_sector_ref pathfinding_sector;
	byte __data3BC[0x40];

	// _equipment_type_armor_lock, e_unit_flags bit 17
	bool __unknown3FC;
	byte __pad3FD[0x3];

	// saber related, used is `unit_delete`, `unit_disconnect_from_structure_bsp`
	long __unknown400[2];

	byte __data408[0x4];
};
static_assert(sizeof(_unit_datum) == 0x40C);
static_assert(0x0AA == offsetof(_unit_datum, __unknownAA));
static_assert(0x0AB == offsetof(_unit_datum, __padAB));
static_assert(0x22E == offsetof(_unit_datum, __data22E));
static_assert(0x234 == offsetof(_unit_datum, __unknown234));
static_assert(0x238 == offsetof(_unit_datum, __unknown238));
static_assert(0x28D == offsetof(_unit_datum, __data28D));
static_assert(0x292 == offsetof(_unit_datum, __data292));
static_assert(0x2F9 == offsetof(_unit_datum, __pad2F9));
static_assert(0x2FC == offsetof(_unit_datum, __unknown2FC));
static_assert(0x310 == offsetof(_unit_datum, __unknown310));
static_assert(0x32E == offsetof(_unit_datum, __unknown32E));
static_assert(0x330 == offsetof(_unit_datum, __unknown330));
static_assert(0x334 == offsetof(_unit_datum, __unknown334));
static_assert(0x336 == offsetof(_unit_datum, __data336));
static_assert(0x3A0 == offsetof(_unit_datum, __unknown3A0_team_index_update_time));
static_assert(0x3A4 == offsetof(_unit_datum, __unknown3A4_team_index));
static_assert(0x3A8 == offsetof(_unit_datum, __unknown3A8_object_index));
static_assert(0x3BC == offsetof(_unit_datum, __data3BC));
static_assert(0x3FC == offsetof(_unit_datum, __unknown3FC));
static_assert(0x3FD == offsetof(_unit_datum, __pad3FD));
static_assert(0x400 == offsetof(_unit_datum, __unknown400));
static_assert(0x408 == offsetof(_unit_datum, __data408));

struct unit_datum
{
	long definition_index;
	_object_datum object;
	_motor_datum motor;
	_unit_datum unit;
};
static_assert(sizeof(unit_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_motor_datum) + sizeof(_unit_datum));

struct unit_seat_source
{
	long vehicle_index;
	short seat_index;
	byte __data6[0x2];
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
extern void __cdecl unit_active_camouflage_disable(long unit_index, real interpolation_time);
extern void __cdecl unit_active_camouflage_enable(long unit_index, real interpolation_time, long end_time);
extern void __cdecl unit_active_camouflage_strength(long unit_index, real active_camouflage);
extern void __cdecl unit_add_equipment_to_inventory(long unit_index, long slot_index, long object_index);
extern bool __cdecl unit_add_grenade_to_inventory(long unit_index, long object_index);
extern short __cdecl unit_add_grenade_type_to_inventory(long unit_index, short grenade_type, short grenade_count);
extern void __cdecl unit_add_starting_profile_equipment(long unit_index, short profile_index, bool clear_player, bool create_new);
extern bool __cdecl unit_add_weapon_to_inventory(long unit_index, long object_index, long weapon_addition_method);
extern void __cdecl unit_control(long unit_index, unit_control_data const* control_data);
extern long __cdecl unit_get_active_primary_weapon(long unit_index, long* parent_unit_index);
extern long __cdecl unit_get_aim_assist_dash_target(long unit_index);
extern bool __cdecl unit_get_aim_position(long unit_index, real_point3d* aim_position);
extern long __cdecl unit_get_aiming_unit_index(long unit_index);
extern void __cdecl unit_get_aiming_vector(long unit_index, vector3d* aiming_vector);
extern short __cdecl unit_get_all_seats(long unit_index, unit_seat_source* sources, short maximum_source_count, bool a4);
extern void __cdecl unit_get_camera_position(long unit_index, real_point3d* position);
extern real __cdecl unit_get_field_of_view(long unit_index, real fov_radians, short zoom_level);
extern void __cdecl unit_get_head_position(long unit_index, real_point3d* position);
extern short __cdecl unit_get_zoom_level(long unit_index);
extern bool __cdecl unit_has_weapon_definition_index(long unit_index, long weapon_definition_index);
extern void __cdecl unit_render_debug(long unit_index);
extern bool __cdecl unit_try_to_drop_weapon(long unit_index, bool drop_secondary);
extern bool __cdecl units_debug_can_select_unit(long unit_index);
extern long __cdecl units_debug_get_closest_unit(long unit_index);
extern long __cdecl units_debug_get_next_unit(long unit_index);

