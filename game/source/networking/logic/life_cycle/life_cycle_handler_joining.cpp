#include "networking/logic/life_cycle/life_cycle_handler_joining.hpp"

#include "cseries/cseries_events.hpp"
#include "networking/session/network_session.hpp"

void c_life_cycle_state_handler_joining::update()
{
	DECLFUNC(0x004937F0, void, __thiscall, c_life_cycle_state_handler_joining*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_joining::update_for_state_transition()
{
	//return DECLFUNC(0x00493890, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_joining*)(this);

	if (!TEST_BIT(m_flags, 0))
		return _life_cycle_state_transition_type_unknown1;

	if (m_enter_state == _life_cycle_state_post_match)
	{
		generate_event(_event_level_message, "networking:logic:life-cycle:joining: exiting joining, moving to post match");
		get_manager()->request_state_change(_life_cycle_state_post_match, 0, NULL);
	}
	else
	{
		generate_event(_event_level_message, "networking:logic:life-cycle:joining: exiting joining, moving to pre-game [squad state %d]",
			get_manager()->get_active_squad_session()->session_mode());
		get_manager()->request_state_change(_life_cycle_state_pre_game, 0, NULL);
	}
	return _life_cycle_state_transition_type_unknown2;
}

void c_life_cycle_state_handler_joining::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00493670, void, __thiscall, c_life_cycle_state_handler_joining*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_joining::exit(c_life_cycle_state_handler* handler)
{
	//DECLFUNC(0x00493750, void, __thiscall, c_life_cycle_state_handler_joining*, c_life_cycle_state_handler*)(this, handler);

	c_life_cycle_state_handler::exit(handler);
}

char const* c_life_cycle_state_handler_joining::get_state_string()
{
	//return DECLFUNC(0x00454710, char const*, __thiscall, c_life_cycle_state_handler_joining*)(this);

	return "joining";
}

void c_life_cycle_state_handler_joining::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00493770, void, __thiscall, c_life_cycle_state_handler_joining*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_joining, &handler_flags, 0, 0);
}

