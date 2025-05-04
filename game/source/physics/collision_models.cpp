#include "physics/collision_models.hpp"

int32 __cdecl collision_model_get_bsp_node_index(collision_model_instance const* instance, uns32 bsp_reference)
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

bool __cdecl collision_model_instance_new(collision_model_instance* instance, int32 object_index)
{
	return INVOKE(0x00980E40, collision_model_instance_new, instance, object_index);
}

