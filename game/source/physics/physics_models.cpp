#include "physics/physics_models.hpp"

#include "render/render_debug.hpp"

enum hkShapeType
{
	HK_SHAPE_ALL = -1,
	HK_SHAPE_CONVEX = 1,
	HK_SHAPE_COLLECTION = 2,
	HK_SHAPE_SPHERE = 3,
	HK_SHAPE_CYLINDER = 4,
	HK_SHAPE_TRIANGLE = 5,
	HK_SHAPE_BOX = 6,
	HK_SHAPE_CAPSULE = 7,
	HK_SHAPE_CONVEX_VERTICES = 8,
	HK_SHAPE_PACKED_CONVEX_VERTICES = 9,
	HK_SHAPE_LIST = 10,
	HK_SHAPE_MOPP_EMBEDDED = 11,
	HK_SHAPE_CONVEX_TRANSLATE = 12,
	HK_SHAPE_CONVEX_TRANSFORM = 13,
	HK_SHAPE_CONVEX_PIECE = 15,
	HK_SHAPE_MULTI_SPHERE = 16,
	HK_SHAPE_CONVEX_LIST = 17,
	HK_SHAPE_TRIANGLE_COLLECTION = 18,
	HK_SHAPE_BV_TREE = 19,
	HK_SHAPE_MULTI_RAY = 20,
	HK_SHAPE_HEIGHT_FIELD = 21,
	HK_SHAPE_SAMPLED_HEIGHT_FIELD = 22,
	HK_SHAPE_TRI_PATCH = 23,
	HK_SHAPE_SPHERE_REP = 24,
	HK_SHAPE_BV = 25,
	HK_SHAPE_PLANE = 26,
	HK_SHAPE_MOPP = 27,
	HK_SHAPE_TRANSFORM = 28,
	HK_SHAPE_PHANTOM_CALLBACK = 29,
	HK_SHAPE_USER0 = 30,
	HK_SHAPE_USER1 = 31,
	HK_SHAPE_USER2 = 32,
};

struct hkVector4
{
	__declspec(align(16)) float x;
	float y;
	float z;
	float w;
};

struct hkBaseObject
{
public:
	struct hkBaseObject_vtbl* __vftable /*VFT*/;
};

struct hkReferencedObject :
	public hkBaseObject
{
public:
	short m_size;
	short m_count;
};

struct hkShape :
	public hkReferencedObject
{
public:
	void* m_user_data;
	hkShapeType m_type;
};

struct hkSphereRepShape :
	public hkShape
{
public:
};

struct hkConvexShape :
	public hkSphereRepShape
{
public:
	real m_radius;
};

struct hkSphereShape :
	public hkConvexShape
{
public:
};

struct hkTriangleShape :
	public hkConvexShape
{
public:
	word m_welding_info;
	byte m_welding_type;
	byte m_is_extruded;
	hkVector4 m_vertices[3];
	hkVector4 m_extrusion;
};

struct hkBoxShape :
	public hkConvexShape
{
public:
	hkVector4 m_half_extents;
};

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

void real_point3d_from_hkVector4(real_point3d* p, hkVector4 const* v)
{
	ASSERT(p && v);

	set_real_point3d(p, v->x, v->y, v->z);
}


void real_rectangle3d_from_hkVector4_half_extents(real_rectangle3d* b, hkVector4 const* v)
{
	ASSERT(b && v);

	set_real_rectangle3d(b, -v->x, v->x, -v->y, v->y, -v->z, v->z);
}

void __cdecl render_debug_physics_shape(hkShape const* shape, real_matrix4x3 const* matrix, real_argb_color const* color)
{
	switch (shape->m_type)
	{
	case HK_SHAPE_SPHERE:
	{
		c_sphere_shape const* convex_shape = static_cast<c_sphere_shape const*>(shape);

		render_debug_sphere(true, &matrix->center, convex_shape->m_radius, color);
	}
	break;
	case HK_SHAPE_TRIANGLE:
	{
		c_triangle_shape const* triangle_shape = static_cast<c_triangle_shape const*>(shape);

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
		c_box_shape const* box_shape = static_cast<c_box_shape const*>(shape);

		real_rectangle3d bounds{};
		real_rectangle3d_from_hkVector4_half_extents(&bounds, &box_shape->m_half_extents);

		render_debug_box_outline_oriented(true, &bounds, matrix, color);
	}
	break;
	//case HK_SHAPE_CAPSULE:
	//{
	//	c_capsule_shape const* capsule_shape = static_cast<c_capsule_shape const*>(shape);
	//
	//	real_point3d points[2]{};
	//	real_point3d_from_hkVector4(&points[0], capsule_shape->getVertex(0));
	//	real_point3d_from_hkVector4(&points[1], capsule_shape->getVertex(1));
	//	matrix4x3_transform_point(matrix, &points[0], &points[0]);
	//	matrix4x3_transform_point(matrix, &points[1], &points[1]);
	//
	//	vector3d height{};
	//	if (magnitude_squared3d(vector_from_points3d(&points[0], &points[1], &height)) >= 0.000001f)
	//		render_debug_pill(true, &points[1], &height, capsule_shape->getRadius(), color);
	//	else
	//		render_debug_sphere(true, &points[1], capsule_shape->getRadius(), color);
	//}
	//break;
	//case HK_SHAPE_CONVEX_VERTICES:
	//{
	//	c_convex_vertices_shape const* convex_vertices_shape = static_cast<c_convex_vertices_shape const*>(shape);
	//
	//
	//}
	//break;
	//case HK_SHAPE_LIST:
	//case HK_SHAPE_BV_TREE:
	//case HK_SHAPE_MOPP:
	//{
	//}
	//break;
	//case HK_SHAPE_CONVEX_TRANSLATE:
	//{
	//	c_convex_translate_shape const* convex_translate_shape = static_cast<c_convex_translate_shape const*>(shape);
	//
	//
	//}
	//break;
	//case HK_SHAPE_CONVEX_TRANSFORM:
	//{
	//	c_convex_transform_shape const* convex_transform_shape = static_cast<c_convex_transform_shape const*>(shape);
	//
	//
	//}
	//break;
	//case HK_SHAPE_MULTI_SPHERE:
	//{
	//	c_multi_sphere_shape const* multi_sphere_shape = static_cast<c_multi_sphere_shape const*>(shape);
	//
	//
	//}
	//break;
	//case HK_SHAPE_BV:
	//{
	//	c_bv_shape const* bv_shape = static_cast<c_bv_shape const*>(shape);
	//
	//
	//}
	//break;
	//case HK_SHAPE_TRANSFORM:
	//{
	//
	//}
	//break;
	//case HK_SHAPE_USER0:
	//{
	//
	//}
	//break;
	}
}

