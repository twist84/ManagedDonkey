#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

enum e_end_game_stats_write_desire
{
	_end_game_stats_write_not_desired_no_reason = 0,
	_end_game_stats_write_not_desired_game_is_not_live,
	_end_game_stats_write_not_desired_game_options_not_valid,
	_end_game_stats_write_not_desired_game_is_campaign,
	_end_game_stats_write_not_desired_game_is_film,
	_end_game_stats_write_not_desired_stats_not_finalized,
	_end_game_stats_write_not_desired_cant_get_final_stats,
	_end_game_stats_write_not_desired_stats_uninitialized,
	_end_game_stats_write_not_desired_stats_unreliable,
	_end_game_stats_write_not_desired_simulation_aborted,
	_end_game_stats_write_not_desired_insufficient_machines,
	_end_game_stats_write_desired,

	k_end_game_stats_write_desire_count,
};

enum e_end_game_write_stats_flags
{
	_end_game_write_stats_session_start_initiated_bit = 0,
	_end_game_write_stats_session_start_completed_bit,
	_end_game_write_stats_write_initiated_bit,
	_end_game_write_stats_write_completed_bit,
	_end_game_write_stats_write_blocked_bit,
	_end_game_write_stats_session_end_initiated_bit,
	_end_game_write_stats_session_end_completed_bit,
	_end_game_write_stats_data_mine_upload_initiated_bit,
	_end_game_write_stats_data_mine_upload_completed_bit,
	_end_game_write_stats_webstats_submitted_bit,
	_end_game_write_stats_initiated_bit,
	_end_game_write_stats_suppressed_bit,
	_end_game_write_stats_bypassed_bit,
	_end_game_write_stats_completed_bit,

	k_end_game_write_stats_flags_count,
};
typedef c_flags<e_end_game_write_stats_flags, int16, k_end_game_write_stats_flags_count> c_end_game_write_stats_flags;

class c_life_cycle_state_handler_end_game_write_stats :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* to) override;
	virtual const char* get_state_string() override;

public:
	c_life_cycle_state_handler_end_game_write_stats();
	void dispose();

private:
	e_end_game_stats_write_desire game_get_stats_write_desire();

public:
	void initialize(c_life_cycle_state_manager* manager);

private:
	void stats_write_mark_complete();
	void update_data_mine_upload();
	void update_session_end();
	void update_session_start();
	void update_submit_webstats();
	void update_write_stats();

private:
	uns32 m_end_game_write_stats_start_time; // 0x28
	c_end_game_write_stats_flags m_flags;    // 0x2C
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_end_game_write_stats) == 0x30);

extern bool g_end_game_write_stats_bypass_override;

