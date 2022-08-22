#pragma once

#include "camera/director.hpp"

struct c_observer_director : public c_director
{
	virtual void select_fallback_target();

	e_camera_mode m_camera_mode;

	long __unknown150;
	bool __unknown154;
	bool __unknown155;

	byte pad[0x2];
};
static_assert(sizeof(c_observer_director) == 0x158);