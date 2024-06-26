#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_vtol
{
public:

protected:

	real __unknown0;
	real __unknown4;
	real __unknown8;

	// STRING_ID(global, engine_power_left)
	byte m_lift_effect_scale_left_history[4];

	// STRING_ID(global, engine_power_right)
	byte m_lift_effect_scale_right_history[4];

	// left values
	euler_angles2d __unknown14;
	euler_angles2d __unknown1C;

	// right values
	euler_angles2d __unknown24;
	euler_angles2d __unknown2C;

	// left values
	vector2d __unknown34;
	vector2d __unknown3C;

	// right values
	vector2d __unknown44;
	vector2d __unknown4C;

	c_animation_id m_left_lift_aim_animation_id;
	c_animation_id m_right_lift_aim_animation_id;

	// STRING_ID(global, gear_position)
	real __unknown5C;
};
static_assert(sizeof(c_vehicle_type_vtol) == 0x60);

