#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_start.hpp"

#include "main/console.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00493980, c_life_cycle_state_handler_matchmaking_start, determine_matchmaking_find_match_role);

bool net_matchmaking_force_gather = true;
bool net_matchmaking_force_search = false;

void c_life_cycle_state_handler_matchmaking_start::update()
{
	DECLFUNC(0x004941D0, void, __thiscall, c_life_cycle_state_handler_matchmaking_start*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_start::update_for_state_transition()
{
	return DECLFUNC(0x00494440, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_matchmaking_start*)(this);
}

void c_life_cycle_state_handler_matchmaking_start::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00493A50, void, __thiscall, c_life_cycle_state_handler_matchmaking_start*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_start::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x00493C60, void, __thiscall, c_life_cycle_state_handler_matchmaking_start*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_matchmaking_start::get_state_string()
{
	return DECLFUNC(0x004547A0, char const*, __thiscall, c_life_cycle_state_handler_matchmaking_start*)(this);
}

void c_life_cycle_state_handler_matchmaking_start::handle_missing_required_session_parameter(e_life_cycle_session_type session_type)
{
	DECLFUNC(0x00494010, void, __thiscall, c_life_cycle_state_handler_matchmaking_start*, e_life_cycle_session_type)(this, session_type);
}

void c_life_cycle_state_handler_matchmaking_start::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00494030, void, __thiscall, c_life_cycle_state_handler_matchmaking_start*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit5, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_matchmaking_start, &handler_flags, 0x240000000, 0);
}

long __thiscall c_life_cycle_state_handler_matchmaking_start::determine_matchmaking_find_match_role(bool a1)
{
	long result = 1;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_life_cycle_state_handler_matchmaking_start, determine_matchmaking_find_match_role, a1);

	if (net_matchmaking_force_gather)
	{
		console_printf("networking:logic:life-cycle:matchmaking_start: FORCE GATHER");
		result = 2;
	}
	else if (net_matchmaking_force_search)
	{
		console_printf("networking:logic:life-cycle:matchmaking_start: FORCE SEARCH");
		result = 1;
	}

	return result;
}

