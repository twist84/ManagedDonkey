#pragma once

#include "units/units.hpp"
#include "units/vehicle_definitions.hpp"
#include "units/vehicle_type_component.hpp"

struct vehicle_data
{
	word_flags flags;
	byte upside_down_on_ground_ticks;
	byte airborne_ticks;
	byte upending_type;
	byte upending_ticks;

	byte __data6[0x6];

	word slipping_wheels_bit_vector;

	byte __data8[0x2];

	real mean_antigrav_fraction;
	c_vehicle_type_component type_component;
	long last_physics_cache_update_time;
	long squad_index;
	long next_vehicle_index;
	long spawn_point_name;
	long bipeds_killed;
	c_static_flags<64> const disallowed_seats;
	long const reserved_seats[128];

	long __unknown2D4;
	short __unknown2D8;
	byte __pad2DA[0x2];
	long __unknown2DC;
	c_sector_ref __sector_ref2E0;
	long __unknown2E4;
	long __unknown2E8;
	long __unknown2EC;
	real_point3d __point2F0;
	long __unknown2FC;

	c_animation_id steering_animation_id;
	c_animation_id roll_animation_id;
	c_animation_id throttle_animation_id;
	c_animation_id velocity_animation_id;
	c_animation_id acceleration_animation_id[3];
	real driver_force_position;
	real driver_force_scale;
	vector3d auto_aiming_vector;
	real_point3d last_soft_ceiling_update_position;
	object_header_block_reference vehicle_physics_cache;
};
static_assert(sizeof(vehicle_data) == 0x340);
static_assert(0x006 == offsetof(vehicle_data, __data6));
static_assert(0x2D4 == offsetof(vehicle_data, __unknown2D4));
static_assert(0x2D8 == offsetof(vehicle_data, __unknown2D8));
static_assert(0x2DA == offsetof(vehicle_data, __pad2DA));
static_assert(0x2DC == offsetof(vehicle_data, __unknown2DC));
static_assert(0x2E0 == offsetof(vehicle_data, __sector_ref2E0));
static_assert(0x2E4 == offsetof(vehicle_data, __unknown2E4));
static_assert(0x2E8 == offsetof(vehicle_data, __unknown2E8));
static_assert(0x2EC == offsetof(vehicle_data, __unknown2EC));
static_assert(0x2F0 == offsetof(vehicle_data, __point2F0));
static_assert(0x2FC == offsetof(vehicle_data, __unknown2FC));

struct vehicle_datum
{
	object_data object;
	motor_data motor;
	unit_data unit;
	vehicle_data vehicle;
};
static_assert(sizeof(vehicle_datum) == sizeof(object_data) + sizeof(motor_data) + sizeof(unit_data) + sizeof(vehicle_data));

extern bool __cdecl vehicle_about_to_detonate_near_any_player(long* out_vehicle_index);
extern void __cdecl vehicle_get_seat_position(long vehicle_index, short seat_index, real_point3d* seat_position);
extern e_vehicle_type __cdecl vehicle_get_type(long vehicle_index);
extern bool __cdecl vehicle_moving_near_any_player(long* out_vehicle_index);
extern void __cdecl vehicle_render_debug(long vehicle_index);

