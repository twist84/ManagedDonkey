#include "networking/logic/network_life_cycle.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x019AB7F0, s_network_life_cycle_globals, life_cycle_globals);

void __cdecl network_life_cycle_dispose()
{
	INVOKE(0x00454A80, network_life_cycle_dispose);

	//multiplayer_game_hopper_dispose();
	//logic_qos_reply_manager_dispose();
	//life_cycle_globals.handler_none.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_pre_game.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_start_game.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_in_game.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_end_game_write_stats.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_leaving.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_joining.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_start.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_match_client.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_match.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_and_assemble_match.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_assemble_match.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_select_host.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_arbitration.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_prepare_map.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_in_match.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_end_match_write_stats.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_post_match.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.initialized = false;
}

void __cdecl network_life_cycle_end()
{
	INVOKE(0x00454B40, network_life_cycle_end);
}

void __cdecl network_life_cycle_get_matchmaking_progress(s_life_cycle_matchmaking_progress* progress_out)
{
	INVOKE(0x00454C20, network_life_cycle_get_matchmaking_progress, progress_out);
}

e_life_cycle_state __cdecl network_life_cycle_get_state()
{
	//return INVOKE(0x00454DB0, network_life_cycle_get_state);

	if (life_cycle_globals.initialized)
		return life_cycle_globals.manager.m_current_state;
	return _life_cycle_state_none;
}

bool __cdecl network_life_cycle_in_squad_session(c_network_session** out_active_squad_session)
{
	return INVOKE(0x00454F20, network_life_cycle_in_squad_session, out_active_squad_session);
}

bool __cdecl network_life_cycle_in_system_link_advertisable_session(c_network_session** out_active_system_link_advertisable_session)
{
	return INVOKE(0x00454F50, network_life_cycle_in_system_link_advertisable_session, out_active_system_link_advertisable_session);
}

bool __cdecl network_life_cycle_initialize(c_network_observer* observer, c_network_session_manager* session_manager, c_network_session* squad_session_one, c_network_session* squad_session_two, c_network_session* group_session)
{
	return INVOKE(0x00454FD0, network_life_cycle_initialize, observer, session_manager, squad_session_one, squad_session_two, group_session);

	//life_cycle_globals.manager.initialize(observer, session_manager, squad_session_one, squad_session_two, group_session);
	//life_cycle_globals.handler_none.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_pre_game.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_start_game.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_in_game.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_end_game_write_stats.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_leaving.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_joining.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_start.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_match_client.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_match.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_and_assemble_match.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_assemble_match.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_select_host.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_arbitration.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_prepare_map.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_in_match.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_end_match_write_stats.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_post_match.initialize(&life_cycle_globals.manager);
	//multiplayer_game_hopper_initialize();
	//logic_qos_reply_manager_initialize();
	//life_cycle_globals.initialized = true;
	//return true;
}

void __cdecl network_life_cycle_request_leave(bool disconnect)
{
	//return INVOKE(0x00455260, network_life_cycle_request_leave, disconnect);

	life_cycle_globals.manager.request_leave_sessions(disconnect);
}

bool __cdecl network_life_cycle_set_pre_game_state()
{
	return INVOKE(0x00455280, network_life_cycle_set_pre_game_state);
}

void __cdecl network_life_cycle_update()
{
	INVOKE(0x00455390, network_life_cycle_update);

	//if (life_cycle_globals.initialized)
	//{
	//	multiplayer_game_hopper_update();
	//	life_cycle_globals.manager.update();
	//	logic_qos_reply_manager_update();
	//}
}

