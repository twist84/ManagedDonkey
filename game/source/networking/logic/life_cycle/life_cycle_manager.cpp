#include "networking/logic/life_cycle/life_cycle_manager.hpp"

#include "cseries/cseries_events.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"
#include "networking/session/network_session.hpp"

c_life_cycle_state_manager::c_life_cycle_state_manager() :
	m_current_state(),
	m_handlers(),
	m_session_manager(),
	m_active_squad_session(),
	m_target_session(),
	m_group_session(),
	m_observer(),
	m_pending_state_change(),
	m_pending_state(),
	m_pending_state_change_entry_data_size(),
	m_pending_state_change_entry_data(),
	m_matchmaking_quality(),
	m_matchmaking_messaging_session_identifier_valid(),
	m_matchmaking_messaging_session_identifier(),
	m_matchmaking_messaging_current_cookie()
{
}

bool c_life_cycle_state_manager::current_state_ready_for_state_transition_or_query() const
{
	return INVOKE_CLASS_MEMBER(0x0048D030, c_life_cycle_state_manager, current_state_ready_for_state_transition_or_query);
}

bool c_life_cycle_state_manager::current_state_ready_for_update() const
{
	return INVOKE_CLASS_MEMBER(0x0048D0C0, c_life_cycle_state_manager, current_state_ready_for_update);
}

void c_life_cycle_state_manager::deregister_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler)
{
	//INVOKE_CLASS_MEMBER(0x0048D180, c_life_cycle_state_manager, deregister_state_handler, state, handler);

	ASSERT(m_handlers[state] == handler);
	m_handlers[state] = nullptr;
}

void c_life_cycle_state_manager::display_state_strings()
{
	for (int32 state = 0; state < m_handlers.get_count(); state++)
	{
		c_life_cycle_state_handler* handler = m_handlers[state];
		event(_event_warning, "%s", handler->get_state_string());
	}
}

void c_life_cycle_state_manager::dispose()
{
}

c_network_session* c_life_cycle_state_manager::get_active_squad_session() const
{
	ASSERT(m_active_squad_session != nullptr);
	return m_active_squad_session;
}

c_life_cycle_state_handler* c_life_cycle_state_manager::get_current_state_handler() const
{
	//return INVOKE_CLASS_MEMBER(0x0048D340, c_life_cycle_state_manager, get_current_state_handler);

	ASSERT(m_handlers[get_current_state()] != nullptr);
	return m_handlers[get_current_state()];
}

c_network_session* c_life_cycle_state_manager::get_group_session() const
{
	ASSERT(m_group_session != nullptr);
	return m_group_session;
}

c_matchmaking_quality* c_life_cycle_state_manager::get_matchmaking_quality()
{
	return &m_matchmaking_quality;
}

c_network_observer* c_life_cycle_state_manager::get_observer() const
{
	ASSERT(m_observer != nullptr);
	return m_observer;
}

c_network_session* c_life_cycle_state_manager::get_target_session() const
{
	ASSERT(m_target_session != nullptr);
	return m_target_session;
}

void c_life_cycle_state_manager::initialize(c_network_observer* observer, c_network_session_manager* session_manager, c_network_session* squad_session_one, c_network_session* squad_session_two, c_network_session* group_session)
{
	//INVOKE_CLASS_MEMBER(0x0048D820, c_life_cycle_state_manager, initialize, observer, session_manager, squad_session_one, squad_session_two, group_session);

	m_handlers.clear();
	m_target_session = squad_session_two;
	m_current_state = _life_cycle_state_none;
	m_group_session = group_session;
	m_observer = observer;
	m_session_manager = session_manager;
	m_active_squad_session = squad_session_one;
	m_pending_state_change = 0;
	m_matchmaking_messaging_session_identifier_valid = 0;
	m_matchmaking_messaging_session_identifier = {};
	m_matchmaking_messaging_current_cookie = NONE;

	// >= play builds
	//m_pause_state_enabled = false;
	//m_pause_state = _life_cycle_state_none;
}

void c_life_cycle_state_manager::notify_expect_squad_join()
{
	//INVOKE_CLASS_MEMBER(0x0048D9F0, c_life_cycle_state_manager, notify_expect_squad_join);

	c_network_session* squad_session = c_life_cycle_state_manager::get_active_squad_session();
	ASSERT(squad_session->disconnected() || squad_session->leaving_session());

	event(_event_message, "networking:logic:life-cycle: life-cycle manager notified that we should expect a squad join, going to joining");
	if (c_life_cycle_state_manager::get_current_state() == _life_cycle_state_joining)
	{
		event(_event_warning, "networking:logic:life-cycle: we are already joining, no need to try again");
		return;
	}
	
	c_life_cycle_state_manager::set_current_state(_life_cycle_state_joining, 0, nullptr);
}

void c_life_cycle_state_manager::notify_lost_connection()
{
	//INVOKE_CLASS_MEMBER(0x0048DA40, c_life_cycle_state_manager, notify_lost_connection);

	c_life_cycle_state_handler* current_state_handler = c_life_cycle_state_manager::get_current_state_handler();
	event(_event_message, "networking:logic:life-cycle: manager notified of connection loss");
	
	if (current_state_handler->test_flag(_life_cycle_state_handler_live_disconnection_returns_to_pre_game_bit))
	{
		event(_event_message, "networking:logic:life-cycle: notified of connection loss in state '%s', going to leaving right now!",
			current_state_handler->get_state_string());
	
		bool leave_and_disconnect = false;
		set_current_state(_life_cycle_state_leaving, sizeof(leave_and_disconnect), &leave_and_disconnect);
	}
}

void c_life_cycle_state_manager::notify_session_disbandment_and_host_assumption(const c_network_session* session)
{
	//INVOKE_CLASS_MEMBER(0x0048DAB0, c_life_cycle_state_manager, notify_session_disbandment_and_host_assumption, session);

	if (session == c_life_cycle_state_manager::get_group_session())
	{
		event(_event_message, "networking:logic:life-cycle: out group session has disbanded and become host, leaving the squad");

		get_active_squad_session()->leave_session();
	}
}

void c_life_cycle_state_manager::register_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler)
{
	ASSERT(m_handlers[state] == nullptr);
	m_handlers[state] = handler;
}

void c_life_cycle_state_manager::request_leave_sessions(bool leave_and_disconnect)
{
	//INVOKE_CLASS_MEMBER(0x0048DDD0, c_life_cycle_state_manager, request_leave_sessions, leave_and_disconnect);

	event(_event_message, "networking:logic:life-cycle: leave requested to life-cycle manager (disconnect %s)",
		leave_and_disconnect ? "TRUE" : "FALSE");

	if (get_current_state() == _life_cycle_state_leaving)
	{
		event(_event_message, "networking:logic:life-cycle: we are already leaving, no need to try again");
	}
	else
	{
		request_state_change(_life_cycle_state_leaving, sizeof(leave_and_disconnect), &leave_and_disconnect);
	}
}

void c_life_cycle_state_manager::request_state_change(e_life_cycle_state state, int32 entry_data_size, const void* entry_data)
{
	//INVOKE_CLASS_MEMBER(0x0048DE50, c_life_cycle_state_manager, request_state_change, state, entry_data_size, entry_data);

	ASSERT(entry_data_size <= k_maximum_state_change_entry_data_size);
	ASSERT(state != m_current_state);

	m_pending_state_change = true;
	m_pending_state = state;
	m_pending_state_change_entry_data_size = entry_data_size;
	csmemset(m_pending_state_change_entry_data, 0, k_maximum_state_change_entry_data_size);

	if (m_pending_state_change_entry_data_size > 0)
	{
		ASSERT(entry_data != nullptr);
		csmemcpy(m_pending_state_change_entry_data, entry_data, m_pending_state_change_entry_data_size);
	}
}

void c_life_cycle_state_manager::set_current_state(e_life_cycle_state state, int32 entry_data_size, void* entry_data)
{
	//INVOKE_CLASS_MEMBER(0x0048E0E0, c_life_cycle_state_manager, set_current_state, state, entry_data_size, entry_data);

	ASSERT((entry_data_size == 0 && entry_data == nullptr) || (entry_data_size > 0 && entry_data != nullptr));

	if (m_current_state != state)
	{
		ASSERT(state >= 0 && state < k_life_cycle_state_count);

		c_life_cycle_state_handler* from_handler = m_handlers[get_current_state()];
		c_life_cycle_state_handler* to_handler = m_handlers[state];
		ASSERT(from_handler != nullptr);
		ASSERT(to_handler != nullptr);

		from_handler->exit(to_handler);
		to_handler->enter(from_handler, entry_data_size, entry_data);
	}
}

void c_life_cycle_state_manager::set_pause_state(const char* state_string, bool enabled)
{
	//INVOKE_CLASS_MEMBER(0x0048E120, c_life_cycle_state_manager, set_pause_state, state_string, enabled);

	// >= play builds
	//for (int32 handler_index = 0; handler_index < k_life_cycle_state_count; handler_index++)
	//{
	//	c_life_cycle_state_handler* handler = m_handlers[handler_index];
	//
	//	if (strncmp_debug(handler->get_state_string(), state_string, strlen_debug(state_string)) == 0)
	//	{
	//		event(_event_warning, "networking:logic:life-cycle: set pause state for '%s' %s",
	//			state_string,
	//			enabled ? "ENABLED" : "DISABLED");
	//
	//		m_pause_state_enabled = enabled;
	//		m_pause_state = m_handlers[handler_index]->m_state;
	//		return;
	//	}
	//}
	//
	//event(_event_warning, "networking:logic:life-cycle: failed to set set pause state for '%s'",
	//	state_string);
}

void c_life_cycle_state_manager::swap_squad_sessions()
{
	//INVOKE_CLASS_MEMBER(0x0048E420, c_life_cycle_state_manager, swap_squad_sessions);

	ASSERT(m_target_session->session_type() == _network_session_type_squad);
	c_network_session* temp_target_session = m_active_squad_session;
	m_active_squad_session = m_target_session;
	m_target_session = temp_target_session;
}

void c_life_cycle_state_manager::swap_target_and_group_sessions()
{
	//INVOKE_CLASS_MEMBER(0x0048E430, c_life_cycle_state_manager, swap_target_and_group_sessions);

	ASSERT(m_target_session->session_type() == _network_session_type_group);
	c_network_session* temp_target_session = m_group_session;
	m_group_session = m_target_session;
	m_target_session = temp_target_session;
}

e_life_cycle_state c_life_cycle_state_manager::get_current_state() const
{
	ASSERT(m_current_state >= _life_cycle_state_none && m_current_state < k_life_cycle_state_count);
	return m_current_state;
}

void c_life_cycle_state_manager::terminate()
{
	//INVOKE_CLASS_MEMBER(0x0048E440, c_life_cycle_state_manager, terminate);

	set_current_state(_life_cycle_state_none, 0, nullptr);
}

void c_life_cycle_state_manager::update()
{
	INVOKE_CLASS_MEMBER(0x0048E4B0, c_life_cycle_state_manager, update);
}

void c_life_cycle_state_manager::update_handle_session_parameters()
{
	INVOKE_CLASS_MEMBER(0x0048E890, c_life_cycle_state_manager, update_handle_session_parameters);
}

void c_life_cycle_state_manager::update_handle_session_states()
{
	INVOKE_CLASS_MEMBER(0x0048E920, c_life_cycle_state_manager, update_handle_session_states);
}

void c_life_cycle_state_manager::update_messaging()
{
	INVOKE_CLASS_MEMBER(0x0048EE30, c_life_cycle_state_manager, update_messaging);
}


