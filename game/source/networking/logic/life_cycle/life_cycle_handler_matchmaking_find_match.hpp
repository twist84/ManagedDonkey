#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"
#include "networking/logic/logic_matchmaking_seeker.hpp"
#include "networking/logic/logic_session_tracker.hpp"

struct c_life_cycle_state_handler_matchmaking_find_match : c_life_cycle_state_handler
{
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* handler) override;
	virtual char const* get_state_string() override;
	virtual void handle_missing_required_session_parameter(e_network_session_type session_type) override;

	void initialize(c_life_cycle_state_manager* manager);

	byte_flags m_flags;
	byte __data29[0x3];
	byte __data2C[0x4];
	c_matchmaking_seeker m_seeker;
	s_network_session_tracker_session_data m_session;
	long m_desirability;
	byte __data19A5C[0xC];
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_find_match) == 0x19A68);

