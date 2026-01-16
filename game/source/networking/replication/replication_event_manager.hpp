#pragma once

#include "cseries/cseries.hpp"

class c_replication_entity_manager;
class c_replication_event_manager_view;
class c_simulation_event_handler;

enum e_replication_event_state
{
	_replication_event_state_initializing = 0,
	_replication_event_state_ready,

	k_replication_event_state_count
};


class c_replication_outgoing_event
{
public:
	e_replication_event_state m_state;
	int32 m_type;
	uns32 m_timestamp;
	int32 m_cancel_timer_milliseconds;
	int32 m_entity_references[2];
	void* m_payload_data;
	int32 m_payload_data_size;
	uns32 m_outgoing_mask;
	uns32 m_pending_acknowledgement_mask;
	c_replication_outgoing_event* m_next;
};
COMPILE_ASSERT(sizeof(c_replication_outgoing_event) == 0x2C);

class c_replication_event_manager
{
public:
	c_replication_entity_manager* m_entity_manager;
	c_simulation_event_handler* m_client;
	uns32 m_view_mask;
	c_replication_event_manager_view* m_views[16];
	int32 m_outgoing_event_count;
	c_replication_outgoing_event* m_outgoing_event_list;
};
COMPILE_ASSERT(sizeof(c_replication_event_manager) == 0x54);

