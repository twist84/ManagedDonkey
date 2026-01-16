#pragma once

#include "physics/physics_model_definitions.hpp"
#include "tag_files/tag_groups.hpp"

class c_cluster_collision_instanced_geometry_shape
{
public:
	s_havok_shape_collection cluster_collsion_shape;
	s_tag_reference structure_bsp_reference;
	int32 cluster_index;
};
COMPILE_ASSERT(sizeof(c_cluster_collision_instanced_geometry_shape) == 0x2C);

class c_cluster_collision_instanced_geometry_mopp_bv_tree_shape
{
public:
	s_havok_shape base;
	void* field_pointer_skip;
	void* shape_collection_ptr;
	void* mopp_code_ptr;
};
COMPILE_ASSERT(sizeof(c_cluster_collision_instanced_geometry_mopp_bv_tree_shape) == 0x1C);

struct s_collision_instanced_geometry_definition
{
	c_cluster_collision_instanced_geometry_shape cluster_shape;
	c_cluster_collision_instanced_geometry_mopp_bv_tree_shape mopp_bv_tree_shape;
	s_tag_block mopp_code_block;
};
COMPILE_ASSERT(sizeof(s_collision_instanced_geometry_definition) == 0x54);

