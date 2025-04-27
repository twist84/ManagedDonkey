#pragma once

#include "cseries/cseries.hpp"
#include "simulation/simulation_event_handler.hpp"

struct s_simulation_entity
{
	uint32 entity_index;
	uint32 entity_type;
	bool exists_in_gameworld;
	uint8 event_reference_count;
	uint32 gamestate_index;
	uint64 pending_update_mask;
	uint64 force_update_mask;
	uint32 creation_data_size;
	void* creation_data;
	uint32 state_data_size;
	void* state_data;
};
static_assert(sizeof(s_simulation_entity) == 0x30);

struct c_simulation_world;
struct c_replication_entity_manager;
struct c_simulation_type_collection;
struct c_simulation_entity_database
{
public:
	void debug_render();

protected:
	bool m_initialized;
	uint8 __unknown1;
	c_simulation_world* m_world;
	c_replication_entity_manager* m_entity_manager;
	c_simulation_type_collection* m_type_collection;
	c_static_array<s_simulation_entity, 1024> m_entities;
	c_simulation_event_handler m_event_handler;
};
static_assert(sizeof(c_simulation_entity_database) == 0xC028);

