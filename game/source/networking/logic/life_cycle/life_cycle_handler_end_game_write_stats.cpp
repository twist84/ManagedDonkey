#include "networking/logic/life_cycle/life_cycle_handler_end_game_write_stats.hpp"

#include "main/main_game.hpp"
#include "networking/logic/network_join.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/session/network_session.hpp"

void c_life_cycle_state_handler_end_game_write_stats::update()
{
	DECLFUNC(0x00492E10, void, __thiscall, c_life_cycle_state_handler_end_game_write_stats*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_end_game_write_stats::update_for_state_transition()
{
	//return DECLFUNC(0x00493170, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_end_game_write_stats*)(this);

	if (!TEST_BIT(m_flags, 13))
		return _life_cycle_state_transition_type_unknown1;

	if (get_manager()->get_active_squad_session()->session_mode() == _network_session_mode_end_game)
		return _life_cycle_state_transition_type_unknown1;

	get_manager()->request_state_change(_life_cycle_state_pre_game, 0, NULL);
	return _life_cycle_state_transition_type_unknown2;
}

void c_life_cycle_state_handler_end_game_write_stats::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00492B50, void, __thiscall, c_life_cycle_state_handler_end_game_write_stats*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_end_game_write_stats::exit(c_life_cycle_state_handler* handler)
{
	//DECLFUNC(0x00492C10, void, __thiscall, c_life_cycle_state_handler_end_game_write_stats*, c_life_cycle_state_handler*)(this, handler);

	c_life_cycle_state_handler::exit(handler);

	if (!main_game_change_in_progress())
		main_menu_launch();
	network_join_set_join_queue_mode(_network_join_open_to_join_squad);
	network_squad_session_set_closed_status(0);
}

char const* c_life_cycle_state_handler_end_game_write_stats::get_state_string()
{
	//return DECLFUNC(0x004546D0, char const*, __thiscall, c_life_cycle_state_handler_end_game_write_stats*)(this);

	return "end-game-write-stats";
}

void c_life_cycle_state_handler_end_game_write_stats::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00492D10, void, __thiscall, c_life_cycle_state_handler_end_game_write_stats*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_end_game_write_stats, &handler_flags, 0, 0);
}

