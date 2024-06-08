#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_none.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_pre_game.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_start_game.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_in_game.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_end_game_write_stats.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_leaving.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_joining.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_start.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_find_match_client.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_find_match.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_find_and_assemble_match.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_assemble_match.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_select_host.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_arbitration.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_prepare_map.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_in_match.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_end_match_write_stats.hpp"
#include "networking/logic/life_cycle/life_cycle_handler_post_match.hpp"

struct s_network_life_cycle_globals
{
	bool initialized;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	c_life_cycle_state_manager manager;
	c_life_cycle_state_handler_none handler_none;
	c_life_cycle_state_handler_pre_game handler_pre_game;
	c_life_cycle_state_handler_start_game handler_start_game;
	c_life_cycle_state_handler_in_game handler_in_game;
	c_life_cycle_state_handler_end_game_write_stats handler_end_game_write_stats;
	c_life_cycle_state_handler_leaving handler_leaving;
	c_life_cycle_state_handler_joining handler_joining;
	c_life_cycle_state_handler_matchmaking_start handler_matchmaking_start;
	c_life_cycle_state_handler_matchmaking_find_match_client handler_matchmaking_find_match_client;
	c_life_cycle_state_handler_matchmaking_find_match handler_matchmaking_find_match;
	c_life_cycle_state_handler_matchmaking_find_and_assemble_match handler_matchmaking_find_and_assemble_match;
	c_life_cycle_state_handler_matchmaking_assemble_match handler_matchmaking_assemble_match;
	c_life_cycle_state_handler_matchmaking_select_host handler_matchmaking_select_host;
	c_life_cycle_state_handler_matchmaking_arbitration handler_matchmaking_arbitration;
	c_life_cycle_state_handler_matchmaking_prepare_map handler_matchmaking_prepare_map;
	c_life_cycle_state_handler_in_match handler_in_match;
	c_life_cycle_state_handler_end_match_write_stats handler_end_match_write_stats;
	c_life_cycle_state_handler_post_match handler_post_match;
};
static_assert(sizeof(s_network_life_cycle_globals) == 0x3D538);

extern s_network_life_cycle_globals& life_cycle_globals;

struct s_life_cycle_matchmaking_progress;
struct c_network_session;
struct c_network_observer;
struct c_network_session_manager;

extern void __cdecl network_life_cycle_dispose();
extern void __cdecl network_life_cycle_end();
extern void __cdecl network_life_cycle_get_matchmaking_progress(s_life_cycle_matchmaking_progress* progress_out);
extern e_life_cycle_state __cdecl network_life_cycle_get_state();
extern bool __cdecl network_life_cycle_in_squad_session(c_network_session** out_active_squad_session);
extern bool __cdecl network_life_cycle_in_system_link_advertisable_session(c_network_session** out_active_system_link_advertisable_session);
extern bool __cdecl network_life_cycle_initialize(c_network_observer* observer, c_network_session_manager* session_manager, c_network_session* squad_session_one, c_network_session* squad_session_two, c_network_session* group_session);
extern void __cdecl network_life_cycle_request_leave(bool disconnect);
extern bool __cdecl network_life_cycle_set_pre_game_state();
extern void __cdecl network_life_cycle_update();

