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
public:
	void describe_status(char* buffer, long buffer_length) const;
	long describe_status_simple() const;
	bool need_to_generate_updates() const;

protected:
	c_simulation_world* m_world;
	c_network_observer* m_observer;
	c_network_session* m_session;
	long m_session_local_update_number;
	long m_machine_last_membership_update_number;

	long __unknown18;

	dword m_local_machine_valid_mask;
	long m_local_machine_index;
	s_machine_identifier m_machine_identifiers[17];
	bool m_machine_update_pending;
	s_player_collection player_collection;
	dword m_player_collection_machine_valid_mask;
	s_machine_identifier m_player_collection_machine_identifiers[17];
	bool m_changes_pending_acknowledgement;
	bool m_game_revert_occured;

	bool __unknown16656;
	bool __unknown16657;
};
static_assert(sizeof(c_simulation_watcher) == 0x16658);

