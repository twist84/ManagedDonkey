#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_stop_counter
{
	byte m_value;
};
static_assert(sizeof(c_vehicle_stop_counter) == sizeof(byte));

struct c_vehicle_braking_counter
{
	byte m_value;
};
static_assert(sizeof(c_vehicle_braking_counter) == sizeof(byte));

struct s_havok_vehicle_physics_instance;
extern void __cdecl vehicle_try_to_join_local_physics(long vehicle_index, s_havok_vehicle_physics_instance const* instance);

