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

vector3d const* c_character_physics_mode_ground_datum::get_ground_plane() const
{
	return INVOKE_CLASS_MEMBER(0x00724BB0, c_character_physics_mode_ground_datum, get_ground_plane);
}

real c_character_physics_mode_ground_datum::get_ground_plane_contact_offset() const
{
	return INVOKE_CLASS_MEMBER(0x00724BB0, c_character_physics_mode_ground_datum, get_ground_plane_contact_offset);
}

real c_character_physics_mode_ground_datum::get_landing_velocity() const
{
	return INVOKE_CLASS_MEMBER(0x00724BB0, c_character_physics_mode_ground_datum, get_landing_velocity);
}

//.text:007235D0

long c_character_physics_mode_ground_datum::get_support_havok_component_index() const
{
	return INVOKE_CLASS_MEMBER(0x00724BB0, c_character_physics_mode_ground_datum, get_support_havok_component_index);
}

real_matrix4x3 const* c_character_physics_mode_ground_datum::get_support_havok_component_matrix() const
{
	return INVOKE_CLASS_MEMBER(0x00724BB0, c_character_physics_mode_ground_datum, get_support_havok_component_matrix);
}

long c_character_physics_mode_ground_datum::get_support_object_index() const
{
	return INVOKE_CLASS_MEMBER(0x00724BB0, c_character_physics_mode_ground_datum, get_support_object_index);
}

long c_character_physics_mode_ground_datum::get_support_rigid_body_index() const
{
	return INVOKE_CLASS_MEMBER(0x00724BB0, c_character_physics_mode_ground_datum, get_support_rigid_body_index);
}

bool havok_contact_point_reference_valid(s_havok_contact_point_reference const* reference)
{
	return INVOKE(0x00723620, havok_contact_point_reference_valid, reference);
}

void c_character_physics_mode_ground_datum::initialize()
{
	INVOKE_CLASS_MEMBER(0x00723640, c_character_physics_mode_ground_datum, initialize);
}

void c_character_physics_mode_ground_datum::initialize_internal(bool a1)
{
	INVOKE_CLASS_MEMBER(0x007236C0, c_character_physics_mode_ground_datum, initialize_internal, a1);
}

void c_character_physics_mode_ground_datum::move(s_character_physics_move_output_datum* output, s_character_physics_move_input_datum const* input)
{
	INVOKE_CLASS_MEMBER(0x00723720, c_character_physics_mode_ground_datum, move, output, input);
}

//.text:00723DC0
//.text:00723DD0

void c_character_physics_mode_ground_datum::notify_in_slip_surface_volume()
{
	INVOKE_CLASS_MEMBER(0x00723E60, c_character_physics_mode_ground_datum, notify_in_slip_surface_volume);
}

void c_character_physics_mode_ground_datum::set_contact(long a1, long a2, vector3d const* a3)
{
	INVOKE_CLASS_MEMBER(0x00723E70, c_character_physics_mode_ground_datum, set_contact, a1, a2, a3);
}

void c_character_physics_mode_ground_datum::set_ground_plane(vector3d const* a1)
{
	INVOKE_CLASS_MEMBER(0x00723F00, c_character_physics_mode_ground_datum, set_ground_plane, a1);
}

//.text:00723F20 ; c_character_physics_mode_ground_datum::snap_facing

void c_character_physics_mode_ground_datum::update(s_character_physics_update_output_datum* output, struct s_character_physics_update_input_datum const* input)
{
	INVOKE_CLASS_MEMBER(0x00724550, c_character_physics_mode_ground_datum, update, output, input);
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

void c_character_physics_mode_ground_datum::update_internal(s_character_physics_update_output_datum* output, s_character_physics_update_input_datum const* input, bool a2, vector3d const* a3)
{
	INVOKE_CLASS_MEMBER(0x00724BB0, c_character_physics_mode_ground_datum, update_internal, output, input, a2, a3);
}

bool c_character_physics_mode_ground_datum::update_localized_physics_space_linear_velocity(s_character_physics_update_input_datum const* input, vector3d* a1, vector3d* a2, matrix3x3* a3)
{
	return INVOKE_CLASS_MEMBER(0x00724BB0, c_character_physics_mode_ground_datum, update_localized_physics_space_linear_velocity, input, a1, a2, a3);
}

//.text:00725A60

