#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

//typedef c_flags<e_end_match_write_stats_flags, int16, k_end_match_write_stats_flags_count> c_end_match_write_stats_flags;
typedef int16 c_end_match_write_stats_flags;

class c_life_cycle_state_handler_end_match_write_stats :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* to) override;
	virtual const char* get_state_string() override;

public:
	c_life_cycle_state_handler_end_match_write_stats();
	void dispose();
	void initialize(c_life_cycle_state_manager* manager);

//protected:
	c_end_match_write_stats_flags m_flags;
	uns32 m_end_match_write_stats_start_time;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_end_match_write_stats) == 0x30);
COMPILE_ASSERT(0x28 == OFFSETOF(c_life_cycle_state_handler_end_match_write_stats, m_flags));
COMPILE_ASSERT(0x2C == OFFSETOF(c_life_cycle_state_handler_end_match_write_stats, m_end_match_write_stats_start_time));

