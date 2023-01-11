#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/replication/replication_control_view.hpp"
#include "networking/replication/replication_entity_manager_view.hpp"
#include "networking/replication/replication_event_manager_view.hpp"
#include "networking/replication/replication_scheduler.hpp"

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

	byte __data[0x22948 - 0x74F0];
	//s_game_results_incremental game_results_incremental;
};
static_assert(sizeof(c_simulation_distributed_view) == 0x22948);

struct c_simulation_world;
struct c_network_observer;
struct c_simulation_view
{
	byte __data0[0x4];
	long m_view_type;
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
	byte __data84[0x3C];
};
static_assert(sizeof(c_simulation_view) == 0xC0);
