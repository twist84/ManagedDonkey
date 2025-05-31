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

const real_vector3d* c_character_physics_mode_ground_datum::get_ground_plane() const
{
	return INVOKE_CLASS_MEMBER(0x00723570, c_character_physics_mode_ground_datum, get_ground_plane);
}

real32 c_character_physics_mode_ground_datum::get_ground_plane_contact_offset() const
{
	return INVOKE_CLASS_MEMBER(0x00723580, c_character_physics_mode_ground_datum, get_ground_plane_contact_offset);
}

real32 c_character_physics_mode_ground_datum::get_landing_velocity() const
{
	return INVOKE_CLASS_MEMBER(0x007235C0, c_character_physics_mode_ground_datum, get_landing_velocity);
}

//.text:007235D0

int32 c_character_physics_mode_ground_datum::get_support_havok_component_index() const
{
	return INVOKE_CLASS_MEMBER(0x007235E0, c_character_physics_mode_ground_datum, get_support_havok_component_index);

	//return m_support_havok_component_index;
}

const real_matrix4x3* c_character_physics_mode_ground_datum::get_support_havok_component_matrix() const
{
	return INVOKE_CLASS_MEMBER(0x007235F0, c_character_physics_mode_ground_datum, get_support_havok_component_matrix);
}

int32 c_character_physics_mode_ground_datum::get_support_object_index() const
{
	return INVOKE_CLASS_MEMBER(0x00723600, c_character_physics_mode_ground_datum, get_support_object_index);
}

int32 c_character_physics_mode_ground_datum::get_support_rigid_body_index() const
{
	return INVOKE_CLASS_MEMBER(0x00723610, c_character_physics_mode_ground_datum, get_support_rigid_body_index);
}

bool havok_contact_point_reference_valid(const s_havok_contact_point_reference* reference)
{
	return INVOKE(0x00723620, havok_contact_point_reference_valid, reference);
}

void c_character_physics_mode_ground_datum::initialize()
{
	INVOKE_CLASS_MEMBER(0x00723640, c_character_physics_mode_ground_datum, initialize);
}

void c_character_physics_mode_ground_datum::initialize_internal(bool initialize_support_data)
{
	INVOKE_CLASS_MEMBER(0x007236C0, c_character_physics_mode_ground_datum, initialize_internal, initialize_support_data);
}

void c_character_physics_mode_ground_datum::move(s_character_physics_move_output_datum* physics_output, const s_character_physics_move_input_datum* physics_input)
{
	INVOKE_CLASS_MEMBER(0x00723720, c_character_physics_mode_ground_datum, move, physics_output, physics_input);
}

//.text:00723DC0
//.text:00723DD0

void c_character_physics_mode_ground_datum::notify_in_slip_surface_volume()
{
	INVOKE_CLASS_MEMBER(0x00723E60, c_character_physics_mode_ground_datum, notify_in_slip_surface_volume);
}

void c_character_physics_mode_ground_datum::set_contact(int32 havok_component_index, const s_havok_contact_point_reference* primary_contact_point_reference, const real_vector3d* ground_plane)
{
	INVOKE_CLASS_MEMBER(0x00723E70, c_character_physics_mode_ground_datum, set_contact, havok_component_index, primary_contact_point_reference, ground_plane);
}

void c_character_physics_mode_ground_datum::set_ground_plane(const real_vector3d* ground_plane)
{
	INVOKE_CLASS_MEMBER(0x00723F00, c_character_physics_mode_ground_datum, set_ground_plane, ground_plane);
}

//.text:00723F20 ; c_character_physics_mode_ground_datum::snap_facing

void c_character_physics_mode_ground_datum::update(s_character_physics_update_output_datum* physics_output, const s_character_physics_update_input_datum* physics_input)
{
	INVOKE_CLASS_MEMBER(0x00724550, c_character_physics_mode_ground_datum, update, physics_output, physics_input);
}

//.text:00724800

// fix jump canceling on forged objects
bool __thiscall c_character_physics_mode_ground_datum::update_ground_velocity(const s_character_physics_update_input_datum* physics_input, real_vector3d* linear_velocity, real_matrix3x3* rotation)
{
	//INVOKE_CLASS_MEMBER(0x00724890, c_character_physics_mode_ground_datum, update_ground_velocity, physics_input, linear_velocity, rotation);

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_character_physics_mode_ground_datum, update_ground_velocity, physics_input, linear_velocity, rotation);

	int32 support_havok_component_index = get_support_havok_component_index();
	if (support_havok_component_index == NONE)
	{
		return result;
	}

	c_havok_component* havok_component = DATUM_TRY_AND_GET(g_havok_component_data, c_havok_component, support_havok_component_index);
	if (!havok_component)
	{
		return result;
	}

	if (havok_component->m_object_index == NONE)
	{
		return result;
	}

	if (game_engine_object_is_fixed(havok_component->m_object_index))
	{
		support_havok_component_index = NONE;
	}

	return result;
}

void c_character_physics_mode_ground_datum::update_internal(s_character_physics_update_output_datum* physics_output, const s_character_physics_update_input_datum* physics_input, bool localize_airborne, const real_vector3d* localized_velocity)
{
	INVOKE_CLASS_MEMBER(0x00724BB0, c_character_physics_mode_ground_datum, update_internal, physics_output, physics_input, localize_airborne, localized_velocity);
}

bool c_character_physics_mode_ground_datum::update_localized_physics_space_linear_velocity(const s_character_physics_update_input_datum* physics_input, real_vector3d* linear_velocity, real_vector3d* space_acceleration, real_matrix3x3* rotation)
{
	return INVOKE_CLASS_MEMBER(0x00724BB0, c_character_physics_mode_ground_datum, update_localized_physics_space_linear_velocity, physics_input, linear_velocity, space_acceleration, rotation);
}

//.text:00725A60

