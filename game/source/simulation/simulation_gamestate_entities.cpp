#include "simulation/simulation_gamestate_entities.hpp"

#include "main/main_game.hpp"
#include "memory/thread_local.hpp"

//.text:00471BC0 ; void __cdecl simulation_gamestate_entities_build_clear_flags(s_simulation_queue_gamestate_clear_data*)
//.text:00471C50 ; void __cdecl simulation_object_glue_clear_by_flags(s_simulation_queue_object_glue_clear_data const*)

void __cdecl simulation_gamestate_entities_dispose()
{
	//INVOKE(0x00471CE0, simulation_gamestate_entities_dispose);

	TLS_DATA_GET_VALUE_REFERENCE(simulation_gamestate_entity_data);

	simulation_gamestate_entity_data = NULL;
}

void __cdecl simulation_gamestate_entities_dispose_from_old_map()
{
	//INVOKE(0x00471D10, simulation_gamestate_entities_dispose_from_old_map);

	TLS_DATA_GET_VALUE_REFERENCE(simulation_gamestate_entity_data);

	if (!main_game_reset_in_progress())
		data_make_invalid(*simulation_gamestate_entity_data);
}

void __cdecl simulation_gamestate_entities_initialize()
{
	//INVOKE(0x00471D40, simulation_gamestate_entities_initialize);

	TLS_DATA_GET_VALUE_REFERENCE(simulation_gamestate_entity_data);

	simulation_gamestate_entity_data = data_new("sim. gamestate entities", 2048, sizeof(s_simulation_gamestate_entity), 0, &g_simulation_gamestate_entity_data_allocator);

	ASSERT(*simulation_gamestate_entity_data != NULL);
}

void __cdecl simulation_gamestate_entities_initialize_for_new_map()
{
	//INVOKE(0x00471D80, simulation_gamestate_entities_initialize_for_new_map);

	TLS_DATA_GET_VALUE_REFERENCE(simulation_gamestate_entity_data);

	data_make_valid(*simulation_gamestate_entity_data);
	data_delete_all(*simulation_gamestate_entity_data);
}

//.text:00471DB0 ; void __cdecl simulation_object_glue_notify_simulation_world_reset(void)
//.text:00471E10 ; long __cdecl simulation_gamestate_entity_create()
//.text:00471E60 ; long __cdecl simulation_object_glue_create_at_index(long)

void __cdecl simulation_object_glue_delete(long object_index)
{
	INVOKE(0x00471EE0, simulation_object_glue_delete, object_index);
}

//.text:00471F10 ; long __cdecl simulation_gamestate_entity_get_object_index(long)
//.text:00471F50 ; long __cdecl simulation_object_glue_get_object_index_type_safe(long, dword)

long __cdecl simulation_object_glue_get_simulation_entity_index(long object_index)
{
	return INVOKE(0x00471FC0, simulation_object_glue_get_simulation_entity_index, object_index);
}

void __cdecl simulation_gamestate_entity_set_object_index(long simulation_object_glue_index, long object_index)
{
	INVOKE(0x00471FF0, simulation_gamestate_entity_set_object_index, simulation_object_glue_index, object_index);
}

//.text:00472020 ; void __cdecl simulation_gamestate_entity_set_simulation_entity_index(long, long)
//.text:00472050 ; void __cdecl simulation_object_glue_index_decode(c_bitstream*, long*)
//.text:00472080 ; void __cdecl simulation_object_glue_index_encode(c_bitstream*, long)
//.text:00472120 ; bool __cdecl simulation_gamestate_index_valid(long)

long __cdecl simulation_entity_create(e_simulation_entity_type entity_type, long object_index, long simulation_object_glue_index)
{
	return INVOKE(0x004AF8A0, simulation_entity_create, entity_type, object_index, simulation_object_glue_index);
}

void __cdecl simulation_entity_delete(long simulation_entity_index, long object_index, long simulation_object_glue_index)
{
	INVOKE(0x004AF910, simulation_entity_delete, simulation_entity_index, object_index, simulation_object_glue_index);
}

void __cdecl simulation_entity_force_update(long simulation_entity_index, long object_index, c_flags<long, qword, 64>& flags)
{
	INVOKE(0x004AF970, simulation_entity_force_update, simulation_entity_index, object_index, flags);
}

//.text:004AF9C0 ; long __cdecl simulation_entity_get_simulation_object_glue_index(long)
//.text:004AFA00 ; 
//.text:004AFA20 ; bool __cdecl simulation_entity_is_object(long)
//.text:004AFA80 ; e_simulation_entity_type __cdecl simulation_entity_type_from_game_engine()
//.text:004AFA90 ; e_simulation_entity_type __cdecl simulation_entity_type_from_object_creation(long, long, bool)
//.text:004AFBF0 ; 

bool __cdecl simulation_entity_type_is_object(e_simulation_entity_type entity_type)
{
	return INVOKE(0x004AFC00, simulation_entity_type_is_object, entity_type);

	//return IN_RANGE_INCLUSIVE(entity_type, _simulation_entity_type_unit, _simulation_entity_type_device);
}

void __cdecl simulation_entity_update(long simulation_entity_index, long object_index, c_flags<long, qword, 64>& flags)
{
	INVOKE(0x004AFC20, simulation_entity_update, simulation_entity_index, object_index, flags);

	//c_simulation_world* world = simulation_get_world();
	//if (world->is_distributed() && world->is_authority())
	//{
	//	c_simulation_entity_database* entity_database = world->get_entity_database();
	//	if (entity_database->entity_is_local())
	//		entity_database->entity_update(simulation_entity_index, flags, false);
	//}
}

