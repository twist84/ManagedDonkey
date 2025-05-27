#include "networking/logic/life_cycle/life_cycle_handler_end_game_write_stats.hpp"

c_life_cycle_state_handler_end_game_write_stats::c_life_cycle_state_handler_end_game_write_stats() :
	c_life_cycle_state_handler(),
	m_end_game_write_stats_start_time(),
	m_flags(0)
{
	//DECLFUNC(0x00453ED0, void, __thiscall, c_life_cycle_state_handler_end_game_write_stats*)(this);
}

char const* c_life_cycle_state_handler_end_game_write_stats::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x004546D0, c_life_cycle_state_handler_end_game_write_stats, get_state_string);

	return "end-game-write-stats";
}

//.text:00492B40 ; public: void c_life_cycle_state_handler_end_game_write_stats::dispose()

void c_life_cycle_state_handler_end_game_write_stats::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00492B50, c_life_cycle_state_handler_end_game_write_stats, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_end_game_write_stats::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00492C10, c_life_cycle_state_handler_end_game_write_stats, exit, to);
}

//.text:00492C40 ; 
//.text:00492C50 ; private: e_end_game_stats_write_desire c_life_cycle_state_handler_end_game_write_stats::game_get_stats_write_desire()

void c_life_cycle_state_handler_end_game_write_stats::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00492D10, c_life_cycle_state_handler_end_game_write_stats, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_end_game_write_stats, &parent_flags, 0ULL, 0ULL);
}

//.text:00492D40 ; 
//.text:00492D50 ; 
//.text:00492D70 ; 
//.text:00492D80 ; 
//.text:00492DA0 ; 
//.text:00492DD0 ; private: void c_life_cycle_state_handler_end_game_write_stats::stats_write_mark_complete()
//.text:00492DF0 ; 

void c_life_cycle_state_handler_end_game_write_stats::update()
{
	INVOKE_CLASS_MEMBER(0x00492E10, c_life_cycle_state_handler_end_game_write_stats, update);
}

//.text:004930E0 ; private: void c_life_cycle_state_handler_end_game_write_stats::update_data_mine_upload()

e_life_cycle_state_transition_type c_life_cycle_state_handler_end_game_write_stats::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00493170, c_life_cycle_state_handler_end_game_write_stats, update_for_state_transition);
}

//.text:004931B0 ; private: void c_life_cycle_state_handler_end_game_write_stats::update_session_end()
//.text:00493240 ; private: void c_life_cycle_state_handler_end_game_write_stats::update_session_start()
//.text:004932C0 ; private: void c_life_cycle_state_handler_end_game_write_stats::update_submit_webstats()
//.text:00493310 ; private: void c_life_cycle_state_handler_end_game_write_stats::update_write_stats()

