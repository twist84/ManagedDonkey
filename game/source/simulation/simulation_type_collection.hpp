#pragma once

#include "cseries/cseries.hpp"
#include "simulation/game_interface/simulation_game_entities.hpp"
#include "simulation/game_interface/simulation_game_events.hpp"

class c_simulation_type_collection
{
public:
	c_simulation_type_collection();
	~c_simulation_type_collection();

	void clear_types();
	void finish_types(int32 entity_type_count, int32 event_type_count);

	int32 get_entity_definition_count() const;
	c_simulation_entity_definition* get_entity_definition(e_simulation_entity_type) const;
	const char* get_entity_type_name(e_simulation_entity_type) const;
	void register_entity_definition(e_simulation_entity_type, c_simulation_entity_definition*);

	int32 get_event_definition_count() const;
	c_simulation_event_definition* get_event_definition(e_simulation_event_type) const;
	const char* get_event_type_name(e_simulation_event_type) const;
	void register_event_definition(e_simulation_event_type, c_simulation_event_definition*);

protected:
	int32 m_entity_type_count;
	c_static_array<c_simulation_entity_definition*, k_simulation_entity_type_maximum_count> m_entity_definitions;

	int32 m_event_type_count;
	c_static_array<c_simulation_event_definition*, k_simulation_event_type_maximum_count> m_event_definitions;
};
int32 const k_simulation_entity_type_maximum_size = sizeof(int32) + (sizeof(c_simulation_entity_definition*) * k_simulation_entity_type_maximum_count);
int32 const k_simulation_event_type_maximum_size = sizeof(int32) + (sizeof(c_simulation_event_definition*) * k_simulation_event_type_maximum_count);
COMPILE_ASSERT(sizeof(c_simulation_type_collection) == k_simulation_entity_type_maximum_size + k_simulation_event_type_maximum_size);

