#pragma once

#include "cseries/cseries.hpp"
#include "physics/collision_bsp_references.hpp"

struct collision_feature_list;
struct collision_model_definition;
struct s_model_definition;

struct collision_model_instance
{
	int32 object_index;
	const s_model_definition* model;
	const collision_model_definition* collision_model;
	const int8* region_permutation_indices;
	const real_matrix4x3* matrices;
};
COMPILE_ASSERT(sizeof(collision_model_instance) == 0x14);

struct collision_model_bsp_iterator
{
	collision_model_bsp_iterator() :
		collision_model(),
		collision_region_permutation_indices(),
		node_index(),
		surface_index(),
		permutation_index(),
		region_index(),
		bsp_reference()
	{
	}

	collision_model_definition* collision_model;
	int8 collision_region_permutation_indices[16];
	uns8 node_index;
	uns8 surface_index;
	uns8 permutation_index;
	uns8 region_index;
	c_collision_bsp_reference bsp_reference;
};
COMPILE_ASSERT(sizeof(collision_model_bsp_iterator) == 0x20);

extern void __cdecl collision_model_bsp_iterator_new(collision_model_bsp_iterator* iterator, const s_model_definition* model, const collision_model_definition* collision_model, const int8* region_permutation_indices);
extern bool __cdecl collision_model_bsp_iterator_next(collision_model_bsp_iterator* iterator);
extern c_collision_bsp_reference __cdecl collision_model_get_bsp(collision_model_instance const* instance, uns32 bsp_reference);
extern int32 __cdecl collision_model_get_bsp_node_index(const collision_model_instance* instance, uns32 bsp_reference);
extern bool __cdecl collision_model_get_features_in_sphere(const collision_model_instance* instance, const real_point3d* center, real32 radius, real32 height, real32 width, collision_feature_list* features);
extern bool __cdecl collision_model_instance_new(collision_model_instance* instance, int32 object_index);
extern void __cdecl render_debug_collision_bsp(c_collision_bsp_reference bsp_reference, const real_matrix4x3* matrix);
extern void __cdecl render_debug_collision_model(const collision_model_instance* instance);

