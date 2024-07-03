#pragma once

#include "ai/sector.hpp"
#include "units/units.hpp"
#include "units/vehicle_definitions.hpp"
#include "units/vehicle_type_component.hpp"

struct _vehicle_datum
{
	word_flags flags;
	byte upside_down_on_ground_ticks;
	byte airborne_ticks;
	byte upending_type;
	byte upending_ticks;
	byte on_ground_ticks;

	byte __data7[0x5];

	word slipping_wheels_bit_vector;

	byte __dataE[0x2];

	real mean_antigrav_fraction;
	c_vehicle_type_component type_component;
	long last_physics_cache_update_time;
	long squad_index;
	long next_vehicle_index;
	long spawn_point_name;
	long bipeds_killed;
	c_static_flags<64> const disallowed_seats;
	long const reserved_seats[128];
	long pathfinding_time;
	short pathfinding_structure_index;
	byte pathfinding__pad2DA[0x2];
	long pathfinding_surface_index;
	c_sector_ref pathfinding_sector;
	long pathfinding__unknown2E4;
	long pathfinding_object_index;
	dword pathfinding_bsp_reference;
	real_point3d pathfinding_point;
	long last_known_animation_graph_index;
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
static_assert(sizeof(_vehicle_datum) == 0x340);
static_assert(0x007 == offsetof(_vehicle_datum, __data7));
static_assert(0x00E == offsetof(_vehicle_datum, __dataE));
static_assert(0x2DA == offsetof(_vehicle_datum, pathfinding__pad2DA));

struct vehicle_datum
{
	_object_datum object;
	_motor_datum motor;
	_unit_datum unit;
	_vehicle_datum vehicle;
};
static_assert(sizeof(vehicle_datum) == sizeof(_object_datum) + sizeof(_motor_datum) + sizeof(_unit_datum) + sizeof(_vehicle_datum));

extern bool __cdecl vehicle_about_to_detonate_near_any_player(long* out_vehicle_index);
extern void __cdecl vehicle_get_seat_position(long vehicle_index, short seat_index, real_point3d* seat_position);
extern e_vehicle_type __cdecl vehicle_get_type(long vehicle_index);
extern bool __cdecl vehicle_moving_near_any_player(long* out_vehicle_index);
extern void __cdecl vehicle_render_debug(long vehicle_index);

