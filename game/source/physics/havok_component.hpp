#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "game/materials.hpp"

#include <hkArray.hpp>
#include <hkMath.hpp>

struct hkWorldObject;

class c_havok_contact_point
{
public:
	uns32 m_environment_shape_key;
	int32 m_impact_datum_index;
	int32 m_contact_surface_index;
	c_global_material_type m_global_material_type_a;
	c_global_material_type m_global_material_type_b;
	int32 m_contact_havok_component_index;
	int32 m_contact_point_object_index;
	real_point3d m_position;
	real_vector3d m_normal;
	real32 m_distance;
	real32 m_friction;
	real32 m_restitution;
	uns16 m_contact_point_id;

	// FLAG(0) _new_bit
	// FLAG(1) _deleted_bit
	uns16 m_life_cycle_flags;

	// FLAG(1) _physics_only_surface_bit
	// FLAG(2) _climbable_surface_bit
	// FLAG(3) _breakable_surface_bit
	// FLAG(6) _slip_surface_bit
	uns8 m_flags;

	int8 m_rigid_body_index_a;
	int8 m_rigid_body_index_b;
	int8 m_structure_bsp_index;
};
COMPILE_ASSERT(sizeof(c_havok_contact_point) == 0x44);

struct hkAabbPhantom;
struct hkConstraintInstance;
struct hkRigidBody;
struct hkSimpleShapePhantom;
class c_havok_component :
	public s_datum_header
{
public:
	void get_bodies_in_phantom_info(int32 body_index, int32* body_object_index, int32* body_rigid_body_index, int32* body_physics_model_material_index) const;
	int32 get_bodies_in_phantoms_count() const;
	int32 get_object_index() const;
	void render_debug(bool water_physics, bool render_physics_model, bool expensive_physics, bool contact_points, bool constraints, bool vehicle_physics, bool render_mass);
	void rigid_body_apply_acceleration(int32 rigid_body_index, const real_vector3d* acceleration);
	void update_phantoms(bool phantom_on);
	void wake_all_bodies_in_phantoms();

	hkRigidBody* get_rigid_body(int32 rigid_body_index)
	{
		//ASSERT(get_havok_rigid_bodies()[rigid_body_index].get_rigid_body() != nullptr);
		ASSERT(m_havok_rigid_bodies.m_data[rigid_body_index].get_rigid_body() != nullptr);

		return m_havok_rigid_bodies.m_data[rigid_body_index].get_rigid_body();
	}

public:
	class c_rigid_body
	{
	public:

		enum
		{
			k_inplace_rigid_body_indexes = 4,
		};

	public:
		hkRigidBody* get_rigid_body()
		{
			ASSERT(m_rigid_body != nullptr);
			return m_rigid_body;
		}

	public:

		struct s_contact_point_buffer
		{
			c_havok_contact_point contact_point;
		};
		COMPILE_ASSERT(sizeof(s_contact_point_buffer) == 0x44);

		hkArray<s_contact_point_buffer> m_havok_contact_points;
		hkInplaceArray<int8, k_inplace_rigid_body_indexes> m_rigid_body_indexes;
		real_point3d m_pre_simulation_position;
		real_vector3d m_pre_simulation_linear_velocity;
		real_vector3d m_pre_simulation_angular_velocity;
		hkRigidBody* m_rigid_body;

		// FLAG(0) _plays_impact_effects_bit
		// FLAG(1) _has_infinite_inertia_tensor_bit
		// FLAG(2) _get_does_not_collide_with_environment_bit
		uns8 m_flags;

		char pad0[3];
	};
	COMPILE_ASSERT(sizeof(c_rigid_body) == 0x48);

	class c_constraint
	{
	public:
		// FLAG(0) powered_bit
		uns8 m_flags;

		int8 m_constraint_type;
		int8 m_constraint_type_index;
		int8 m_stretched_counter;
		int32 m_constraint_impact_datum_index;
		hkConstraintInstance* m_constraint;
	};
	COMPILE_ASSERT(sizeof(c_constraint) == 0xC);

	struct s_body_in_phantom
	{
		int8 body_physics_model_material_index;
		int8 body_rigid_body_index;
		int32 body_object_index;
		int32 reference_count;
	};
	COMPILE_ASSERT(sizeof(s_body_in_phantom) == 0xC);

	// FLAG(0) _records_contact_points_bit
	// FLAG(1) _ignores_gravity_bit
	// FLAG(2) _connected_to_world_bit
	// FLAG(3) _keyframed_bit
	// FLAG(4) _sentinal_bit
	// FLAG(5) _one_body_object_space_component_bit
	// FLAG(6) _constrained_to_world_bit
	// FLAG(7) _physics_model_havok_component_bit
	uns32 m_flags;

	int32 m_object_index;
	uns8 m_object_postion_controlling_rigid_body_index;
	uns8 m_object_postion_controlling_node_index;
	uns8 m_maximum_rigid_body_size;
	int32 m_impact_array_index;
	hkArray<c_rigid_body> m_havok_rigid_bodies;
	c_rigid_body m_rigid_body;
	hkArray<c_constraint> m_constraints;
	hkArray<s_body_in_phantom>* m_bodies_in_phantom;
	hkSimpleShapePhantom* m_simple_shape_phantom;
	hkAabbPhantom* m_aabb_phantom;
};
COMPILE_ASSERT(sizeof(c_havok_component) == 0x80);

extern c_smart_data_array<c_havok_component>& g_havok_component_data;

extern int32 __cdecl havok_entity_get_havok_component_index(const hkWorldObject* world_object);
extern int32 __cdecl havok_entity_get_havok_component_rigid_body_index(const hkWorldObject* world_object);

