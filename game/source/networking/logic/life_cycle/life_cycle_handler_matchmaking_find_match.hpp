#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"
#include "networking/logic/logic_matchmaking_seeker.hpp"
#include "networking/logic/logic_session_tracker.hpp"

struct s_life_cycle_matchmaking_progress;

//typedef c_flags<e_matchmaking_find_match_flags, uns8, k_matchmaking_find_match_flags_count> c_matchmaking_find_match_flags;
typedef uns8 c_matchmaking_find_match_flags;

struct s_suitable_matchmaking_session
{
	s_network_session_tracker_session_data tracked_session_data;
};
COMPILE_ASSERT(sizeof(s_suitable_matchmaking_session) == sizeof(s_network_session_tracker_session_data));

class c_life_cycle_state_handler_matchmaking_find_match :
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
	c_life_cycle_state_handler_matchmaking_find_match();
	void dispose();
	void get_progress(s_life_cycle_matchmaking_progress* progress_out);
	bool get_search_party_properties(s_matchmaking_search_party_properties* search_party_properties_out);
	void initialize(c_life_cycle_state_manager* manager);
	
//protected:
	c_matchmaking_find_match_flags m_flags;
	c_matchmaking_seeker m_matchmaking_seeker;
	s_suitable_matchmaking_session m_current_session_join_attempt;
	uns32 m_desparation_start_time;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_matchmaking_find_match) == 0x19A68);
COMPILE_ASSERT(0x00028 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_match, m_flags));
COMPILE_ASSERT(0x00030 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_match, m_matchmaking_seeker));
COMPILE_ASSERT(0x03538 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_match, m_current_session_join_attempt));
COMPILE_ASSERT(0x19A60 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_match, m_desparation_start_time));

