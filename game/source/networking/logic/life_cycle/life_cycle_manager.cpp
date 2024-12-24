#include "networking/logic/life_cycle/life_cycle_manager.hpp"

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"
#include "cseries/cseries_events.hpp"

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
	m_handlers[state] = NULL;
}

c_life_cycle_state_handler* c_life_cycle_state_manager::get_current_state_handler() const
{
	//return INVOKE_CLASS_MEMBER(0x0048D340, c_life_cycle_state_manager, get_current_state_handler);

	ASSERT(m_handlers[get_current_state()] != NULL);
	return m_handlers[get_current_state()];
}

void c_life_cycle_state_manager::initialize(c_network_observer* observer, c_network_session_manager* session_manager, c_network_session* active_squad_session, c_network_session* target_session, c_network_session* group_session)
{
	INVOKE_CLASS_MEMBER(0x0048D820, c_life_cycle_state_manager, initialize, observer, session_manager, active_squad_session, target_session, group_session);
}

void c_life_cycle_state_manager::notify_expect_squad_join()
{
	INVOKE_CLASS_MEMBER(0x0048D9F0, c_life_cycle_state_manager, notify_expect_squad_join);
}

//.text:0048DA40 ; public: void __cdecl c_life_cycle_state_manager::notify_lost_connection(e_network_lost_connection_type)
//.text:0048DAB0 ; c_life_cycle_state_manager::notify_session_disbandment_and_host_assumption

void c_life_cycle_state_manager::request_leave_sessions(bool disconnect)
{
	//INVOKE_CLASS_MEMBER(0x0048DDD0, c_life_cycle_state_manager, request_leave_sessions, disconnect);

	MESSAGE_EVENT("networking:logic:life-cycle: leave requested to life-cycle manager (disconnect %s)", disconnect ? "TRUE" : "FALSE");

	if (m_current_state == _life_cycle_state_leaving)
	{
		MESSAGE_EVENT("networking:logic:life-cycle: we are already leaving, no need to try again");
	}
	else
		request_state_change(_life_cycle_state_leaving, sizeof(bool), &disconnect);
}

void c_life_cycle_state_manager::request_state_change(e_life_cycle_state state, long entry_data_size, void const* entry_data)
{
	//INVOKE_CLASS_MEMBER(0x0048DE50, c_life_cycle_state_manager, request_state_change, state, entry_data_size, entry_data);

	ASSERT(entry_data_size <= k_maximum_state_change_entry_data_size);
	ASSERT(state != m_current_state);

	m_pending_state_change = true;
	m_pending_state = state;
	m_entry_data_size = entry_data_size;
	csmemset(m_entry_data, 0, k_maximum_state_change_entry_data_size);

	if (m_entry_data_size > 0)
	{
		ASSERT(entry_data != NULL);
		csmemcpy(m_entry_data, entry_data, m_entry_data_size);
	}
}

void c_life_cycle_state_manager::set_current_state(e_life_cycle_state state, long entry_data_size, void* entry_data)
{
	//INVOKE_CLASS_MEMBER(0x0048E0E0, c_life_cycle_state_manager, set_current_state, state, entry_data_size, entry_data);

	ASSERT((entry_data_size == 0 && entry_data == NULL) || (entry_data_size > 0 && entry_data != NULL));

	if (m_current_state != state)
	{
		ASSERT(state >= 0 && state < k_life_cycle_state_count);

		c_life_cycle_state_handler* from_handler = m_handlers[get_current_state()];
		c_life_cycle_state_handler* to_handler = m_handlers[state];
		ASSERT(from_handler != NULL);
		ASSERT(to_handler != NULL);

		from_handler->exit(to_handler);
		to_handler->enter(from_handler, entry_data_size, entry_data);
	}
}

void c_life_cycle_state_manager::swap_squad_sessions()
{
	INVOKE_CLASS_MEMBER(0x0048E420, c_life_cycle_state_manager, swap_squad_sessions);
}

void c_life_cycle_state_manager::swap_target_and_group_sessions()
{
	INVOKE_CLASS_MEMBER(0x0048E430, c_life_cycle_state_manager, swap_target_and_group_sessions);
}

e_life_cycle_state c_life_cycle_state_manager::get_current_state() const
{
	ASSERT(m_current_state.get() >= _life_cycle_state_none && m_current_state.get() < k_life_cycle_state_count);
	return m_current_state;
}

void c_life_cycle_state_manager::terminate()
{
	//INVOKE_CLASS_MEMBER(0x0048E440, c_life_cycle_state_manager, terminate);

	set_current_state(_life_cycle_state_none, 0, NULL);
}

void c_life_cycle_state_manager::update()
{
	INVOKE_CLASS_MEMBER(0x0048E4B0, c_life_cycle_state_manager, update);
}

//.text:0048E890 ; private: void __cdecl c_life_cycle_state_manager::update_handle_session_parameters()
//.text:0048E920 ; private: void __cdecl c_life_cycle_state_manager::update_handle_session_states()
//.text:0048EE30 ; private: void __cdecl c_life_cycle_state_manager::update_messaging()

c_network_session* c_life_cycle_state_manager::get_active_squad_session() const
{
	ASSERT(m_active_squad_session != NULL);
	return m_active_squad_session;
}

c_network_session* c_life_cycle_state_manager::get_target_session() const
{
	ASSERT(m_target_session != NULL);
	return m_target_session;
}

c_network_session* c_life_cycle_state_manager::get_group_session() const
{
	ASSERT(m_group_session != NULL);
	return m_group_session;
}

void c_life_cycle_state_manager::register_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler)
{
	ASSERT(m_handlers[state] == NULL);
	m_handlers[state] = handler;
}

c_network_observer* c_life_cycle_state_manager::get_observer() const
{
	ASSERT(m_observer != NULL);
	return m_observer;
}

