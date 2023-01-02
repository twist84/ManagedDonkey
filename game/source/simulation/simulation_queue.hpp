#pragma once

#include "cseries/cseries.hpp"

enum e_simulation_queue_element_type
{
	_simulation_queue_element_type_none = 0,
	_simulation_queue_element_type_event,
	_simulation_queue_element_type_entity_creation,
	_simulation_queue_element_type_entity_update,
	_simulation_queue_element_type_entity_deletion,
	_simulation_queue_element_type_entity_promotion,
	_simulation_queue_element_type_game_global_event,
	_simulation_queue_element_type_game_global_bookkeeping_event,
	_simulation_queue_element_type_player_event,
	_simulation_queue_element_type_player_update,
	_simulation_queue_element_type_gamestates_delete,
	_simulation_queue_element_type_sandbox_event,

	_simulation_queue_element_type_unknown12,

	k_simulation_queue_element_type_count
};

struct c_simulation_queue_element
{
	c_enum<e_simulation_queue_element_type, long, k_simulation_queue_element_type_count> m_type;
	c_simulation_queue_element* m_next;
	long m_data_size;
	byte* m_data;
};
static_assert(sizeof(c_simulation_queue_element) == 0x10);

struct c_simulation_queue
{
	bool m_initialized;
	long m_allocated_count;
	long m_allocated_size;
	long m_queued_count;
	long m_size;
	c_simulation_queue_element* m_head;
	c_simulation_queue_element* m_elements;
};
static_assert(sizeof(c_simulation_queue) == 0x1C);

