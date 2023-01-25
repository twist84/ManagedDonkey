#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/replication/replication_control_view.hpp"
#include "networking/replication/replication_entity_manager_view.hpp"
#include "networking/replication/replication_event_manager_view.hpp"
#include "networking/replication/replication_scheduler.hpp"

enum e_simulation_view_reason
{
	_simulation_view_reason_none = 0,
	_simulation_view_reason_disconnected,
	_simulation_view_reason_out_of_sync,
	_simulation_view_reason_failed_to_join,
	_simulation_view_reason_blocking,
	_simulation_view_reason_catchup_fail,
	_simulation_view_reason_ended,
	_simulation_view_reason_mode_error,
	_simulation_view_reason_player_error,
	_simulation_view_reason_replication_entity,
	_simulation_view_reason_replication_event,
	_simulation_view_reason_replication_game_results,
	_simulation_view_reason_no_longer_authority,
	_simulation_view_reason_signature_invalid,

	k_simulation_view_reason_count
};

enum e_simulation_view_type
{
	_simulation_view_type_none = 0,
	_simulation_view_type_synchronous_to_remote_authority,
	_simulation_view_type_synchronous_to_remote_client,
	_simulation_view_type_distributed_to_remote_authority,
	_simulation_view_type_distributed_to_remote_client,

	k_simulation_view_type_count
};

/*
	built from `c_simulation_view::get_statistics`

	char* status_line = g_simulation_view_send_status_lines[world_view_index];
	status_line.print(
		"events%d/%d/%d updates%d/%d creations%d/%d deletions%d/%d",
		event_statistics.events_pending,
		event_statistics.events_in_transit,
		event_statistics.__unknown0,
		entity_statistics.updates_pending,
		entity_statistics.updates_unknown,
		entity_statistics.creations_pending,
		entity_statistics.creations_unknown,
		entity_statistics.deletions_pending,
		entity_statistics.deletions_unknown
	);
*/
struct s_simulation_view_statistics
{
	s_replication_entity_manager_view_statistics entity_statistics;
	s_replication_event_manager_view_statistics event_statistics;
};
static_assert(sizeof(s_simulation_view_statistics) == 0x24);

struct c_simulation_distributed_view
{
	byte __data0[0x4];
	c_replication_scheduler m_scheduler;
	c_replication_entity_manager_view m_entity_manager_view;
	c_replication_event_manager_view m_event_manager_view;
	c_replication_control_view m_control_view;
};
static_assert(sizeof(c_simulation_distributed_view) == 0x22948);

struct c_simulation_world;
struct c_network_observer;
struct c_simulation_view
{
	byte __data0[0x4];
	c_enum<e_simulation_view_type, long, k_simulation_view_type_count> m_view_type;
	long m_view_datum_index;
	c_simulation_distributed_view* m_distributed_view;
	c_simulation_world* m_world;
	long m_world_view_index;
	s_machine_identifier m_remote_machine_identifier;
	long m_remote_machine_index;
	c_network_observer* m_observer;
	long m_observer_channel_index;
	long m_view_death_reason;
	long m_view_establishment_mode;
	dword m_view_establishment_identifier;
	dword m_valid_view_establishment_identifier;
	byte __data44[0x4];
	c_network_channel* m_channel;
	dword m_channel_connection_identifier;
	c_network_channel_simulation_interface m_simulation_interface;
	bool m_simulation_active;
	dword_flags m_acknowledged_player_mask;

	// synchronous catchup data
	// edit: is this `c_simulation_view_synchronous_catchup_manager` from reach?
	byte __data8C[0x8];
	dword m_synchronous_catchup_attempt_count;
	dword m_synchronous_catchup_update_number;
	dword m_synchronous_catchup_finish_time;
	dword m_synchronous_catchup_unknownA0;
	dword m_synchronous_catchup_progress;
	dword m_synchronous_catchup_unknownA8;
	byte __dataAC[0x14];
};
static_assert(sizeof(c_simulation_view) == 0xC0);

