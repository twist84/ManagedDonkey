#pragma once

#include "cseries/cseries.hpp"

enum
{
	_collision_feature_sphere,
	_collision_feature_cylinder,
	_collision_feature_prism,

	NUMBER_OF_COLLISION_FEATURE_TYPES
};

enum
{
	MAXIMUM_POINTS_PER_COLLISION_PRISM = 0x8,
};

enum
{
	MAXIMUM_COLLISION_FEATURES_PER_TEST = 256,
	MAXIMUM_COLLISION_LEAVES_PER_TEST = 256,
};

struct collision_sphere
{
	int32 object_index;
	int32 bsp_reference;
	int32 structure_bsp_index;
	int32 surface_index;
	uns8 flags; // e_surface_flags
	uns8 breakable_surface_index;
	int16 material_index;
	real_point3d center;
	real32 radius;
};
static_assert(sizeof(collision_sphere) == 0x24);

struct collision_cylinder
{
	int32 object_index;
	uns32 bsp_reference;
	int32 structure_bsp_index;
	int32 surface_index;
	uns8 flags; // e_surface_flags
	uns8 breakable_surface_index;
	int16 material_index;
	real_point3d base;
	real_vector3d height;
	real32 width;
};
static_assert(sizeof(collision_cylinder) == 0x30);

struct collision_prism
{
	int32 object_index;
	uns32 bsp_reference;
	int32 objestructure_bsp_indexct_index;
	int32 surface_index;
	uns8 flags; // e_surface_flags
	uns8 breakable_surface_index;
	int16 material_index;
	real_plane3d plane;
	real32 height;
	int16 projection_axis;
	bool projection_sign;
	int32 point_count;
	real_point2d points[MAXIMUM_POINTS_PER_COLLISION_PRISM];
};
static_assert(sizeof(collision_prism) == 0x70);

struct collision_plane
{
	int32 object_index;
	uns32 bsp_reference;
	int32 surface_index;
	uns8 flags; // e_surface_flags
	uns8 breakable_surface_index;
	int16 material_index;
	real32 t;
	real_point3d point;
	real_plane3d plane;
};
static_assert(sizeof(collision_plane) == 0x30);

struct collision_feature_list
{
	int16 count[NUMBER_OF_COLLISION_FEATURE_TYPES];
	collision_sphere spheres[MAXIMUM_COLLISION_FEATURES_PER_TEST];
	collision_cylinder cylinders[MAXIMUM_COLLISION_FEATURES_PER_TEST];
	collision_prism prisms[MAXIMUM_COLLISION_FEATURES_PER_TEST];
};
static_assert(sizeof(collision_feature_list) == 0xC408);
static_assert(0x0000 == OFFSETOF(collision_feature_list, count));
static_assert(0x0008 == OFFSETOF(collision_feature_list, spheres));
static_assert(0x2408 == OFFSETOF(collision_feature_list, cylinders));
static_assert(0x5408 == OFFSETOF(collision_feature_list, prisms));

extern bool __cdecl collision_features_test_point(collision_feature_list const* features, real_point3d const* point, collision_plane* plane);

extern void render_debug_collision_features(collision_feature_list const* features);
extern void render_debug_collision_prism(collision_prism const* prism, real_argb_color const* color);
extern void render_debug_collision_cylinder(collision_cylinder const* cylinder, real_argb_color const* color);
extern void render_debug_collision_sphere(collision_sphere const* sphere, real_argb_color const* color);

