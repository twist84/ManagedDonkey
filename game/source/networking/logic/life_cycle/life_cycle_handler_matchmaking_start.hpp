#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

struct s_life_cycle_matchmaking_progress;

enum e_matchmaking_start_mode
{
	_matchmaking_start_mode_default = 0,
	_matchmaking_start_mode_force_gather,
};

enum e_life_cycle_matchmaking_find_match_role
{
	_life_cycle_matchmaking_find_match_role_client = 0,
	_life_cycle_matchmaking_find_match_role_search,
	_life_cycle_matchmaking_find_match_role_search_gather,

	k_life_cycle_matchmaking_find_match_role_count,
};

enum e_matchmaking_start_flags
{
	_matchmaking_start_left_initial_group_bit = 0,
	_matchmaking_start_squad_ready_bit,
	_matchmaking_start_allowed_to_start_bit,
	_matchmaking_start_waiting_to_start_bit,
	_matchmaking_start_group_creation_initiated_bit,
	_matchmaking_start_group_created_bit,

	k_matchmaking_start_flags_count,
};
typedef c_flags<e_matchmaking_start_flags, uns8, k_matchmaking_start_flags_count> c_matchmaking_start_flags;

struct s_matchmaking_start_data
{
	e_matchmaking_start_mode start_mode;
	int32 initial_desperation_sessions_found_count;
	int32 initial_session_search_count;
};
COMPILE_ASSERT(sizeof(s_matchmaking_start_data) == 0xC);

class c_life_cycle_state_handler_matchmaking_start :
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
	c_life_cycle_state_handler_matchmaking_start();
	void dispose();
	void get_progress(s_life_cycle_matchmaking_progress* progress_out);
	void initialize(c_life_cycle_state_manager* manager);

	e_life_cycle_matchmaking_find_match_role __thiscall determine_matchmaking_find_match_role(bool force_gather);
	
//protected:
	c_matchmaking_start_flags m_flags;
	e_life_cycle_matchmaking_find_match_role m_matchmaking_find_match_role;
	s_matchmaking_start_data m_entry_data;
	uns32 m_waiting_to_start_timer;
	uns32 m_last_warning_toast_time;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_matchmaking_start) == 0x48);
COMPILE_ASSERT(0x28 == OFFSETOF(c_life_cycle_state_handler_matchmaking_start, m_flags));
COMPILE_ASSERT(0x2C == OFFSETOF(c_life_cycle_state_handler_matchmaking_start, m_matchmaking_find_match_role));
COMPILE_ASSERT(0x30 == OFFSETOF(c_life_cycle_state_handler_matchmaking_start, m_entry_data));
COMPILE_ASSERT(0x3C == OFFSETOF(c_life_cycle_state_handler_matchmaking_start, m_waiting_to_start_timer));
COMPILE_ASSERT(0x40 == OFFSETOF(c_life_cycle_state_handler_matchmaking_start, m_last_warning_toast_time));

