#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_find_match.hpp"
#include "networking/logic/logic_matchmaking_seeker.hpp"
#include "networking/logic/logic_session_tracker.hpp"

struct s_life_cycle_matchmaking_progress;

//typedef c_flags<e_matchmaking_find_and_assemble_match_flags, uns8, 7> c_matchmaking_find_and_assemble_match_flags;
typedef uns8 c_matchmaking_find_and_assemble_match_flags;

struct s_find_and_assemble_data
{
	bool force_seeker_to_desperation;
	int32 initial_desperation_sessions_found_count;
	int32 initial_desperation_session_search_count;
};
COMPILE_ASSERT(sizeof(s_find_and_assemble_data) == 0xC);

class c_life_cycle_state_handler_matchmaking_find_and_assemble_match :
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
	c_life_cycle_state_handler_matchmaking_find_and_assemble_match();
	void dispose();
	bool get_gather_party_properties(s_matchmaking_gather_party_properties* gather_party_properties_out);
	void get_progress(s_life_cycle_matchmaking_progress* progress_out);
	bool get_search_party_properties(s_matchmaking_search_party_properties* search_party_properties_out);
	void initialize(c_life_cycle_state_manager* manager);
	void notify_qos_reply_block_set();
	
//protected:
	c_matchmaking_find_and_assemble_match_flags m_flags;
	c_matchmaking_seeker m_matchmaking_seeker;
	s_suitable_matchmaking_session m_current_session_join_attempt;
	uns32 m_gather_start_time;
	uns32 m_desparation_start_time;
	bool m_force_gather_in_matchmaking_start;
	byte pad[0x3];
	s_matchmaking_session_properties m_advertised_session_properties;
	s_find_and_assemble_data m_enter_find_and_assemble_data;
	s_qos_listener_data m_qos_listener_data;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_matchmaking_find_and_assemble_match) == 0x19B60);
COMPILE_ASSERT(0x00028 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_and_assemble_match, m_flags));
COMPILE_ASSERT(0x00030 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_and_assemble_match, m_matchmaking_seeker));
COMPILE_ASSERT(0x03538 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_and_assemble_match, m_current_session_join_attempt));
COMPILE_ASSERT(0x19A60 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_and_assemble_match, m_gather_start_time));
COMPILE_ASSERT(0x19A64 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_and_assemble_match, m_desparation_start_time));
COMPILE_ASSERT(0x19A68 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_and_assemble_match, m_force_gather_in_matchmaking_start));
COMPILE_ASSERT(0x19A69 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_and_assemble_match, pad));
COMPILE_ASSERT(0x19A6C == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_and_assemble_match, m_advertised_session_properties));
COMPILE_ASSERT(0x19B30 == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_and_assemble_match, m_enter_find_and_assemble_data));
COMPILE_ASSERT(0x19B3C == OFFSETOF(c_life_cycle_state_handler_matchmaking_find_and_assemble_match, m_qos_listener_data));

