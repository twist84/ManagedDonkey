#pragma once

#include "camera/camera.hpp"

struct c_first_person_camera : public c_camera
{
	real m_field_of_view;
	real_euler_angles2d m_facing;
	bool __unknown1C;
};
static_assert(sizeof(c_first_person_camera) == 0x20);

struct c_authored_camera : public c_camera
{
	bool __unknown10;
	bool __unknown11;
	c_first_person_camera m_first_person_camera;
	real_point3d m_position;
	real_vector3d m_forward;
};
static_assert(sizeof(c_authored_camera) == 0x4C);