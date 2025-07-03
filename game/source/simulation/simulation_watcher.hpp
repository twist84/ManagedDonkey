#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/session/network_session.hpp"
#include "simulation/simulation_players.hpp"

class c_simulation_world;
class c_network_observer;
class c_network_session;
class c_simulation_watcher :
	public c_network_channel_owner
{
public:
	void describe_status(char* buffer, int32 buffer_length) const;
	int32 describe_status_simple() const;
	bool in_online_networked_session();
	void initialize_watcher(c_simulation_world* world);
	bool need_to_generate_updates() const;
	void setup_connection();

protected:
	c_simulation_world* m_world;
	c_network_observer* m_observer;
	c_network_session* m_session;
	int32 m_machine_last_local_membership_update_number;
	int32 m_machine_last_membership_update_number;
	int32 m_player_last_local_membership_update_number;
	uns32 m_machine_valid_mask;
	int32 m_local_machine_index;
	s_machine_identifier m_machine_identifiers[17];
	bool m_machine_update_pending;
	s_player_collection m_player_collection;
	uns32 m_player_collection_machine_valid_mask;
	s_machine_identifier m_player_collection_machine_identifiers[17];
	bool m_changes_pending_acknowledgement;
	bool m_game_revert_occured;

	bool __unknown16656;
	bool __unknown16657;
};
static_assert(sizeof(c_simulation_watcher) == 0x16658);

