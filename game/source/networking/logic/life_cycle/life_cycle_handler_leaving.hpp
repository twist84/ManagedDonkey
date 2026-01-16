#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

//typedef c_flags<e_leaving_flags, uns8, k_leaving_flags_count> c_leaving_flags;
typedef uns8 c_leaving_flags;

class c_life_cycle_state_handler_leaving :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* to) override;
	virtual const char* get_state_string() override;

public:
	c_life_cycle_state_handler_leaving();
	void dispose();
	void initialize(c_life_cycle_state_manager* manager);
	
//protected:
	c_leaving_flags m_flags;
	bool m_leave_and_disconnect;
	e_gui_game_mode m_failure_game_mode;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_leaving) == 0x30);
COMPILE_ASSERT(0x28 == OFFSETOF(c_life_cycle_state_handler_leaving, m_flags));
COMPILE_ASSERT(0x29 == OFFSETOF(c_life_cycle_state_handler_leaving, m_leave_and_disconnect));
COMPILE_ASSERT(0x2C == OFFSETOF(c_life_cycle_state_handler_leaving, m_failure_game_mode));

