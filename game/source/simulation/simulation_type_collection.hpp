#pragma once

#include "cseries/cseries.hpp"
#include "simulation/game_interface/simulation_game_entities.hpp"
#include "simulation/game_interface/simulation_game_events.hpp"

struct c_simulation_type_collection
{
public:
	c_simulation_type_collection();
	~c_simulation_type_collection();

	void clear_types();
	void finish_types(long entity_type_count, long event_type_count);

	long get_entity_definition_count() const;
	c_simulation_entity_definition* get_entity_definition(e_simulation_entity_type) const;
	char const* get_entity_type_name(e_simulation_entity_type) const;
	void register_entity_definition(e_simulation_entity_type, c_simulation_entity_definition*);

	long get_event_definition_count() const;
	c_simulation_event_definition* get_event_definition(e_simulation_event_type) const;
	char const* get_event_type_name(e_simulation_event_type) const;
	void register_event_definition(e_simulation_event_type, c_simulation_event_definition*);

protected:
	long m_entity_type_count;
	c_static_array<c_simulation_entity_definition*, k_simulation_entity_type_maximum_count> m_entity_definitions;

	long m_event_type_count;
	c_static_array<c_simulation_event_definition*, k_simulation_event_type_maximum_count> m_event_definitions;
};
long const k_simulation_entity_type_maximum_size = sizeof(long) + (sizeof(c_simulation_entity_definition*) * k_simulation_entity_type_maximum_count);
long const k_simulation_event_type_maximum_size = sizeof(long) + (sizeof(c_simulation_event_definition*) * k_simulation_event_type_maximum_count);
static_assert(sizeof(c_simulation_type_collection) == k_simulation_entity_type_maximum_size + k_simulation_event_type_maximum_size);

