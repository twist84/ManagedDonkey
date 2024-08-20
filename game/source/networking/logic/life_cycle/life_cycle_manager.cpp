#include "networking/logic/life_cycle/life_cycle_manager.hpp"

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"
#include "cseries/cseries_events.hpp"

bool c_life_cycle_state_manager::current_state_ready_for_state_transition_or_query() const
{
	return DECLFUNC(0x0048D030, bool, __thiscall, c_life_cycle_state_manager const*)(this);
}

bool c_life_cycle_state_manager::current_state_ready_for_update() const
{
	return DECLFUNC(0x0048D0C0, bool, __thiscall, c_life_cycle_state_manager const*)(this);
}

void c_life_cycle_state_manager::deregister_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler)
{
	//DECLFUNC(0x0048D180, void, __thiscall, c_life_cycle_state_manager*, e_life_cycle_state, c_life_cycle_state_handler*)(this, state, handler);

	ASSERT(m_handlers[state] == handler);
	m_handlers[state] = NULL;
}

c_life_cycle_state_handler* c_life_cycle_state_manager::get_current_state_handler() const
{
	//return DECLFUNC(0x0048D340, c_life_cycle_state_handler*, __thiscall, c_life_cycle_state_manager const*)(this);

	ASSERT(m_handlers[get_current_state()] != NULL);
	return m_handlers[get_current_state()];
}

void c_life_cycle_state_manager::initialize(c_network_observer* observer, c_network_session_manager* session_manager, c_network_session* active_squad_session, c_network_session* target_session, c_network_session* group_session)
{
	DECLFUNC(0x0048D820, void, __thiscall, c_life_cycle_state_manager*, c_network_observer*, c_network_session_manager*, c_network_session*, c_network_session*, c_network_session*)(this, observer, session_manager, active_squad_session, target_session, group_session);
}

void c_life_cycle_state_manager::notify_expect_squad_join()
{
	DECLFUNC(0x0048D9F0, void, __thiscall, c_life_cycle_state_manager*)(this);
}

//.text:0048DA40 ; public: void __cdecl c_life_cycle_state_manager::notify_lost_connection(e_network_lost_connection_type)
//.text:0048DAB0 ; c_life_cycle_state_manager::notify_session_disbandment_and_host_assumption

void c_life_cycle_state_manager::request_leave_sessions(bool disconnect)
{
	//DECLFUNC(0x0048DDD0, void, __thiscall, c_life_cycle_state_manager*, bool)(this, disconnect);

	c_console::write_line("networking:logic:life-cycle: leave requested to life-cycle manager (disconnect %s)", disconnect ? "TRUE" : "FALSE");

	if (m_current_state == _life_cycle_state_leaving)
		c_console::write_line("networking:logic:life-cycle: we are already leaving, no need to try again");
	else
		request_state_change(_life_cycle_state_leaving, sizeof(bool), &disconnect);
}

void c_life_cycle_state_manager::request_state_change(e_life_cycle_state state, long entry_data_size, void const* entry_data)
{
	//DECLFUNC(0x0048DE50, void, __thiscall, c_life_cycle_state_manager*, e_life_cycle_state, long, void const*)(this, state, entry_data_size, entry_data);

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
	//DECLFUNC(0x0048E0E0, void, __thiscall, c_life_cycle_state_manager*, e_life_cycle_state, long, void*)(this, state, entry_data_size, entry_data);

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
	DECLFUNC(0x0048E420, void, __thiscall, c_life_cycle_state_manager*)(this);
}

void c_life_cycle_state_manager::swap_target_and_group_sessions()
{
	DECLFUNC(0x0048E430, void, __thiscall, c_life_cycle_state_manager*)(this);
}

e_life_cycle_state c_life_cycle_state_manager::get_current_state() const
{
	ASSERT(m_current_state.get() >= _life_cycle_state_none && m_current_state.get() < k_life_cycle_state_count);
	return m_current_state;
}

void c_life_cycle_state_manager::terminate()
{
	//DECLFUNC(0x0048E440, void, __thiscall, c_life_cycle_state_manager*)(this);

	set_current_state(_life_cycle_state_none, 0, NULL);
}

void c_life_cycle_state_manager::update()
{
	DECLFUNC(0x0048E4B0, void, __thiscall, c_life_cycle_state_manager*);
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

