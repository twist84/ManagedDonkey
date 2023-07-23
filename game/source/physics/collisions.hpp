#pragma once

#include "cseries/cseries.hpp"
#include "game/materials.hpp"

// #TODO: confirm this
enum e_collision_test_flag
{
	_collision_test_structure_bit = 0,
	_collision_test_water_bit = 1,
	_collision_test_instanced_geometry_bit = 3,
	_collision_test_render_only_bsps_bit = 4,
	_collision_test_try_to_keep_location_valid_bit = 17,
	_collision_test_objects_bit = 32,
};

using s_collision_test_flags = c_static_flags<64>;

struct collision_result
{
	collision_result() :
		type(),
		__real4(),
		position(),
		__location14(),
		__location18(),
		material_type(c_global_material_type()),
		__pad1E(),
		__vector24(),
		__data30(),
		instanced_geometry_instance_index(),
		object_index(),
		__data3C(),
		node_index(),
		bsp_reference(),
		structure_bsp_index(),
		__data48()
	{
	}

	long type;
	real __real4;
	real_point3d position;
	s_location __location14;
	s_location __location18;
	c_global_material_type material_type;
	byte __pad1E[0x4];
	vector3d __vector24;
	byte __data30[0x4];
	long instanced_geometry_instance_index;
	long object_index;
	byte __data3C[0x2];
	short node_index;
	long bsp_reference;
	long structure_bsp_index;
	byte __data48[0x14];
};
static_assert(sizeof(collision_result) == 0x5C);

extern bool __cdecl collision_test_vector(s_collision_test_flags flags, real_point3d const* point, vector3d const* vector, long first_ignore_object_index, long second_ignore_object_index, collision_result* collision);

