#pragma once

#include "cseries/location.hpp"
#include "game/materials.hpp"
#include "physics/collision_constants.hpp"
#include "physics/collision_bsp_references.hpp"

enum e_collision_result_type
{
	_collision_result_none = 0,
	_collision_result_structure,
	_collision_result_water,
	_collision_result_instanced_geometry,
	_collision_result_object,

	k_collision_result_type_count
};

struct collision_result
{
	collision_result() :
		type(_collision_result_none),
		t(),
		point(),
		start_location(),
		location(),
		global_material_type(c_global_material_type()),
		plane(),
		instanced_geometry_instance_index(),
		object_index(),
		region_index(),
		node_index(),
		bsp_reference(),
		structure_bsp_index(),
		leaf_index(),
		surface_index(),
		plane_designator(),
		edge_index(),
		flags(),
		breakable_surface_index(),
		material_index(),
		breakable_surface_set_index()
	{
	}

	e_collision_result_type type;
	real32 t;
	real_point3d point;
	s_location start_location;
	s_location location;
	c_collision_bsp_reference collision_bsp_reference;
	c_global_material_type global_material_type;
	real_plane3d plane;
	int32 instanced_geometry_instance_index;
	int32 object_index;
	int16 region_index;
	int16 node_index;
	uns32 bsp_reference;
	int32 structure_bsp_index;
	int32 leaf_index;
	int32 surface_index;
	int32 plane_designator;
	int8 edge_index;
	uns8 flags; // e_surface_flags
	uns8 breakable_surface_index;
	int16 material_index;
	int8 breakable_surface_set_index;
};
COMPILE_ASSERT(sizeof(collision_result) == 0x5C);

struct collision_bsp_test_vector_result
{
	real32 t;
	const real_plane3d* plane;
	int32 leaf_index;
	int32 surface_index;
	int32 plane_designator;
	int8 edge_index;
	uns8 flags;
	uns8 breakable_surface_index;
	int16 material_index;
	int32 leaf_count;
	int32 leaf_indices[256];
	int32 breakable_surface_set_index;
};
COMPILE_ASSERT(sizeof(collision_bsp_test_vector_result) == 0x424);

struct collision_model_test_vector_result
{
	uns32 bsp_reference;
	int16 node_index;
	int16 region_index;
	collision_bsp_test_vector_result bsp_result;
};
COMPILE_ASSERT(sizeof(collision_model_test_vector_result) == 0x42C);

struct collision_bsp_test_sphere_result
{
	int32 surface_count;
	int32 surface_indices[256];
	int32 surface_leaf_indices[256];

	int32 edge_count;
	int32 edge_indices[256];
	int32 edge_leaf_indices[256];

	int32 vertex_count;
	int32 vertex_indices[256];
	int32 vertex_leaf_indices[256];

	int32 leaf_count;
	int32 leaf_indices[256];
};
COMPILE_ASSERT(sizeof(collision_bsp_test_sphere_result) == 0x1C10);

struct collision_feature_list;
struct collision_plane;

extern bool __cdecl collision_get_features_in_sphere(s_collision_test_flags flags, const real_point3d* point, real32 radius, real32 height, real32 width, int32 ignore_object_index, int32 a7, collision_feature_list* features);
extern int16 __cdecl collision_move_pill(s_collision_test_flags flags, const real_point3d* old_position, const real_vector3d* old_velocity, real32 height, real32 radius, int32 first_ignore_object_index, int32 second_ignore_object_index, real_point3d* new_position, real_vector3d* new_velocity, int16 maximum_collision_count, collision_plane* collisions);
extern int16 __cdecl collision_move_point(const real_point3d* old_position, const real_vector3d* old_velocity, const collision_feature_list* features, real_point3d* new_position, real_vector3d* new_velocity, int16 maximum_collision_count, collision_plane* collisions);
extern int16 __cdecl collision_move_sphere(s_collision_test_flags flags, const real_point3d* old_position, const real_vector3d* old_velocity, real32 radius, int32 first_ignore_object_index, int32 second_ignore_object_index, real_point3d* new_position, real_vector3d* new_velocity, int16 maximum_collision_count, collision_plane* collisions);
extern bool __cdecl collision_test_line(s_collision_test_flags flags, const real_point3d* point0, const real_point3d* point1, int32 first_ignore_object_index, int32 second_ignore_object_index, collision_result* collision);
extern bool __cdecl collision_test_point(s_collision_test_flags flags, const real_point3d* point, int32 first_ignore_object_index, int32 second_ignore_object_index);
extern bool __cdecl collision_test_point(s_collision_test_flags flags, const real_point3d* point, int32 first_ignore_object_index, int32 second_ignore_object_index, e_collision_result_type* collision_result_type);
extern bool __cdecl collision_test_sphere(int32 structure_bsp_index, s_collision_test_flags flags, const real_point3d* center, real32 radius, int32 a5, int32 a6);
extern bool __cdecl collision_test_sphere(s_collision_test_flags flags, const real_point3d* center, real32 radius, int32 a4, int32 a5);
extern bool __cdecl collision_test_vector(s_collision_test_flags flags, const real_point3d* point, const real_vector3d* vector, int32 first_ignore_object_index, int32 second_ignore_object_index, collision_result* collision);
extern bool __cdecl collision_test_vector(s_collision_test_flags flags, bool a2, const real_point3d* point, const real_vector3d* vector, int32 first_ignore_object_index, int32 second_ignore_object_index, int32 third_ignore_object_index, collision_result* collision);

