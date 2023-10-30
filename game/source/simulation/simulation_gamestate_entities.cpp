#include "simulation/simulation_gamestate_entities.hpp"

#include "main/main_game.hpp"
#include "memory/thread_local.hpp"

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

