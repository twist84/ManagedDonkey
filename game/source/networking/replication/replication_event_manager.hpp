#pragma once

#include "cseries/cseries.hpp"

struct c_simulation_event_handler;
struct c_replication_event_manager_view;
struct c_replication_event_manager
{
	byte __data0[4];
	c_simulation_event_handler* m_event_handler;
	byte __data8[4];
	c_static_array<c_replication_event_manager_view*, 16> m_event_manager_views;
	byte __data[8];
};
static_assert(sizeof(c_replication_event_manager) == 0x54);

