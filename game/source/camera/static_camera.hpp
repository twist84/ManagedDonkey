#pragma once

#include "camera/camera.hpp"

struct c_static_camera : public c_camera
{
	real_point3d m_position;
	real_vector3d m_forward;
	real m_field_of_view;

	real __unknown2C;
	dword __unknown30;

	byte unused[0x18];
};
static_assert(sizeof(c_static_camera) == 0x4C);

struct c_scripted_camera : public c_camera
{
	byte unused[0x3C];
};
static_assert(sizeof(c_scripted_camera) == 0x4C);