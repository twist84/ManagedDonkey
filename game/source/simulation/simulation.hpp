#pragma once

#include "cseries/cseries.hpp"
#include "game/player_control.hpp"
#include "game/players.hpp"
#include "replication/replication_encoding.hpp"
#include "shell/shell.hpp"
#include "simulation/simulation_queue.hpp"
#include "simulation/simulation_view.hpp"
#include "units/units.hpp"

class c_wait_for_render_thread
{
public:
	c_wait_for_render_thread(char const* file, long line);
	~c_wait_for_render_thread();

protected:
	uint32 m_flags;
};

struct simulation_machine_update
{
	uint32 machine_valid_mask;
	c_static_array<s_machine_identifier, 17> machine_identifiers;
};
static_assert(sizeof(simulation_machine_update) == 0x114);

struct s_simulation_camera_update
{
	long camera_type;
	long player_absolute_index;
	real_point3d position;
	real_vector3d forward;
	bool zoomed;
};
static_assert(sizeof(s_simulation_camera_update) == 0x24);

struct simulation_update
{
	long update_number;

	c_flags<e_simulation_update_flags, uint16, k_simulation_update_flags_count> flags;

	uint32 player_flags;

	uint32 action_test_flags;
	c_static_array<player_action, 16> player_actions;

	uint32 valid_actor_mask;
	c_static_array<long, 16> actor_unit_indices;
	c_static_array<unit_control_data, 16> actor_control;

	bool machine_update_exists;
	simulation_machine_update machine_update;

	uint32 valid_player_prediction_mask;
	c_static_array<s_player_prediction, 16> player_prediction;

	long verify_game_time;
	long verify_random;

	//s_determinism_verification determinism_verification;

	uint32 valid_camera_mask;
	c_static_array<s_simulation_camera_update, 1> camera_updates;

	c_simulation_queue bookkeeping_simulation_queue;
	c_simulation_queue game_simulation_queue;
};
static_assert(sizeof(struct simulation_update) == 0x1658);

struct s_simulation_update_metadata
{
	c_flags<e_simulation_update_metadata_flags, uint16, k_simulation_update_metadata_flags_count> flags;

	long saved_film_position;
	long saved_film_tick;
};
static_assert(sizeof(s_simulation_update_metadata) == 0xC);

struct s_simulation_queued_update
{
	e_update_queue_node node_type;
	struct
	{
		struct simulation_update update;
		s_simulation_update_metadata metadata;
	} update;

	struct
	{
		e_simulation_playback_event event_type;
		long event_data;
		long event_update_number;
	} playback_event;
	s_simulation_queued_update* next_node;
};
static_assert(sizeof(s_simulation_queued_update) == 0x1680);

struct c_simulation_world;
struct c_simulation_watcher;
struct c_simulation_type_collection;
struct s_simulation_globals
{
	bool initialized;
	bool simulation_fatal_error;
	bool simulation_deferred;
	bool simulation_aborted;
	uint32 simulation_aborted_timestamp;
	e_simulation_abort_reason simulation_aborted_reason;
	bool simulation_in_initial_state;
	bool simulation_reset_in_progress;
	bool simulation_in_online_networked_session;
	bool gamestate_load_in_progress;
	bool recording_film;
	bool must_close_saved_film;
	bool performed_main_save_and_exit_campaign_immediately_this_map;
	c_simulation_world* world;
	c_simulation_watcher* watcher;
	c_simulation_type_collection* type_collection;
	s_data_array* view_data_array;
	char status_buffer[256];
	bool simulation_paused;
	bool handled_determinism_failure;
};
static_assert(sizeof(s_simulation_globals) == 0x128);

extern s_simulation_globals& simulation_globals;

struct s_simulation_player_netdebug_data;

extern void __cdecl simulation_abort_immediate(e_simulation_abort_reason abort_reason);
extern bool __cdecl simulation_aborted();
extern void __cdecl simulation_add_view_to_world(e_simulation_view_type view_type, s_machine_identifier const* remote_machine_identifier, long remote_machine_index, char const* remote_machine_name);
extern void __cdecl simulation_apply_after_game(struct simulation_update const* update);
extern void __cdecl simulation_apply_before_game(struct simulation_update const* update);
extern bool __cdecl simulation_boot_machine(s_machine_identifier const* machine, e_network_session_boot_reason boot_reason);
extern void __cdecl simulation_build_machine_update(bool* machine_update_valid, simulation_machine_update* machine_update);
extern void __cdecl simulation_build_player_updates();
extern void __cdecl simulation_build_update(bool should_build, struct simulation_update* update, s_simulation_update_metadata* metadata);
extern void __cdecl simulation_clear_errors();
extern void __cdecl simulation_describe_status(char* buffer, long buffer_size);
extern void __cdecl simulation_destroy_update(struct simulation_update* update);
extern void __cdecl simulation_dispose();
extern void __cdecl simulation_dispose_from_old_map();
extern void __cdecl simulation_end(e_simulation_abort_reason abort_reason);
extern void __cdecl simulation_fatal_error();
extern bool __cdecl simulation_film_retrieve_updates(long ticks_remaining, long* updates_read_out);
extern bool __cdecl simulation_film_start_recording();
extern void __cdecl simulation_film_stop_recording();
extern bool __cdecl simulation_format_player_netdebug_data(long player_index, s_simulation_player_netdebug_data const* netdebug_data, long* filled_bar_count);
extern char const* __cdecl simulation_get_abort_reason_string();
extern e_simulation_abort_reason __cdecl simulation_get_aborted_reason();
extern void __cdecl simulation_get_game_description(c_static_string<260>* game_description);
extern bool __cdecl simulation_get_machine_active_in_game(s_machine_identifier const* machine);
extern bool __cdecl simulation_get_machine_bandwidth_events(s_machine_identifier const* machine, long bandwidth_event_type_count, long* bandwidth_event_counters);
extern bool __cdecl simulation_get_machine_connectivity(s_machine_identifier const* machine);
extern bool __cdecl simulation_get_machine_is_host(s_machine_identifier const* machine);
extern uint32 __cdecl simulation_get_network_time_since_abort();
extern long __cdecl simulation_get_player_netdebug_filled_bar_count(long player_index);
extern c_simulation_view* __cdecl simulation_get_remote_view_by_channel(c_network_channel* channel);
extern char const* simulation_get_starting_up_description();
extern long __cdecl simulation_get_status();
extern c_simulation_type_collection* __cdecl simulation_get_type_collection();
extern bool __cdecl simulation_get_view_netdebug_data(c_simulation_view const* view, long* rtt_msec, long* packet_rate, long* bandwidth_bps, long* packet_loss);
extern c_simulation_world* __cdecl simulation_get_world();
extern c_simulation_world* __cdecl simulation_get_world_if_exists();
extern bool __cdecl simulation_in_progress();
extern void __cdecl simulation_initialize();
extern void __cdecl simulation_initialize_for_new_map();
extern void __cdecl simulation_initialize_for_saved_game(long game_state_proc_flags);
extern void __cdecl simulation_invalidate();
extern void __cdecl simulation_must_close_saved_film();
extern void __cdecl simulation_notify_channel_closure(void* simulation_context);
extern void __cdecl simulation_notify_core_save();
extern void __cdecl simulation_notify_going_active();
extern void __cdecl simulation_notify_initial_core_load(long update_number);
extern void __cdecl simulation_notify_players_created();
extern void __cdecl simulation_notify_reset_complete();
extern void __cdecl simulation_notify_reset_initiate();
extern void __cdecl simulation_notify_revert();
extern void __cdecl simulation_notify_saved_film_ended();
extern void __cdecl simulation_notify_saved_film_revert(long history_record_index, long next_update_number);
extern bool __cdecl simulation_performed_main_save_and_exit_campaign_immediately_this_map();
extern void __cdecl simulation_player_joined_game(long player_index);
extern void __cdecl simulation_player_left_game(long player_index);
extern void __cdecl simulation_prepare_to_load_saved_game(long game_state_proc_flags);
extern void __cdecl simulation_prepare_to_send();
extern void __cdecl simulation_process_actor_control(long simulation_actor_index, unit_control_data const* actor_control);
extern void __cdecl simulation_process_input(uint32 user_action_mask, player_action const* user_actions);
extern void __cdecl simulation_record_update(struct simulation_update const* update);
extern void __cdecl simulation_remove_view_from_world(c_simulation_view* view);
extern void __cdecl simulation_reset();
extern bool __cdecl simulation_reset_in_progress();
extern void __cdecl simulation_saved_film_revert(long history_record_index, long next_update_number);
extern void __cdecl simulation_set_performed_main_save_and_exit_campaign_immediately_this_map(bool save_and_quit_performed);
extern bool __cdecl simulation_should_transmit_simulation_data();
extern void __cdecl simulation_start();
extern bool __cdecl simulation_starting_up();
extern void __cdecl simulation_stop();
extern long __cdecl simulation_time_get_maximum_available(bool* match_remote_time);
extern void __cdecl simulation_update();
extern void __cdecl simulation_update_aftermath(struct simulation_update const* update, s_simulation_update_metadata* metadata);
extern void __cdecl simulation_update_out_of_sync();
extern bool __cdecl simulation_update_player_netdebug_data(long player_index, s_simulation_player_netdebug_data* netdebug_data);
extern void __cdecl simulation_update_pregame();
extern bool __cdecl simulation_update_read_from_buffer(struct simulation_update* update, long buffer_size, uint8 const* buffer);
extern bool __cdecl simulation_update_write_to_buffer(struct simulation_update const* update, long buffer_size, uint8* buffer, long* out_update_length);

extern void simulation_debug_render();

