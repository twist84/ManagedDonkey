#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_arbitration.hpp"

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

char const* c_life_cycle_state_handler_matchmaking_arbitration::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454730, c_life_cycle_state_handler_matchmaking_arbitration, get_state_string);

	return "matchmaking-arbitration";
}

void c_life_cycle_state_handler_matchmaking_arbitration::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x004946E0, c_life_cycle_state_handler_matchmaking_arbitration, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_arbitration::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x004947E0, c_life_cycle_state_handler_matchmaking_arbitration, exit, to);
}

void c_life_cycle_state_handler_matchmaking_arbitration::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00494960, c_life_cycle_state_handler_matchmaking_arbitration, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_matchmaking_arbitration::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x004949D0, c_life_cycle_state_handler_matchmaking_arbitration, initialize, manager);
}

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

bool c_life_cycle_state_handler_matchmaking_arbitration::ready_to_start()
{
	return INVOKE_CLASS_MEMBER(0x00494AF0, c_life_cycle_state_handler_matchmaking_arbitration, ready_to_start);

	c_network_session* group_session = get_manager()->get_group_session();
	bool arbitration_completed = 0;
	bool all_players_have_initial_participants = 0;
	bool all_players_session_started = 0;
	bool stats_written = 0;

	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: c_life_cycle_state_handler_matchmaking_arbitration::ready_to_start");

	const c_network_session_membership* session_membership = group_session->get_session_membership();

	if (m_flags.test(_matchmaking_arbitration_complete_bit))
	{
		arbitration_completed = !m_flags.test(_matchmaking_arbitration_failed_bit);
	}

	if (m_flags.test(_matchmaking_arbitration_set_initial_participants_bit))
	{
		c_network_session_parameters* session_parameters = group_session->get_session_parameters();
		all_players_have_initial_participants = session_parameters->parameters_transmitted_to_peers(FLAG(_network_session_parameter_type_initial_participants)) != 0;
		if (!all_players_have_initial_participants)
		{
			c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Warning! It seems a player hasn't got initial participants!");
		}
	}

	if (m_flags.test(_matchmaking_arbitration_start_match_session_completed_bit) && !m_flags.test(_matchmaking_arbitration_start_match_session_failed_bit))
	{
		all_players_session_started = session_membership->peer_property_flag_test(_peer_property_flag_test_all_peers, _network_session_peer_properties_status_match_started_bit);
		if (!all_players_session_started)
		{
			c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Warning! It seems a peer hasn't finished starting their session!");
		}
	}

	if (m_flags.test(_matchmaking_arbitration_start_match_initial_stats_write_completed_bit) && !m_flags.test(_matchmaking_arbitration_start_match_initial_stats_write_failed_bit))
	{
		stats_written = session_membership->peer_property_flag_test(_peer_property_flag_test_all_peers, _network_session_peer_properties_status_match_initial_stats_written_bit);
	}
	else
	{
		stats_written = 0;
	}

	if (m_flags.test(_matchmaking_arbitration_start_match_session_failed_bit))
	{
		c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Warning! Match start has failed!");
	}

	if (m_flags.test(_matchmaking_arbitration_start_match_initial_stats_write_failed_bit))
	{
		c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Warning! Stats write has failed!");
	}

	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Arbitration completed? %s", arbitration_completed ? "YES" : "NO");
	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Peers have initial participants? %s", all_players_have_initial_participants ? "YES" : "NO");
	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Session started? %s", all_players_session_started ? "YES" : "NO");
	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Stats written? %s", stats_written ? "YES" : "NO");
	c_console::write_line("donkey:networking:logic:life_cycle:matchmaking_arbitration: Are we ready? %s", arbitration_completed && all_players_have_initial_participants && all_players_session_started && stats_written ? "YES" : "NO");

	return arbitration_completed && all_players_have_initial_participants && all_players_session_started && stats_written;
}

bool c_life_cycle_state_handler_matchmaking_arbitration::session_membership_matches_arbitration_registration_results()
{
	//return INVOKE_CLASS_MEMBER(0x00494C80, c_life_cycle_state_handler_matchmaking_arbitration, session_membership_matches_arbitration_registration_results);

	return true;
}

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

	// $TODO: confirm this function gets called without the hook
	//INVOKE_CLASS_MEMBER(0x00494EF0, c_life_cycle_state_handler_matchmaking_arbitration, update);
	HOOK_INVOKE_CLASS_MEMBER(, c_life_cycle_state_handler_matchmaking_arbitration, update);
}

void c_life_cycle_state_handler_matchmaking_arbitration::update_arbitration()
{
	INVOKE_CLASS_MEMBER(0x004952F0, c_life_cycle_state_handler_matchmaking_arbitration, update_arbitration);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_arbitration::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x004952F0, c_life_cycle_state_handler_matchmaking_arbitration, update_for_state_transition);
}

