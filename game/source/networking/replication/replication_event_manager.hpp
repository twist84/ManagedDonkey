#pragma once

#include "cseries/cseries.hpp"

struct c_replication_outgoing_event
{
	long m_state;
	long m_type;
	dword m_ready_time;
	dword m_cancel_timer;
	long m_entity_references[2];
	void* m_payload;
	long m_payload_size;
	dword m_outgoing_mask;
	dword m_pending_acknowledgement_mask;
	dword __unknown28;
};
static_assert(sizeof(c_replication_outgoing_event) == 0x2C);

struct c_replication_entity_manager;
struct c_simulation_event_handler;
struct c_replication_event_manager_view;
struct c_replication_event_manager
{
	c_replication_entity_manager* m_entity_manager;
	c_simulation_event_handler* m_event_handler;
	dword m_view_mask;
	c_static_array<c_replication_event_manager_view*, 16> m_views;
	long m_outgoing_event_count;
	c_replication_outgoing_event* m_outgoing_event_list;
};
static_assert(sizeof(c_replication_event_manager) == 0x54);

