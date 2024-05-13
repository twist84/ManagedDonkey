#pragma once

#include "units/units.hpp"
#include "units/vehicle_definitions.hpp"

struct vehicle_datum
{
	unit_datum unit;

	byte __data590[0xCC];

	c_static_flags<64> const disallowed_seats;
	long const reserved_seats[128];

	byte __data864[0x6C];
};
static_assert(sizeof(vehicle_datum) == 0x8D0);

extern bool __cdecl vehicle_about_to_detonate_near_any_player(long* out_vehicle_index);
extern void __cdecl vehicle_get_seat_position(long vehicle_index, short seat_index, real_point3d* seat_position);
extern e_vehicle_type __cdecl vehicle_get_type(long vehicle_index);
extern bool __cdecl vehicle_moving_near_any_player(long* out_vehicle_index);
extern void __cdecl vehicle_render_debug(long vehicle_index);

