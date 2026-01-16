#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct s_life_cycle_matchmaking_progress;

enum e_life_cycle_post_match_flags
{
	_life_cycle_post_match_in_main_menu_bit = 0,
	_life_cycle_post_match_timer_countdown_bit,
	_life_cycle_post_match_rematch_squad_host_bit,
	_life_cycle_post_match_rematch_joining_bit,
	_life_cycle_post_match_refreshed_stats_bit,
	_life_cycle_post_match_ready_for_matchmaking_start_bit,
	_life_cycle_post_match_ready_for_next_match_bit,
	_life_cycle_post_match_rematch_expected_bit,
	_life_cycle_post_match_waiting_for_stats_replication_bit,

	k_life_cycle_post_match_flags_count,
};
typedef c_flags<e_life_cycle_post_match_flags, int16, k_life_cycle_post_match_flags_count> c_life_cycle_post_match_flags;

class c_life_cycle_state_handler_post_match :
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
	c_life_cycle_state_handler_post_match();
	void dispose();
	void get_progress(s_life_cycle_matchmaking_progress* progress_out);
	void initialize(c_life_cycle_state_manager* manager);
	bool rematch_join_in_progress() const;

//protected:
	c_life_cycle_post_match_flags m_flags;
	uns32 m_countdown_seconds_timer;
	uns32 m_rematch_squad_host_update_timer;
	uns32 m_ready_for_next_match_timer;
	uns32 m_waiting_for_stats_replication_timer;
	int32 m_pre_rematch_squad_peer_count;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_post_match) == 0x40);
COMPILE_ASSERT(0x28 == OFFSETOF(c_life_cycle_state_handler_post_match, m_flags));
COMPILE_ASSERT(0x2C == OFFSETOF(c_life_cycle_state_handler_post_match, m_countdown_seconds_timer));
COMPILE_ASSERT(0x30 == OFFSETOF(c_life_cycle_state_handler_post_match, m_rematch_squad_host_update_timer));
COMPILE_ASSERT(0x34 == OFFSETOF(c_life_cycle_state_handler_post_match, m_ready_for_next_match_timer));
COMPILE_ASSERT(0x38 == OFFSETOF(c_life_cycle_state_handler_post_match, m_waiting_for_stats_replication_timer));
COMPILE_ASSERT(0x3C == OFFSETOF(c_life_cycle_state_handler_post_match, m_pre_rematch_squad_peer_count));

