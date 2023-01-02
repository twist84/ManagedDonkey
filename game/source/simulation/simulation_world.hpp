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

	k_simulation_world_state_count
};

struct c_simulation_distributed_world
{
	byte __data[0xD0C8];
};
static_assert(sizeof(c_simulation_distributed_world) == 0xD0C8);

struct c_simulation_view;
struct c_simulation_world
{
	bool exists()
	{
		return m_world_type > _simulation_world_type_none;
	}

	bool is_active()
	{
		//assert(exists());

		return m_world_state == _simulation_world_state_active;
	}

	c_simulation_watcher* m_watcher;
	c_simulation_distributed_world* m_distributed_world;
	c_enum<e_simulation_world_type, long, k_simulation_world_type_count> m_world_type;

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
	c_enum<e_simulation_world_state, long, k_simulation_world_state_count> m_world_state;
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

	c_static_array<c_simulation_view*, 16> m_views;

	byte __dataA4[0x4];

	c_static_array<c_simulation_player, 16> m_players;
	c_static_array<c_simulation_actor, 16> m_actors;

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

