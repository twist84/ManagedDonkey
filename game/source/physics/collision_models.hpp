#pragma once

#include "cseries/cseries.hpp"

struct s_model_definition;
struct collision_model_definition;
struct collision_model_instance
{
	long object_index;
	s_model_definition* model_definition;
	collision_model_definition* collision_model;
	char* collision_region_permutation_indices;
	real_matrix4x3* nodes;
};
static_assert(sizeof(collision_model_instance) == 0x14);

extern long __cdecl collision_model_get_bsp_node_index(collision_model_instance const* instance, dword bsp_reference);
