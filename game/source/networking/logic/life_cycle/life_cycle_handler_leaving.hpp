#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_leaving :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* handler, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* handler) override;
	virtual char const* get_state_string() override;

	void initialize(c_life_cycle_state_manager* manager);
	
//protected:
	uns8 m_flags;
	bool m_disconnected;
	byte __data[0x6];
};
static_assert(sizeof(c_life_cycle_state_handler_leaving) == 0x30);

