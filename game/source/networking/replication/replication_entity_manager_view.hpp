#pragma once

#include "cseries/cseries.hpp"
#include "networking/replication/replication_scheduler.hpp"

struct s_replication_entity_view_data
{
	uns16 flags;
	int16 view_state;
	int32 entity_index;
	uns64 update_mask;

	byte __data[0x8];
};
COMPILE_ASSERT(sizeof(s_replication_entity_view_data) == 0x18);

struct s_replication_entity_manager_view_statistics
{
	int32 creations_sent;
	int32 creations_pending;
	int32 updates_sent;
	int32 updates_pending;
	int32 deletions_sent;
	int32 deletions_pending;
};
COMPILE_ASSERT(sizeof(s_replication_entity_manager_view_statistics) == 0x18);

class c_replication_entity_manager;
class c_replication_entity_packet_record;
class c_replication_entity_status_record;
class c_replication_entity_manager_view :
	public c_replication_scheduler_client
{
public:

//private:
	bool m_initialized;
	bool m_replicating;
	bool m_fatal_error;
	int32 m_view_index;
	uns32 m_view_mask;
	uns32 m_replication_start_time;
	c_replication_entity_manager* m_entity_manager;
	c_replication_entity_packet_record* m_packet_list;
	c_replication_entity_status_record* m_outgoing_packet;
	int32 m_current_absolute_index_position;
	s_replication_entity_view_data m_entity_data[1024];
	uns64 m_valid_entity_mask[16];
	s_replication_entity_manager_view_statistics m_statistics;
};
COMPILE_ASSERT(sizeof(c_replication_entity_manager_view) == 0x60C0);

