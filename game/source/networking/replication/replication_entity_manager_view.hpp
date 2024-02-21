#pragma once

#include "cseries/cseries.hpp"
#include "networking/replication/replication_scheduler.hpp"

struct s_replication_entity_view_data
{
	word_flags flags;
	short view_state;
	long entity_index;
	qword_flags update_mask;

	byte __data[0x8];
};
static_assert(sizeof(s_replication_entity_view_data) == 0x18);

struct s_replication_entity_manager_view_statistics
{
	long creations_unknown;
	long creations_pending;

	long updates_unknown;
	long updates_pending;

	long deletions_unknown;
	long deletions_pending;
};
static_assert(sizeof(s_replication_entity_manager_view_statistics) == 0x18);

struct c_replication_entity_manager;
struct c_replication_entity_packet_record;
struct c_replication_entity_status_record;
struct c_replication_entity_manager_view :
	c_replication_scheduler_client
{
	byte __data4[0x4];

	bool m_initialized;
	bool m_replicating;
	bool m_fatal_error;
	byte __unknownB; // pad?

	long m_view_index;
	dword m_view_mask;

	dword m_replication_start_time;

	c_replication_entity_manager* m_entity_manager;
	c_replication_entity_packet_record* m_packet_records;
	c_replication_entity_status_record* m_outgoing_packet;

	byte __data14[0x4];

	c_static_array<s_replication_entity_view_data, 1024> m_views;
	c_static_flags<1024> __flags6028;
	s_replication_entity_manager_view_statistics m_statistics;
};
static_assert(sizeof(c_replication_entity_manager_view) == 0x60C0);

