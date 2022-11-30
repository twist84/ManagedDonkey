#pragma once

#include "camera/camera.hpp"

struct c_dead_camera : public c_camera
{
	real_point3d m_position;
	euler_angles2d m_facing;
	real m_focus_distance;
	real m_field_of_view;
	real __unknown2C;
	datum_index m_target_player_index;
	datum_index m_target_object_index;
	bool __unknown38;
	byte m_user_index;

	byte pad[0x2];
	byte unused[0x10];

	void ctor(long user_index)
	{
		DECLFUNC(0x00729E60, void, __thiscall, c_camera*, long)(this, user_index);
	}
};
static_assert(sizeof(c_dead_camera) == 0x4C);