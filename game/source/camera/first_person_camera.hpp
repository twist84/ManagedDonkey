#pragma once

#include "camera/camera.hpp"

struct s_observer_command;

struct c_first_person_camera :
	public c_camera
{
public:
	void __thiscall update_(long user_index, real32 dt, s_observer_command* result);

	void constructor(long unit_index)
	{
		INVOKE_CLASS_MEMBER(0x0065F410, c_first_person_camera, constructor, unit_index);
	}
	
//protected:
	real32 m_last_horizontal_field_of_view;
	real_euler_angles2d m_orientation_offset;
	bool m_allow_controlled_facing;
};
static_assert(sizeof(c_first_person_camera) == 0x20);

struct c_authored_camera :
	public c_camera
{
public:
	void constructor(long user_index)
	{
		INVOKE_CLASS_MEMBER(0x0072F2E0, c_authored_camera, constructor, user_index);
	}
	
//protected:
	bool m_last_update_first_person;
	bool m_last_zoomed;
	c_first_person_camera m_first_person_camera;
	real_point3d m_position;
	real_vector3d m_forward;
};
static_assert(sizeof(c_authored_camera) == 0x4C);

