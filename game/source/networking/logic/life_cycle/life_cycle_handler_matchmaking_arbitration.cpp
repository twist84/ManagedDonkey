#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_arbitration.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/session/network_session.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00494AF0, c_life_cycle_state_handler_matchmaking_arbitration, ready_to_start);
HOOK_DECLARE_CLASS_MEMBER(0x00494C80, c_life_cycle_state_handler_matchmaking_arbitration, session_membership_matches_arbitration_registration_results);
HOOK_DECLARE_CLASS_MEMBER(0x00494EF0, c_life_cycle_state_handler_matchmaking_arbitration, update);

c_life_cycle_state_handler_matchmaking_arbitration::c_life_cycle_state_handler_matchmaking_arbitration() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_arbitration_start_time(),
	m_arbitration_waiting_for_completion_start_time(),
	m_registration_result()
{
	//DECLFUNC(0x00453F90, void, __thiscall, c_life_cycle_state_handler_matchmaking_arbitration*)(this);
}

const char* c_life_cycle_state_handler_matchmaking_arbitration::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454730, c_life_cycle_state_handler_matchmaking_arbitration, get_state_string);

	return "matchmaking-arbitration";
}

void c_life_cycle_state_handler_matchmaking_arbitration::dispose()
{
	//INVOKE_CLASS_MEMBER(0x004946D0, c_life_cycle_state_handler_matchmaking_arbitration, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_matchmaking_arbitration::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x004946E0, c_life_cycle_state_handler_matchmaking_arbitration, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_arbitration::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x004947E0, c_life_cycle_state_handler_matchmaking_arbitration, exit, to);
}

//.text:00494930 ; 
//.text:00494940 ; 

void c_life_cycle_state_handler_matchmaking_arbitration::get_progress(s_life_cycle_matchmaking_progress* progress_out)
{
	INVOKE_CLASS_MEMBER(0x00494950, c_life_cycle_state_handler_matchmaking_arbitration, get_progress, progress_out);

	//c_life_cycle_state_handler::get_matchmaking_progress_for_setting_up_match(progress_out);
}

void c_life_cycle_state_handler_matchmaking_arbitration::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00494960, c_life_cycle_state_handler_matchmaking_arbitration, handle_missing_required_session_parameter, session_type);

	//ASSERT(session_type == _network_session_type_group);
	//event(_event_warning, "networking:logic:life_cycle:matchmaking_arbitration: became host without required session parameters, restarting matchmaking");
	//c_life_cycle_state_handler::disband_group_session();
}

void c_life_cycle_state_handler_matchmaking_arbitration::initial_stats_write_mark_complete()
{
	//INVOKE_CLASS_MEMBER(0x00494980, c_life_cycle_state_handler_matchmaking_arbitration, initial_stats_write_mark_complete);

	ASSERT(!m_flags.test(_matchmaking_arbitration_start_match_initial_stats_write_completed_bit));
	network_session_interface_set_peer_status_flag(_network_session_peer_properties_status_match_initial_stats_written_bit, true);
	m_flags.set(_matchmaking_arbitration_start_match_initial_stats_write_completed_bit, true);
}

void c_life_cycle_state_handler_matchmaking_arbitration::initial_stats_write_mark_failed()
{
	//INVOKE_CLASS_MEMBER(0x004949A0, c_life_cycle_state_handler_matchmaking_arbitration, initial_stats_write_mark_failed);

	ASSERT(!m_flags.test(_matchmaking_arbitration_start_match_initial_stats_write_failed_bit));
	c_life_cycle_state_handler_matchmaking_arbitration::start_match_mark_complete();
	network_session_interface_set_peer_status_flag(_network_session_peer_properties_status_match_initial_stats_write_failed_bit, true);
	m_flags.set(_matchmaking_arbitration_start_match_initial_stats_write_failed_bit, true);
}

void c_life_cycle_state_handler_matchmaking_arbitration::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x004949D0, c_life_cycle_state_handler_matchmaking_arbitration, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_requires_group_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	parent_flags.set(_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit, true);
	parent_flags.set(_life_cycle_state_handler_live_disconnection_returns_to_pre_game_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_arbitration, &parent_flags, 0x0ULL, 0xAE0004C80ULL);
}

//.text:00494A00 ; 

void c_life_cycle_state_handler_matchmaking_arbitration::mark_arbitration_complete()
{
	//INVOKE_CLASS_MEMBER(0x00494A10, c_life_cycle_state_handler_matchmaking_arbitration, mark_arbitration_complete);

	ASSERT(!m_flags.test(_matchmaking_arbitration_complete_bit));
	ASSERT(!m_flags.test(_matchmaking_arbitration_complete_as_host_bit));

	network_session_interface_set_peer_status_flag(_network_session_peer_properties_status_match_arbitration_succeeded_bit, true);
	m_flags.set(_matchmaking_arbitration_complete_bit, true);

	if (get_manager()->get_group_session()->is_host())
	{
		m_flags.set(_matchmaking_arbitration_complete_as_host_bit, true);
	}
}

void c_life_cycle_state_handler_matchmaking_arbitration::mark_arbitration_failed()
{
	//INVOKE_CLASS_MEMBER(0x00494A40, c_life_cycle_state_handler_matchmaking_arbitration, mark_arbitration_failed);

	ASSERT(!m_flags.test(_matchmaking_arbitration_failed_bit));

	c_life_cycle_state_handler_matchmaking_arbitration::mark_arbitration_complete();
	network_session_interface_set_peer_status_flag(_network_session_peer_properties_status_match_arbitration_failed_bit, true);
	m_flags.set(_matchmaking_arbitration_failed_bit, true);
}

//.text:00494A90 ; 
//.text:00494AA0 ; 
//.text:00494AC0 ; 
//.text:00494AD0 ; 

bool c_life_cycle_state_handler_matchmaking_arbitration::ready_to_start()
{
	//return INVOKE_CLASS_MEMBER(0x00494AF0, c_life_cycle_state_handler_matchmaking_arbitration, ready_to_start);

	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: c_life_cycle_state_handler_matchmaking_arbitration::ready_to_start");

	c_network_session* group_session = get_manager()->get_group_session();
	const c_network_session_membership* session_membership = group_session->get_session_membership();

	bool arbitration_completed = false;
	if (m_flags.test(_matchmaking_arbitration_complete_bit))
	{
		arbitration_completed = !m_flags.test(_matchmaking_arbitration_failed_bit);
	}

	bool initial_participants_transmitted_to_peers = false;
	if (m_flags.test(_matchmaking_arbitration_set_initial_participants_bit))
	{
		c_network_session_parameters* session_parameters = group_session->get_session_parameters();
		initial_participants_transmitted_to_peers = session_parameters->parameters_transmitted_to_peers(FLAG(_network_session_parameter_type_initial_participants)) != 0;
		if (!initial_participants_transmitted_to_peers)
		{
			c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Warning! It seems a player hasn't got initial participants!");
		}
	}

	bool match_started_for_peers = false;
	if (m_flags.test(_matchmaking_arbitration_start_match_session_completed_bit) && !m_flags.test(_matchmaking_arbitration_start_match_session_failed_bit))
	{
		match_started_for_peers = session_membership->peer_property_flag_test(_peer_property_flag_test_all_peers, _network_session_peer_properties_status_match_started_bit);
		if (!match_started_for_peers)
		{
			c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Warning! It seems a peer hasn't finished starting their session!");
		}
	}

	bool stats_written_for_peers = false;
	if (m_flags.test(_matchmaking_arbitration_start_match_initial_stats_write_completed_bit) && !m_flags.test(_matchmaking_arbitration_start_match_initial_stats_write_failed_bit))
	{
		stats_written_for_peers = session_membership->peer_property_flag_test(_peer_property_flag_test_all_peers, _network_session_peer_properties_status_match_initial_stats_written_bit);
	}

	if (m_flags.test(_matchmaking_arbitration_start_match_session_failed_bit))
	{
		c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Warning! Match start has failed!");
	}

	if (m_flags.test(_matchmaking_arbitration_start_match_initial_stats_write_failed_bit))
	{
		c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Warning! Stats write has failed!");
	}

	bool ready = arbitration_completed && initial_participants_transmitted_to_peers && match_started_for_peers && stats_written_for_peers;

	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Arbitration completed? %s", arbitration_completed ? "YES" : "NO");
	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Peers have initial participants? %s", initial_participants_transmitted_to_peers ? "YES" : "NO");
	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Session started? %s", match_started_for_peers ? "YES" : "NO");
	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Stats written? %s", stats_written_for_peers ? "YES" : "NO");
	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Are we ready? %s", ready ? "YES" : "NO");

	return ready;
}

bool c_life_cycle_state_handler_matchmaking_arbitration::ready_to_update_repeated_play()
{
	return INVOKE_CLASS_MEMBER(0x00494BF0, c_life_cycle_state_handler_matchmaking_arbitration, ready_to_update_repeated_play);
}

bool c_life_cycle_state_handler_matchmaking_arbitration::session_membership_matches_arbitration_registration_results()
{
	//return INVOKE_CLASS_MEMBER(0x00494C80, c_life_cycle_state_handler_matchmaking_arbitration, session_membership_matches_arbitration_registration_results);

	return true;
}

//.text:00494E50 ; 

void c_life_cycle_state_handler_matchmaking_arbitration::start_match_mark_complete()
{
	//INVOKE_CLASS_MEMBER(0x00494E80, c_life_cycle_state_handler_matchmaking_arbitration, start_match_mark_complete);

	ASSERT(!m_flags.test(_matchmaking_arbitration_start_match_session_completed_bit));
	network_session_interface_set_peer_status_flag(_network_session_peer_properties_status_match_started_bit, true);
	m_flags.set(_matchmaking_arbitration_start_match_session_completed_bit, true);
}

void c_life_cycle_state_handler_matchmaking_arbitration::start_match_mark_failed()
{
	//INVOKE_CLASS_MEMBER(0x00494EA0, c_life_cycle_state_handler_matchmaking_arbitration, start_match_mark_failed);

	ASSERT(!m_flags.test(_matchmaking_arbitration_start_match_session_failed_bit));
	c_life_cycle_state_handler_matchmaking_arbitration::start_match_mark_complete();
	network_session_interface_set_peer_status_flag(_network_session_peer_properties_status_match_start_failed_bit, true);
	m_flags.set(_matchmaking_arbitration_start_match_session_failed_bit, true);
}

//.text:00494ED0 ; 

void c_life_cycle_state_handler_matchmaking_arbitration::update()
{
	//INVOKE_CLASS_MEMBER(0x00494EF0, c_life_cycle_state_handler_matchmaking_arbitration, update);

	c_network_session* group_session = get_manager()->get_group_session();
	if (group_session->is_host() && !m_flags.test(_matchmaking_arbitration_set_initial_participants_bit))
	{
		// arbitration isn't implemented so we mark it as complete to make the game progress.
		c_life_cycle_state_handler_matchmaking_arbitration::mark_arbitration_complete();

		// setup_initial_participants should be called by this function but isn't, so call it manually.
		c_life_cycle_state_handler::setup_initial_participants(group_session);

		m_flags.set(_matchmaking_arbitration_set_initial_participants_bit, true);
	}

	// $TODO confirm this function gets called without the hook
	//INVOKE_CLASS_MEMBER(0x00494EF0, c_life_cycle_state_handler_matchmaking_arbitration, update);
	HOOK_INVOKE_CLASS_MEMBER(, c_life_cycle_state_handler_matchmaking_arbitration, update);
}

void c_life_cycle_state_handler_matchmaking_arbitration::update_arbitration()
{
	INVOKE_CLASS_MEMBER(0x004952F0, c_life_cycle_state_handler_matchmaking_arbitration, update_arbitration);

	//c_life_cycle_state_manager* manager = get_manager();
	//c_network_session* group_session = manager->get_group_session();
	//int32 managed_session_index = group_session->m_managed_session_index;
	//
	//ASSERT(!m_flags.test(_matchmaking_arbitration_complete_bit));
	//
	//if (!m_flags.test(_matchmaking_arbitration_initiated_bit))
	//{
	//	if (network_arbitration_get_status() == _network_arbitration_status_registration_in_progress)
	//	{
	//		if (!m_flags.test(_matchmaking_arbitration_blocked_bit))
	//		{
	//			event(_event_warning, "networking:logic:life_cycle:arbitration: arbitration blocked, can't initiate");
	//			m_flags.set(_matchmaking_arbitration_blocked_bit, true);
	//		}
	//	}
	//	else
	//	{
	//		c_network_session_parameters* session_parameters = group_session->get_session_parameters();
	//		uns64 arbitration_nonce = *session_parameters->matchmaking_arbitration_nonce.get();
	//		if (m_flags.test(_matchmaking_arbitration_blocked_bit))
	//		{
	//			event(_event_warning, "networking:logic:life_cycle:arbitration: arbitration unblocked, starting...");
	//			m_flags.set(_matchmaking_arbitration_blocked_bit, false);
	//		}
	//
	//		if (!network_arbitration_initiate(managed_session_index, arbitration_nonce))
	//		{
	//			event(_event_error, "networking:logic:life_cycle:arbitration: failed to initiate arbitration for managed session 0x%08X",
	//				managed_session_index);
	//			c_life_cycle_state_handler_matchmaking_arbitration::mark_arbitration_failed();
	//		}
	//
	//		m_flags.set(_matchmaking_arbitration_initiated_bit, true);
	//	}
	//}
	//
	//if (m_flags.test(_matchmaking_arbitration_initiated_bit) && m_flags.test(_matchmaking_arbitration_failed_bit))
	//{
	//	e_network_arbitration_status status = network_arbitration_get_status();
	//	switch (status)
	//	{
	//	case _network_arbitration_status_none:
	//	{
	//		event(_event_error, "networking:logic:life_cycle:arbitration: found arbitration in bad state for managed session 0x%08X",
	//			managed_session_index);
	//		c_life_cycle_state_handler_matchmaking_arbitration::mark_arbitration_failed();
	//	}
	//	break;
	//	//case _network_arbitration_status_registration_in_progress:
	//	//{
	//	//}
	//	//break;
	//	case _network_arbitration_status_registered:
	//	{
	//		if (!network_arbitration_get_results(managed_session_index, &m_registration_result))
	//		{
	//			event(_event_error, "networking:logic:life_cycle:arbitration: failed to get arbitration results for managed session 0x%08X",
	//				managed_session_index);
	//			c_life_cycle_state_handler_matchmaking_arbitration::mark_arbitration_failed();
	//			break;
	//		}
	//
	//		event(_event_message, "networking:logic:life_cycle:arbitration: arbitration completed successfully");
	//		c_life_cycle_state_handler_matchmaking_arbitration::mark_arbitration_complete();
	//	}
	//	break;
	//	case _network_arbitration_status_registration_failed:
	//	{
	//		event(_event_error, "networking:logic:life_cycle:arbitration: arbitration failed for managed session 0x%08X",
	//			managed_session_index);
	//	}
	//	break;
	//	}
	//}
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_arbitration::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x004952F0, c_life_cycle_state_handler_matchmaking_arbitration, update_for_state_transition);
}

void c_life_cycle_state_handler_matchmaking_arbitration::update_initial_stats_write()
{
	INVOKE_CLASS_MEMBER(0x00495550, c_life_cycle_state_handler_matchmaking_arbitration, update_initial_stats_write);
}

void c_life_cycle_state_handler_matchmaking_arbitration::update_progress()
{
	INVOKE_CLASS_MEMBER(0x00495700, c_life_cycle_state_handler_matchmaking_arbitration, update_progress);

	//c_life_cycle_state_manager* manager = get_manager();
	//c_network_session* group_session = manager->get_group_session();
	//c_network_session_parameters* session_parameters = group_session->get_session_parameters();
	//
	//ASSERT(group_session->is_host());
	//
	//s_replicated_life_cycle_matchmaking_progress replicated_progress{};
	//replicated_progress.progress_type = _life_cycle_matchmaking_progress_setting_up_match;
	//session_parameters->matchmaking_progress.set(&replicated_progress);
}

void c_life_cycle_state_handler_matchmaking_arbitration::update_repeated_play()
{
	INVOKE_CLASS_MEMBER(0x00495750, c_life_cycle_state_handler_matchmaking_arbitration, update_repeated_play);
}

void c_life_cycle_state_handler_matchmaking_arbitration::update_start_match()
{
	INVOKE_CLASS_MEMBER(0x00495900, c_life_cycle_state_handler_matchmaking_arbitration, update_start_match);
}

