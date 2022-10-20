#include "camera/editor_director.hpp"

#include "cseries/console.hpp"

void __cdecl c_editor_director::bump_speed()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00727FC0, bump_speed);
}

void __cdecl editor_director_save_position()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00727FE0, editor_director_save_position);
}

void __cdecl editor_director_set_position(real_point3d const* position, real_euler_angles2d* facing)
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00728030, editor_director_set_position, position, facing);
}

bool __cdecl editor_director_valid()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x007280E0, editor_director_valid);
}

c_editor_director* __cdecl get_editor_director()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00728110, get_editor_director);
}

real __cdecl c_editor_director::get_speed()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00728170, get_speed);
}

void __cdecl c_editor_director::set_position_and_roll(real_point3d const* position, real_euler_angles2d const* facing)
{
	FUNCTION_BEGIN(true);

	DECLFUNC(0x00728320, void, __thiscall, c_editor_director const*, real_point3d const*, real_euler_angles2d const*)(this, position, facing);
}

void __cdecl c_editor_director::set_scripted(bool scripted)
{
	FUNCTION_BEGIN(true);

	DECLFUNC(0x00728410, void, __thiscall, c_editor_director const*, bool)(this, scripted);
}
