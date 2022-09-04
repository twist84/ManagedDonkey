#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "simulation/simulation_actors.hpp"
#include "simulation/simulation_players.hpp"
#include "simulation/simulation_queue.hpp"

enum e_simulation_world_type
{
	_simulation_world_type_none = 0,
	_simulation_world_type_local,
	_simulation_world_type_local_playback,
	_simulation_world_type_synchronous_server,
	_simulation_world_type_synchronous_client,
	_simulation_world_type_distributed_server,
	_simulation_world_type_distributed_client,

	k_simulation_world_type_count
};

struct c_simulation_distributed_world
{
	byte __data[0xD0C8];
};
static_assert(sizeof(c_simulation_distributed_world) == 0xD0C8);

#pragma pack(push, 1)
struct c_simulation_view;
struct c_simulation_world
{
	c_simulation_watcher* m_watcher;
	c_simulation_distributed_world* m_distributed_world;
	c_enum<e_simulation_world_type, long, k_simulation_world_type_count> m_world_type;
	bool m_local_machine_identifier_valid;
	s_machine_identifier m_local_machine_identifier;
	byte __align1D[0x3];
	long m_local_machine_index;
	long m_world_state; // e_simulation_world_state
	dword m_last_time_disconnected;
	byte __data2C[0x4];
	bool m_time_running;
	bool m_time_immediate_update;
	byte __data32[0x2];
	long __unknown34;
	long m_current_update_number;
	byte __data3C[0x4];
	bool __unknown40;
	bool __unknown41;
	bool m_attached_to_map;
	bool __unknown43;
	long m_join_attempt_count;
	dword m_last_time_active;
	long __unknown4C;
	long __unknown50;
	byte __data54[0x10];
	c_simulation_view* m_views[16];
	byte __dataA4[0x4];
	c_simulation_player m_players[16];
	c_simulation_actor m_actors[16];
	byte __data14E8[0x4];
	long m_next_update_dequeue;
	long m_update_queue_latest_entry_received_type;
	long m_update_queue_latest_entry_received_update_number;
	long m_update_queue_length;
	long m_update_queue_number_of_updates;
	long* m_update_queue_head;
	long* m_update_queue_tail;
	c_simulation_queue m_bookkeeping_simulation_queue;
	c_simulation_queue m_game_simulation_queue;
};
static_assert(sizeof(c_simulation_world) == 0x1540);
#pragma pack(pop)
