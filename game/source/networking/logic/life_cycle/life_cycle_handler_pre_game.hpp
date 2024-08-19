#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

extern bool net_skip_countdown;

struct c_life_cycle_state_handler_pre_game :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* handler) override;
	virtual char const* get_state_string() override;

	void initialize(c_life_cycle_state_manager* manager);
	
//protected:
	dword m_time;
	long m_countdown_timer;
	byte __data[0x8];
};
static_assert(sizeof(c_life_cycle_state_handler_pre_game) == 0x38);

