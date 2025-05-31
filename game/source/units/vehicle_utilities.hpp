#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_stop_counter
{
	uns8 m_value;
};
static_assert(sizeof(c_vehicle_stop_counter) == sizeof(uns8));

struct c_vehicle_braking_counter
{
	uns8 m_value;
};
static_assert(sizeof(c_vehicle_braking_counter) == sizeof(uns8));

struct s_havok_vehicle_physics_instance;
extern void __cdecl vehicle_try_to_join_local_physics(int32 vehicle_index, const s_havok_vehicle_physics_instance* instance);

