#pragma once

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

extern bool __cdecl network_get_machine_name(wchar_t* machine_name, long machine_name_len);

extern void bandwidth_profiler_render();

