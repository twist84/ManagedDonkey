#pragma once

#include "cseries/cseries.hpp"

enum e_pathfinding_sphere_flags
{
	_pathfinding_sphere_flag_remains_when_open_bit = 0,
	_pathfinding_sphere_flag_vehicle_only_bit,
	_pathfinding_sphere_flag_with_sectors_bit,

	k_pathfinding_sphere_flags
};

struct collision_model_pathfinding_sphere
{
	int16 node;
	c_flags<e_pathfinding_sphere_flags, int16, k_pathfinding_sphere_flags> flags;
	real_point3d center;
	real32 radius;
};
COMPILE_ASSERT(sizeof(collision_model_pathfinding_sphere) == 0x14);

enum e_collision_model_flags
{
	_collision_model_contains_open_edges_bit = 0,

	k_collision_model_flags
};

struct collision_model_material
{
	c_string_id name;
};
COMPILE_ASSERT(sizeof(collision_model_material) == sizeof(c_string_id));

struct collision_model_definition
{
	uns32 import_info_checksum;
	s_tag_block errors;
	c_flags<e_collision_model_flags, uns32, k_collision_model_flags> flags;
	c_typed_tag_block<collision_model_material> materials;
	s_tag_block regions;
	s_tag_block pathfinding_spheres;
	s_tag_block nodes;
};
COMPILE_ASSERT(sizeof(collision_model_definition) == 0x44);

