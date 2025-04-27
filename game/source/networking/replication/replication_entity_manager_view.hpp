#pragma once

#include "cseries/cseries.hpp"
#include "networking/replication/replication_scheduler.hpp"

struct s_replication_entity_view_data
{
	uint16 flags;
	short view_state;
	long entity_index;
	uint64 update_mask;

	uint8 __data[0x8];
};
static_assert(sizeof(s_replication_entity_view_data) == 0x18);

struct s_replication_entity_manager_view_statistics
{
	long creations_sent;
	long creations_pending;
	long updates_sent;
	long updates_pending;
	long deletions_sent;
	long deletions_pending;
};
static_assert(sizeof(s_replication_entity_manager_view_statistics) == 0x18);

struct c_replication_entity_manager;
struct c_replication_entity_packet_record;
struct c_replication_entity_status_record;
struct c_replication_entity_manager_view :
	public c_replication_scheduler_client
{
public:

//private:
	bool m_initialized;
	bool m_replicating;
	bool m_fatal_error;
	long m_view_index;
	uint32 m_view_mask;
	uint32 m_replication_start_time;
	c_replication_entity_manager* m_entity_manager;
	c_replication_entity_packet_record* m_packet_list;
	c_replication_entity_status_record* m_outgoing_packet;
	long m_current_absolute_index_position;
	s_replication_entity_view_data m_entity_data[1024];
	uint64 m_valid_entity_mask[16];
	s_replication_entity_manager_view_statistics m_statistics;
};
static_assert(sizeof(c_replication_entity_manager_view) == 0x60C0);

