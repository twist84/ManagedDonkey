#pragma once

#include "camera/camera.hpp"

enum e_output_user_index;
struct s_observer_command;

struct c_first_person_camera :
	public c_camera
{
	void __thiscall _update(e_output_user_index output_user_index, real a2, s_observer_command* result);

	real m_field_of_view;
	euler_angles2d m_facing;
	bool m_allow_controlled_facing;

	void constructor(long unit_index)
	{
		INVOKE_CLASS_MEMBER(0x0065F410, c_first_person_camera, constructor, unit_index);
	}
};
static_assert(sizeof(c_first_person_camera) == 0x20);

struct c_authored_camera :
	public c_camera
{
	bool __unknown10;
	bool __unknown11;
	c_first_person_camera m_first_person_camera;
	real_point3d m_position;
	vector3d m_forward;

	void constructor(long user_index)
	{
		INVOKE_CLASS_MEMBER(0x0072F2E0, c_authored_camera, constructor, user_index);
	}
};
static_assert(sizeof(c_authored_camera) == 0x4C);

