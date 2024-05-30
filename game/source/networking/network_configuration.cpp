#include "networking/network_configuration.hpp"

REFERENCE_DECLARE(0x019A62B8, bool, g_network_configuration_initialized);
REFERENCE_DECLARE(0x019A62C0, s_network_configuration, g_network_configuration);

//.text:0043C890 ; 

bool __cdecl network_configuration_custom_games_enabled()
{
	return INVOKE(0x0043C8A0, network_configuration_custom_games_enabled);
}

bool __cdecl network_configuration_disable_bad_bandwidth_anticheating()
{
	return INVOKE(0x0043C8B0, network_configuration_disable_bad_bandwidth_anticheating);
}

bool __cdecl network_configuration_disable_bad_client_anticheating()
{
	return INVOKE(0x0043C8C0, network_configuration_disable_bad_client_anticheating);
}

bool __cdecl network_configuration_disable_bad_connectivity_anticheating()
{
	return INVOKE(0x0043C8D0, network_configuration_disable_bad_connectivity_anticheating);
}

void __cdecl network_configuration_dispose()
{
	INVOKE(0x0043C8E0, network_configuration_dispose);

	//g_network_configuration_initialized = false;
	//csmemset(&g_network_configuration, 0, sizeof(g_network_configuration));
}

//long __cdecl network_configuration_get_map_index_from_map_id(e_map_id)
long __cdecl network_configuration_get_map_index_from_map_id(long map_id)
{
	return INVOKE(0x0043C900, network_configuration_get_map_index_from_map_id, map_id);
}

//.text:0043C930 ; 

//e_network_configuration_state __cdecl network_configuration_get_state()
long __cdecl network_configuration_get_state()
{
	return INVOKE(0x0043C940, network_configuration_get_state);
}

void __cdecl network_configuration_initialize(bool a1)
{
	INVOKE(0x0043C960, network_configuration_initialize, a1);

	//network_configuration_initialize_private();
	//g_network_configuration_download_buffer.__unknown684 = a1 ? 0 : 2;
	//g_network_configuration_initialized = true;
}

void __cdecl network_configuration_initialize_observer_constants()
{
	INVOKE(0x0043C990, network_configuration_initialize_observer_constants);
}

void __cdecl network_configuration_initialize_private()
{
	INVOKE(0x0043D2B0, network_configuration_initialize_private);
}

void __cdecl network_configuration_initialize_simulation_constants()
{
	INVOKE(0x0043E2C0, network_configuration_initialize_simulation_constants);
}

void __cdecl network_configuration_initialize_simulation_entity_constants()
{
	INVOKE(0x0043E4D0, network_configuration_initialize_simulation_entity_constants);
}

void __cdecl network_configuration_initialize_simulation_entity_creation_constants()
{
	INVOKE(0x0043E4E0, network_configuration_initialize_simulation_entity_creation_constants);
}

void __cdecl network_configuration_initialize_simulation_entity_update_constants()
{
	INVOKE(0x0043E6A0, network_configuration_initialize_simulation_entity_update_constants);
}

void __cdecl network_configuration_initialize_simulation_event_constants()
{
	INVOKE(0x0043F040, network_configuration_initialize_simulation_event_constants);
}

//.text:0043F4D0 ; 

long __cdecl network_configuration_maximum_multiplayer_split_screen()
{
	return INVOKE(0x0043F4E0, network_configuration_maximum_multiplayer_split_screen);
}

void __cdecl network_configuration_update()
{
	INVOKE(0x0043F4F0, network_configuration_update);
}

