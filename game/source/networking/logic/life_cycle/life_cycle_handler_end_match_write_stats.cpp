#include "networking/logic/life_cycle/life_cycle_handler_end_match_write_stats.hpp"

c_life_cycle_state_handler_end_match_write_stats::c_life_cycle_state_handler_end_match_write_stats() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_end_match_write_stats_start_time()
{
	//DECLFUNC(0x00453EF0, void, __thiscall, c_life_cycle_state_handler_end_match_write_stats*)(this);
}

char const* c_life_cycle_state_handler_end_match_write_stats::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x004546E0, c_life_cycle_state_handler_end_match_write_stats, get_state_string);

	return "end-match-write-stats";
}

void c_life_cycle_state_handler_end_match_write_stats::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00499620, c_life_cycle_state_handler_end_match_write_stats, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_end_match_write_stats::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00499630, c_life_cycle_state_handler_end_match_write_stats, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_end_match_write_stats::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00499680, c_life_cycle_state_handler_end_match_write_stats, exit, to);
}

//.text:004996B0 ; 
//.text:004996C0 ; private: bool c_life_cycle_state_handler_end_match_write_stats::game_results_ready_for_stats_write()

void c_life_cycle_state_handler_end_match_write_stats::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x00499740, c_life_cycle_state_handler_end_match_write_stats, initialize, manager);
}

//.text:00499770 ; 
//.text:00499780 ; 
//.text:004997A0 ; 
//.text:004997B0 ; 
//.text:004997D0 ; 
//.text:00499800 ; private: void c_life_cycle_state_handler_end_match_write_stats::stats_write_mark_complete()
//.text:00499820 ; 

void c_life_cycle_state_handler_end_match_write_stats::update()
{
	INVOKE_CLASS_MEMBER(0x00499840, c_life_cycle_state_handler_end_match_write_stats, update);
}

//.text:00499AE0 ; private: void c_life_cycle_state_handler_end_match_write_stats::update_data_mine_upload()

e_life_cycle_state_transition_type c_life_cycle_state_handler_end_match_write_stats::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00499AE0, c_life_cycle_state_handler_end_match_write_stats, update_for_state_transition);
}

//.text:00499BA0 ; private: void c_life_cycle_state_handler_end_match_write_stats::update_session_end()
//.text:00499C40 ; private: void c_life_cycle_state_handler_end_match_write_stats::update_submit_webstats()
//.text:00499C90 ; private: void c_life_cycle_state_handler_end_match_write_stats::update_write_stats()

