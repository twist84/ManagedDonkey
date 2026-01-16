#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

extern bool net_skip_countdown;

class c_life_cycle_state_handler_pre_game :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* to) override;
	virtual const char* get_state_string() override;

public:
	c_life_cycle_state_handler_pre_game();
	void dispose();
	void initialize(c_life_cycle_state_manager* manager);
	
//protected:
	uns32 m_last_countdown_timestamp;
	int32 m_last_countdown_timer;
	uns32 m_game_start_timestamp;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_pre_game) == 0x38);
COMPILE_ASSERT(0x28 == OFFSETOF(c_life_cycle_state_handler_pre_game, m_last_countdown_timestamp));
COMPILE_ASSERT(0x2C == OFFSETOF(c_life_cycle_state_handler_pre_game, m_last_countdown_timer));
COMPILE_ASSERT(0x30 == OFFSETOF(c_life_cycle_state_handler_pre_game, m_game_start_timestamp));

