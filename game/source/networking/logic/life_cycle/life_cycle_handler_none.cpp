#include "networking/logic/life_cycle/life_cycle_handler_none.hpp"

#include "cseries/cseries_events.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/network_join.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/session/network_session.hpp"

c_life_cycle_state_handler_none::c_life_cycle_state_handler_none() :
	c_life_cycle_state_handler()
{
	//DECLFUNC(0x004540D0, void, __thiscall, c_life_cycle_state_handler_none*)(this);
}

const char* c_life_cycle_state_handler_none::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x004547B0, c_life_cycle_state_handler_none, get_state_string);

	return "none";
}

void c_life_cycle_state_handler_none::dispose()
{
	//INVOKE_CLASS_MEMBER(0x0048F170, c_life_cycle_state_handler_none, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_none::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	//INVOKE_CLASS_MEMBER(0x0048F180, c_life_cycle_state_handler_none, enter, from, entry_data_size, entry_data);

	ASSERT(entry_data_size == 0);
	ASSERT(entry_data == NULL);

	c_life_cycle_state_handler::enter(from, 0, NULL);
	get_manager()->get_target_session()->force_disconnect();
	network_squad_session_set_closed_status(_network_session_closed_not_closed);
	network_join_set_join_queue_mode(_network_join_closed_to_all_joins);
}

void c_life_cycle_state_handler_none::exit(c_life_cycle_state_handler* to)
{
	//INVOKE_CLASS_MEMBER(0x0048F1C0, c_life_cycle_state_handler_none, exit, to);

	c_life_cycle_state_handler::exit(to);
}

void c_life_cycle_state_handler_none::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x0048F1D0, c_life_cycle_state_handler_none, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_none, &parent_flags, 0x0ULL, 0x0ULL);
}

void c_life_cycle_state_handler_none::update()
{
	//INVOKE_CLASS_MEMBER(0x0048F200, c_life_cycle_state_handler_none, update);

	c_life_cycle_state_manager* manager = get_manager();
	c_network_session* squad_session = manager->get_active_squad_session();
	if (!squad_session->disconnected() && !squad_session->leaving_session())
	{
		if (squad_session->is_host())
		{
			c_network_session_parameters* squad_session_parameters = squad_session->get_session_parameters();
			squad_session_parameters->m_parameters.session_mode.set(_network_session_mode_idle);
		}

		event(_event_message, "lifecycle: state none with a squad, entering pregame");
		manager->request_state_change(_life_cycle_state_pre_game, 0, NULL);
	}
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_none::update_for_state_transition()
{
	//return INVOKE_CLASS_MEMBER(0x0048F260, c_life_cycle_state_handler_none, update_for_state_transition);

	return _life_cycle_state_transition_unchanged;
}

