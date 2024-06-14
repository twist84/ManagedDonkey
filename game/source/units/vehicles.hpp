#pragma once

#include "units/units.hpp"
#include "units/vehicle_definitions.hpp"

struct vehicle_data
{
	byte __data0[0xCC];

	c_static_flags<64> const disallowed_seats;
	long const reserved_seats[128];

	byte __data2D4[0x6C];
};
static_assert(sizeof(vehicle_data) == 0x340);

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

