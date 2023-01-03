#pragma once

#include "cseries/cseries.hpp"

struct s_replication_entity_data
{
	byte_flags flags;
	byte seed;
	byte __data[2];
};
static_assert(sizeof(s_replication_entity_data) == 0x4);

struct c_simulation_entity_database;
struct c_replication_entity_manager_view;
struct c_replication_entity_manager
{
	c_simulation_entity_database* m_client;
	c_static_array<c_replication_entity_manager_view*, 16> m_views;
	dword m_view_mask;
	c_static_array<s_replication_entity_data, 1024> m_entity_data;
	dword __unknown1048;
};
static_assert(sizeof(c_replication_entity_manager) == 0x104C);

