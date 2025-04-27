#pragma once

#include "cseries/cseries.hpp"

struct c_replication_entity_manager;
struct c_replication_event_manager_view;
struct c_simulation_event_handler;

enum e_replication_event_state
{
	_replication_event_state_initializing = 0,
	_replication_event_state_ready,

	k_replication_event_state_count
};


struct c_replication_outgoing_event
{
	e_replication_event_state m_state;
	int32 m_type;
	uint32 m_timestamp;
	int32 m_cancel_timer_milliseconds;
	int32 m_entity_references[2];
	void* m_payload_data;
	int32 m_payload_data_size;
	uint32 m_outgoing_mask;
	uint32 m_pending_acknowledgement_mask;
	c_replication_outgoing_event* m_next;
};
static_assert(sizeof(c_replication_outgoing_event) == 0x2C);

struct c_replication_event_manager
{
	c_replication_entity_manager* m_entity_manager;
	c_simulation_event_handler* m_client;
	uint32 m_view_mask;
	c_replication_event_manager_view* m_views[16];
	int32 m_outgoing_event_count;
	c_replication_outgoing_event* m_outgoing_event_list;
};
static_assert(sizeof(c_replication_event_manager) == 0x54);

