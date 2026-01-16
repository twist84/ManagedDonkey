#pragma once

#include "camera/camera.hpp"

class c_orbiting_camera :
	public c_camera
{
public:
	void constructor(int32 unit_index)
	{
		INVOKE_CLASS_MEMBER(0x0072A5E0, c_orbiting_camera, constructor, unit_index);
	}

//protected:
	real_euler_angles2d m_orientation;
	real32 m_distance;
	real32 m_vertical_offset;
	bool m_orientation_enabled;
	bool m_movement_enabled;
	bool m_external_distance_set;

	byte pad[0x1];
	byte unused[0x28];
};
COMPILE_ASSERT(sizeof(c_orbiting_camera) == 0x4C);

