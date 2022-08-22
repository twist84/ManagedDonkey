#pragma once

#include "camera/camera.hpp"

struct c_orbiting_camera : public c_camera
{
	real_euler_angles2d m_facing;
	real m_distance;
	real m_z_offset;

	bool __unknown20;
	bool __unknown21;
	bool __unknown22;

	byte pad[0x1];
	byte unused[0x28];
};
static_assert(sizeof(c_orbiting_camera) == 0x4C);