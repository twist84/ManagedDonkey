#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

class c_life_cycle_state_handler_none :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* to) override;
	virtual const char* get_state_string() override;

public:
	c_life_cycle_state_handler_none();
	void dispose();
	void initialize(c_life_cycle_state_manager* manager);
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_none) == 0x28);

