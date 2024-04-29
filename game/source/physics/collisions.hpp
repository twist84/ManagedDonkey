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
		__unknown50(),
		__unknown54(),
		surface_flags(),
		__unknown56(),
		__unknown57(),
		material_index(),
		__unknown5A(),
		__data5B()
	{
	}

	c_enum<e_collision_result_type, long, _collision_result_none, k_collision_result_type_count> type;
	real scale;
	real_point3d position;
	s_cluster_reference __location14;
	s_cluster_reference position_location;
	c_collision_bsp_reference collision_bsp_reference;
	c_global_material_type material_type;
	byte __pad22[0x2];
	plane3d plane;
	long instanced_geometry_instance_index;
	long object_index;
	short region_index;
	short node_index;
	long bsp_reference;
	long bsp_index;
	long leaf_index;
	long surface_index;
	long __unknown50;
	byte __unknown54;
	byte_flags surface_flags; // c_flags<e_surface_flags, byte, k_surface_flags>
	byte __unknown56;
	byte __unknown57;
	short material_index;
	byte __unknown5A;
	byte __data5B[0x1];
};
static_assert(sizeof(collision_result) == 0x5C);

struct collision_feature_list;

extern bool __cdecl collision_get_features_in_sphere(s_collision_test_flags flags, real_point3d const* point, real radius, real height, real width, long ignore_object_index, long a7, collision_feature_list* features);
extern bool __cdecl collision_test_vector(s_collision_test_flags flags, real_point3d const* point, vector3d const* vector, long first_ignore_object_index, long second_ignore_object_index, collision_result* collision);

