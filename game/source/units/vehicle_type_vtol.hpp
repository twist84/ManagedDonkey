#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_vtol
{
public:

protected:
	enum e_lift_vector
	{
		_down_vector = 0,
		_forward_vector,
		_left_vector,
		_turn_vector,

		k_lift_vector_count
	};

	real m_steering;
	real m_turn;
	real m_target_elevation;
	byte m_lift_effect_scale_left_history[4];
	byte m_lift_effect_scale_right_history[4];
	euler_angles2d m_left_lift_angles;
	euler_angles2d m_left_render_lift_angles;
	euler_angles2d m_right_lift_angles;
	euler_angles2d m_right_render_lift_angles;
	vector2d m_left_lift_velocity;
	vector2d m_left_render_lift_velocity;
	vector2d m_right_lift_velocity;
	vector2d m_right_render_lift_velocity;
	c_animation_id m_lift_left_id;
	c_animation_id m_lift_right_id;
	real m_gear_position;
};
static_assert(sizeof(c_vehicle_type_vtol) == 0x60);

