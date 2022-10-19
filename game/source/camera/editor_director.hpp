#pragma once

#include "cseries/cseries.hpp"
#include "camera/director.hpp"

struct c_editor_director : public c_director
{
	long __unknown168;
	bool m_scripted;
	bool __unknown16D;

	byte pad[0x2];
	byte unused[0xC];

	void ctor(long user_index)
	{
		DECLFUNC(0x00727EA0, void, __thiscall, c_director*, long)(this, user_index);
	}

	static void __cdecl bump_speed();
	static real __cdecl get_speed();
	void __cdecl set_position_and_roll(real_point3d const* position, real_euler_angles2d const* facing);
	void __cdecl set_scripted(bool scripted);
};
static_assert(sizeof(c_editor_director) == 0x160);

extern void __cdecl editor_director_save_position();
extern void __cdecl editor_director_set_position(real_point3d const* position, real_euler_angles2d* facing);
extern bool __cdecl editor_director_valid();
extern c_editor_director* __cdecl get_editor_director();