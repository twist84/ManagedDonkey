#pragma once

#include "camera/observer_director.hpp"

class c_saved_film_director :
	public c_observer_director
{
public:
	void __thiscall update_(real32 dt);

public:
	void constructor(int32 user_index)
	{
		INVOKE_CLASS_MEMBER(0x007276C0, c_saved_film_director, constructor, user_index);
	}

	void notify_revert();
	static bool __cdecl player_is_film_recorder(int32 player_index);

//protected:
	bool m_speed_controlled_by_trigger;
	bool m_show_control_pad_button_pressed;

	byte pad[0x2];
	byte unused[0x4];
};
COMPILE_ASSERT(sizeof(c_saved_film_director) == 0x160);

