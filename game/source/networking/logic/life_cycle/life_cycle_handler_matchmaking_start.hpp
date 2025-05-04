#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct c_life_cycle_state_handler_matchmaking_start :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* handler, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* handler) override;
	virtual char const* get_state_string() override;
	virtual void handle_missing_required_session_parameter(e_network_session_type session_type) override;

	void initialize(c_life_cycle_state_manager* manager);

	// e_life_cycle_matchmaking_find_match_role
	int32 __thiscall determine_matchmaking_find_match_role(bool a1);
	
//protected:
	uns8 m_flags;
	int32 m_find_match_role;
	int32 m_start_mode;
	int32 m_initial_desperation_sessions_found_count;
	int32 m_initial_session_search_count;
	uns32 __time3C;
	uns32 __time40;
	byte __data[0x4];
};
static_assert(sizeof(c_life_cycle_state_handler_matchmaking_start) == 0x48);

