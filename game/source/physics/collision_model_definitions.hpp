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
	short node;
	c_flags<e_pathfinding_sphere_flags, short, k_pathfinding_sphere_flags> flags;
	real_point3d center;
	real32 radius;
};
static_assert(sizeof(collision_model_pathfinding_sphere) == 0x14);

enum e_collision_model_flags
{
	_collision_model_contains_open_edges_bit = 0,

	k_collision_model_flags
};

struct collision_model_material
{
	c_string_id name;
};
static_assert(sizeof(collision_model_material) == sizeof(c_string_id));

struct collision_model_definition
{
	uint32 import_info_checksum;
	s_tag_block errors;
	c_flags<e_collision_model_flags, uint32, k_collision_model_flags> flags;
	c_typed_tag_block<collision_model_material> materials;
	s_tag_block regions;
	s_tag_block pathfinding_spheres;
	s_tag_block nodes;
};
static_assert(sizeof(collision_model_definition) == 0x44);

