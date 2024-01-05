#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_in_game : c_life_cycle_state_handler
{
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* handler) override;
	virtual char const* get_state_string() override;

	void initialize(c_life_cycle_state_manager* manager);

	byte_flags m_flags;
	bool m_handled_simulation_abort;
	long m_time;
	dword m_simulation_aborted_timestamp;
	qword m_game_instance;
};
static_assert(sizeof(c_life_cycle_state_handler_in_game) == 0x40);

