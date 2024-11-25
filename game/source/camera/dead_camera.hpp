#pragma once

#include "camera/camera.hpp"

struct c_dead_camera :
	public c_camera
{
public:
	void constructor(long user_index)
	{
		DECLFUNC(0x00729E60, void, __thiscall, c_camera*, long)(this, user_index);
	}
	
//protected:
	real_point3d m_falling_camera_position;
	euler_angles2d m_orientation;
	real m_distance;
	real m_field_of_view;
	real m_timer;
	long m_player_index;
	long m_killer_object_index;
	bool m_unit_fell_to_death;
	byte m_user_index;

	byte pad[0x2];
	byte unused[0x10];
};
static_assert(sizeof(c_dead_camera) == 0x4C);

