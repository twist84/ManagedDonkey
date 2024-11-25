#pragma once

#include "camera/camera.hpp"

struct c_orbiting_camera :
	public c_camera
{
public:
	void constructor(long unit_index)
	{
		INVOKE_CLASS_MEMBER(0x0072A5E0, c_orbiting_camera, constructor, unit_index);
	}

//protected:
	euler_angles2d m_orientation;
	real m_distance;
	real m_vertical_offset;
	bool m_orientation_enabled;
	bool m_movement_enabled;
	bool m_external_distance_set;

	byte pad[0x1];
	byte unused[0x28];
};
static_assert(sizeof(c_orbiting_camera) == 0x4C);

