#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/session/network_session.hpp"
#include "simulation/simulation_players.hpp"

class c_simulation_world;
class c_network_observer;
class c_network_session;
struct simulation_machine_update;
class c_simulation_watcher :
	public c_network_channel_owner
{
private:
	void apply_machines_to_players();

public:
	bool boot_machine(const s_machine_identifier* machine_identifier, e_network_session_boot_reason boot_reason) const;
	void boot_machines(uns32 boot_machine_mask, e_network_session_boot_reason boot_reason) const;
	void describe_status(char* buffer, int32 buffer_length) const;
	int32 describe_status_simple() const;
	void destroy_watcher();
	void generate_machine_update(bool* machine_update_valid, simulation_machine_update* machine_update);
	void generate_player_updates(int32* player_update_count, int32 maximum_player_update_count, simulation_player_update* player_updates);
	uns32 get_machine_ban_flags(int32 machine_index) const;
	bool get_machine_connectivity(const s_machine_identifier* machine_identifier) const;
	int32 get_machine_index_by_identifier(const s_machine_identifier* remote_machine_identifier) const;
	bool get_machine_is_host(const s_machine_identifier* machine_identifier) const;
	uns32 get_machine_valid_mask() const;
	bool get_pending_acknowledgement(bool force_acknowledgement, uns32* machine_valid_mask, s_machine_identifier* machine_identifiers, uns32* player_valid_mask, uns32* player_in_game_mask, s_player_identifier* player_identifiers);
	uns32 get_player_in_game_mask() const;
	bool get_player_is_in_game(int32 player_index, const s_player_identifier* player_identifier) const;
	void handle_new_gamestate(uns32 game_state_proc_flags);
	void handle_player_creation();
	void handle_player_leave_game(int32 player_index);
	bool in_online_networked_session();
	void initialize_watcher(c_simulation_world* world);
	void log_acknowledgement_status(bool force_acknowledgement, uns32 machine_valid_mask, uns32 player_valid_mask, uns32 player_in_game_mask) const;
	bool maintain_connection();

private:
	bool maintain_simulation_authority(c_network_session* session, int32 local_peer_index, c_static_flags<17> peer_valid_flags, const s_network_session_peer* peers);
	bool maintain_simulation_client(int32 host_peer_index);

public:
	bool need_to_generate_updates() const;
	void notify_game_revert();

private:
	static simulation_player_update* __cdecl player_update_allocate(int32* player_update_count, int32 maximum_player_update_count, simulation_player_update* player_updates);
	static void __cdecl player_update_find_swap_location(const s_player_collection* players, int32 add_player_index, const s_player_identifier* add_player_identifier, int32* swap_player_index, bool* remove_existing_player);
	static bool __cdecl player_update_must_leave_game(int32 player_index, const s_player_collection_player* player, c_static_flags<17> peer_valid_flags, const s_machine_identifier* network_machine_identifiers, c_static_flags<17> valid_network_player_flags, const s_network_session_player* network_players);

public:
	void reset_tracking_arrays();
	void setup_connection();

private:
	bool setup_view(int32 machine_index, const s_network_session_peer* session_peer, int32 observer_channel_index);
	bool update_machines(const s_transport_secure_address* session_host_address, int32 session_local_update_number, int32 session_update_number, int32 session_peer_index, c_static_flags<17> peer_valid_flags, const s_network_session_peer* session_peers);

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
COMPILE_ASSERT(sizeof(c_simulation_watcher) == 0x16658);

