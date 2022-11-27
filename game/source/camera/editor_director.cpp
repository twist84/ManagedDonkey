#include "camera/editor_director.hpp"

#include "cseries/console.hpp"

void __cdecl c_editor_director::bump_speed()
{
	INVOKE(0x00727FC0, bump_speed);
}

void __cdecl editor_director_save_position()
{
	INVOKE(0x00727FE0, editor_director_save_position);
}

void __cdecl editor_director_set_position(real_point3d const* position, real_euler_angles2d* facing)
{
	INVOKE(0x00728030, editor_director_set_position, position, facing);
}

bool __cdecl editor_director_valid()
{
	return INVOKE(0x007280E0, editor_director_valid);
}

c_editor_director* __cdecl get_editor_director()
{
	return INVOKE(0x00728110, get_editor_director);
}

real __cdecl c_editor_director::get_speed()
{
	return INVOKE(0x00728170, get_speed);
}

void __cdecl c_editor_director::set_position_and_roll(real_point3d const* position, real_euler_angles2d const* facing)
{
	DECLFUNC(0x00728320, void, __thiscall, c_editor_director const*, real_point3d const*, real_euler_angles2d const*)(this, position, facing);
}

void __cdecl c_editor_director::set_scripted(bool scripted)
{
	DECLFUNC(0x00728410, void, __thiscall, c_editor_director const*, bool)(this, scripted);
}
