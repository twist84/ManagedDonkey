#pragma once

#include "camera/director.hpp"

class c_debug_director :
	public c_director
{
public:
	void __thiscall update_(real32 dt);

	void constructor(int32 user_index);
	void changed_camera();
	void cycle_camera();
	bool finished_cycle();

//protected:
	int32 m_current_camera_mode_index;
	bool m_input_inhibited;
	bool m_flying_camera_biped_control;

	byte pad[0x2];
	byte unused[0xC];
};
COMPILE_ASSERT(sizeof(c_debug_director) == 0x160);

