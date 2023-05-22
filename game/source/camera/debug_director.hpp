#pragma once

#include "camera/director.hpp"

struct c_debug_director : public c_director
{
	//  camera mode cycle  - F12
	// control mode toggle - middle-mouse click

	// 0: _camera_mode_flying
	// 1: _camera_mode_following
	long m_camera_mode_index;

	// k_debug_camera_modes[m_camera_mode_index] == _camera_mode_flying
	// = !m_biped_control_mode && editor_input_inhibited()
	bool m_collision_disabled;

	// 0: _camera_mode_flying
	// 1: _camera_mode_static
	bool m_biped_control_mode;

	byte pad[2];
	byte unused[0xC];

	void ctor(long user_index)
	{
		DECLFUNC(0x007260D0, void, __thiscall, c_director*, long)(this, user_index);
	}
};
static_assert(sizeof(c_debug_director) == 0x160);