#pragma once

#include "cseries/cseries.hpp"
#include "simulation/simulation_event_handler.hpp"

struct s_simulation_entity
{
	dword entity_index;
	dword entity_type;
	bool exists_in_gameworld;
	byte event_reference_count;
	dword gamestate_index;
	qword_flags pending_update_mask;
	qword_flags force_update_mask;
	dword creation_data_size;
	void* creation_data;
	dword state_data_size;
	void* state_data;
};
static_assert(sizeof(s_simulation_entity) == 0x30);

struct c_simulation_world;
struct c_replication_entity_manager;
struct c_simulation_type_collection;
struct c_simulation_entity_database
{
	bool m_initialized;
	byte __unknown1;
	c_simulation_world* m_world;
	c_replication_entity_manager* m_entity_manager;
	c_simulation_type_collection* m_type_collection;
	c_static_array<s_simulation_entity, 1024> m_entities;
	c_simulation_event_handler m_event_handler;
};
static_assert(sizeof(c_simulation_entity_database) == 0xC028);

