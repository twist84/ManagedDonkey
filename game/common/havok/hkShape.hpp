#pragma once

#include "hkArray.hpp"
#include "hkBase.hpp"
#include "hkMath.hpp"

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
	float m_radius;
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
	uns16 m_welding_info;
	unsigned char m_welding_type;
	unsigned char m_is_extruded;
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
	}*__vftable;
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
	hkSingleShapeContainer m_childShape;
	hkQuaternion m_rotation;
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

	int m_num_spheres;
	hkVector4 m_spheres[MAX_SPHERES];
};

struct hkConvexVerticesShape :
	public hkConvexShape
{
	struct FourVectors
	{
		hkVector4 m_x;
		hkVector4 m_y;
		hkVector4 m_z;
	};

	hkVector4 m_aabbHalfExtents;
	hkVector4 m_aabbCenter;
	hkArray<FourVectors> m_rotatedVertices;
	hkArray<hkVector4> m_planeEquations;
};
