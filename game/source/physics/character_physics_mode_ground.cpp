#include "physics/character_physics_mode_ground.hpp"

#include "game/game_engine_sandbox.hpp"
#include "game/materials.hpp"
#include "memory/module.hpp"
#include "physics/havok_component.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00724890, c_character_physics_mode_ground_datum, update_ground_velocity);

//.text:00723490
//.text:007234B0
//.text:007234E0
//.text:00723510
//.text:00723530
//.text:00723550 ; c_character_physics_mode_ground_datum::get_ground_material_type() const

vector3d const* __cdecl c_character_physics_mode_ground_datum::get_ground_plane() const
{
	return DECLFUNC(0x00723570, vector3d const*, __thiscall, c_character_physics_mode_ground_datum const*)(this);
}

real __cdecl c_character_physics_mode_ground_datum::get_ground_plane_contact_offset() const
{
	return DECLFUNC(0x00723580, real, __thiscall, c_character_physics_mode_ground_datum const*)(this);
}

real __cdecl c_character_physics_mode_ground_datum::get_landing_velocity() const
{
	return DECLFUNC(0x007235C0, real, __thiscall, c_character_physics_mode_ground_datum const*)(this);
}

//.text:007235D0

long __cdecl c_character_physics_mode_ground_datum::get_support_havok_component_index() const
{
	return DECLFUNC(0x007235E0, long, __thiscall, c_character_physics_mode_ground_datum const*)(this);
}

real_matrix4x3 const* __cdecl c_character_physics_mode_ground_datum::get_support_havok_component_matrix() const
{
	return DECLFUNC(0x007235F0, real_matrix4x3 const*, __thiscall, c_character_physics_mode_ground_datum const*)(this);
}

long __cdecl c_character_physics_mode_ground_datum::get_support_object_index() const
{
	return DECLFUNC(0x00723600, long, __thiscall, c_character_physics_mode_ground_datum const*)(this);
}

long __cdecl c_character_physics_mode_ground_datum::get_support_rigid_body_index() const
{
	return DECLFUNC(0x00723610, long, __thiscall, c_character_physics_mode_ground_datum const*)(this);
}

bool __cdecl havok_contact_point_reference_valid(s_havok_contact_point_reference const* reference)
{
	return INVOKE(0x00723620, havok_contact_point_reference_valid, reference);
}

void __cdecl c_character_physics_mode_ground_datum::initialize()
{
	DECLFUNC(0x00723640, void, __thiscall, c_character_physics_mode_ground_datum*)(this);
}

void __cdecl c_character_physics_mode_ground_datum::initialize_internal(bool a1)
{
	DECLFUNC(0x007236C0, void, __thiscall, c_character_physics_mode_ground_datum*, bool)(this, a1);
}

void __cdecl c_character_physics_mode_ground_datum::move(s_character_physics_move_output_datum* output, s_character_physics_move_input_datum const* input)
{
	DECLFUNC(0x00723720, void, __thiscall, c_character_physics_mode_ground_datum*, s_character_physics_move_output_datum*, s_character_physics_move_input_datum const*)(this, output, input);
}

//.text:00723DC0
//.text:00723DD0

void __cdecl c_character_physics_mode_ground_datum::notify_in_slip_surface_volume()
{
	DECLFUNC(0x00723E60, void, __thiscall, c_character_physics_mode_ground_datum*)(this);
}

void __cdecl c_character_physics_mode_ground_datum::set_contact(long a1, long a2, vector3d const* a3)
{
	DECLFUNC(0x00723E70, void, __thiscall, c_character_physics_mode_ground_datum*, long, long, vector3d const*)(this, a1, a2, a3);
}

void __cdecl c_character_physics_mode_ground_datum::set_ground_plane(vector3d const* a1)
{
	DECLFUNC(0x00723F00, void, __thiscall, c_character_physics_mode_ground_datum*, vector3d const*)(this, a1);
}

//.text:00723F20 ; c_character_physics_mode_ground_datum::snap_facing

void __cdecl c_character_physics_mode_ground_datum::update(s_character_physics_update_output_datum* output, struct s_character_physics_update_input_datum const* input)
{
	DECLFUNC(0x00724550, void, __thiscall, c_character_physics_mode_ground_datum*, s_character_physics_update_output_datum*, s_character_physics_update_input_datum const*)(this, output, input);
}

//.text:00724800

// fix jump canceling on forged objects
bool __thiscall c_character_physics_mode_ground_datum::update_ground_velocity(s_character_physics_update_input_datum const* physics_input, vector3d* linear_velocity, matrix3x3* rotation)
{
	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_character_physics_mode_ground_datum, update_ground_velocity, physics_input, linear_velocity, rotation);

	//long support_havok_component_index = get_support_havok_component_index();
	REFERENCE_DECLARE(((byte*)this) + 0x1C, long, support_havok_component_index);

	if (support_havok_component_index == NONE)
		return result;

	c_havok_component* havok_component = (c_havok_component*)datum_try_and_get(*g_havok_component_data, support_havok_component_index);
	if (!havok_component)
		return result;

	if (havok_component->m_object_index == NONE)
		return result;

	if (game_engine_object_is_fixed(havok_component->m_object_index))
		support_havok_component_index = NONE;

	return result;
}

void __cdecl c_character_physics_mode_ground_datum::update_internal(s_character_physics_update_output_datum* output, s_character_physics_update_input_datum const* input, bool a2, vector3d const* a3)
{
	DECLFUNC(0x00724BB0, void, __thiscall, c_character_physics_mode_ground_datum*, s_character_physics_update_output_datum*, s_character_physics_update_input_datum const*, bool, vector3d const*)(this, output, input, a2, a3);
}

bool __cdecl c_character_physics_mode_ground_datum::update_localized_physics_space_linear_velocity(s_character_physics_update_input_datum const* input, vector3d* a1, vector3d* a2, matrix3x3* a3)
{
	return DECLFUNC(0x00724BB0, bool, __thiscall, c_character_physics_mode_ground_datum*, s_character_physics_update_input_datum const*, vector3d*, vector3d*, matrix3x3*)(this, input, a1, a2, a3);
}

//.text:00725A60

