#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

#include "cseries/cseries_events.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/session/network_session.hpp"

void c_life_cycle_state_handler::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	//DECLFUNC(0x0048D220, void, __thiscall, c_life_cycle_state_handler*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);

	generate_event(_event_level_message, "networking:logic:life-cycle: entering state %s", get_state_string());

	ASSERT(entry_data_size == 0);
	ASSERT(entry_data == NULL);

	if (!test_flag(_life_cycle_state_handler_unknown_bit1) && !test_flag(_life_cycle_state_handler_allows_group_session_bit))
	{
		c_network_session* group_session = m_manager->get_group_session();
		if (!group_session->disconnected() && !group_session->leaving_session())
		{
			generate_event(_event_level_message, "networking:logic:life-cycle: entering state %s which does not require nor allow a group, leaving", get_state_string());
		}
	}
}

void c_life_cycle_state_handler::exit(c_life_cycle_state_handler* handler)
{
	//DECLFUNC(0x0048D280, void, __thiscall, c_life_cycle_state_handler*, c_life_cycle_state_handler*)(this, handler);

	generate_event(_event_level_message, "networking:logic:life-cycle: exiting state %s", get_state_string());
}

void c_life_cycle_state_handler::handle_missing_required_session_parameter(e_life_cycle_session_type session_type)
{
	//DECLFUNC(0x0048D7D0, void, __thiscall, c_life_cycle_state_handler*, e_life_cycle_session_type)(this, session_type);
}

c_life_cycle_state_handler::c_life_cycle_state_handler() :
	m_state(_life_cycle_state_none),
	m_manager(NULL),
	m_handler_flags(0),
	m_required_squad_session_parameter_mask(0),
	m_required_group_session_parameter_mask(0)
{
	//DECLFUNC(0x00453EC0, void, __thiscall, c_life_cycle_state_handler*)(this);
}

void c_life_cycle_state_handler::initialize(c_life_cycle_state_manager* manager, e_life_cycle_state state, c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> const* handler_flags, qword required_squad_session_parameter_mask, qword required_group_session_parameter_mask)
{
	m_manager = manager;
	m_state = state;
	m_handler_flags = *handler_flags;
	m_required_squad_session_parameter_mask = required_squad_session_parameter_mask;
	m_required_group_session_parameter_mask = required_group_session_parameter_mask;

	if (m_handler_flags.test(_life_cycle_state_handler_unknown_bit1))
	{
		ASSERT(m_handler_flags.test(_life_cycle_state_handler_allows_group_session_bit));
		ASSERT(m_handler_flags.test(_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit) || m_handler_flags.test(_life_cycle_state_handler_group_session_disconnect_recreates_group_bit));
	}
	m_manager->register_state_handler(state, this);
}

bool c_life_cycle_state_handler::test_flag(e_life_cycle_state_handler_flags bit)
{
	return m_handler_flags.test(bit);
}

c_life_cycle_state_manager* c_life_cycle_state_handler::get_manager() const
{
	ASSERT(m_manager != NULL);
	return m_manager;
}

