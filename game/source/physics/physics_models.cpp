#include "physics/physics_models.hpp"

#include "physics/havok_component.hpp"
#include "render/render_debug.hpp"

#include <hkShape.hpp>

struct c_sphere_shape :
	public hkSphereShape
{
public:
};

struct c_triangle_shape :
	public hkTriangleShape
{
public:
};

struct c_box_shape :
	public hkBoxShape
{
public:
};

struct c_capsule_shape :
	public hkCapsuleShape
{
public:
};

struct c_bv_shape :
	public hkBvShape
{
public:
};

struct c_multi_sphere_shape :
	public hkMultiSphereShape
{
public:
};

struct c_convex_vertices_shape :
	public hkConvexVerticesShape
{
public:
};

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
	matrix3x3_from_hkMatrix3((real_matrix3x3*)&matrix->forward, &transform->rotation.matrix);
	real_point3d_from_hkVector4(&matrix->position, &transform->translation);
}

void __cdecl render_debug_physics_shape(const hkShape* shape, const real_matrix4x3* matrix, const real_argb_color* color)
{
	switch (shape->m_type)
	{
	case HK_SHAPE_SPHERE:
	{
		const c_sphere_shape* convex_shape = static_cast<const c_sphere_shape*>(shape);

		render_debug_sphere(true, &matrix->position, convex_shape->m_radius, color);
	}
	break;
	case HK_SHAPE_TRIANGLE:
	{
		const c_triangle_shape* triangle_shape = static_cast<const c_triangle_shape*>(shape);

		real_point3d points[3]{};
		real_point3d_from_hkVector4(&points[0], &triangle_shape->m_vertices[0]);
		real_point3d_from_hkVector4(&points[1], &triangle_shape->m_vertices[1]);
		real_point3d_from_hkVector4(&points[2], &triangle_shape->m_vertices[2]);
		matrix4x3_transform_point(matrix, &points[0], &points[0]);
		matrix4x3_transform_point(matrix, &points[1], &points[1]);
		matrix4x3_transform_point(matrix, &points[2], &points[2]);

		render_debug_triangle(true, &points[0], &points[1], &points[2], color);
	}
	break;
	case HK_SHAPE_BOX:
	{
		const c_box_shape* box_shape = static_cast<const c_box_shape*>(shape);

		real_rectangle3d bounds{};
		real_rectangle3d_from_hkVector4_half_extents(&bounds, &box_shape->m_half_extents);

		render_debug_box_outline_oriented(true, &bounds, matrix, color);
	}
	break;
	case HK_SHAPE_CAPSULE:
	{
		const c_capsule_shape* capsule_shape = static_cast<const c_capsule_shape*>(shape);

		real_point3d points[2]{};
		real_point3d_from_hkVector4(&points[0], &capsule_shape->m_vertices[0]);
		real_point3d_from_hkVector4(&points[1], &capsule_shape->m_vertices[1]);
		matrix4x3_transform_point(matrix, &points[0], &points[0]);
		matrix4x3_transform_point(matrix, &points[1], &points[1]);

		real_vector3d height{};
		if (magnitude_squared3d(vector_from_points3d(&points[0], &points[1], &height)) >= k_real_tiny_epsilon)
		{
			render_debug_pill(true, &points[1], &height, capsule_shape->m_radius, color);
		}
		else
		{
			render_debug_sphere(true, &points[1], capsule_shape->m_radius, color);
		}
	}
	break;
	case HK_SHAPE_CONVEX_VERTICES:
	{
		const c_convex_vertices_shape* convex_vertices_shape = static_cast<const c_convex_vertices_shape*>(shape);

		for (int32 i = 0; i < convex_vertices_shape->m_rotatedVertices.m_size; i++)
		{
			hkConvexVerticesShape::FourVectors* rotated_vertices = &convex_vertices_shape->m_rotatedVertices.m_data[i];

			real_point3d point{};
			set_real_point3d(&point, rotated_vertices->m_x.m_quad.m128_f32[0], rotated_vertices->m_y.m_quad.m128_f32[0], rotated_vertices->m_z.m_quad.m128_f32[0]);
			matrix4x3_transform_point(matrix, &point, &point);
			render_debug_point(true, &point, 0.125f, color);
		}

	}
	break;
	//case HK_SHAPE_LIST:
	//case HK_SHAPE_BV_TREE:
	//case HK_SHAPE_MOPP:
	//{
	//	if (shape->m_type == HK_SHAPE_LIST)
	//	{
	//
	//	}
	//}
	//break;
	//case HK_SHAPE_CONVEX_TRANSLATE:
	//{
	//	const c_convex_translate_shape* convex_translate_shape = static_cast<const c_convex_translate_shape*>(shape);
	//
	//
	//}
	//break;
	//case HK_SHAPE_CONVEX_TRANSFORM:
	//{
	//	const c_convex_transform_shape* convex_transform_shape = static_cast<const c_convex_transform_shape*>(shape);
	//
	//
	//}
	//break;
	case HK_SHAPE_MULTI_SPHERE:
	{
		const c_multi_sphere_shape* multi_sphere_shape = static_cast<const c_multi_sphere_shape*>(shape);

		ASSERT(IN_RANGE_INCLUSIVE(multi_sphere_shape->m_num_spheres, 0, hkMultiSphereShape::MAX_SPHERES));
		for (int32 i = 0; i < multi_sphere_shape->m_num_spheres; i++)
		{
			const hkVector4* sphere = &multi_sphere_shape->m_spheres[i];

			real_point3d point{};
			real_point3d_from_hkVector4(&point, sphere);
			matrix4x3_transform_point(matrix, &point, &point);

			render_debug_sphere(true, &point, sphere->m_quad.m128_f32[3], color);
		}
	}
	break;
	case HK_SHAPE_BV:
	{
		const c_bv_shape* bv_shape = static_cast<const c_bv_shape*>(shape);

		render_debug_physics_shape(bv_shape->m_bounding_volume_shape, matrix, color);
	}
	break;
	case HK_SHAPE_TRANSFORM:
	{
		const hkTransformShape* transform_shape = static_cast<const hkTransformShape*>(shape);

		real_matrix4x3 transform_matrix{};
		matrix4x3_from_hkTransform(&transform_matrix, &transform_shape->m_transform);
		matrix4x3_multiply(matrix, &transform_matrix, &transform_matrix);

		render_debug_physics_shape(transform_shape->m_childShape.m_child_shape, &transform_matrix, color);
	}
	break;
	case HK_SHAPE_USER0:
	{
		// $TODO: implement me
	}
	break;
	default:
		break;
	}
}

