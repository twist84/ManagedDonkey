#pragma once

#include "cseries/cseries.hpp"
#include "game/game_results.hpp"
#include "game/players.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/replication/replication_control_view.hpp"
#include "networking/replication/replication_entity_manager_view.hpp"
#include "networking/replication/replication_event_manager_view.hpp"
#include "networking/replication/replication_scheduler.hpp"
#include "simulation/simulation_view_telemetry.hpp"
#include "shell/shell.hpp"

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

struct c_simulation_distributed_view :
	s_datum_header
{
	c_replication_scheduler m_replication_scheduler;
	c_replication_entity_manager_view m_entity_view;
	c_replication_event_manager_view m_event_view;
	c_replication_control_view m_control_view;
	c_simulation_view_telemetry_provider m_telemetry_provider;
	c_game_results_replicator m_game_results_replicator;
};
static_assert(sizeof(c_simulation_distributed_view) == 0x22948);

enum e_simulation_view_establishment_mode;
enum e_network_synchronous_playback_control;
struct s_player_action;

enum e_simulation_view_synchronous_catchup_stage
{
	_synchronous_catchup_not_in_progress = 0,
	_synchronous_catchup_intiate,
	_synchronous_catchup_preparing_for_gamestate,
	_synchronous_catchup_in_progress,
	_synchronous_catchup_finish,
	_synchronous_catchup_client_decompressing_gamestate,

	k_synchronous_catchup_stage_count
};

enum e_synchronous_gamestate_message_type
{
	_synchronous_gamestate_message_initiate_join = 0,
	_synchronous_gamestate_message_gamestate_chunk,
	_synchronous_gamestate_message_checksums,

	k_number_of_synchronous_gamestate_message_types
};

struct s_network_message_synchronous_gamestate
{
	c_enum<e_synchronous_gamestate_message_type, byte, _synchronous_gamestate_message_initiate_join, k_number_of_synchronous_gamestate_message_types> message_type;

	union
	{
		long chunk_offset;
		long next_update_number;
		dword compressed_checksum;
	};

	long chunk_size;
	dword decompressed_checksum;
};
static_assert(sizeof(s_network_message_synchronous_gamestate) == 0x10);

struct c_simulation_world;
struct c_network_observer;
struct c_simulation_view :
	s_datum_header
{
	e_simulation_view_type view_type() const;
	char const* get_view_description() const;
	long get_view_establishment_identifier() const;
	e_simulation_view_establishment_mode get_view_establishment_mode() const;
	bool handle_distributed_game_results(long message_establishment_identifier, long incremental_update_number, s_game_results_incremental_update const* incremental_update);
	bool handle_player_acknowledge(dword player_valid_mask, dword player_in_game_mask, s_player_identifier const* player_identifiers);
	bool handle_remote_establishment(e_simulation_view_establishment_mode establishment_mode, long establishment_identifier, long signature_size, byte const* signature_data);
	bool handle_synchronous_acknowledge(long current_update_number);
	bool handle_synchronous_actions(long action_number, long current_action_number, dword user_flags, s_player_action const* actions);
	bool handle_synchronous_gamestate(s_network_message_synchronous_gamestate const* synchronous_gamestate, void const* chunk);
	bool handle_synchronous_playback_control(e_network_synchronous_playback_control type, long identifier, long update_number);
	bool handle_synchronous_update(struct simulation_update const* update);
	bool is_client_view() const;
	long synchronous_catchup_attempt_count() const;
	void synchronous_catchup_complete();
	bool synchronous_catchup_in_progress() const;
	bool synchronous_catchup_initiate();
	void synchronous_catchup_send_data();
	void synchronous_catchup_terminate();
	long synchronous_client_get_acknowledged_update_number();

	e_simulation_view_type m_view_type;
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
	long m_remote_establishment_mode;
	dword m_remote_establishment_identifier;
	c_network_channel* m_channel;
	dword m_channel_connection_identifier;
	c_network_channel_simulation_interface m_channel_interface;
	bool m_simulation_active;
	dword m_simulation_player_acknowledged_mask;
	long m_synchronous_received_action_number;
	long m_synchronous_acknowledged_update_number;
	long m_synchronous_catchup_attempt_count;
	long m_synchronous_catchup_start_update;
	long m_synchronous_catchup_time_of_last_activity;
	long m_synchronous_catchup_finish_time;
	long m_synchronous_catchup_buffer_offset;
	e_simulation_view_synchronous_catchup_stage m_synchronous_catchup_stage;
	s_network_message_synchronous_gamestate m_checksum_message;
	long m_synchronous_next_action_number;
};
static_assert(sizeof(c_simulation_view) == 0xC0);

