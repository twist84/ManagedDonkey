#pragma once

#include "cseries/cseries.hpp"

extern void __cdecl actor_move_initialize();
extern bool __cdecl actor_moving_nondirectional_flying_vector_avoidance(int32 actor_index, int32 unit_index, real_vector3d* facing_vector, real_vector3d const* movement_vector, real_vector3d* avoidance_impulse, real32* throttle_scale, real32* urgency, bool forward_movement);

