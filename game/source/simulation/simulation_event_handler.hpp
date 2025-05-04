#pragma once

#include "cseries/cseries.hpp"

struct c_simulation_world;
struct c_replication_event_manager;
struct c_simulation_type_collection;
struct c_simulation_entity_database;
struct c_simulation_event_handler
{
	bool m_initialized;
	c_simulation_world* m_world;
	c_replication_event_manager* m_event_manager;
	c_simulation_type_collection* m_type_collection;
	c_simulation_entity_database* m_entity_database;
	byte __data[0x4];
};
static_assert(sizeof(c_simulation_event_handler) == 0x18);

