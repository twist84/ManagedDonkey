#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"
#include "networking/transport/transport_qos.hpp"

struct s_life_cycle_matchmaking_progress;

//typedef c_flags<e_matchmaking_assemble_match_flags, uns8, k_matchmaking_assemble_match_flags_count> c_matchmaking_assemble_match_flags;
typedef uns8 c_matchmaking_assemble_match_flags;

class c_life_cycle_state_handler_matchmaking_assemble_match :
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
	c_life_cycle_state_handler_matchmaking_assemble_match();
	void dispose();
	bool get_gather_party_properties(s_matchmaking_gather_party_properties* gather_party_properties_out);
	void get_progress(s_life_cycle_matchmaking_progress* progress_out);
	void initialize(c_life_cycle_state_manager* manager);
	void notify_qos_reply_block_set();
	
//protected:
	c_matchmaking_assemble_match_flags m_flags;
	int32 m_gather_start_time;
	int32 m_ready_to_start_time;
	s_matchmaking_session_properties m_advertised_session_properties;
	uns32 m_qos_listener_stats_query_time;
	int32 m_last_qos_listener_data_replies_sent;
	s_qos_listener_data m_qos_listener_data;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_matchmaking_assemble_match) == 0x128);
COMPILE_ASSERT(0x028 == OFFSETOF(c_life_cycle_state_handler_matchmaking_assemble_match, m_flags));
COMPILE_ASSERT(0x02C == OFFSETOF(c_life_cycle_state_handler_matchmaking_assemble_match, m_gather_start_time));
COMPILE_ASSERT(0x030 == OFFSETOF(c_life_cycle_state_handler_matchmaking_assemble_match, m_ready_to_start_time));
COMPILE_ASSERT(0x034 == OFFSETOF(c_life_cycle_state_handler_matchmaking_assemble_match, m_advertised_session_properties));
COMPILE_ASSERT(0x0F8 == OFFSETOF(c_life_cycle_state_handler_matchmaking_assemble_match, m_qos_listener_stats_query_time));
COMPILE_ASSERT(0x0FC == OFFSETOF(c_life_cycle_state_handler_matchmaking_assemble_match, m_last_qos_listener_data_replies_sent));
COMPILE_ASSERT(0x100 == OFFSETOF(c_life_cycle_state_handler_matchmaking_assemble_match, m_qos_listener_data));

