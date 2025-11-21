#include "networking/logic/network_life_cycle.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "game/multiplayer_game_hopper.hpp"
#include "memory/module.hpp"
#include "networking/logic/logic_qos_reply_manager.hpp"
#include "networking/session/network_session.hpp"
#include "networking/session/network_session_parameters_matchmaking.hpp"
#include "text/unicode.hpp"

//REFERENCE_DECLARE_ARRAY(0x01860878, const char*, k_life_cycle_state_descriptions, k_life_cycle_state_count);
REFERENCE_DECLARE(0x019AB7F0, s_network_life_cycle_globals, life_cycle_globals);

// $TODO remove this when we have full control over `life_cycle_globals`
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

bool __cdecl network_life_cycle_can_change_network_files()
{
	//return INVOKE(0x004548E0, network_life_cycle_can_change_network_files);

	bool result = false;
	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_none:
	case _life_cycle_state_pre_game:
	case _life_cycle_state_in_game:
	case _life_cycle_state_end_game_write_stats:
	case _life_cycle_state_in_match:
	case _life_cycle_state_post_match:
	{
		result = true;
	}
	break;
	}
	return result;
}

//.text:00454930 ; bool __cdecl network_life_cycle_create_local_squad(e_network_session_class)

bool __cdecl network_life_cycle_desires_qos_reply_block_for_group()
{
	//return INVOKE(0x00454A10, network_life_cycle_desires_qos_reply_block_for_group);

	bool result = false;
	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_matchmaking_find_and_assemble_match:
	case _life_cycle_state_matchmaking_assemble_match:
	{
		result = true;
	}
	break;
	}
	return result;
}

void __cdecl network_life_cycle_disconnect_sessions()
{
	//INVOKE(0x00454A30, network_life_cycle_disconnect_sessions);

	event(_event_message, "networking:lifecycle: FORCE-DISCONNECTING SQUAD");
	life_cycle_globals.life_cycle_state_manager.get_active_squad_session()->force_disconnect();

	event(_event_message, "networking:lifecycle: FORCE-DISCONNECTING GROUP");
	life_cycle_globals.life_cycle_state_manager.get_group_session()->force_disconnect();
}

void __cdecl network_life_cycle_display_state_strings()
{
	//INVOKE(0x00454A70, network_life_cycle_display_state_strings);

	life_cycle_globals.life_cycle_state_manager.display_state_strings();
}

void __cdecl network_life_cycle_dispose()
{
	//INVOKE(0x00454A80, network_life_cycle_dispose);

	multiplayer_game_hopper_dispose();
	logic_qos_reply_manager_dispose();
	
	life_cycle_globals.life_cycle_state_handler_none.dispose();
	life_cycle_globals.life_cycle_state_handler_pre_game.dispose();
	life_cycle_globals.life_cycle_state_handler_start_game.dispose();
	life_cycle_globals.life_cycle_state_handler_in_game.dispose();
	life_cycle_globals.life_cycle_state_handler_end_game_write_stats.dispose();
	life_cycle_globals.life_cycle_state_handler_leaving.dispose();
	life_cycle_globals.life_cycle_state_handler_joining.dispose();
	life_cycle_globals.life_cycle_state_handler_matchmaking_start.dispose();
	life_cycle_globals.life_cycle_state_handler_matchmaking_find_match_client.dispose();
	life_cycle_globals.life_cycle_state_handler_matchmaking_find_match.dispose();
	life_cycle_globals.life_cycle_state_handler_matchmaking_find_and_assemble_match.dispose();
	life_cycle_globals.life_cycle_state_handler_matchmaking_assemble_match.dispose();
	life_cycle_globals.life_cycle_state_handler_matchmaking_select_host.dispose();
	life_cycle_globals.life_cycle_state_handler_matchmaking_arbitration.dispose();
	life_cycle_globals.life_cycle_state_handler_matchmaking_prepare_map.dispose();
	life_cycle_globals.life_cycle_state_handler_in_match.dispose();
	life_cycle_globals.life_cycle_state_handler_end_match_write_stats.dispose();
	life_cycle_globals.life_cycle_state_handler_post_match.dispose();
	
	life_cycle_globals.initialized = false;
}

void __cdecl network_life_cycle_end()
{
	//INVOKE(0x00454B40, network_life_cycle_end);

	event(_event_message, "networking:lifecycle: calling life cycle end");
	
	network_life_cycle_disconnect_sessions();
	life_cycle_globals.life_cycle_state_manager.terminate();
}

bool __cdecl network_life_cycle_game_ending()
{
	//return INVOKE(0x00454B90, network_life_cycle_game_ending);

	bool result = false;
	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_end_game_write_stats:
	case _life_cycle_state_end_match_write_stats:
	{
		result = true;
	}
	break;
	}
	return result;
}

bool __cdecl network_life_cycle_game_is_in_progress()
{
	//return INVOKE(0x00454BB0, network_life_cycle_game_is_in_progress);

	bool result = false;
	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_in_game:
	case _life_cycle_state_in_match:
	{
		result = true;
	}
	break;
	}
	return result;
}

c_network_session* __cdecl network_life_cycle_get_group_session_for_join()
{
	//return INVOKE(0x00454BD0, network_life_cycle_get_group_session_for_join);

	if (!life_cycle_globals.initialized)
	{
		return NULL;
	}

	return life_cycle_globals.life_cycle_state_manager.get_group_session();
}

bool __cdecl network_life_cycle_get_matchmaking_gather_party_properties(s_matchmaking_gather_party_properties* gather_party_properties_out)
{
	//return INVOKE(0x00454BE0, network_life_cycle_get_matchmaking_gather_party_properties, gather_party_properties_out);

	bool result = false;
	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_matchmaking_find_and_assemble_match:
	{
		result = life_cycle_globals.life_cycle_state_handler_matchmaking_find_and_assemble_match.get_gather_party_properties(gather_party_properties_out);
	}
	break;
	case _life_cycle_state_matchmaking_assemble_match:
	{
		result = life_cycle_globals.life_cycle_state_handler_matchmaking_assemble_match.get_gather_party_properties(gather_party_properties_out);
	}
	break;
	}
	return result;
}

void __cdecl network_life_cycle_get_matchmaking_progress(s_life_cycle_matchmaking_progress* progress_out)
{
	//INVOKE(0x00454C20, network_life_cycle_get_matchmaking_progress, progress_out);

	csmemset(progress_out, 0, sizeof(s_life_cycle_matchmaking_progress));
	progress_out->progress_type = _life_cycle_matchmaking_progress_none;

	if (!life_cycle_globals.life_cycle_state_manager.current_state_ready_for_state_transition_or_query())
	{
		return;
	}

	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_matchmaking_start:
	{
		life_cycle_globals.life_cycle_state_handler_matchmaking_start.get_progress(progress_out);
	}
	break;
	case _life_cycle_state_matchmaking_find_match_client:
	{
		life_cycle_globals.life_cycle_state_handler_matchmaking_find_match_client.get_progress(progress_out);
	}
	break;
	case _life_cycle_state_matchmaking_find_match:
	{
		life_cycle_globals.life_cycle_state_handler_matchmaking_find_match.get_progress(progress_out);
	}
	break;
	case _life_cycle_state_matchmaking_find_and_assemble_match:
	{
		life_cycle_globals.life_cycle_state_handler_matchmaking_find_and_assemble_match.get_progress(progress_out);
	}
	break;
	case _life_cycle_state_matchmaking_assemble_match:
	{
		life_cycle_globals.life_cycle_state_handler_matchmaking_assemble_match.get_progress(progress_out);
	}
	break;
	case _life_cycle_state_matchmaking_arbitration:
	{
		life_cycle_globals.life_cycle_state_handler_matchmaking_arbitration.get_progress(progress_out);
	}
	break;
	case _life_cycle_state_matchmaking_select_host:
	{
		life_cycle_globals.life_cycle_state_handler_matchmaking_select_host.get_progress(progress_out);
	}
	break;
	case _life_cycle_state_matchmaking_prepare_map:
	{
		life_cycle_globals.life_cycle_state_handler_matchmaking_prepare_map.get_progress(progress_out);
	}
	break;
	case _life_cycle_state_post_match:
	{
		life_cycle_globals.life_cycle_state_handler_post_match.get_progress(progress_out);
	}
	break;
	}
}

bool __cdecl network_life_cycle_get_matchmaking_search_party_properties(s_matchmaking_search_party_properties* search_party_properties_out)
{
	//return INVOKE(0x00454D30, network_life_cycle_get_matchmaking_search_party_properties, search_party_properties_out);

	bool result = false;
	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_matchmaking_find_match:
	{
		result = life_cycle_globals.life_cycle_state_handler_matchmaking_find_match.get_search_party_properties(search_party_properties_out);
	}
	break;
	case _life_cycle_state_matchmaking_find_and_assemble_match:
	{
		result = life_cycle_globals.life_cycle_state_handler_matchmaking_find_and_assemble_match.get_search_party_properties(search_party_properties_out);
	}
	break;
	}
	return result;
}

bool __cdecl network_life_cycle_get_observer(c_network_observer** observer)
{
	//return INVOKE(0x00454D70, network_life_cycle_get_observer, observer);

	ASSERT(observer);
	*observer = NULL;

	if (!life_cycle_globals.initialized)
	{
		return false;
	}

	*observer = life_cycle_globals.life_cycle_state_manager.get_observer();
	return true;
}

c_network_session* __cdecl network_life_cycle_get_squad_session_for_join()
{
	//return INVOKE(0x00454DA0, network_life_cycle_get_squad_session_for_join);

	if (!life_cycle_globals.initialized)
	{
		return NULL;
	}

	return life_cycle_globals.life_cycle_state_manager.get_active_squad_session();
}

e_life_cycle_state __cdecl network_life_cycle_get_state()
{
	//return INVOKE(0x00454DB0, network_life_cycle_get_state);

	if (!life_cycle_globals.initialized)
	{
		return _life_cycle_state_none;
	}

	return life_cycle_globals.life_cycle_state_manager.get_current_state();
}

const char* __cdecl network_life_cycle_get_state_description()
{
	//return INVOKE(0x00454DC0, network_life_cycle_get_state_description);

	return k_life_cycle_state_descriptions[network_life_cycle_get_state()];
}

c_network_session* __cdecl network_life_cycle_get_target_session_for_join()
{
	//return INVOKE(0x00454DE0, network_life_cycle_get_target_session_for_join);

	if (!life_cycle_globals.initialized)
	{
		return NULL;
	}

	return life_cycle_globals.life_cycle_state_manager.get_target_session();
}

bool __cdecl network_life_cycle_in_group_session(c_network_session** session)
{
	//return INVOKE(0x00454DF0, network_life_cycle_in_group_session, session);

	if (!life_cycle_globals.initialized)
	{
		return false;
	}
	
	c_network_session* group_session = life_cycle_globals.life_cycle_state_manager.get_group_session();
	if (group_session->disconnected())
	{
		return false;
	}
	
	if (session)
	{
		*session = group_session;
	}
	
	return true;
}

bool __cdecl network_life_cycle_in_interactive_session(c_network_session** session)
{
	//return INVOKE(0x00454E20, network_life_cycle_in_interactive_session, session);

	bool result = false;
	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_pre_game:
	case _life_cycle_state_start_game:
	case _life_cycle_state_in_game:
	case _life_cycle_state_end_game_write_stats:
	case _life_cycle_state_matchmaking_start:
	case _life_cycle_state_matchmaking_find_match_client:
	case _life_cycle_state_matchmaking_find_match:
	case _life_cycle_state_matchmaking_find_and_assemble_match:
	case _life_cycle_state_matchmaking_assemble_match:
	case _life_cycle_state_matchmaking_arbitration:
	case _life_cycle_state_matchmaking_select_host:
	{
		result = network_life_cycle_in_squad_session(session);
	}
	break;
	case _life_cycle_state_matchmaking_prepare_map:
	case _life_cycle_state_in_match:
	case _life_cycle_state_end_match_write_stats:
	case _life_cycle_state_post_match:
	{
		result = network_life_cycle_in_group_session(session);
	}
	break;
	}
	return result;
}

bool __cdecl network_life_cycle_in_session(c_network_session** session)
{
	//return INVOKE(0x00454EA0, network_life_cycle_in_session, session);

	bool result = false;
	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_pre_game:
	case _life_cycle_state_start_game:
	case _life_cycle_state_in_game:
	case _life_cycle_state_end_game_write_stats:
	case _life_cycle_state_leaving:
	case _life_cycle_state_joining:
	case _life_cycle_state_matchmaking_start:
	case _life_cycle_state_matchmaking_find_match_client:
	case _life_cycle_state_matchmaking_find_match:
	case _life_cycle_state_matchmaking_find_and_assemble_match:
	case _life_cycle_state_matchmaking_assemble_match:
	{
		result = network_life_cycle_in_squad_session(session);
	}
	break;
	case _life_cycle_state_matchmaking_arbitration:
	case _life_cycle_state_matchmaking_select_host:
	case _life_cycle_state_matchmaking_prepare_map:
	case _life_cycle_state_in_match:
	case _life_cycle_state_end_match_write_stats:
	case _life_cycle_state_post_match:
	{
		result = network_life_cycle_in_group_session(session);
	}
	break;
	}
	return result;
}

bool __cdecl network_life_cycle_in_squad_session(c_network_session** session)
{
	//return INVOKE(0x00454F20, network_life_cycle_in_squad_session, session);

	if (!life_cycle_globals.initialized)
	{
		return false;
	}

	c_network_session* active_squad_session = life_cycle_globals.life_cycle_state_manager.get_active_squad_session();
	if (active_squad_session->disconnected())
	{
		return false;
	}

	if (session)
	{
		*session = active_squad_session;
	}

	return true;
}

bool __cdecl network_life_cycle_in_system_link_advertisable_session(c_network_session** session)
{
	//return INVOKE(0x00454F50, network_life_cycle_in_system_link_advertisable_session, session);

	if (!life_cycle_globals.initialized)
	{
		return false;
	}

	c_network_session* active_squad_session = life_cycle_globals.life_cycle_state_manager.get_active_squad_session();
	if (!active_squad_session->is_host())
	{
		return false;
	}

	if (active_squad_session->session_class() != _network_session_class_system_link ||
		!active_squad_session->has_managed_session_connection())
	{
		return false;
	}

	if (session)
	{
		*session = active_squad_session;
	}

	return true;
}

bool __cdecl network_life_cycle_in_target_session(c_network_session** session)
{
	//return INVOKE(0x00454FA0, network_life_cycle_in_target_session, session);

	if (!life_cycle_globals.initialized)
	{
		return false;
	}

	c_network_session* target_session = life_cycle_globals.life_cycle_state_manager.get_target_session();
	if (target_session->disconnected())
	{
		return false;
	}

	if (session)
	{
		*session = target_session;
	}

	return true;
}

bool __cdecl network_life_cycle_initialize(c_network_observer* observer, c_network_session_manager* session_manager, c_network_session* squad_session_one, c_network_session* squad_session_two, c_network_session* group_session)
{
	//return INVOKE(0x00454FD0, network_life_cycle_initialize, observer, session_manager, squad_session_one, squad_session_two, group_session);

	life_cycle_globals.life_cycle_state_manager.initialize(observer, session_manager, squad_session_one, squad_session_two, group_session);
	
	life_cycle_globals.life_cycle_state_handler_none.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_pre_game.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_start_game.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_in_game.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_end_game_write_stats.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_leaving.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_joining.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_matchmaking_start.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_matchmaking_find_match_client.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_matchmaking_find_match.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_matchmaking_find_and_assemble_match.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_matchmaking_assemble_match.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_matchmaking_select_host.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_matchmaking_arbitration.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_matchmaking_prepare_map.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_in_match.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_end_match_write_stats.initialize(&life_cycle_globals.life_cycle_state_manager);
	life_cycle_globals.life_cycle_state_handler_post_match.initialize(&life_cycle_globals.life_cycle_state_manager);
	
	multiplayer_game_hopper_initialize();
	logic_qos_reply_manager_initialize();
	
	life_cycle_globals.initialized = true;
	return true;
}

bool __cdecl network_life_cycle_map_load_pending()
{
	//return INVOKE(0x00455110, network_life_cycle_map_load_pending);

	bool result = false;
	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_in_game:
	{
		result = ((c_life_cycle_state_handler_in_game*)life_cycle_globals.life_cycle_state_manager.get_current_state_handler())->is_map_load_pending();
		//result = life_cycle_globals.life_cycle_state_handler_in_game.is_map_load_pending();
	}
	break;
	case _life_cycle_state_in_match:
	{
		result = ((c_life_cycle_state_handler_in_match*)life_cycle_globals.life_cycle_state_manager.get_current_state_handler())->is_map_load_pending();
		//result = life_cycle_globals.life_cycle_state_handler_in_match.is_map_load_pending();
	}
	break;
	}
	return result;
}

void __cdecl network_life_cycle_notify_core_load()
{
	//INVOKE(0x00455160, network_life_cycle_notify_core_load);

	if (network_life_cycle_get_state() != _life_cycle_state_in_game)
	{
		return;
	}

	life_cycle_globals.life_cycle_state_handler_in_game.notify_core_load();
}

void __cdecl network_life_cycle_notify_lost_connection()
{
	//INVOKE(0x00455180, network_life_cycle_notify_lost_connection);
}

void __cdecl network_life_cycle_notify_out_of_sync(bool determinism_failure)
{
	INVOKE(0x00455190, network_life_cycle_notify_out_of_sync, determinism_failure);

	//c_network_session_parameters* session_parameters = life_cycle_globals.life_cycle_state_manager.get_active_squad_session()->get_session_parameters();
	//s_network_session_parameter_synchronous_out_of_sync out_of_sync
	//{
	//	.out_of_sync = true,
	//	.determinism_failure = determinism_failure
	//};
	//session_parameters->synchronous_out_of_sync.set(&out_of_sync);
}

void __cdecl network_life_cycle_notify_qos_reply_block_set_for_group()
{
	//return INVOKE(0x004551C0, network_life_cycle_notify_qos_reply_block_set_for_group);

	if (!life_cycle_globals.initialized)
	{
		return;
	}

	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_matchmaking_find_and_assemble_match:
	{
		life_cycle_globals.life_cycle_state_handler_matchmaking_find_and_assemble_match.notify_qos_reply_block_set();
	}
	break;
	case _life_cycle_state_matchmaking_assemble_match:
	{
		life_cycle_globals.life_cycle_state_handler_matchmaking_assemble_match.notify_qos_reply_block_set();
	}
	break;
	}
}

void __cdecl network_life_cycle_notify_session_disbandment_and_host_assumption(const c_network_session* session)
{
	//INVOKE(0x004551F0, network_life_cycle_notify_session_disbandment_and_host_assumption, session);

	life_cycle_globals.life_cycle_state_manager.notify_session_disbandment_and_host_assumption(session);
}

void __cdecl network_life_cycle_pause(const char* state_string, bool enabled)
{
	//INVOKE(0x00455210, network_life_cycle_pause, state_string, enabled);

	life_cycle_globals.life_cycle_state_manager.set_pause_state(state_string, enabled);
}

bool __cdecl network_life_cycle_post_match_rematch_join_in_progress()
{
	//return INVOKE(0x00455220, network_life_cycle_post_match_rematch_join_in_progress);

	if (!life_cycle_globals.initialized)
	{
		return false;
	}

	bool result = false;
	switch (network_life_cycle_get_state())
	{
	case _life_cycle_state_joining:
	{
		result = life_cycle_globals.life_cycle_state_handler_joining.rematch_join_in_progress();
	}
	break;
	case _life_cycle_state_post_match:
	{
		result = life_cycle_globals.life_cycle_state_handler_post_match.rematch_join_in_progress();
	}
	break;
	}
	return result;
}

void __cdecl network_life_cycle_prepare_for_squad_join()
{
	//INVOKE(0x00455250, network_life_cycle_prepare_for_squad_join);

	life_cycle_globals.life_cycle_state_manager.notify_expect_squad_join();
}

void __cdecl network_life_cycle_request_leave(bool leave_and_disconnect)
{
	//return INVOKE(0x00455260, network_life_cycle_request_leave, leave_and_disconnect);

	life_cycle_globals.life_cycle_state_manager.request_leave_sessions(leave_and_disconnect);
}

bool __cdecl network_life_cycle_set_pre_game_state()
{
	//return INVOKE(0x00455280, network_life_cycle_set_pre_game_state);

	ASSERT(life_cycle_globals.initialized);

	if (life_cycle_globals.life_cycle_state_manager.get_current_state() == _life_cycle_state_none)
	{
		return false;
	}

	if (life_cycle_globals.life_cycle_state_manager.get_current_state() == _life_cycle_state_pre_game)
	{
		return true;
	}

	c_network_session* active_squad_session = life_cycle_globals.life_cycle_state_manager.get_active_squad_session();
	if (!active_squad_session->is_leader())
	{
		return false;
	}

	c_network_session_parameters* session_parameters = active_squad_session->get_session_parameters();
	if (!session_parameters->m_parameters.session_mode.request_change(_network_session_mode_idle))
	{
		return false;
	}

	return true;
}

bool __cdecl network_life_cycle_squad_is_offline()
{
	//return INVOKE(0x004552D0, network_life_cycle_squad_is_offline);

	ASSERT(life_cycle_globals.initialized);

	c_network_session* squad_session = life_cycle_globals.life_cycle_state_manager.get_active_squad_session();
	return squad_session->established() && squad_session->session_class() == _network_session_class_offline;
}

bool __cdecl network_life_cycle_squad_local_peer_is_host()
{
	//return INVOKE(0x004552F0, network_life_cycle_squad_local_peer_is_host);

	c_network_session* squad_session = NULL;
	if (!network_life_cycle_in_squad_session(&squad_session))
	{
		return false;
	}

	ASSERT(squad_session);
	return squad_session->is_host();
}

bool __cdecl network_life_cycle_squad_local_peer_is_leader()
{
	//return INVOKE(0x00455320, network_life_cycle_squad_local_peer_is_leader);

	c_network_session* squad_session = NULL;
	if (!network_life_cycle_in_squad_session(&squad_session))
	{
		return false;
	}

	ASSERT(squad_session);
	return squad_session->is_leader();
}

void __cdecl network_life_cycle_swap_squad_sessions()
{
	//INVOKE(0x00455350, network_life_cycle_swap_squad_sessions);

	if (!life_cycle_globals.initialized)
	{
		return;
	}

	life_cycle_globals.life_cycle_state_manager.swap_squad_sessions();
}

void __cdecl network_life_cycle_swap_target_and_group_sessions()
{
	//INVOKE(0x00455370, network_life_cycle_swap_target_and_group_sessions);

	if (!life_cycle_globals.initialized)
	{
		return;
	}

	life_cycle_globals.life_cycle_state_manager.swap_target_and_group_sessions();
}

void __cdecl network_life_cycle_update()
{
	//INVOKE(0x00455390, network_life_cycle_update);

	if (!life_cycle_globals.initialized)
	{
		return;
	}

	multiplayer_game_hopper_update();
	life_cycle_globals.life_cycle_state_manager.update();
	logic_qos_reply_manager_update();
}

const char* k_life_cycle_state_descriptions[k_life_cycle_state_count] // 01860878
{
	"none",
	"pre-game",
	"start-game",
	"in-game",
	"end-game-write-stats",
	"leaving",
	"joining",
	"matchmaking-start",
	"matchmaking-find-match-client",
	"matchmaking-find-match",
	"matchmaking-find-and-assemble-match",
	"matchmaking-assemble-match",
	"matchmaking-arbitration",
	"matchmaking-select-host",
	"matchmaking-prepare-map",
	"in-match",
	"end-match-write-stats",
	"post-match",
};

