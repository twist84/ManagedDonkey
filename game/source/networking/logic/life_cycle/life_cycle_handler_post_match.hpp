#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

//typedef c_flags<e_life_cycle_post_match_flags, int16, k_life_cycle_post_match_flags_count> c_life_cycle_post_match_flags;
typedef int16 c_life_cycle_post_match_flags;

struct c_life_cycle_state_handler_post_match :
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

//protected:
	c_life_cycle_post_match_flags m_flags;
	uns32 m_countdown_seconds_timer;
	uns32 m_rematch_squad_host_update_timer;
	uns32 m_ready_for_next_match_timer;
	uns32 m_waiting_for_stats_replication_timer;
	int32 m_pre_rematch_squad_peer_count;
};
static_assert(sizeof(c_life_cycle_state_handler_post_match) == 0x40);
static_assert(0x28 == OFFSETOF(c_life_cycle_state_handler_post_match, m_flags));
static_assert(0x2C == OFFSETOF(c_life_cycle_state_handler_post_match, m_countdown_seconds_timer));
static_assert(0x30 == OFFSETOF(c_life_cycle_state_handler_post_match, m_rematch_squad_host_update_timer));
static_assert(0x34 == OFFSETOF(c_life_cycle_state_handler_post_match, m_ready_for_next_match_timer));
static_assert(0x38 == OFFSETOF(c_life_cycle_state_handler_post_match, m_waiting_for_stats_replication_timer));
static_assert(0x3C == OFFSETOF(c_life_cycle_state_handler_post_match, m_pre_rematch_squad_peer_count));

