#include "physics/collision_models.hpp"

#include "physics/collision_features.hpp"
#include "physics/collision_bsp.hpp"

void __cdecl collision_model_bsp_iterator_new(collision_model_bsp_iterator* iterator, const s_model_definition* model, const collision_model_definition* collision_model, const int8* region_permutation_indices)
{
	INVOKE(0x00980AF0, collision_model_bsp_iterator_new, iterator, model, collision_model, region_permutation_indices);
}

bool __cdecl collision_model_bsp_iterator_next(collision_model_bsp_iterator* iterator)
{
	return INVOKE(0x00980BC0, collision_model_bsp_iterator_next, iterator);
}

c_collision_bsp_reference __cdecl collision_model_get_bsp(collision_model_instance const* instance, uns32 bsp_reference)
{
	return INVOKE(0x00980CE0, collision_model_get_bsp, instance, bsp_reference);
}

int32 __cdecl collision_model_get_bsp_node_index(const collision_model_instance* instance, uns32 bsp_reference)
{
	return INVOKE(0x00980D30, collision_model_get_bsp_node_index, instance, bsp_reference);

	//ASSERT(instance);
	//ASSERT(instance->collision_model);
	//ASSERT(bsp_reference != NONE);
	//
	//int32 node_index = bsp_reference & 0xFF;
	//ASSERT(node_index >= 0 && node_index < instance->collision_model->nodes.count);
	//
	//return node_index;
}

bool __cdecl collision_model_get_features_in_sphere(const collision_model_instance* instance, const real_point3d* center, real32 radius, real32 height, real32 width, collision_feature_list* features)
{
	return INVOKE(0x00980D40, collision_model_get_features_in_sphere, instance, center, radius, height, width, features);
}

bool __cdecl collision_model_instance_new(collision_model_instance* instance, int32 object_index)
{
	return INVOKE(0x00980E40, collision_model_instance_new, instance, object_index);
}

void __cdecl render_debug_collision_bsp(c_collision_bsp_reference bsp_reference, const real_matrix4x3* matrix)
{
	for (int32 edge_index = 0; edge_index < bsp_reference.get_edge_count(); edge_index++)
	{
		render_debug_collision_edge(bsp_reference, edge_index, matrix, global_real_argb_green);
	}
}

void __cdecl render_debug_collision_model(const collision_model_instance* instance)
{
	collision_model_bsp_iterator iterator;
	collision_model_bsp_iterator_new(&iterator, instance->model, instance->collision_model, instance->region_permutation_indices);
	while (collision_model_bsp_iterator_next(&iterator))
	{
		if (iterator.bsp_reference.get_surface_count() > 0 && iterator.bsp_reference.get_edge_count() > 0 && iterator.bsp_reference.get_vertex_count() > 0)
		{
			render_debug_collision_bsp(iterator.bsp_reference, &instance->matrices[iterator.node_index]);
		}
	}
}

