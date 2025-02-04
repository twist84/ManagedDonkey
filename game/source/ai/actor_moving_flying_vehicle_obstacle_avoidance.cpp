#include "ai/actor_moving_flying_vehicle_obstacle_avoidance.hpp"

void __cdecl actor_move_initialize()
{
	INVOKE(0x014C4220, actor_move_initialize);
}

bool __cdecl actor_moving_nondirectional_flying_vector_avoidance(long actor_index, long unit_index, real_vector3d* facing_vector, real_vector3d const* movement_vector, real_vector3d* avoidance_impulse, real* throttle_scale, real* urgency, bool forward_movement)
{
	return INVOKE(0x014C4450, actor_moving_nondirectional_flying_vector_avoidance, actor_index, unit_index, facing_vector, movement_vector, avoidance_impulse, throttle_scale, urgency, forward_movement);
}

