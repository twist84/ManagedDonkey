#pragma once

#include "game/players.hpp"
#include "networking/replication/replication_entity_manager.hpp"
#include "networking/replication/replication_event_manager.hpp"
#include "simulation/simulation_actors.hpp"
#include "simulation/simulation_entity_database.hpp"
#include "simulation/simulation_players.hpp"
#include "simulation/simulation_queue.hpp"

struct s_simulation_world_view_iterator
{
	uns32 view_type_mask;
	int32 next_world_view_index;
};
static_assert(sizeof(s_simulation_world_view_iterator) == 0x8);

class c_simulation_distributed_world
{
public:
	c_replication_entity_manager m_entity_manager;
	c_replication_event_manager m_event_manager;
	c_simulation_entity_database m_entity_database;
	c_simulation_event_handler m_event_handler;
};
static_assert(sizeof(c_simulation_distributed_world) == 0xD0C8);

class c_network_channel;
class c_simulation_watcher;
class c_simulation_view;
struct s_simulation_queued_update;
struct s_simulation_update_metadata;
class c_simulation_world
{
public:
	enum e_join_progress
	{
		_join_progress_waiting = 0,
		_join_progress_ready,
		_join_progress_complete,
		_join_progress_failed,

		k_join_progress_count,
	};

	struct s_world_state_data_disconnected
	{
		uns32 disconnected_timestamp;
	};
	static_assert(sizeof(s_world_state_data_disconnected) == 0x4);

	struct s_world_state_data_joining
	{
		uns32 join_start_timestamp;
		uns32 join_client_machine_mask;
	};
	static_assert(sizeof(s_world_state_data_joining) == 0x8);

	struct s_world_state_data_active
	{
		uns32 active_client_machine_mask;
	};
	static_assert(sizeof(s_world_state_data_active) == 0x4);

	struct s_world_state_data
	{
		union
		{
			s_world_state_data_disconnected disconnected;
			s_world_state_data_joining joining;
			s_world_state_data_active active;
		};
	};
	static_assert(sizeof(s_world_state_data) == 0x8);

public:
	bool actor_exists(int32 simulation_actor_index) const;
	void advance_update(const struct simulation_update* update);
	bool all_client_views_active() const;
	void apply_simulation_queue(const c_simulation_queue* simulation_queue);

private:
	void attach_simulation_queues_to_update(struct simulation_update* update);

public:
	void attach_to_map();
	int32 attach_view(c_simulation_view* view);

private:
	bool authority_join_timeout_expired() const;

public:
	void build_player_actions(struct simulation_update* update);
	void build_update(bool build_for_simulation_in_progress, struct simulation_update* update, s_simulation_update_metadata* metadata);
	bool can_generate_updates() const;
	void change_state_active();
	void change_state_dead();
	void change_state_disconnected();
	void change_state_internal(e_simulation_world_state new_state);
	void change_state_joining(uns32 joining_client_machine_mask);
	void change_state_leaving();
	void claim_authority_gameworld();
	int32 control_stream_get_unit_index(int32 control_index) const;
	int32 create_actor(int32 simulation_actor_index, int32 unit_index);
	void create_player(int32 player_index, e_game_simulation_type simulation_type);
	void deactivate_all_players();
	void debug_render();
	void delete_actor(int32 simulation_actor_index);

private:
	void delete_all_actors();

public:
	void delete_all_players();
	void delete_player(int32 player_index);
	static void destroy_update(struct simulation_update* update);
	void destroy_world();
	void detach_from_map();
	void detach_view(c_simulation_view* view, int32 view_index);
	void disconnect();
	void distribute_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata);

private:
	void distributed_authority_dispatch_actor_control(uns32 actor_valid_mask, const unit_control_data* actor_control);
	void distributed_authority_dispatch_player_actions(uns32 player_valid_mask, const player_action* player_actions);

public:
	void drop_simulation_from_active_to_joining();
	bool exists() const;
	c_simulation_player* find_player_by_machine(const s_machine_identifier* machine_identifier, int32 user_index);
	void gamestate_flush();

private:
	uns32 get_acknowledged_player_mask() const;

public:
	c_simulation_view* get_authority_view() const;
	c_simulation_view* get_client_view_by_machine_identifier(const s_machine_identifier* remote_machine_identifier);
	c_simulation_view* get_client_view_by_machine_index(int32 remote_machine_index);
	void get_disconnected_status(int32* disconnected_time_elapsed, int32* disconnected_time_to_failure) const;
	bool get_failure_status(int32* failure_count, int32* failure_timeout) const;
	void get_join_status(
		int32* join_time_elapsed,
		int32* join_time_to_abort,
		int32* join_attempt_count,
		int32* join_attempt_maximum,
		int32* join_client_establishing_count,
		int32* join_client_waiting_count,
		int32* join_client_joining_count,
		int32* join_client_complete_count,
		int32* join_client_total_count,
		int32* join_time_to_failure) const;
	void get_machine_identifier(s_machine_identifier* identifier) const;
	int32 get_machine_index() const;
	int32 get_machine_index_by_identifier(const s_machine_identifier* remote_machine_identifier) const;
	int32 get_next_update_number() const;
	uns32 get_player_active_mask() const;
	uns32 get_player_exists_mask() const;
	uns32 get_player_in_game_mask() const;
	void get_player_machine(int32 player_index, s_machine_identifier* machine_identifier) const;
	e_simulation_world_state get_state() const;
	static const char* get_state_string(int32 state);
	void get_synchronous_client_status(int32* gamestate_write_progress) const;
	static const char* get_type_string(int32 world_type);
	void get_update_queue_status(int32* next_update_dequeue, int32* next_update_expected) const;
	int32 get_time() const;
	c_simulation_view* get_view_by_channel(const c_network_channel* channel);
	c_simulation_view* get_view_by_observer(int32 observer_channel_index);
	int32 get_view_count() const;
	e_simulation_world_type get_world_type() const;
	void go_out_of_sync(bool determinism_failure);
	bool handle_playback_update(const struct simulation_update* update, s_simulation_update_metadata* metadata);
	void handle_synchronous_client_actions(const s_machine_identifier* remote_machine_identifier, uns32 valid_user_mask, const player_action* user_actions);
	bool handle_synchronous_playback_control(e_network_synchronous_playback_control type, int32 identifier, int32 update_number);
	bool handle_synchronous_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata);
	void handle_view_activation(c_simulation_view* view, bool active);
	void handle_view_establishment(c_simulation_view* view, bool established);
	void initialize_world(e_game_simulation_type simulation_type, e_game_playback_type playback_type, bool reset_next_update_number, c_simulation_type_collection* type_collection, c_simulation_watcher* watcher, c_simulation_distributed_world* distributed_world);
	bool is_active() const;
	bool is_authority() const;
	bool is_connected() const;
	bool is_dead() const;
	bool is_distributed() const;
	bool is_joining() const;
	bool is_local() const;
	bool is_out_of_sync() const;
	bool is_playback() const;
	bool is_synchronous() const;
	static void iterator_begin(s_simulation_world_view_iterator* iterator, uns32 view_type_mask);
	bool iterator_next(s_simulation_world_view_iterator* iterator, c_simulation_view** view) const;
	void mark_player_pending_deletion(int32 player_index);
	void notify_gamestate_flush();
	void notify_gamestate_flush_outside_game_tick();
	void notify_initial_gamestate_load(int32 update_number);
	void notify_playback_control(e_network_synchronous_playback_control type, int32 identifier, int32 update_number);
	bool player_exists(int32 player_index, s_player_identifier* out_player_identifier) const;
	bool player_is_active(int32 player_index) const;
	bool player_is_in_game(int32 player_index, const s_player_identifier* player_identifier) const;
	void process_actor_control(int32 simulation_actor_index, const unit_control_data* actor_control);
	void process_input(uns32 user_action_mask, struct player_action* user_actions);
	void process_pending_updates();
	void process_playback_events();
	void recreate_players(e_game_simulation_type simulation_type);
	void remove_all_views();
	void reset_world();
	bool runs_simulation() const;
	void send_player_acknowledgements(bool force_acknowledgement);
	void send_synchronous_acknowledgements(bool force_acknowledgement);
	void set_machine_identifier(const s_machine_identifier* identifier);
	void set_machine_index(int32 machine_index);
	void simulation_queue_allocate(e_simulation_queue_element_type type, int32 data_size, s_simulation_queue_element** element_out);
	void simulation_queue_enqueue(s_simulation_queue_element* element);
	void simulation_queue_free(s_simulation_queue_element* element);
	bool simulation_queues_empty();

private:
	void synchronous_authority_dispatch_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata);
	int32 synchronous_authority_get_maximum_update_queue_size();
	int32 synchronous_authority_get_maximum_updates();

public:
	bool synchronous_catchup_in_progress() const;
	int32 time_get_available(bool* out_match_remote_time, int32* out_updates_available);
	bool time_running() const;
	void time_set_immediate_update(bool immediate_update);
	void time_set_next_update_number(int32 next_update_number, bool flush_update_queue);
	void time_start(int32 next_update_number, bool flush_update_queue);
	void time_stop();
	void update();

private:
	void update_authority_active();
	void update_authority_join_initiate();
	void update_authority_join_progress();
	void update_client_disconnection();
	void update_client_failure();
	void update_client_join_initiate();
	void update_client_join_progress();
	void update_establishing_view(c_simulation_view* view);
	e_join_progress update_joining_view(c_simulation_view* view);
	void update_player_activation();
	int32 update_queue_get_available_updates() const;
	int32 update_queue_get_next_expected_update_number() const;
	bool update_queue_handle_playback_event(e_simulation_playback_event event_type, int32 event_data, int32 event_update_number);
	bool update_queue_handle_server_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata);
	void update_queue_reset();
	bool update_queue_retrieve_event(e_simulation_playback_event* out_event_type, int32* out_event_data, int32* out_event_update_number);
	void update_queue_retrieve_update(struct simulation_update* update, s_simulation_update_metadata* metadata);
	void update_queue_start(int32 next_update_number, bool flush_update_queue);
	void update_queue_stop();

public:
	bool view_has_acknowledged_active_players(const c_simulation_view* view) const;

//protected:
	c_simulation_watcher* m_watcher;
	c_simulation_distributed_world* m_distributed_world;
	e_simulation_world_type m_world_type;
	bool m_local_machine_identifier_valid;
#pragma pack(push, 1)
	s_machine_identifier m_local_machine_identifier;
#pragma pack(pop)
	int32 m_local_machine_index;
	e_simulation_world_state m_world_state;
	s_world_state_data m_world_state_data;
	bool m_time_running;
	bool m_time_immediate_update;
	int32 m_next_update_number;
	int32 m_acknowledged_update_number;
	bool m_out_of_sync;
	bool m_out_of_sync_determinism_failure;
	bool m_world_clean;
	bool m_gamestate_flushed;
	bool m_gamestate_flushed_outside_game_tick;
	bool m_gamestate_modified_initial_state;
	bool m_attached_to_map;
	bool m_gamestate_flush_client_skip;
	int32 m_unsuccessful_join_attempts;
	uns32 m_last_active_timestamp;
	int32 m_next_view_establishment_identifier;
	int32 m_joining_total_wait_msec;
	int32 m_maximum_queued_updates;
	e_update_queue_state m_update_queue_state;
	uns32 m_update_queue_state_time;
	int32 m_view_count;
	c_simulation_view* m_views[k_simulation_world_maximum_views];
	c_simulation_player m_players[16];
	c_simulation_actor m_actors[16];
	uns32 m_synchronous_catchup_initiation_failure_timestamp;
	int32 m_update_queue_next_update_number_to_dequeue;
	e_update_queue_node m_update_queue_latest_entry_received_type;
	int32 m_update_queue_latest_entry_received_update_number;
	int32 m_update_queue_length;
	int32 m_update_queue_number_of_updates;
	s_simulation_queued_update* m_update_queue_head;
	s_simulation_queued_update* m_update_queue_tail;
	c_simulation_queue m_bookkeeping_simulation_queue;
	c_simulation_queue m_game_simulation_queue;
};
static_assert(sizeof(c_simulation_world) == 0x1540);
static_assert(0x0000 == OFFSETOF(c_simulation_world, m_watcher));
static_assert(0x0004 == OFFSETOF(c_simulation_world, m_distributed_world));
static_assert(0x0008 == OFFSETOF(c_simulation_world, m_world_type));
static_assert(0x000C == OFFSETOF(c_simulation_world, m_local_machine_identifier_valid));
static_assert(0x000D == OFFSETOF(c_simulation_world, m_local_machine_identifier));
static_assert(0x0020 == OFFSETOF(c_simulation_world, m_local_machine_index));
static_assert(0x0024 == OFFSETOF(c_simulation_world, m_world_state));
static_assert(0x0028 == OFFSETOF(c_simulation_world, m_world_state_data));
static_assert(0x0030 == OFFSETOF(c_simulation_world, m_time_running));
static_assert(0x0031 == OFFSETOF(c_simulation_world, m_time_immediate_update));
static_assert(0x0034 == OFFSETOF(c_simulation_world, m_next_update_number));
static_assert(0x0038 == OFFSETOF(c_simulation_world, m_acknowledged_update_number));
static_assert(0x003C == OFFSETOF(c_simulation_world, m_out_of_sync));
static_assert(0x003D == OFFSETOF(c_simulation_world, m_out_of_sync_determinism_failure));
static_assert(0x003E == OFFSETOF(c_simulation_world, m_world_clean));
static_assert(0x003F == OFFSETOF(c_simulation_world, m_gamestate_flushed));
static_assert(0x0040 == OFFSETOF(c_simulation_world, m_gamestate_flushed_outside_game_tick));
static_assert(0x0041 == OFFSETOF(c_simulation_world, m_gamestate_modified_initial_state));
static_assert(0x0042 == OFFSETOF(c_simulation_world, m_attached_to_map));
static_assert(0x0043 == OFFSETOF(c_simulation_world, m_gamestate_flush_client_skip));
static_assert(0x0044 == OFFSETOF(c_simulation_world, m_unsuccessful_join_attempts));
static_assert(0x0048 == OFFSETOF(c_simulation_world, m_last_active_timestamp));
static_assert(0x004C == OFFSETOF(c_simulation_world, m_next_view_establishment_identifier));
static_assert(0x0050 == OFFSETOF(c_simulation_world, m_joining_total_wait_msec));
static_assert(0x0054 == OFFSETOF(c_simulation_world, m_maximum_queued_updates));
static_assert(0x0058 == OFFSETOF(c_simulation_world, m_update_queue_state));
static_assert(0x005C == OFFSETOF(c_simulation_world, m_update_queue_state_time));
static_assert(0x0060 == OFFSETOF(c_simulation_world, m_view_count));
static_assert(0x0064 == OFFSETOF(c_simulation_world, m_views));
static_assert(0x00A8 == OFFSETOF(c_simulation_world, m_players));
static_assert(0x0BA8 == OFFSETOF(c_simulation_world, m_actors));
static_assert(0x14E8 == OFFSETOF(c_simulation_world, m_synchronous_catchup_initiation_failure_timestamp));
static_assert(0x14EC == OFFSETOF(c_simulation_world, m_update_queue_next_update_number_to_dequeue));
static_assert(0x14F0 == OFFSETOF(c_simulation_world, m_update_queue_latest_entry_received_type));
static_assert(0x14F4 == OFFSETOF(c_simulation_world, m_update_queue_latest_entry_received_update_number));
static_assert(0x14F8 == OFFSETOF(c_simulation_world, m_update_queue_length));
static_assert(0x14FC == OFFSETOF(c_simulation_world, m_update_queue_number_of_updates));
static_assert(0x1500 == OFFSETOF(c_simulation_world, m_update_queue_head));
static_assert(0x1504 == OFFSETOF(c_simulation_world, m_update_queue_tail));
static_assert(0x1508 == OFFSETOF(c_simulation_world, m_bookkeeping_simulation_queue));
static_assert(0x1524 == OFFSETOF(c_simulation_world, m_game_simulation_queue));

