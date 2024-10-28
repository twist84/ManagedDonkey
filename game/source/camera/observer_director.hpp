#pragma once

#include "camera/director.hpp"

struct c_observer_director :
	public c_director
{
	virtual void select_fallback_target();

	e_camera_mode m_camera_mode;

	long __unknown150;
	bool __unknown154;
	bool __unknown155;

	byte pad[0x2];

	void constructor(long user_index)
	{
		INVOKE_CLASS_MEMBER(0x00726430, c_observer_director, constructor, user_index);
	}
};
static_assert(sizeof(c_observer_director) == 0x158);

