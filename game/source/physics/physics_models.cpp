#include "physics/physics_models.hpp"

#include "cache/cache_files.hpp"
#include "models/model_definitions.hpp"
#include "objects/objects.hpp"
#include "physics/havok_component.hpp"
#include "render/render_debug.hpp"

#include <hkShape.hpp>

struct s_physics_model_definition
{
	uns32 flags;
	real32 mass;
	real32 high_frequency_deactivator_scale;
	real32 low_frequency_deactivator_scale;
	real32 custom_shape_radius;
	real32 maximum_penetration_depth_scale;
	int8 import_version;
	int8 pad[3];
	s_tag_block damped_spring_motors_block;
	s_tag_block position_motors_block;
	s_tag_block phantom_types;
	s_tag_block powered_chains_block;
	s_tag_block node_constraint_edges;
	s_tag_block rigid_bodies;
	s_tag_block materials;
	s_tag_block spheres;
	s_tag_block multi_spheres;
	s_tag_block pills;
	s_tag_block boxes;
	s_tag_block triangles;
	s_tag_block polyhedra;
	s_tag_block polyhedron_vertices;
	s_tag_block polyhedron_planes;
	s_tag_block mass_distributions;
	s_tag_block lists;
	s_tag_block list_shapes;
	s_tag_block mopps;
	s_tag_data mopp_codes;
	s_tag_block hinge_constraints;
	s_tag_block ragdoll_constraints;
	s_tag_block regions;
	s_tag_block nodes;
	s_tag_block errors;
	s_tag_block point_to_path_curves;
	s_tag_block limited_hinge_constraints;
	s_tag_block ball_and_socket_constraints;
	s_tag_block stiff_spring_constraints;
	s_tag_block prismatic_constraints;
	s_tag_block phantom_shapes;
};
static_assert(sizeof(s_physics_model_definition) == 0x198);

struct s_model_region
{
	string_id name;
	int8 collision_region_index;
	int8 physics_region_index;
	byte pad[0x2];
	s_tag_block permutations;
};
static_assert(sizeof(s_model_region) == 0x14);

struct s_model_permutation
{
	string_id name;
	uns8 flags;
	int8 collision_permutation_index;
	int8 physics_permutation_index;
	byte pad;
};
static_assert(sizeof(s_model_permutation) == 0x8);

struct s_physics_model_region
{
	string_id name;
	s_tag_block permutations;
};
static_assert(sizeof(s_physics_model_region) == 0x10);

struct s_physics_model_permutation
{
	string_id name;
	s_tag_block rigid_body_indices;
};
static_assert(sizeof(s_physics_model_permutation) == 0x10);

struct s_physics_model_rigid_body
{
	int16 node_index;
	int16 region_index;
	int16 permutation_index;
	int16 pad;
	real_point3d bounding_sphere_model_position;
	real32 bounding_sphere_radius;
	uns16 flags;
	int16 motion_type;
	int16 no_phantom_power_version_rigid_body_index;
	int16 rigid_body_size;
	real32 inertia_tensor_scale;
	real32 linear_damping;
	real32 angular_damping;
	real_vector3d center_of_mass_offset;
	real_rectangle3d water_physics_aabb_offset;
	real32 pad4[2];
	const hkShape* shape;
	real32 mass;
	hkVector4 center_of_mass;
	hkMatrix3 inertia_tensor;
	real32 bounding_sphere_pad;
	real32 pad2[2];
	int16 pad3;
	uns16 runtime_flags;
};
static_assert(sizeof(s_physics_model_rigid_body) == 0xB0);

class c_sphere_shape :
	public hkSphereShape
{
public:
};

class c_triangle_shape :
	public hkTriangleShape
{
public:
};

class c_box_shape :
	public hkBoxShape
{
public:
};

class c_capsule_shape :
	public hkCapsuleShape
{
public:
};

class c_bv_shape :
	public hkBvShape
{
public:
};

class c_multi_sphere_shape :
	public hkMultiSphereShape
{
public:
};

class c_convex_vertices_shape :
	public hkConvexVerticesShape
{
public:
};

class c_convex_translate_shape : 
	public hkConvexTranslateShape
{
public:
};

class c_convex_transform_shape :
	public hkConvexTransformShape
{
public:
};

class c_collision_surface_shape :
	public hkConvexShape
{
public:
	enum e_flag
	{
		_is_structure_bsp_surface_bit = 0,
		_is_dead_bit = 1,
	};

public:
	int32 get_vertex_count() const
	{
		ASSERT(IN_RANGE_INCLUSIVE(m_point_count, 0, NUMBEROF(m_points)));
		return m_point_count;
	}

	const hkVector4* get_vertex(int32 vertex_index) const
	{
		ASSERT(VALID_INDEX(vertex_index, get_vertex_count()));
		return &m_points[vertex_index];
	}

public:
	const int32 m_surface_index;
	const int16 m_structure_bsp_index;
	int8 m_point_count;
	uns8 m_best_plane_calculation_vertex_index;
	int16 m_material_index;
	uns8 m_collision_surface_flags;
	uns8 m_flags;
	const int32 m_model_definition_index;
	const real32 m_scale;
	hkVector4 m_points[8];
};

bool __cdecl physics_model_instance_new(s_physics_model_instance* instance, int32 object_index)
{
	return INVOKE(0x00786430, physics_model_instance_new, instance, object_index);

	//object_datum* object = OBJECT_GET(object_index);
	//
	//struct object_definition* object_definition = TAG_GET(OBJECT_TAG, struct object_definition, object->definition_index);
	//if (object_definition->object.model.index == NONE)
	//{
	//	return false;
	//}
	//
	//s_model_definition* model = TAG_GET(MODEL_TAG, s_model_definition, object_definition->object.model.index);
	//if (model->physics_model.index == NONE)
	//{
	//	return false;
	//}
	//
	//s_physics_model_definition* physics_model = TAG_GET(PHYSICS_MODEL_TAG, s_physics_model_definition, model->physics_model.index);
	//
	//int32 region_count = 0;
	//char* region_permutation_indices = NULL;
	//object_get_region_information(object_index, &region_count, &region_permutation_indices, NULL, NULL);
	//
	//int32 node_matrices_count = 0;
	//real_matrix4x3* node_matrices = object_get_node_matrices(object_index, &node_matrices_count);
	//
	//ASSERT(region_count == model->runtime_regions.count);
	//ASSERT(node_matrices_count == model->runtime_nodes.count);
	//
	//instance->object_index = object_index;
	//instance->havok_component_index = object->object.havok_component_index;
	//object_get_world_matrix(object_index, &instance->object_matrix);
	//instance->model_definition_index = object_definition->object.model.index;
	//instance->physics_model_definition_index = model->physics_model.index;
	//instance->model = model;
	//instance->physics_model = physics_model;
	//instance->region_permutation_indices = region_permutation_indices;
	//instance->matrices = node_matrices;
	//
	//return true;
}

void real_point3d_from_hkVector4(real_point3d* p, const hkVector4* v)
{
	ASSERT(p && v);

	set_real_point3d(p, v->m_quad.m128_f32[0], v->m_quad.m128_f32[1], v->m_quad.m128_f32[2]);
}

void real_rectangle3d_from_hkVector4_half_extents(real_rectangle3d* b, const hkVector4* v)
{
	ASSERT(b && v);

	set_real_rectangle3d(b, -v->m_quad.m128_f32[0], v->m_quad.m128_f32[0], -v->m_quad.m128_f32[1], v->m_quad.m128_f32[1], -v->m_quad.m128_f32[2], v->m_quad.m128_f32[2]);
}

void real_vector3d_from_hkVector4(real_vector3d* v, const hkVector4* w)
{
	ASSERT(v && w);

	v->i = w->m_quad.m128_f32[0];
	v->j = w->m_quad.m128_f32[1];
	v->k = w->m_quad.m128_f32[2];
}

void matrix3x3_from_hkMatrix3(real_matrix3x3* matrix, const hkMatrix3* rotation)
{
	ASSERT(matrix && rotation);

	real_vector3d_from_hkVector4(&matrix->forward, &rotation->forward);
	real_vector3d_from_hkVector4(&matrix->left, &rotation->left);
	real_vector3d_from_hkVector4(&matrix->up, &rotation->up);
}

void matrix4x3_from_hkTransform(real_matrix4x3* matrix, const hkTransform* transform)
{
	ASSERT(matrix && transform);

	matrix->scale = 1.0f;
	matrix3x3_from_hkMatrix3(&matrix->matrix3x3, &transform->rotation.matrix);
	real_point3d_from_hkVector4(&matrix->position, &transform->translation);
}

void __cdecl render_debug_physics_model(const s_physics_model_instance* instance, const real_argb_color* color)
{
	// $TODO rewrite this implementation
	 
	ASSERT(instance && instance->model && instance->physics_model);

	object_datum* object = OBJECT_GET(instance->object_index);
	struct object_definition* object_definition = TAG_GET(OBJECT_TAG, struct object_definition, object->definition_index);

	s_model_definition* model = instance->model;
	s_physics_model_definition* physics_model = instance->physics_model;

	for (int region_index = 0; region_index < model->runtime_regions.count; region_index++)
	{
		const s_model_region* region = TAG_BLOCK_GET_ELEMENT(&model->runtime_regions, region_index, s_model_region);
		ASSERT(region_index < model->runtime_regions.count);
		ASSERT(region != NULL);

		const int16 physics_region_index = region->physics_region_index;
		const int16 region_permutation_index = instance->region_permutation_indices[region_index];

		if (physics_region_index == NONE || region_permutation_index == NONE)
		{
			continue;
		}

		const s_model_permutation* region_permutation = TAG_BLOCK_GET_ELEMENT(&region->permutations, region_permutation_index, s_model_permutation);
		ASSERT(region_permutation != NULL);

		const int16 physics_permutation_index = region_permutation->physics_permutation_index;
		if (physics_permutation_index == NONE)
		{
			continue;
		}

		const s_physics_model_region* physics_region = TAG_BLOCK_GET_ELEMENT(&physics_model->regions, physics_region_index, s_physics_model_region);
		ASSERT(physics_region != NULL);

		const s_physics_model_permutation* physics_permutation = TAG_BLOCK_GET_ELEMENT(&physics_region->permutations, physics_permutation_index, s_physics_model_permutation);
		ASSERT(physics_permutation != NULL);

		for (int32 rigid_body_num = 0; rigid_body_num < physics_permutation->rigid_body_indices.count; rigid_body_num++)
		{
			const int16* rigid_body_indices = TAG_BLOCK_GET_ELEMENT(&physics_permutation->rigid_body_indices, rigid_body_num, int16);
			const int16 rigid_body_index = *rigid_body_indices;
			const s_physics_model_rigid_body* rigid_body = TAG_BLOCK_GET_ELEMENT(&physics_model->rigid_bodies, rigid_body_index, s_physics_model_rigid_body);
			ASSERT(rigid_body != NULL);

			const int16 node_index = rigid_body->node_index;
			const real_matrix4x3* matrix = &instance->object_matrix;
			if (node_index != NONE)
			{
				matrix = &instance->matrices[node_index];
			}

			ASSERT(node_index == NONE || node_index < physics_model->nodes.count);

			real_point3d center_of_mass{};
			real_point3d_from_hkVector4(&center_of_mass, &rigid_body->center_of_mass);
			matrix4x3_transform_point(matrix, &center_of_mass, &center_of_mass);

			render_debug_vectors(true, &center_of_mass, &matrix->forward, &matrix->up, object_definition->object.bounding_radius);
			render_debug_physics_shape(rigid_body->shape, matrix, color);
		}
	}
}

void __cdecl render_debug_physics_shape(const hkShape* shape, const real_matrix4x3* matrix, const real_argb_color* color)
{
	switch (shape->m_type)
	{
	case HK_SHAPE_SPHERE:
	{
		const c_sphere_shape* convex_shape = (const c_sphere_shape*)shape;

		render_debug_sphere(true, &matrix->position, convex_shape->m_radius, color);
	}
	break;
	case HK_SHAPE_TRIANGLE:
	{
		const c_triangle_shape* triangle_shape = (const c_triangle_shape*)shape;

		real_point3d transformed_position0{};
		real_point3d transformed_position1{};
		real_point3d transformed_position2{};
		real_point3d_from_hkVector4(&transformed_position0, &triangle_shape->m_vertexA);
		real_point3d_from_hkVector4(&transformed_position1, &triangle_shape->m_vertexB);
		real_point3d_from_hkVector4(&transformed_position2, &triangle_shape->m_vertexC);
		matrix4x3_transform_point(matrix, &transformed_position0, &transformed_position0);
		matrix4x3_transform_point(matrix, &transformed_position1, &transformed_position1);
		matrix4x3_transform_point(matrix, &transformed_position2, &transformed_position2);

		render_debug_triangle(true, &transformed_position0, &transformed_position1, &transformed_position2, color);
	}
	break;
	case HK_SHAPE_BOX:
	{
		const c_box_shape* box_shape = (const c_box_shape*)shape;

		real_rectangle3d bounds{};
		real_rectangle3d_from_hkVector4_half_extents(&bounds, &box_shape->m_halfExtents);

		render_debug_box_outline_oriented(true, &bounds, matrix, color);
	}
	break;
	case HK_SHAPE_CAPSULE:
	{
		const c_capsule_shape* capsule_shape = (const c_capsule_shape*)shape;

		real_point3d transformed_position0{};
		real_point3d transformed_position1{};
		real_point3d_from_hkVector4(&transformed_position0, &capsule_shape->m_vertexA);
		real_point3d_from_hkVector4(&transformed_position1, &capsule_shape->m_vertexB);
		matrix4x3_transform_point(matrix, &transformed_position0, &transformed_position0);
		matrix4x3_transform_point(matrix, &transformed_position1, &transformed_position1);

		real_vector3d height{};
		if (magnitude_squared3d(vector_from_points3d(&transformed_position0, &transformed_position1, &height)) >= k_real_tiny_epsilon)
		{
			render_debug_pill(true, &transformed_position0, &height, capsule_shape->m_radius, color);
		}
		else
		{
			render_debug_sphere(true, &transformed_position0, capsule_shape->m_radius, color);
		}
	}
	break;
	case HK_SHAPE_CONVEX_VERTICES:
	{
		const c_convex_vertices_shape* convex_vertices_shape = (const c_convex_vertices_shape*)shape;

		for (int32 i = 0; i < convex_vertices_shape->m_rotatedVertices.m_size; i++)
		{
			hkConvexVerticesShape::FourVectors* rotated_vertices = &convex_vertices_shape->m_rotatedVertices.m_data[i];

			real_point3d transformed_position{};
			set_real_point3d(&transformed_position, rotated_vertices->m_x.m_quad.m128_f32[0], rotated_vertices->m_y.m_quad.m128_f32[0], rotated_vertices->m_z.m_quad.m128_f32[0]);
			matrix4x3_transform_point(matrix, &transformed_position, &transformed_position);
			render_debug_point(true, &transformed_position, 0.125f, color);
		}

	}
	break;
	case HK_SHAPE_LIST:
	case HK_SHAPE_BV_TREE:
	case HK_SHAPE_MOPP:
	{
		//hkShapeContainer* shape_container = shape->getContainer();
		//if (shape->m_type == HK_SHAPE_LIST)
		//{
		//	c_list_shape* list_shape = (const c_list_shape*)shape
		//	shape_container = &list_shape->hkShapeContainer;
		//}
		//
		//char buffer[512]{};
		//for (unsigned int shape_key = shape_container->getFirstKey();
		//	shape_key != -1;
		//	shape_key = shape_container->getNextKey(shape_key))
		//{
		//	render_debug_physics_shape(shape_container->getChildShape(shape_key, buffer), matrix, color);
		//}
	}
	break;
	case HK_SHAPE_CONVEX_TRANSLATE:
	{
		const c_convex_translate_shape* convex_translate_shape = (const c_convex_translate_shape*)shape;

		real_vector3d transformed_vector{};
		real_matrix4x3 transform_matrix = *global_identity4x3;

		real_vector3d_from_hkVector4(&transformed_vector, &convex_translate_shape->m_translation);
		transform_matrix.n[3][0] += transformed_vector.n[0];
		transform_matrix.n[3][1] += transformed_vector.n[1];
		transform_matrix.n[3][2] += transformed_vector.n[2];
		matrix4x3_multiply(matrix, &transform_matrix, &transform_matrix);
		render_debug_physics_shape(convex_translate_shape->m_childShape.m_childShape, &transform_matrix, color);
	}
	break;
	case HK_SHAPE_CONVEX_TRANSFORM:
	{
		const c_convex_transform_shape* convex_transform_shape = (const c_convex_transform_shape*)shape;

		real_matrix4x3 transform_matrix{};
		matrix4x3_from_hkTransform(&transform_matrix, &convex_transform_shape->m_transform);
		matrix4x3_multiply(matrix, &transform_matrix, &transform_matrix);
		render_debug_physics_shape(convex_transform_shape->m_childShape.m_childShape, &transform_matrix, color);
	}
	break;
	case HK_SHAPE_MULTI_SPHERE:
	{
		const c_multi_sphere_shape* multi_sphere_shape = (const c_multi_sphere_shape*)shape;

		//ASSERT(IN_RANGE_INCLUSIVE(multi_sphere_shape->getNumSpheres(), 0, hkMultiSphereShape::MAX_SPHERES));
		ASSERT(IN_RANGE_INCLUSIVE(multi_sphere_shape->m_numSpheres, 0, hkMultiSphereShape::MAX_SPHERES));
		for (int sphere_index = 0; sphere_index < multi_sphere_shape->m_numSpheres; sphere_index++)
		{
			const hkVector4* sphere = &multi_sphere_shape->m_spheres[sphere_index];

			real_point3d point{};
			real_point3d_from_hkVector4(&point, sphere);
			matrix4x3_transform_point(matrix, &point, &point);

			render_debug_sphere(true, &point, sphere->m_quad.m128_f32[3], color);
		}
	}
	break;
	case HK_SHAPE_BV:
	{
		const c_bv_shape* bv_shape = (const c_bv_shape*)shape;

		render_debug_physics_shape(bv_shape->m_boundingVolumeShape, matrix, color);
	}
	break;
	case HK_SHAPE_TRANSFORM:
	{
		const hkTransformShape* transform_shape = (const hkTransformShape*)shape;

		real_matrix4x3 transform_matrix{};
		matrix4x3_from_hkTransform(&transform_matrix, &transform_shape->m_transform);
		matrix4x3_multiply(matrix, &transform_matrix, &transform_matrix);

		render_debug_physics_shape(transform_shape->m_childShape.m_childShape, &transform_matrix, color);
	}
	break;
	case HK_SHAPE_USER0:
	{
		const c_collision_surface_shape* collision_surface_shape = (const c_collision_surface_shape*)shape;

		for (int32 vertex_index = 0; vertex_index < collision_surface_shape->get_vertex_count(); vertex_index++)
		{
			int32 next_vertex_index = (vertex_index + 1) % collision_surface_shape->get_vertex_count();

			const hkVector4* vertex = collision_surface_shape->get_vertex(vertex_index);
			const hkVector4* next_vertex = collision_surface_shape->get_vertex(next_vertex_index);

			real_point3d transformed_position0{};
			real_point3d transformed_position1{};
			real_point3d_from_hkVector4(&transformed_position0, vertex);
			real_point3d_from_hkVector4(&transformed_position1, next_vertex);
			matrix4x3_transform_point(matrix, &transformed_position0, &transformed_position0);
			matrix4x3_transform_point(matrix, &transformed_position1, &transformed_position1);

			render_debug_line(true, &transformed_position0, &transformed_position1, color);
		}
	}
	break;
	default:
		return;
	}
}


