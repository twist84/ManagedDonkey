#pragma once

#include "cseries/cseries.hpp"
#include "simulation/simulation_event_handler.hpp"

struct s_simulation_entity
{
	uns32 entity_index;
	uns32 entity_type;
	bool exists_in_gameworld;
	uns8 event_reference_count;
	uns32 gamestate_index;
	uns64 pending_update_mask;
	uns64 force_update_mask;
	uns32 creation_data_size;
	void* creation_data;
	uns32 state_data_size;
	void* state_data;
};
COMPILE_ASSERT(sizeof(s_simulation_entity) == 0x30);

class c_simulation_world;
class c_replication_entity_manager;
class c_simulation_type_collection;
class c_simulation_entity_database
{
public:
	void debug_render();

protected:
	bool m_initialized;
	bool m_resetting;
	c_simulation_world* m_world;
	c_replication_entity_manager* m_entity_manager;
	c_simulation_type_collection* m_type_collection;
	s_simulation_entity m_entity_data[1024];
};
COMPILE_ASSERT(sizeof(c_simulation_entity_database) == 0xC010);
//COMPILE_ASSERT(sizeof(c_simulation_entity_database) == 0xC028);

