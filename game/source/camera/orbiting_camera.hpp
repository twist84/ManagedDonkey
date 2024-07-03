#pragma once

#include "camera/camera.hpp"

struct c_orbiting_camera :
	public c_camera
{
	euler_angles2d m_orientation;
	real m_distance;
	real m_z_offset;

	bool m_orientation_enabled;
	bool m_movement_enabled;
	bool m_distance_updated;

	byte pad[0x1];
	byte unused[0x28];

	void constructor(long unit_index)
	{
		DECLFUNC(0x0072A5E0, void, __thiscall, c_camera*, long)(this, unit_index);
	}
};
static_assert(sizeof(c_orbiting_camera) == 0x4C);

