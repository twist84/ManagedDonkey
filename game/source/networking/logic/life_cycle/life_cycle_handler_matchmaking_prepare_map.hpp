#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct s_life_cycle_matchmaking_progress;

//typedef c_flags<e_matchmaking_prepare_map_flags, uns8, k_matchmaking_prepare_map_flags_count> c_matchmaking_prepare_map_flags;
typedef uns8 c_matchmaking_prepare_map_flags;

class c_life_cycle_state_handler_matchmaking_prepare_map :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* to) override;
	virtual const char* get_state_string() override;
	virtual void handle_missing_required_session_parameter(e_network_session_type session_type) override;

public:
	c_life_cycle_state_handler_matchmaking_prepare_map();
	void dispose();
	void get_progress(s_life_cycle_matchmaking_progress* progress_out);
	void initialize(c_life_cycle_state_manager* manager);
	
//protected:
	c_matchmaking_prepare_map_flags m_flags;
	uns32 m_prepare_map_start_time;
	uns32 m_countdown_start_time;
	uns32 m_loading_start_time;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_matchmaking_prepare_map) == 0x38);
COMPILE_ASSERT(0x28 == OFFSETOF(c_life_cycle_state_handler_matchmaking_prepare_map, m_flags));
COMPILE_ASSERT(0x2C == OFFSETOF(c_life_cycle_state_handler_matchmaking_prepare_map, m_prepare_map_start_time));
COMPILE_ASSERT(0x30 == OFFSETOF(c_life_cycle_state_handler_matchmaking_prepare_map, m_countdown_start_time));
COMPILE_ASSERT(0x34 == OFFSETOF(c_life_cycle_state_handler_matchmaking_prepare_map, m_loading_start_time));

