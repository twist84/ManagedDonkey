#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"
#include "networking/logic/network_arbitration.hpp"

struct s_life_cycle_matchmaking_progress;

enum e_matchmaking_arbitration_flags
{
	_matchmaking_arbitration_entered_as_host_bit = 0,
	_matchmaking_arbitration_initiated_bit,
	_matchmaking_arbitration_blocked_bit,
	_matchmaking_arbitration_complete_bit,
	_matchmaking_arbitration_complete_as_host_bit,
	_matchmaking_arbitration_set_teams_bit,
	_matchmaking_arbitration_repeated_play_set_bit,
	_matchmaking_arbitration_set_initial_participants_bit,
	_matchmaking_arbitration_failed_bit,
	_matchmaking_arbitration_start_match_session_initiated_bit,
	_matchmaking_arbitration_start_match_session_completed_bit,
	_matchmaking_arbitration_start_match_session_failed_bit,
	_matchmaking_arbitration_start_match_initial_stats_write_initiated_bit,
	_matchmaking_arbitration_start_match_initial_stats_write_completed_bit,
	_matchmaking_arbitration_start_match_initial_stats_write_failed_bit,
	_matchmaking_arbitration_start_match_initial_stats_write_blocked_bit,
	_matchmaking_arbitration_failed_not_enough_hosts_bit,
	_matchmaking_arbitration_waiting_for_repeated_play_ready_bit,
	_matchmaking_arbitration_waiting_for_clients_to_arbitrate_bit,

	k_matchmaking_arbitration_flags_count,
};

typedef c_flags<e_matchmaking_arbitration_flags, uns32, k_matchmaking_arbitration_flags_count> c_matchmaking_arbitration_flags;

class c_life_cycle_state_handler_matchmaking_arbitration :
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
	c_life_cycle_state_handler_matchmaking_arbitration();
	void dispose();
	void get_progress(s_life_cycle_matchmaking_progress* progress_out);

private:
	void initial_stats_write_mark_complete();
	void initial_stats_write_mark_failed();

public:
	void initialize(c_life_cycle_state_manager* manager);

private:
	void mark_arbitration_complete();
	void mark_arbitration_failed();

public:
	bool ready_to_start();

private:
	bool ready_to_update_repeated_play();

public:
	bool session_membership_matches_arbitration_registration_results();

private:
	void start_match_mark_complete();
	void start_match_mark_failed();
	void update_arbitration();
	void update_initial_stats_write();
	void update_progress();
	void update_repeated_play();
	void update_start_match();

public:
//protected:
	c_matchmaking_arbitration_flags m_flags;
	uns32 m_arbitration_start_time;
	uns32 m_arbitration_waiting_for_completion_start_time;
	s_arbitration_registration_result m_registration_result;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_matchmaking_arbitration) == 0x1C0);
COMPILE_ASSERT(0x28 == OFFSETOF(c_life_cycle_state_handler_matchmaking_arbitration, m_flags));
COMPILE_ASSERT(0x2C == OFFSETOF(c_life_cycle_state_handler_matchmaking_arbitration, m_arbitration_start_time));
COMPILE_ASSERT(0x30 == OFFSETOF(c_life_cycle_state_handler_matchmaking_arbitration, m_arbitration_waiting_for_completion_start_time));
COMPILE_ASSERT(0x38 == OFFSETOF(c_life_cycle_state_handler_matchmaking_arbitration, m_registration_result));

