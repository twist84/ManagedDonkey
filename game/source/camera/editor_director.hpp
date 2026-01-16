#pragma once

#include "cseries/cseries.hpp"
#include "camera/director.hpp"

class c_editor_director :
	public c_director
{
public:
	void constructor(int32 user_index)
	{
		INVOKE_CLASS_MEMBER(0x00727EA0, c_editor_director, constructor, user_index);
	}

	static void __cdecl bump_speed();
	static real32 __cdecl get_speed();
	void set_position_and_roll(const real_point3d* position, const real_euler_angles2d* facing);
	void set_scripted(bool scripted);
	
//protected:
	int32 m_unit_focus;
	bool m_is_scripted;
	bool m_use_roll;

	byte pad[0x2];
	byte unused[0xC];
};
COMPILE_ASSERT(sizeof(c_editor_director) == 0x160);

extern void __cdecl editor_director_save_position();
extern void __cdecl editor_director_set_position(const real_point3d* position, real_euler_angles2d* facing);
extern bool __cdecl editor_director_valid();
extern c_editor_director* __cdecl get_editor_director();

