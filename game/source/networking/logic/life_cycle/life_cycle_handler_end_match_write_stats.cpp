#include "networking/logic/life_cycle/life_cycle_handler_end_match_write_stats.hpp"

#include "main/main_game.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/network_time.hpp"
#include "networking/session/network_observer.hpp"
#include "networking/session/network_session.hpp"

void c_life_cycle_state_handler_end_match_write_stats::update()
{
	DECLFUNC(0x00499840, void, __thiscall, c_life_cycle_state_handler_end_match_write_stats*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_end_match_write_stats::update_for_state_transition()
{
	//return DECLFUNC(0x00499B60, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_end_match_write_stats*)(this);

	if (!TEST_BIT(m_flags, 10))
		return _life_cycle_state_transition_type_unknown1;

	if (get_manager()->get_group_session()->session_mode() == _network_session_mode_end_game)
		return _life_cycle_state_transition_type_unknown1;

	get_manager()->request_state_change(_life_cycle_state_post_match, 0, NULL);
	return _life_cycle_state_transition_type_unknown2;
}

void c_life_cycle_state_handler_end_match_write_stats::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00499630, void, __thiscall, c_life_cycle_state_handler_end_match_write_stats*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);

	//c_life_cycle_state_handler::enter(handler, 0, NULL);
	//m_flags = 0;
	//__time2C = network_time_get();
	//network_squad_session_set_closed_status(3);
	//network_join_set_join_queue_mode(_network_join_queue_closed_while_in_match);
	//network_banhammer_upload_network_statistics();
}

void c_life_cycle_state_handler_end_match_write_stats::exit(c_life_cycle_state_handler* handler)
{
	//DECLFUNC(0x00499680, void, __thiscall, c_life_cycle_state_handler_end_match_write_stats*, c_life_cycle_state_handler*)(this, handler);

	c_life_cycle_state_handler::exit(handler);
	get_manager()->get_observer()->observer_prioritize_upload_bandwidth(false);
	if (!main_game_change_in_progress())
		main_menu_launch();
}

char const* c_life_cycle_state_handler_end_match_write_stats::get_state_string()
{
	//return DECLFUNC(0x004546E0, char const*, __thiscall, c_life_cycle_state_handler_end_match_write_stats*)(this);

	return "end-match-write-stats";
}

void c_life_cycle_state_handler_end_match_write_stats::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x00499740, void, __thiscall, c_life_cycle_state_handler_end_match_write_stats*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit1, true);
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	handler_flags.set(_life_cycle_state_handler_group_session_disconnect_recreates_group_bit, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_end_match_write_stats, &handler_flags, 0, 0);
}

