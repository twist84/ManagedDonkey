#pragma once

#include "camera/camera.hpp"

struct c_dead_camera :
	public c_camera
{
public:
	void constructor(int32 user_index)
	{
		DECLFUNC(0x00729E60, void, __thiscall, c_camera*, int32)(this, user_index);
	}
	
//protected:
	real_point3d m_falling_camera_position;
	real_euler_angles2d m_orientation;
	real32 m_distance;
	real32 m_field_of_view;
	real32 m_timer;
	int32 m_player_index;
	int32 m_killer_object_index;
	bool m_unit_fell_to_death;
	uint8 m_user_index;

	uint8 pad[0x2];
	uint8 unused[0x10];
};
static_assert(sizeof(c_dead_camera) == 0x4C);

