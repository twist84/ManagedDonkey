#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_matchmaking_select_host :
	c_life_cycle_state_handler
{
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* handler) override;
	virtual char const* get_state_string() override;
	virtual void handle_missing_required_session_parameter(e_network_session_type session_type) override;

	void initialize(c_life_cycle_state_manager* manager);

	byte __data[0x8];
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_select_host) == 0x30);

