#pragma once

#include "camera/camera.hpp"

struct c_flying_camera :
	public c_camera
{
	enum e_flags
	{
		_zoomed_bit = 0,            // not the actual name
		_lock_in_xy_plane_bit = 1,
		_collision_bit = 2,         // default
		_orientation_bit = 3,       // default
		_movement_bit = 4           // default
	};

	real_point3d m_position;
	euler_angles2d m_orientation;
	real m_roll;

	real __unknown28;
	real __unknown2C;
	bool __unknown30;
	real __unknown34;

	dword_flags m_flags;

	byte unused[0x10];

	void constructor(long user_index)
	{
		DECLFUNC(0x0072ACA0, void, __thiscall, c_camera*, long)(this, user_index);
	}

	void set_lock_in_xy_plane(bool value)
	{
		SET_BIT(m_flags, _lock_in_xy_plane_bit, value);
	}

	void set_collision(bool value)
	{
		SET_BIT(m_flags, _collision_bit, value);
	}

	void enable_orientation(bool value)
	{
		SET_BIT(m_flags, _orientation_bit, value);
	}

	void enable_movement(bool value)
	{
		SET_BIT(m_flags, _movement_bit, value);
	}
};
static_assert(sizeof(c_flying_camera) == 0x4C);

