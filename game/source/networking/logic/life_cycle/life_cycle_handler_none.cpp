#include "networking/logic/life_cycle/life_cycle_handler_none.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/logic/network_join.hpp"
#include "networking/session/network_session.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x004547B0, c_life_cycle_state_handler_none, get_state_string);
HOOK_DECLARE_CLASS_MEMBER(0x0048F180, c_life_cycle_state_handler_none, enter);
HOOK_DECLARE_CLASS_MEMBER(0x0048F1C0, c_life_cycle_state_handler_none, exit);
HOOK_DECLARE_CLASS_MEMBER(0x0048F200, c_life_cycle_state_handler_none, update);
HOOK_DECLARE_CLASS_MEMBER(0x0048F260, c_life_cycle_state_handler_none, update_for_state_transition);

void c_life_cycle_state_handler_none::update()
{
	//DECLFUNC(0x0048F200, void, __thiscall, c_life_cycle_state_handler_none*)(this);

	c_network_session* active_squad_session = get_manager()->get_active_squad_session();
	if (!active_squad_session->disconnected() && !active_squad_session->leaving_session())
	{
		if (active_squad_session->is_host())
			active_squad_session->get_session_parameters()->m_parameters_internal.session_mode.set(_network_session_mode_idle);

		generate_event(_event_level_message, "lifecycle: state none with a squad, entering pregame");
		get_manager()->request_state_change(_life_cycle_state_pre_game, 0, NULL);
	}
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_none::update_for_state_transition()
{
	//return DECLFUNC(0x0048F260, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_none*)(this);

	return _life_cycle_state_transition_type_unknown1;
}

void c_life_cycle_state_handler_none::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	//DECLFUNC(0x0048F180, void, __thiscall, c_life_cycle_state_handler_none*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);

	ASSERT(entry_data_size == 0);
	ASSERT(entry_data == NULL);

	c_life_cycle_state_handler::enter(handler, 0, NULL);
	get_manager()->get_target_session()->force_disconnect();
	network_squad_session_set_closed_status(0);
	network_join_set_join_queue_mode(_network_join_closed_to_all_joins);

	//get_manager()->get_target_session()->disconnect();
	//network_squad_session_set_closed_for_matchmaking(false);
	//network_squad_session_set_disallow_local_joins(false);
	//network_group_session_set_disable_host_election(false);
	//network_join_set_join_queue_mode(_network_join_closed_to_all_joins);
}

void c_life_cycle_state_handler_none::exit(c_life_cycle_state_handler* handler)
{
	//DECLFUNC(0x0048F1C0, void, __thiscall, c_life_cycle_state_handler_none*, c_life_cycle_state_handler*)(this, handler);

	c_life_cycle_state_handler::exit(handler);
}

char const* c_life_cycle_state_handler_none::get_state_string()
{
	//return DECLFUNC(0x004547B0, char const*, __thiscall, c_life_cycle_state_handler_none*)(this);

	return "none";
}

c_life_cycle_state_handler_none::c_life_cycle_state_handler_none() :
	c_life_cycle_state_handler()
{
	//DECLFUNC(0x00453EC0, void, __thiscall, c_life_cycle_state_handler*)(this);
}

void c_life_cycle_state_handler_none::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x0048F1D0, void, __thiscall, c_life_cycle_state_handler_none*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_none, &handler_flags, 0, 0);
}

