#pragma once

#include "game/players.hpp"
#include "networking/replication/replication_entity_manager.hpp"
#include "networking/replication/replication_event_manager.hpp"
#include "simulation/simulation_actors.hpp"
#include "simulation/simulation_entity_database.hpp"
#include "simulation/simulation_players.hpp"
#include "simulation/simulation_queue.hpp"

struct c_simulation_distributed_world
{
	c_replication_entity_manager m_entity_manager;
	c_replication_event_manager m_event_manager;
	c_simulation_entity_database m_entity_database;
	c_simulation_event_handler m_event_handler;
};
static_assert(sizeof(c_simulation_distributed_world) == 0xD0C8);

struct c_simulation_watcher;
struct c_simulation_view;
struct s_simulation_queued_update;
struct c_simulation_world
{
public:
	bool exists() const;
	bool is_active();
	bool is_authority() const;
	bool is_distributed() const;
	bool is_local() const;
	
	static void destroy_update(struct simulation_update* update);

	void debug_render();

	static const char* get_state_string(int32 state);

	void get_join_status(
		int32* join_time_elapsed,
		int32* join_time_to_abort,
		int32* join_attempt_count,
		int32* join_attempt_maximum,
		int32* join_client_establishing_count,
		int32* join_client_waiting_count,
		int32* join_client_joining_count,
		int32* join_client_complete_count,
		int32* join_client_total_count,
		int32* join_time_to_failure) const;

public:
	struct s_world_state_data_disconnected
	{
		uns32 disconnected_timestamp;
	};
	static_assert(sizeof(s_world_state_data_disconnected) == 0x4);

	struct s_world_state_data_joining
	{
		uns32 join_start_timestamp;
		uns32 join_client_machine_mask;
	};
	static_assert(sizeof(s_world_state_data_joining) == 0x8);

	struct s_world_state_data_active
	{
		uns32 active_client_machine_mask;
	};
	static_assert(sizeof(s_world_state_data_active) == 0x4);

	struct s_world_state_data
	{
		union
		{
			s_world_state_data_disconnected disconnected;
			s_world_state_data_joining joining;
			s_world_state_data_active active;
		};
	};
	static_assert(sizeof(s_world_state_data) == 0x8);

//protected:
	c_simulation_watcher* m_watcher;
	c_simulation_distributed_world* m_distributed_world;
	e_simulation_world_type m_world_type;
	bool m_local_machine_identifier_valid;
#pragma pack(push, 1)
	s_machine_identifier m_local_machine_identifier;
#pragma pack(pop)
	int32 m_local_machine_index;
	e_simulation_world_state m_world_state;
	s_world_state_data m_world_state_data;
	bool m_time_running;
	bool m_time_immediate_update;
	int32 m_next_update_number;
	int32 m_acknowledged_update_number;
	bool m_out_of_sync;
	bool m_out_of_sync_determinism_failure;
	bool m_world_clean;
	bool m_gamestate_flushed;
	bool m_gamestate_flushed_outside_game_tick;
	bool m_gamestate_modified_initial_state;
	bool m_attached_to_map;
	bool m_gamestate_flush_client_skip;
	int32 m_unsuccessful_join_attempts;
	uns32 m_last_active_timestamp;
	int32 m_next_view_establishment_identifier;
	int32 m_joining_total_wait_msec;
	int32 m_maximum_queued_updates;
	e_update_queue_state m_update_queue_state;
	uns32 m_update_queue_state_time;
	int32 m_view_count;
	c_simulation_view* m_views[16];
	c_simulation_player m_players[16];
	c_simulation_actor m_actors[16];
	uns32 m_synchronous_catchup_initiation_failure_timestamp;
	int32 m_update_queue_next_update_number_to_dequeue;
	e_update_queue_node m_update_queue_latest_entry_received_type;
	int32 m_update_queue_latest_entry_received_update_number;
	int32 m_update_queue_length;
	int32 m_update_queue_number_of_updates;
	s_simulation_queued_update* m_update_queue_head;
	s_simulation_queued_update* m_update_queue_tail;
	c_simulation_queue m_bookkeeping_simulation_queue;
	c_simulation_queue m_game_simulation_queue;
};
static_assert(sizeof(c_simulation_world) == 0x1540);
static_assert(0x0000 == OFFSETOF(c_simulation_world, m_watcher));
static_assert(0x0004 == OFFSETOF(c_simulation_world, m_distributed_world));
static_assert(0x0008 == OFFSETOF(c_simulation_world, m_world_type));
static_assert(0x000C == OFFSETOF(c_simulation_world, m_local_machine_identifier_valid));
static_assert(0x000D == OFFSETOF(c_simulation_world, m_local_machine_identifier));
static_assert(0x0020 == OFFSETOF(c_simulation_world, m_local_machine_index));
static_assert(0x0024 == OFFSETOF(c_simulation_world, m_world_state));
static_assert(0x0028 == OFFSETOF(c_simulation_world, m_world_state_data));
static_assert(0x0030 == OFFSETOF(c_simulation_world, m_time_running));
static_assert(0x0031 == OFFSETOF(c_simulation_world, m_time_immediate_update));
static_assert(0x0034 == OFFSETOF(c_simulation_world, m_next_update_number));
static_assert(0x0038 == OFFSETOF(c_simulation_world, m_acknowledged_update_number));
static_assert(0x003C == OFFSETOF(c_simulation_world, m_out_of_sync));
static_assert(0x003D == OFFSETOF(c_simulation_world, m_out_of_sync_determinism_failure));
static_assert(0x003E == OFFSETOF(c_simulation_world, m_world_clean));
static_assert(0x003F == OFFSETOF(c_simulation_world, m_gamestate_flushed));
static_assert(0x0040 == OFFSETOF(c_simulation_world, m_gamestate_flushed_outside_game_tick));
static_assert(0x0041 == OFFSETOF(c_simulation_world, m_gamestate_modified_initial_state));
static_assert(0x0042 == OFFSETOF(c_simulation_world, m_attached_to_map));
static_assert(0x0043 == OFFSETOF(c_simulation_world, m_gamestate_flush_client_skip));
static_assert(0x0044 == OFFSETOF(c_simulation_world, m_unsuccessful_join_attempts));
static_assert(0x0048 == OFFSETOF(c_simulation_world, m_last_active_timestamp));
static_assert(0x004C == OFFSETOF(c_simulation_world, m_next_view_establishment_identifier));
static_assert(0x0050 == OFFSETOF(c_simulation_world, m_joining_total_wait_msec));
static_assert(0x0054 == OFFSETOF(c_simulation_world, m_maximum_queued_updates));
static_assert(0x0058 == OFFSETOF(c_simulation_world, m_update_queue_state));
static_assert(0x005C == OFFSETOF(c_simulation_world, m_update_queue_state_time));
static_assert(0x0060 == OFFSETOF(c_simulation_world, m_view_count));
static_assert(0x0064 == OFFSETOF(c_simulation_world, m_views));
static_assert(0x00A8 == OFFSETOF(c_simulation_world, m_players));
static_assert(0x0BA8 == OFFSETOF(c_simulation_world, m_actors));
static_assert(0x14E8 == OFFSETOF(c_simulation_world, m_synchronous_catchup_initiation_failure_timestamp));
static_assert(0x14EC == OFFSETOF(c_simulation_world, m_update_queue_next_update_number_to_dequeue));
static_assert(0x14F0 == OFFSETOF(c_simulation_world, m_update_queue_latest_entry_received_type));
static_assert(0x14F4 == OFFSETOF(c_simulation_world, m_update_queue_latest_entry_received_update_number));
static_assert(0x14F8 == OFFSETOF(c_simulation_world, m_update_queue_length));
static_assert(0x14FC == OFFSETOF(c_simulation_world, m_update_queue_number_of_updates));
static_assert(0x1500 == OFFSETOF(c_simulation_world, m_update_queue_head));
static_assert(0x1504 == OFFSETOF(c_simulation_world, m_update_queue_tail));
static_assert(0x1508 == OFFSETOF(c_simulation_world, m_bookkeeping_simulation_queue));
static_assert(0x1524 == OFFSETOF(c_simulation_world, m_game_simulation_queue));

