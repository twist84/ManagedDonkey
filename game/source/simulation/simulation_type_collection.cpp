#include "simulation/simulation_type_collection.hpp"

int32 const k_simulation_entity_maximum_state_data_size = 1024;   // halo 3/odst: 1024, reach: 8192
int32 const k_simulation_entity_maximum_creation_data_size = 128; // halo 3/odst/reach: 128
int32 const k_simulation_entity_maximum_update_flag_count = 36;   // halo 3: 32, odst: 36, reach: 64
int32 const k_simulation_event_maximum_entity_references = 2;     // halo 3/odst/reach: 2

// simulation/network_memory.cpp
c_simulation_type_collection::c_simulation_type_collection()
{
	clear_types();
}

// simulation/network_memory.cpp
c_simulation_type_collection::~c_simulation_type_collection()
{
}

// 004704B0
void c_simulation_type_collection::clear_types()
{
	m_entity_type_count = NONE;
	m_entity_definitions.clear();

	m_event_type_count = NONE;
	m_event_definitions.clear();
}

// 004704F0
void c_simulation_type_collection::finish_types(int32 entity_type_count, int32 event_type_count)
{
	ASSERT(entity_type_count <= k_simulation_entity_type_maximum_count);
	ASSERT(m_entity_type_count == NONE);

	m_entity_type_count = entity_type_count;
	for (int32 entity_type_index = 0; entity_type_index < k_simulation_entity_type_maximum_count; entity_type_index++)
	{
		ASSERT((entity_type_index < m_entity_type_count) == (m_entity_definitions[entity_type_index] != nullptr));
	}

	ASSERT(event_type_count <= k_simulation_event_type_maximum_count);
	ASSERT(m_event_type_count == NONE);

	m_event_type_count = event_type_count;
	for (int32 event_type = 0; event_type < k_simulation_event_type_maximum_count; event_type++)
	{
		ASSERT((event_type < m_event_type_count) == (m_event_definitions[event_type] != nullptr));
	}
}

int32 c_simulation_type_collection::get_entity_definition_count() const
{
	ASSERT(m_entity_type_count >= 0);

	return m_entity_type_count;
}

// 00470510
c_simulation_entity_definition* c_simulation_type_collection::get_entity_definition(e_simulation_entity_type entity_type) const
{
	ASSERT(entity_type >= 0 && entity_type < m_entity_type_count);

	c_simulation_entity_definition* entity_definition = m_entity_definitions[entity_type];
	ASSERT(entity_definition != nullptr);
	ASSERT(entity_definition->entity_type() == entity_type);

	return entity_definition;
}

// 00470520
const char* c_simulation_type_collection::get_entity_type_name(e_simulation_entity_type entity_type) const
{
	if (VALID_INDEX(entity_type, m_entity_type_count))
	{
		return get_entity_definition(entity_type)->entity_type_name();
	}

	return "unknown";
}

// 004705A0
void c_simulation_type_collection::register_entity_definition(e_simulation_entity_type entity_type, c_simulation_entity_definition* entity_definition)
{
	ASSERT(entity_type >= 0 && entity_type < k_simulation_entity_type_maximum_count);
	ASSERT(entity_definition != nullptr);
	ASSERT(entity_definition->entity_type() == entity_type);
	ASSERT(entity_definition->entity_type_name() != nullptr);
	ASSERT(entity_definition->state_data_size() <= k_simulation_entity_maximum_state_data_size);
	ASSERT(entity_definition->update_flag_count() <= k_simulation_entity_maximum_update_flag_count);

	//uns64 mask = 0;
	//ASSERT((entity_definition->initial_update_mask(mask) & ~MASK(entity_definition->update_flag_count())) == 0); // $TODO

	ASSERT(m_entity_type_count == NONE);
	ASSERT(m_entity_definitions[entity_type] == nullptr);

	m_entity_definitions[entity_type] = entity_definition;
}

int32 c_simulation_type_collection::get_event_definition_count() const
{
	ASSERT(m_event_type_count >= 0);

	return m_event_type_count;
}

// 00470550
c_simulation_event_definition* c_simulation_type_collection::get_event_definition(e_simulation_event_type event_type) const
{
	ASSERT(event_type >= 0 && event_type < m_event_type_count);

	c_simulation_event_definition* event_definition = m_event_definitions[event_type];
	ASSERT(event_definition != nullptr);
	ASSERT(event_definition->event_type() == event_type);

	return event_definition;
}

// 00470570
const char* c_simulation_type_collection::get_event_type_name(e_simulation_event_type event_type) const
{
	if (VALID_INDEX(event_type, m_event_type_count))
	{
		return get_event_definition(event_type)->event_type_name();
	}

	return "unknown";
}

// 004705C0
void c_simulation_type_collection::register_event_definition(e_simulation_event_type event_type, c_simulation_event_definition* event_definition)
{
	ASSERT(event_type >= 0 && event_type < k_simulation_event_type_maximum_count);
	ASSERT(event_definition != nullptr);
	ASSERT(event_definition->event_type() == event_type);
	ASSERT(event_definition->event_type_name() != nullptr);
	ASSERT(event_definition->number_of_entity_references() <= k_simulation_event_maximum_entity_references);
	ASSERT(!event_definition->reference_delays_entity_deletion() || event_definition->number_of_entity_references() > 0);
	ASSERT(m_event_type_count == NONE);
	ASSERT(m_event_definitions[event_type] == nullptr);

	m_event_definitions[event_type] = event_definition;
}

