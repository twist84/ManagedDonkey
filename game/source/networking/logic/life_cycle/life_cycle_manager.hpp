#pragma once

#include "cseries/cseries.hpp"

enum e_life_cycle_state_transition_type
{

};

enum e_life_cycle_session_type
{

};

enum e_life_cycle_state
{
	_life_cycle_state_none = 0,
	_life_cycle_state_pre_game,
	_life_cycle_state_start_game,
	_life_cycle_state_in_game,
	_life_cycle_state_end_game_write_stats,
	_life_cycle_state_leaving,
	_life_cycle_state_joining,
	_life_cycle_state_matchmaking_start,
	_life_cycle_state_matchmaking_find_match_client,
	_life_cycle_state_matchmaking_find_match,
	_life_cycle_state_matchmaking_find_and_assemble_match,
	_life_cycle_state_matchmaking_assemble_match,
	_life_cycle_state_matchmaking_select_host,
	_life_cycle_state_matchmaking_arbitration,
	_life_cycle_state_matchmaking_prepare_map,
	_life_cycle_state_in_match,
	_life_cycle_state_end_match_write_stats,
	_life_cycle_state_post_match,

	k_life_cycle_state_count,
};

enum e_life_cycle_state_handler_flags
{

	k_life_cycle_state_handler_bit_count = 6
};

struct c_life_cycle_state_manager;
struct c_life_cycle_state_handler
{
	virtual void update(void);
	virtual e_life_cycle_state_transition_type update_for_state_transition(long);
	virtual void enter(c_life_cycle_state_handler*, long, void*);
	virtual void exit(c_life_cycle_state_handler*);
	virtual char const* get_state_string(void);
	virtual void handle_missing_required_session_parameter(e_life_cycle_session_type);

	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;

	c_enum<e_life_cycle_state, long, k_life_cycle_state_count> m_state;
	c_life_cycle_state_manager* m_manager;

	c_flags<e_life_cycle_state_handler_flags, byte_flags, k_life_cycle_state_handler_bit_count> m_handler_flags;
	byte : 8;
	byte : 8;
	byte : 8;

	char __data14[0x14];
};
static_assert(sizeof(c_life_cycle_state_handler) == 0x28);

struct c_life_cycle_state_manager
{
	c_enum<e_life_cycle_state, long, k_life_cycle_state_count> m_current_state;
	c_static_array<c_life_cycle_state_handler*, k_life_cycle_state_count> m_handlers;
	struct c_network_session_manager* m_session_manager;
	struct c_network_session* m_active_squad_session;
	struct c_network_session* m_target_session;
	struct c_network_session* m_group_session;
	struct c_network_observer* m_observer;

	bool m_pending_state_change;
	byte : 8;
	byte : 8;
	byte : 8;
	c_enum<e_life_cycle_state, long, k_life_cycle_state_count> m_pending_state;

	long m_entry_data_size;
	byte m_entry_data[0x9924];
};
static_assert(sizeof(c_life_cycle_state_manager) == 0x9990);