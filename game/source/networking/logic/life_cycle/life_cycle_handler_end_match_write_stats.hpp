#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

//typedef c_flags<e_end_match_write_stats_flags, int16, k_end_match_write_stats_flags_count> c_end_match_write_stats_flags;
typedef int16 c_end_match_write_stats_flags;

struct c_life_cycle_state_handler_end_match_write_stats :
	public c_life_cycle_state_handler
{
public:

//protected:
	c_end_match_write_stats_flags m_flags;
	uns32 m_end_match_write_stats_start_time;
};
static_assert(sizeof(c_life_cycle_state_handler_end_match_write_stats) == 0x30);
static_assert(0x28 == OFFSETOF(c_life_cycle_state_handler_end_match_write_stats, m_flags));
static_assert(0x2C == OFFSETOF(c_life_cycle_state_handler_end_match_write_stats, m_end_match_write_stats_start_time));

