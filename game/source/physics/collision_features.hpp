#pragma once

#include "cseries/cseries.hpp"

#define MAXIMUM_POINTS_PER_COLLISION_PRISM 8
#define MAXIMUM_COLLISION_FEATURES_PER_TEST 256

enum e_collision_feature
{
	_collision_feature_sphere,
	_collision_feature_cylinder,
	_collision_feature_prism,

	k_collision_feature_count
};

struct collision_sphere
{
	long __unknown0;

	long bsp_reference;
	long object_index;
	long surface_index;
	byte_flags flags; // e_surface_flags
	byte breakable_surface_index;
	short material_index;
	real_point3d center;
	real radius;
};
static_assert(sizeof(collision_sphere) == 0x24);

struct collision_cylinder
{
	long __unknown0;

	long bsp_reference;
	long object_index;
	long surface_index;
	byte_flags flags; // e_surface_flags
	byte breakable_surface_index;
	short material_index;
	real_point3d base;
	vector3d height;
	real width;
};
static_assert(sizeof(collision_cylinder) == 0x30);

struct collision_prism
{
	long __unknown0;

	long bsp_reference;
	long object_index;
	long surface_index;
	byte_flags flags; // e_surface_flags
	byte breakable_surface_index;
	short material_index;
	plane3d plane;
	real height;
	short projection_axis;
	byte projection_sign;
	long point_count;
	real_point2d points[MAXIMUM_POINTS_PER_COLLISION_PRISM];
};
static_assert(sizeof(collision_prism) == 0x70);

struct collision_feature_list
{
	short count[k_collision_feature_count];
	collision_sphere spheres[MAXIMUM_COLLISION_FEATURES_PER_TEST];
	collision_cylinder cylinders[MAXIMUM_COLLISION_FEATURES_PER_TEST];
	collision_prism prisms[MAXIMUM_COLLISION_FEATURES_PER_TEST];
};
static_assert(sizeof(collision_feature_list) == 0xC408);
static_assert(0x0000 == OFFSETOF(collision_feature_list, count));
static_assert(0x0008 == OFFSETOF(collision_feature_list, spheres));
static_assert(0x2408 == OFFSETOF(collision_feature_list, cylinders));
static_assert(0x5408 == OFFSETOF(collision_feature_list, prisms));

struct collision_plane
{
	long object_index;
	long bsp_reference;
	long surface_index;
	byte_flags flags; // e_surface_flags
	byte breakable_surface_index;
	short material_index;
	real t;
	real_point3d point;
	plane3d plane;
};
static_assert(sizeof(collision_plane) == 0x30);

extern bool __cdecl collision_features_test_point(collision_feature_list const* features, real_point3d const* point, collision_plane* plane);

extern void render_debug_collision_features(collision_feature_list const* features);
extern void render_debug_collision_prism(collision_prism const* prism, real_argb_color const* color);
extern void render_debug_collision_cylinder(collision_cylinder const* cylinder, real_argb_color const* color);
extern void render_debug_collision_sphere(collision_sphere const* sphere, real_argb_color const* color);

