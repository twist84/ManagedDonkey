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

struct s_simulation_view_statistics
{
	s_replication_entity_manager_view_statistics entity;
	s_replication_event_manager_view_statistics event;
};
COMPILE_ASSERT(sizeof(s_simulation_view_statistics) == 0x24);

class c_simulation_distributed_view :
	public s_datum_header
{
public:
	c_replication_scheduler m_replication_scheduler;
	c_replication_entity_manager_view m_entity_view;
	c_replication_event_manager_view m_event_view;
	c_replication_control_view m_control_view;
	c_simulation_view_telemetry_provider m_telemetry_provider;
	c_game_results_replicator m_game_results_replicator;
};
COMPILE_ASSERT(sizeof(c_simulation_distributed_view) == 0x22948);

struct player_action;

struct s_network_message_synchronous_gamestate
{
	c_enum<e_synchronous_gamestate_message_type, uns8, _synchronous_gamestate_message_initiate_join, k_number_of_synchronous_gamestate_message_types> message_type;

	union
	{
		int32 chunk_offset;
		int32 next_update_number;
		uns32 compressed_checksum;
	};

	int32 chunk_size;
	uns32 decompressed_checksum;
};
COMPILE_ASSERT(sizeof(s_network_message_synchronous_gamestate) == 0x10);

class c_simulation_world;
class c_network_observer;
class c_simulation_view :
	public s_datum_header
{
public:
	bool client_join_is_finished() const;
	bool established() const;
	bool exists() const;
	e_simulation_view_type view_type() const;
	void get_statistics(s_simulation_view_statistics* statistics);
	int32 get_remote_establishment_identifier() const;
	e_simulation_view_establishment_mode get_remote_establishment_mode() const;
	static const char* get_type_string(int32 view_type);
	const char* get_view_description() const;
	int32 get_view_establishment_identifier() const;
	e_simulation_view_establishment_mode get_view_establishment_mode() const;
	int32 get_world_view_index() const;
	bool handle_distributed_game_results(int32 message_establishment_identifier, int32 incremental_update_number, const s_game_results_incremental_update* incremental_update);
	bool handle_player_acknowledge(uns32 player_valid_mask, uns32 player_in_game_mask, const s_player_identifier* player_identifiers);
	bool handle_remote_establishment(e_simulation_view_establishment_mode establishment_mode, int32 establishment_identifier, int32 signature_size, const uns8* signature_data);
	bool handle_synchronous_acknowledge(int32 current_update_number);
	bool handle_synchronous_actions(int32 action_number, int32 current_action_number, uns32 user_flags, const player_action* actions);
	bool handle_synchronous_gamestate(const s_network_message_synchronous_gamestate* synchronous_gamestate, const void* chunk);
	bool handle_synchronous_playback_control(e_network_synchronous_playback_control type, int32 identifier, int32 update_number);
	bool handle_synchronous_update(const s_simulation_update* update);
	bool is_client_view() const;
	bool is_dead(int32* death_reason) const;
	int32 synchronous_catchup_attempt_count() const;
	void synchronous_catchup_complete();
	bool synchronous_catchup_in_progress() const;
	bool synchronous_catchup_initiate();
	void synchronous_catchup_send_data();
	void synchronous_catchup_terminate();
	int32 synchronous_client_get_acknowledged_update_number();

	e_simulation_view_type m_view_type;
	int32 m_view_datum_index;
	c_simulation_distributed_view* m_distributed_view;
	c_simulation_world* m_world;
	int32 m_world_view_index;
	s_machine_identifier m_remote_machine_identifier;
	int32 m_remote_machine_index;
	c_network_observer* m_observer;
	int32 m_observer_channel_index;
	int32 m_view_death_reason;
	int32 m_view_establishment_mode;
	uns32 m_view_establishment_identifier;
	e_simulation_view_establishment_mode m_remote_establishment_mode;
	uns32 m_remote_establishment_identifier;
	c_network_channel* m_channel;
	uns32 m_channel_connection_identifier;
	c_network_channel_simulation_interface m_channel_interface;
	bool m_simulation_active;
	uns32 m_simulation_player_acknowledged_mask;
	int32 m_synchronous_received_action_number;
	int32 m_synchronous_acknowledged_update_number;
	int32 m_synchronous_catchup_attempt_count;
	int32 m_synchronous_catchup_start_update;
	int32 m_synchronous_catchup_time_of_last_activity;
	int32 m_synchronous_catchup_finish_time;
	int32 m_synchronous_catchup_buffer_offset;
	e_simulation_view_synchronous_catchup_stage m_synchronous_catchup_stage;
	s_network_message_synchronous_gamestate m_checksum_message;
	int32 m_synchronous_next_action_number;
};
COMPILE_ASSERT(sizeof(c_simulation_view) == 0xC0);

