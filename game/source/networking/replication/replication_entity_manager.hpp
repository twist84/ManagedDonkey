#pragma once

#include "cseries/cseries.hpp"

class c_replication_entity_manager_view;
class c_simulation_entity_database;

enum e_replication_entity_flags
{
	_replication_entity_allocated_flag = 0,
	_replication_entity_marked_for_deletion_flag,
	_replication_entity_local_flag,

	k_replication_entity_flags_count
};
using c_replication_entity_flags = c_flags<e_replication_entity_flags, uns8, k_replication_entity_flags_count>;

struct s_replication_entity_data
{
	c_replication_entity_flags flags;
	uns8 seed;
	uns16 deletion_mask;
};
COMPILE_ASSERT(sizeof(s_replication_entity_data) == 0x4);

class c_replication_entity_manager
{
public:
	c_simulation_entity_database* m_client;
	c_replication_entity_manager_view* m_views[16];
	uns32 m_view_mask;
	s_replication_entity_data m_entity_data[1024];
	int32 m_entity_creation_start_position;
};
COMPILE_ASSERT(sizeof(c_replication_entity_manager) == 0x104C);

