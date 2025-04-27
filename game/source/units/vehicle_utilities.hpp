#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_stop_counter
{
	uint8 m_value;
};
static_assert(sizeof(c_vehicle_stop_counter) == sizeof(uint8));

struct c_vehicle_braking_counter
{
	uint8 m_value;
};
static_assert(sizeof(c_vehicle_braking_counter) == sizeof(uint8));

struct s_havok_vehicle_physics_instance;
extern void __cdecl vehicle_try_to_join_local_physics(long vehicle_index, s_havok_vehicle_physics_instance const* instance);

