#pragma once

#include "cseries/cseries.hpp"
#include "networking/replication/replication_scheduler.hpp"

struct s_replication_event_manager_view_statistics
{
	long __unknown0;
	long events_pending;
	long events_in_transit;
};
static_assert(sizeof(s_replication_event_manager_view_statistics) == 0xC);

struct c_replication_event_manager;
struct c_replication_event_manager_view : c_replication_scheduler_client
{
	byte __data4[0x14];
	c_replication_event_manager* m_event_manager;
	s_replication_event_manager_view_statistics m_view_statistics;
};
static_assert(sizeof(c_replication_event_manager_view) == 0x28);

