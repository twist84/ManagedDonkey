#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/session/network_session.hpp"
#include "simulation/simulation_players.hpp"

struct c_simulation_world;
struct c_network_observer;
struct c_network_session;
struct c_simulation_watcher :
	public c_network_channel_owner
{
	c_simulation_world* m_world;
	c_network_observer* m_observer;
	c_network_session* m_session;
	dword m_session_local_update_number;
	dword m_machine_last_membership_update_number;
	dword __unknown18;
	dword m_local_machine_valid_mask;
	dword m_local_machine_index;
	s_machine_identifier m_local_machine_identifiers[17];
	bool __unknown134;
	s_player_collection player_collection;
	dword m_machine_valid_mask;
	s_machine_identifier m_machine_identifiers[17];
	bool __unknown16654;
	bool m_game_reverted;
	bool __unknown16656;
	bool __unknown16657;
};
static_assert(sizeof(c_simulation_watcher) == 0x16658);

