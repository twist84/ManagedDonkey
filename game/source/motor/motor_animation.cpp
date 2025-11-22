#include "motor/motor_animation.hpp"

#include "cseries/cseries.hpp"

bool __cdecl motor_animation_resource_busy(long mover_index, e_motor_resource resource)
{
	return INVOKE(0x00B8FE10, motor_animation_resource_busy, mover_index, resource);
}

