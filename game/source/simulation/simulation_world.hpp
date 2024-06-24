#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/replication/replication_entity_manager.hpp"
#include "networking/replication/replication_event_manager.hpp"
#include "simulation/simulation_entity_database.hpp"
#include "simulation/simulation_actors.hpp"
#include "simulation/simulation_players.hpp"
#include "simulation/simulation_queue.hpp"

enum e_simulation_world_type
{
	_simulation_world_type_none = 0,
	_simulation_world_type_local,
	_simulation_world_type_local_playback,
	_simulation_world_type_synchronous_game_server,
	_simulation_world_type_synchronous_game_client,
	_simulation_world_type_synchronous_film_server,
	_simulation_world_type_synchronous_film_client,
	_simulation_world_type_distributed_server,
	_simulation_world_type_distributed_client,

	k_simulation_world_type_count
};

enum e_simulation_world_state
{
	_simulation_world_state_none = 0,
	_simulation_world_state_dead,
	_simulation_world_state_disconnected,
	_simulation_world_state_joining,
	_simulation_world_state_active,
	_simulation_world_state_handoff,
	_simulation_world_state_leaving,

	k_simulation_world_state_count
};

struct c_simulation_distributed_world
{
	c_replication_entity_manager m_entity_manager;
	c_replication_event_manager m_event_manager;
	c_simulation_entity_database m_entity_database;
};
static_assert(sizeof(c_simulation_distributed_world) == 0xD0C8);

struct c_simulation_watcher;
struct c_simulation_view;
struct s_simulation_update_node;
struct c_simulation_world
{
	bool exists() const;
	bool is_active();
	bool is_authority() const;
	bool is_distributed() const;
	bool is_local() const;
	
	static void destroy_update(struct simulation_update* update);

	void debug_render();

	static char const* get_state_string(long state);

	void get_join_status(
		long* join_time_elapsed,
		long* join_time_to_abort,
		long* join_attempt_count,
		long* join_attempt_maximum,
		long* join_client_establishing_count,
		long* join_client_waiting_count,
		long* join_client_joining_count,
		long* join_client_complete_count,
		long* join_client_total_count,
		long* join_time_to_failure) const;

	c_simulation_watcher* m_watcher;
	c_simulation_distributed_world* m_distributed_world;
	c_enum<e_simulation_world_type, long, _simulation_world_type_none, k_simulation_world_type_count> m_world_type;

	union
	{
#pragma pack(push, 1)
		struct
		{
			bool m_local_machine_identifier_valid;
			s_machine_identifier m_local_machine_identifier;
			byte __align1D[0x3];
		} s;
#pragma pack(pop)

		byte data[sizeof(s)];
	};

	long m_local_machine_index;
	c_enum<e_simulation_world_state, long, _simulation_world_state_none, k_simulation_world_state_count> m_world_state;

	dword m_last_time_disconnected;

	long __unknown2C;

	bool m_time_running;
	bool m_time_immediate_update;
	byte __unknown32; // pad?
	byte __unknown33; // pad?

	long m_next_update_number;
	long m_current_update_number;

	bool m_synchronous_out_of_sync;
	bool m_synchronous_determinism_failure;
	byte __unknown3E; // pad?

	// c_simulation_world::notify_gamestate_flush
	bool m_notify_gamestate_flushed;

	// c_simulation_world::notify_gamestate_flush_outside_game_tick
	bool m_notify_gamestate_flushed_outside_game_tick;

	// c_simulation_world::update_joining_view
	bool __unknown41;

	// c_simulation_world::attach_to_map
	bool m_attached_to_map;

	// c_simulation_world::skip_next_gamestate_flush
	bool m_skipped_next_gamestate_flush;

	long m_join_attempt_count;
	dword m_last_time_active;
	long m_view_establishment_identifier;

	long __unknown50;

	long __unknown54;
	long __unknown58;
	long __unknown5C;

	long m_view_count;
	c_static_array<c_simulation_view*, 16> m_views;

	long __unknownA4;

	c_static_array<c_simulation_player, 16> m_players;
	c_static_array<c_simulation_actor, 16> m_actors;

	long m_next_update_dequeue;
	long m_update_queue_next_update_number_to_dequeue;
	long m_update_queue_latest_entry_received_type;
	long m_update_queue_latest_entry_received_update_number;
	long m_update_queue_length;
	long m_update_queue_number_of_updates;
	s_simulation_update_node* m_update_queue_head;
	s_simulation_update_node* m_update_queue_tail;
	c_simulation_queue m_bookkeeping_simulation_queue;
	c_simulation_queue m_game_simulation_queue;
};
static_assert(sizeof(c_simulation_world) == 0x1540);

