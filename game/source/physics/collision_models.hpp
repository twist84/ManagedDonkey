#pragma once

#include "cseries/cseries.hpp"

struct s_model_definition;
struct collision_model_definition;
struct collision_model_instance
{
	int32 object_index;
	s_model_definition* model_definition;
	collision_model_definition* collision_model;
	int8* collision_region_permutation_indices;
	real_matrix4x3* nodes;
};
static_assert(sizeof(collision_model_instance) == 0x14);

extern int32 __cdecl collision_model_get_bsp_node_index(const collision_model_instance* instance, uns32 bsp_reference);
extern bool __cdecl collision_model_instance_new(collision_model_instance* instance, int32 object_index);

