#pragma once

#include "cseries/cseries.hpp"
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
		scale(),
		position(),
		__location14(),
		position_location(),
		material_type(c_global_material_type()),
		__pad22(),
		plane(),
		instanced_geometry_instance_index(),
		object_index(),
		region_index(),
		node_index(),
		bsp_reference(),
		bsp_index(),
		leaf_index(),
		surface_index(),
		plane_index(),
		edge_index(),
		surface_flags(),
		breakable_surface_index(),
		__unknown57(),
		material_index(),
		breakable_surface_set_index(),
		__data5B()
	{
	}

	c_enum<e_collision_result_type, long, _collision_result_none, k_collision_result_type_count> type;
	real scale;
	real_point3d position;
	s_location __location14;
	s_location position_location;
	c_collision_bsp_reference collision_bsp_reference;
	c_global_material_type material_type;
	byte __pad22[0x2];
	plane3d plane;
	long instanced_geometry_instance_index;
	long object_index;
	short region_index;
	short node_index;
	dword bsp_reference;
	long bsp_index;
	long leaf_index;
	long surface_index;
	long plane_index;
	char edge_index;
	byte_flags surface_flags; // c_flags<e_surface_flags, byte, k_surface_flags>
	char breakable_surface_index;
	byte __unknown57;
	short material_index;
	char breakable_surface_set_index;
	byte __data5B[0x1];
};
static_assert(sizeof(collision_result) == 0x5C);

struct collision_bsp_test_vector_result
{
	real t;
	plane3d const* plane;
	long leaf_index;
	long surface_index;
	long plane_index;
	char edge_index;
	byte_flags flags;
	byte breakable_surface_index;
	short material_index;
	long leaf_count;
	long leaf_indices[256];
	long breakable_surface_set_index;
};
static_assert(sizeof(collision_bsp_test_vector_result) == 0x424);

struct collision_model_test_vector_result
{
	long bsp_index;
	short node_index;
	short region_index;
	collision_bsp_test_vector_result bsp_result;
};
static_assert(sizeof(collision_model_test_vector_result) == 0x42C);

struct collision_bsp_test_sphere_result
{
	// #TODO: confirm this

	long surface_count;
	long surface_indices[255];
	long unknown0_count;
	long unknown0_indices[256];
	long edge_count;
	long edge_indices[256];
	long unknown1_count;
	long unknown1_indices[255];
	long vertex_count;
	long vertex_indices[256];
	long unknown2_count;
	long unknown2_indices[255];
	long leaf_count;
	long leaf_indices[256];
};
static_assert(sizeof(collision_bsp_test_sphere_result) == 0x1C10);

struct collision_feature_list;
struct collision_plane;

extern bool __cdecl collision_get_features_in_sphere(s_collision_test_flags flags, real_point3d const* point, real radius, real height, real width, long ignore_object_index, long a7, collision_feature_list* features);
extern short __cdecl collision_move_pill(s_collision_test_flags flags, real_point3d const* old_position, vector3d const* old_velocity, real height, real radius, long first_ignore_object_index, long second_ignore_object_index, real_point3d* new_position, vector3d* new_velocity, short maximum_collision_count, collision_plane* collisions);
extern short __cdecl collision_move_point(real_point3d const* old_position, vector3d const* old_velocity, collision_feature_list const* features, real_point3d* new_position, vector3d* new_velocity, short maximum_collision_count, collision_plane* collisions);
extern short __cdecl collision_move_sphere(s_collision_test_flags flags, real_point3d const* old_position, vector3d const* old_velocity, real radius, long first_ignore_object_index, long second_ignore_object_index, real_point3d* new_position, vector3d* new_velocity, short maximum_collision_count, collision_plane* collisions);
extern bool __cdecl collision_test_point(s_collision_test_flags flags, real_point3d const* point, long first_ignore_object_index, long second_ignore_object_index);
extern bool __cdecl collision_test_point(s_collision_test_flags flags, real_point3d const* point, long first_ignore_object_index, long second_ignore_object_index, e_collision_result_type* collision_result_type);
extern bool __cdecl collision_test_sphere(long structure_bsp_index, s_collision_test_flags flags, real_point3d const* center, real radius, long a5, long a6);
extern bool __cdecl collision_test_sphere(s_collision_test_flags flags, real_point3d const* center, real radius, long a4, long a5);
extern bool __cdecl collision_test_vector(s_collision_test_flags flags, real_point3d const* point, vector3d const* vector, long first_ignore_object_index, long second_ignore_object_index, collision_result* collision);
extern bool __cdecl collision_test_vector(s_collision_test_flags flags, bool a2, real_point3d const* point, vector3d const* vector, long first_ignore_object_index, long second_ignore_object_index, long third_ignore_object_index, collision_result* collision);

