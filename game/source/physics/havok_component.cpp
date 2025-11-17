#include "physics/havok_component.hpp"

#include "objects/objects.hpp"
#include "physics/havok.hpp"
#include "physics/physics_models.hpp"

#include <hkWorld.hpp>

REFERENCE_DECLARE(0x02446080, c_smart_data_array<c_havok_component>, g_havok_component_data);

int32 __cdecl havok_entity_get_havok_component_index(const hkWorldObject* world_object)
{
	return INVOKE(0x005EA8E0, havok_entity_get_havok_component_index, world_object);
}

int32 __cdecl havok_entity_get_havok_component_rigid_body_index(const hkWorldObject* world_object)
{
	return INVOKE(0x005EA930, havok_entity_get_havok_component_rigid_body_index, world_object);
}

void c_havok_component::render_debug(
    bool water_physics,
    bool render_physics_model,
    bool expensive_physics,
    bool contact_points,
    bool constraints,
    bool vehicle_physics,
    bool render_mass)
{
	if (TEST_BIT(m_flags, 2))
	{
		if (m_simple_shape_phantom && render_physics_model)
		{
			//ASSERT(m_simple_shape_phantom->getCollidable()->getShape() != NULL);
			ASSERT(m_simple_shape_phantom->m_collidable.m_shape != NULL);
			real_matrix4x3 direct_transform{};
			matrix4x3_from_hkTransform(&direct_transform, &m_simple_shape_phantom->m_motionState.m_transform);
			render_debug_physics_shape(m_simple_shape_phantom->m_collidable.m_shape, &direct_transform, global_real_argb_blue);
		}
	}

	if (TEST_BIT(m_flags, 2) && water_physics)
	{
		// $IMPLEMENT
	}

	if (TEST_BIT(m_flags, 2) && expensive_physics)
	{
		// $IMPLEMENT
	}

	if (TEST_BIT(m_flags, 2) && m_havok_rigid_bodies.m_size > 0)
	{
		if (render_physics_model)
		{
			for (int32 rigid_body_index = 0; rigid_body_index < (int32)m_havok_rigid_bodies.m_size; rigid_body_index++)
			{
				hkRigidBody* rigid_body = c_havok_component::get_rigid_body(rigid_body_index);

				const real_argb_color* color = global_real_argb_black;
				if (rigid_body->m_motion.m_type.m_storage != hkMotion::MOTION_FIXED)
				{
					color = global_real_argb_grey;
					if (rigid_body->isActive())
					{
						color = global_real_argb_white;
					}
				}
				
				object_datum* object = OBJECT_GET(object_datum, m_object_index);
				if (object->object.physics_flags.test(_object_build_with_inexpensive_physics_bit))
				{
					color = global_real_argb_lightblue;
					if (rigid_body->isActive())
					{
						color = global_real_argb_magenta;
					}
				}

				//ASSERT(rigid_body->getCollidable()->getShape() != NULL);
				ASSERT(rigid_body->m_collidable.m_shape != NULL);

				real_matrix4x3 direct_transform{};
				matrix4x3_from_hkTransform(&direct_transform, &rigid_body->m_motion.m_motionState.m_transform);
				render_debug_physics_shape(rigid_body->m_collidable.m_shape, &direct_transform, color);
			}
		}

		if (contact_points && TEST_BIT(m_flags, 0))
		{
			// $IMPLEMENT
		}

		if (render_mass)
		{
			// $IMPLEMENT
		}

		if (constraints)
		{
			// $IMPLEMENT
		}

		if (g_havok_constants.havok_render_collision_damage)
		{
			// $IMPLEMENT
		}
	}
}

void c_havok_component::rigid_body_apply_acceleration(int32 rigid_body_index, const real_vector3d* acceleration)
{
	INVOKE_CLASS_MEMBER(0x005ECC50, c_havok_component, rigid_body_apply_acceleration, rigid_body_index, acceleration);
}

