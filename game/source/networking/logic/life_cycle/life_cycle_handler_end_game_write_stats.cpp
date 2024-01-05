#include "networking/logic/life_cycle/life_cycle_handler_end_game_write_stats.hpp"

void c_life_cycle_state_handler_end_game_write_stats::update()
{
	DECLFUNC(0x00492E10, void, __thiscall, c_life_cycle_state_handler_end_game_write_stats*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_end_game_write_stats::update_for_state_transition()
{
	return DECLFUNC(0x00493170, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_end_game_write_stats*)(this);
}

void c_life_cycle_state_handler_end_game_write_stats::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00492B50, void, __thiscall, c_life_cycle_state_handler_end_game_write_stats*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_end_game_write_stats::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x00492C10, void, __thiscall, c_life_cycle_state_handler_end_game_write_stats*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_end_game_write_stats::get_state_string()
{
	return DECLFUNC(0x004546D0, char const*, __thiscall, c_life_cycle_state_handler_end_game_write_stats*)(this);
}

void c_life_cycle_state_handler_end_game_write_stats::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00492D10, void, __thiscall, c_life_cycle_state_handler_end_game_write_stats*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_end_game_write_stats, &handler_flags, 0, 0);
}

