#include "networking/logic/network_life_cycle.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "game/multiplayer_game_hopper.hpp"
#include "memory/module.hpp"
#include "networking/session/network_session.hpp"
#include "networking/session/network_session_parameters_matchmaking.hpp"
#include "text/unicode.hpp"

REFERENCE_DECLARE(0x019AB7F0, s_network_life_cycle_globals, life_cycle_globals);

// $TODO: remove this when we have full control over `life_cycle_globals`
void __cdecl dynamic_initializer_for_life_cycle_globals()
{
	static s_network_life_cycle_globals temp;
	csmemcpy(&life_cycle_globals, &temp, sizeof(s_network_life_cycle_globals));
}
HOOK_DECLARE(0x01594C20, dynamic_initializer_for_life_cycle_globals);

s_network_life_cycle_globals::s_network_life_cycle_globals() :
	initialized(false),
	life_cycle_state_manager(),
	life_cycle_state_handler_none(),
	life_cycle_state_handler_pre_game(),
	life_cycle_state_handler_start_game(),
	life_cycle_state_handler_in_game(),
	life_cycle_state_handler_end_game_write_stats(),
	life_cycle_state_handler_leaving(),
	life_cycle_state_handler_joining(),
	life_cycle_state_handler_matchmaking_start(),
	life_cycle_state_handler_matchmaking_find_match_client(),
	life_cycle_state_handler_matchmaking_find_match(),
	life_cycle_state_handler_matchmaking_find_and_assemble_match(),
	life_cycle_state_handler_matchmaking_assemble_match(),
	life_cycle_state_handler_matchmaking_select_host(),
	life_cycle_state_handler_matchmaking_arbitration(),
	life_cycle_state_handler_matchmaking_prepare_map(),
	life_cycle_state_handler_in_match(),
	life_cycle_state_handler_end_match_write_stats(),
	life_cycle_state_handler_post_match()
{
	//DECLFUNC(0x00454200, void, __thiscall, s_network_life_cycle_globals*)(this);

	zero_array(pad);
}

//.text:00454830 ; void __cdecl network_life_cycle_abort_matchmaking()
//.text:004548E0 ; bool __cdecl network_life_cycle_can_change_network_files()
//.text:00454930 ; bool __cdecl network_life_cycle_create_local_squad(e_network_session_class)
//.text:00454A10 ; bool __cdecl network_life_cycle_desires_qos_reply_block_for_group()

void __cdecl network_life_cycle_disconnect_sessions()
{
	INVOKE(0x00454A30, network_life_cycle_disconnect_sessions);
}

//.text:00454A70 ; 

void __cdecl network_life_cycle_dispose()
{
	INVOKE(0x00454A80, network_life_cycle_dispose);

	//multiplayer_game_hopper_dispose();
	//logic_qos_reply_manager_dispose();
	//
	//life_cycle_globals.handler_none.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_pre_game.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_start_game.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_in_game.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_end_game_write_stats.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_leaving.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_joining.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_start.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_find_match_client.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_find_match.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_find_and_assemble_match.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_assemble_match.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_select_host.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_arbitration.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_prepare_map.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_in_match.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_end_match_write_stats.dispose(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_post_match.dispose(&life_cycle_globals.life_cycle_state_manager);
	//
	//life_cycle_globals.initialized = false;
}

void __cdecl network_life_cycle_end()
{
	//INVOKE(0x00454B40, network_life_cycle_end);

	event(_event_message, "networking:lifecycle: calling life cycle end");
	
	network_life_cycle_disconnect_sessions();
	life_cycle_globals.life_cycle_state_manager.terminate();
}

//.text:00454B90 ; bool __cdecl network_life_cycle_game_ending()
//.text:00454BB0 ; bool __cdecl network_life_cycle_game_is_in_progress()
//.text:00454BD0 ; c_network_session* __cdecl network_life_cycle_get_group_session_for_join()
//.text:00454BE0 ; bool __cdecl network_life_cycle_get_matchmaking_gather_party_properties(s_matchmaking_gather_party_properties*)

void __cdecl network_life_cycle_get_matchmaking_progress(s_life_cycle_matchmaking_progress* progress_out)
{
	INVOKE(0x00454C20, network_life_cycle_get_matchmaking_progress, progress_out);

	//csmemset(progress_out, 0, sizeof(s_life_cycle_matchmaking_progress));
	//progress_out->progress_type = 0;
	//
	//if (life_cycle_globals.life_cycle_state_manager.current_state_ready_for_state_transition_or_query())
	//{
	//	e_life_cycle_state state = network_life_cycle_get_state();
	//	switch (state)
	//	{
	//	case _life_cycle_state_matchmaking_start:
	//		life_cycle_globals.handler_matchmaking_start.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_find_match_client:
	//		life_cycle_globals.handler_matchmaking_find_match_client.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_find_match:
	//		life_cycle_globals.handler_matchmaking_find_match.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_find_and_assemble_match:
	//		life_cycle_globals.handler_matchmaking_find_and_assemble_match.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_assemble_match:
	//		life_cycle_globals.handler_matchmaking_assemble_match.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_arbitration:
	//		life_cycle_globals.handler_matchmaking_arbitration.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_select_host:
	//		life_cycle_globals.handler_matchmaking_select_host.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_prepare_map:
	//		life_cycle_globals.handler_matchmaking_prepare_map.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_post_match:
	//		life_cycle_globals.handler_post_match.get_progress(progress_out);
	//		break;
	//	}
	//}
}

//.text:00454D30 ; bool __cdecl network_life_cycle_get_matchmaking_search_party_properties(s_matchmaking_search_party_properties*)
//.text:00454D70 ; bool __cdecl network_life_cycle_get_observer(c_network_observer**)
//.text:00454DA0 ; c_network_session* __cdecl network_life_cycle_get_squad_session_for_join()

e_life_cycle_state __cdecl network_life_cycle_get_state()
{
	//return INVOKE(0x00454DB0, network_life_cycle_get_state);

	if (life_cycle_globals.initialized)
		return life_cycle_globals.life_cycle_state_manager.m_current_state;

	return _life_cycle_state_none;
}

//.text:00454DC0 ; char const* __cdecl network_life_cycle_get_state_description()
//.text:00454DE0 ; c_network_session* __cdecl network_life_cycle_get_target_session_for_join()
//.text:00454DF0 ; bool __cdecl network_life_cycle_in_group_session(c_network_session**)
//.text:00454E20 ; bool __cdecl network_life_cycle_in_interactive_session(c_network_session**)
//.text:00454EA0 ; bool __cdecl network_life_cycle_in_session(c_network_session**)

bool __cdecl network_life_cycle_in_squad_session(c_network_session** out_active_squad_session)
{
	//return INVOKE(0x00454F20, network_life_cycle_in_squad_session, out_active_squad_session);

	if (life_cycle_globals.initialized)
	{
		c_network_session* active_squad_session = life_cycle_globals.life_cycle_state_manager.get_active_squad_session();
		if (!active_squad_session->disconnected())
		{
			if (out_active_squad_session)
				*out_active_squad_session = active_squad_session;

			return true;
		}
	}

	return false;
}

bool __cdecl network_life_cycle_in_system_link_advertisable_session(c_network_session** out_active_system_link_advertisable_session)
{
	//return INVOKE(0x00454F50, network_life_cycle_in_system_link_advertisable_session, out_active_system_link_advertisable_session);

	if (life_cycle_globals.initialized)
	{
		c_network_session* active_system_link_advertisable_session = life_cycle_globals.life_cycle_state_manager.get_active_squad_session();
		if (active_system_link_advertisable_session->is_host() &&
			active_system_link_advertisable_session->session_class() == _network_session_class_system_link &&
			active_system_link_advertisable_session->has_managed_session_connection())
		{
			if (out_active_system_link_advertisable_session)
				*out_active_system_link_advertisable_session = active_system_link_advertisable_session;

			return true;
		}
	}

	return false;
}

//.text:00454FA0 ; bool __cdecl network_life_cycle_in_target_session(c_network_session**)

bool __cdecl network_life_cycle_initialize(c_network_observer* observer, c_network_session_manager* session_manager, c_network_session* squad_session_one, c_network_session* squad_session_two, c_network_session* group_session)
{
	return INVOKE(0x00454FD0, network_life_cycle_initialize, observer, session_manager, squad_session_one, squad_session_two, group_session);

	//life_cycle_globals.life_cycle_state_manager.initialize(observer, session_manager, squad_session_one, squad_session_two, group_session);
	//
	//life_cycle_globals.handler_none.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_pre_game.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_start_game.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_in_game.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_end_game_write_stats.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_leaving.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_joining.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_start.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_find_match_client.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_find_match.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_find_and_assemble_match.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_assemble_match.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_select_host.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_arbitration.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_matchmaking_prepare_map.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_in_match.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_end_match_write_stats.initialize(&life_cycle_globals.life_cycle_state_manager);
	//life_cycle_globals.handler_post_match.initialize(&life_cycle_globals.life_cycle_state_manager);
	//
	//multiplayer_game_hopper_initialize();
	//logic_qos_reply_manager_initialize();
	//
	//life_cycle_globals.initialized = true;
	//return true;
}

//.text:00455110 ; bool __cdecl network_life_cycle_map_load_pending()
//.text:00455160 ; void __cdecl network_life_cycle_notify_core_load()
//.text:00455180 ; void __cdecl network_life_cycle_notify_lost_connection(
//.text:00455190 ; void __cdecl network_life_cycle_notify_out_of_sync(bool)
//.text:004551C0 ; void __cdecl network_life_cycle_notify_qos_reply_block_set_for_group()
//.text:004551F0 ; void __cdecl network_life_cycle_notify_session_disbandment_and_host_assumption(c_network_session const*)
//.text:00455210 ; 
//.text:00455220 ; bool __cdecl network_life_cycle_post_match_rematch_join_in_progress()
//.text:00455250 ; void __cdecl network_life_cycle_prepare_for_squad_join()

void __cdecl network_life_cycle_request_leave(bool disconnect)
{
	//return INVOKE(0x00455260, network_life_cycle_request_leave, disconnect);

	life_cycle_globals.life_cycle_state_manager.request_leave_sessions(disconnect);
}

bool __cdecl network_life_cycle_set_pre_game_state()
{
	//return INVOKE(0x00455280, network_life_cycle_set_pre_game_state);

	ASSERT(life_cycle_globals.initialized);

	if (life_cycle_globals.life_cycle_state_manager.get_current_state())
	{
		if (life_cycle_globals.life_cycle_state_manager.get_current_state() == _life_cycle_state_pre_game)
			return true;

		c_network_session* active_squad_session = life_cycle_globals.life_cycle_state_manager.get_active_squad_session();
		if (active_squad_session->is_leader())
		{
			c_network_session_parameters* session_parameters = active_squad_session->get_session_parameters();
			if (session_parameters->session_mode.request_change(_network_session_mode_idle))
				return true;
		}
	}

	return false;
}

//.text:004552D0 ; bool __cdecl network_life_cycle_squad_is_offline()
//.text:004552F0 ; bool __cdecl network_life_cycle_squad_local_peer_is_host()
//.text:00455320 ; bool __cdecl network_life_cycle_squad_local_peer_is_leader()
//.text:00455350 ; void __cdecl network_life_cycle_swap_squad_sessions()
//.text:00455370 ; void __cdecl network_life_cycle_swap_target_and_group_sessions()

void __cdecl logic_qos_reply_manager_update()
{
	INVOKE(0x0049B290, logic_qos_reply_manager_update);
}

void __cdecl network_life_cycle_update()
{
	//INVOKE(0x00455390, network_life_cycle_update);

	if (life_cycle_globals.initialized)
	{
		multiplayer_game_hopper_update();
		life_cycle_globals.life_cycle_state_manager.update();
		logic_qos_reply_manager_update();
	}
}

