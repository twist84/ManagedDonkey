#include "networking/logic/life_cycle/life_cycle_handler_end_game_write_stats.hpp"

c_life_cycle_state_handler_end_game_write_stats::c_life_cycle_state_handler_end_game_write_stats() :
	c_life_cycle_state_handler()
{
	DECLFUNC(0x00453ED0, void, __thiscall, c_life_cycle_state_handler_end_game_write_stats*)(this);
}

char const* c_life_cycle_state_handler_end_game_write_stats::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x004546D0, c_life_cycle_state_handler_end_game_write_stats, get_state_string);

	return "end-game-write-stats";
}

void c_life_cycle_state_handler_end_game_write_stats::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00492B50, c_life_cycle_state_handler_end_game_write_stats, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_end_game_write_stats::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00492C10, c_life_cycle_state_handler_end_game_write_stats, exit, to);
}

void c_life_cycle_state_handler_end_game_write_stats::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00492D10, c_life_cycle_state_handler_end_game_write_stats, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_end_game_write_stats, &parent_flags, 0ULL,0ULL);
}

void c_life_cycle_state_handler_end_game_write_stats::update()
{
	INVOKE_CLASS_MEMBER(0x00492E10, c_life_cycle_state_handler_end_game_write_stats, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_end_game_write_stats::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00493170, c_life_cycle_state_handler_end_game_write_stats, update_for_state_transition);
}

