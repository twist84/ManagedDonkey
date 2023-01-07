#pragma once

#include "cseries/cseries.hpp"
#include "networking/replication/replication_scheduler.hpp"

struct s_replication_entity_view_data
{
	word_flags flags;
	byte __data[0x16];
};
static_assert(sizeof(s_replication_entity_view_data) == 0x18);

struct c_replication_entity_manager_view : c_replication_scheduler_client
{
	byte __data4[0x24];

	c_static_array<s_replication_entity_view_data, 1024> m_views;

	byte __data[0x96];
};
static_assert(sizeof(c_replication_entity_manager_view) == 0x60C0);

