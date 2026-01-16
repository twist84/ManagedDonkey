#pragma once

#include "camera/director.hpp"

enum
{
	_observer_director_show_hud_state_both_on = 0,
	_observer_director_show_hud_state_saved_film_off,
	_observer_director_show_hud_state_both_off,

	k_observer_director_show_hud_state_count,
};

class c_observer_director :
	public c_director
{
public:
	bool __thiscall should_draw_hud_saved_film_();

public:
	virtual void select_fallback_target();

	void constructor(int32 user_index)
	{
		INVOKE_CLASS_MEMBER(0x00726430, c_observer_director, constructor, user_index);
	}

//protected:
	e_camera_mode m_desired_camera_mode;
	int32 m_show_hud_state;
	uns8 m_current_camera_mode_index;
	bool m_ever_had_target;

	byte pad[0x2];
};
COMPILE_ASSERT(sizeof(c_observer_director) == 0x158);

