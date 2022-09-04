#pragma once

struct c_simulation_entity_definition;
struct c_simulation_event_definition;
struct c_simulation_type_collection
{
	long m_entity_type_count;
	c_simulation_entity_definition* m_entity_definitions[32];

	long m_event_type_count;
	c_simulation_event_definition* m_event_definitions[64];
};
