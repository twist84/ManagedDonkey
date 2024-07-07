#include "physics/physics_models.hpp"

#include "physics/havok_component.hpp"
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

struct hkQuaternion
{
	hkVector4 vector;
};

struct hkMatrix3
{
	hkVector4 forward;
	hkVector4 left;
	hkVector4 up;
};

struct hkRotation
{
	hkMatrix3 matrix;
};

struct hkTransform
{
	hkRotation rotation;
	hkVector4 translation;
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

struct hkCapsuleShape :
	public hkConvexShape
{
public:
	hkVector4 m_vertices[2];
};

struct hkShapeContainer
{
	struct
	{
		void* destructor;
		void* getNumChildShapes;
		void* getFirstKey;
		void* getNextKey;
		void* getCollisionFilterInfo;
		void* getChildShape;
		void* isWeldingEnabled;
	}* __vftable;
};

struct hkSingleShapeContainer :
	public hkShapeContainer
{
	hkShape* m_child_shape;
};

struct hkBvShape :
	public hkShape
{
	hkShape* m_bounding_volume_shape;
	hkSingleShapeContainer m_shape_container;
};

struct hkTransformShape :
	public hkShape
{
	hkSingleShapeContainer m_shape_container;
	byte __data18[0x18];
	hkTransform m_transform;
};

struct hkMultiSphereShape :
	public hkSphereRepShape
{
public:
	enum
	{
		MAX_SPHERES = 8
	};

	long m_num_spheres;
	hkVector4 m_spheres[MAX_SPHERES];
};

struct hkConvexVerticesShape :
	public hkConvexShape
{
	struct FourVectors
	{
		hkVector4 x;
		hkVector4 y;
		hkVector4 z;
	};

	hkVector4 __unknown20;
	hkVector4 __unknown30;
	hkArray<FourVectors> m_four_vectors;
	hkArray<hkVector4> __unknown4C;
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

void real_vector3d_from_hkVector4(vector3d* v, hkVector4 const* w)
{
	ASSERT(v && w);

	v->i = w->x;
	v->j = w->y;
	v->k = w->z;
}

void matrix3x3_from_hkMatrix3(matrix3x3* matrix, hkMatrix3 const* rotation)
{
	ASSERT(matrix && rotation);

	real_vector3d_from_hkVector4(&matrix->forward, &rotation->forward);
	real_vector3d_from_hkVector4(&matrix->left, &rotation->left);
	real_vector3d_from_hkVector4(&matrix->up, &rotation->up);
}

void matrix4x3_from_hkTransform(real_matrix4x3* matrix, hkTransform const* transform)
{
	ASSERT(matrix && transform);

	matrix->scale = 1.0f;
	matrix3x3_from_hkMatrix3((matrix3x3*)&matrix->forward, &transform->rotation.matrix);
	real_point3d_from_hkVector4(&matrix->position, &transform->translation);
}


void __cdecl render_debug_physics_shape(hkShape const* shape, real_matrix4x3 const* matrix, real_argb_color const* color)
{
	while (2)
	{
		switch (shape->m_type)
		{
		case HK_SHAPE_SPHERE:
		{
			c_sphere_shape const* convex_shape = static_cast<c_sphere_shape const*>(shape);

			render_debug_sphere(true, &matrix->position, convex_shape->m_radius, color);
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
		case HK_SHAPE_CAPSULE:
		{
			c_capsule_shape const* capsule_shape = static_cast<c_capsule_shape const*>(shape);
		
			real_point3d points[2]{};
			real_point3d_from_hkVector4(&points[0], &capsule_shape->m_vertices[0]);
			real_point3d_from_hkVector4(&points[1], &capsule_shape->m_vertices[1]);
			matrix4x3_transform_point(matrix, &points[0], &points[0]);
			matrix4x3_transform_point(matrix, &points[1], &points[1]);
		
			vector3d height{};
			if (magnitude_squared3d(vector_from_points3d(&points[0], &points[1], &height)) >= k_real_tiny_epsilon)
				render_debug_pill(true, &points[1], &height, capsule_shape->m_radius, color);
			else
				render_debug_sphere(true, &points[1], capsule_shape->m_radius, color);
		}
		break;
		case HK_SHAPE_CONVEX_VERTICES:
		{
			c_convex_vertices_shape const* convex_vertices_shape = static_cast<c_convex_vertices_shape const*>(shape);
		
			for (long i = 0; i < convex_vertices_shape->m_four_vectors.m_size; i++)
			{
				hkConvexVerticesShape::FourVectors* four_vectors = &convex_vertices_shape->m_four_vectors.m_data[i];

				real_point3d point{};
				set_real_point3d(&point, four_vectors->x.x, four_vectors->y.x, four_vectors->z.x);
				matrix4x3_transform_point(matrix, &point, &point);
				render_debug_point(true, &point, 0.125f, color);
			}
		
		}
		break;
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
		case HK_SHAPE_MULTI_SPHERE:
		{
			c_multi_sphere_shape const* multi_sphere_shape = static_cast<c_multi_sphere_shape const*>(shape);

			ASSERT(IN_RANGE_INCLUSIVE(multi_sphere_shape->m_num_spheres, 0, hkMultiSphereShape::MAX_SPHERES));
			for (long i = 0; i < multi_sphere_shape->m_num_spheres; i++)
			{
				hkVector4 const* sphere = &multi_sphere_shape->m_spheres[i];

				real_point3d point{};
				real_point3d_from_hkVector4(&point, sphere);
				matrix4x3_transform_point(matrix, &point, &point);

				render_debug_sphere(true, &point, sphere->w, color);
			}
		}
		break;
		case HK_SHAPE_BV:
		{
			c_bv_shape const* bv_shape = static_cast<c_bv_shape const*>(shape);
			if (bv_shape->m_bounding_volume_shape->m_type <= HK_SHAPE_USER0)
				continue;
		}
		break;
		case HK_SHAPE_TRANSFORM:
		{
			hkTransformShape const* transform_shape = static_cast<hkTransformShape const*>(shape);

			real_matrix4x3 transform_matrix{};
			matrix4x3_from_hkTransform(&transform_matrix, &transform_shape->m_transform);
			matrix4x3_multiply(matrix, &transform_matrix, &transform_matrix);

			render_debug_physics_shape(transform_shape->m_shape_container.m_child_shape, &transform_matrix, color);
		}
		break;
		case HK_SHAPE_USER0:
		{
		
		}
		break;
		default:
			return;
		}
		break;
	}
}

