#pragma once

#include "cseries/cseries.hpp"
#include "camera/director.hpp"

struct c_editor_director :
	public c_director
{
public:
	void constructor(long user_index)
	{
		INVOKE_CLASS_MEMBER(0x00727EA0, c_editor_director, constructor, user_index);
	}

	static void __cdecl bump_speed();
	static real __cdecl get_speed();
	void set_position_and_roll(real_point3d const* position, euler_angles2d const* facing);
	void set_scripted(bool scripted);
	
//protected:
	long m_unit_focus;
	bool m_is_scripted;
	bool m_use_roll;

	byte pad[0x2];
	byte unused[0xC];
};
static_assert(sizeof(c_editor_director) == 0x160);

extern void __cdecl editor_director_save_position();
extern void __cdecl editor_director_set_position(real_point3d const* position, euler_angles2d* facing);
extern bool __cdecl editor_director_valid();
extern c_editor_director* __cdecl get_editor_director();

