#pragma once

#include "cseries/cseries.hpp"
#include "game/player_control.hpp"
#include "game/players.hpp"
#include "replication/replication_encoding.hpp"
#include "simulation/simulation_queue.hpp"
#include "simulation/simulation_view.hpp"
#include "units/units.hpp"

struct s_simulation_player_netdebug_data;

#define RENDER_THREAD_LOCK c_wait_for_render_thread CONCAT(__render_thread_lock, __LINE__)(__FILE__, __LINE__)
class c_wait_for_render_thread
{
public:
	c_wait_for_render_thread(const char* file, int32 line);
	~c_wait_for_render_thread();

protected:
	uns32 m_flags;
};

struct simulation_machine_update
{
	uns32 machine_valid_mask;
	c_static_array<s_machine_identifier, 17> machine_identifiers;
};
static_assert(sizeof(simulation_machine_update) == 0x114);

struct s_simulation_camera_update
{
	int32 camera_type;
	int32 player_absolute_index;
	real_point3d position;
	real_vector3d forward;
	bool zoomed;
};
static_assert(sizeof(s_simulation_camera_update) == 0x24);

struct simulation_update
{
	int32 update_number;

	c_flags<e_simulation_update_flags, uns16, k_simulation_update_flags_count> flags;

	uns32 player_flags;

	uns32 action_test_flags;
	player_action player_actions[16];

	uns32 valid_actor_mask;
	int32 actor_unit_indices[16];
	unit_control_data actor_control[16];

	bool machine_update_exists;
	simulation_machine_update machine_update;

	uns32 valid_player_prediction_mask;
	s_player_prediction player_prediction[16];

	int32 verify_game_time;
	int32 verify_random;

	//s_determinism_verification determinism_verification;

	uns32 valid_camera_update_mask;
	s_simulation_camera_update camera_updates[1];

	c_simulation_queue bookkeeping_simulation_queue;
	c_simulation_queue game_simulation_queue;
};
static_assert(sizeof(struct simulation_update) == 0x1658);

struct s_simulation_update_metadata
{
	c_flags<e_simulation_update_metadata_flags, uns16, k_simulation_update_metadata_flags_count> flags;

	int32 saved_film_position;
	int32 saved_film_tick;
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
		int32 event_data;
		int32 event_update_number;
	} playback_event;
	s_simulation_queued_update* next_node;
};
static_assert(sizeof(s_simulation_queued_update) == 0x1680);

class c_simulation_world;
class c_simulation_watcher;
class c_simulation_type_collection;
struct s_simulation_globals
{
	bool initialized;
	bool simulation_fatal_error;
	bool simulation_deferred;
	bool simulation_aborted;
	uns32 simulation_aborted_timestamp;
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

extern int32 g_simulation_bandwidth_eater;
extern bool g_simulation_status_world;
extern bool g_simulation_status_views;

extern void __cdecl simulation_abort_immediate(e_simulation_abort_reason abort_reason);
extern bool __cdecl simulation_aborted();
extern void __cdecl simulation_add_view_to_world(e_simulation_view_type view_type, const s_machine_identifier* remote_machine_identifier, int32 remote_machine_index, const char* remote_machine_name);
extern void __cdecl simulation_apply_after_game(const struct simulation_update* update);
extern void __cdecl simulation_apply_before_game(const struct simulation_update* update);
extern bool __cdecl simulation_boot_machine(const s_machine_identifier* machine, e_network_session_boot_reason boot_reason);
extern void __cdecl simulation_build_machine_update(bool* machine_update_valid, simulation_machine_update* machine_update);
extern void __cdecl simulation_build_player_updates();
extern void __cdecl simulation_build_update(bool should_build, struct simulation_update* update, s_simulation_update_metadata* metadata);
extern void __cdecl simulation_clear_errors();
extern void __cdecl simulation_describe_status(char* buffer, int32 buffer_size);
extern void __cdecl simulation_destroy_update(struct simulation_update* update);
extern void __cdecl simulation_dispose();
extern void __cdecl simulation_dispose_from_old_map();
extern void __cdecl simulation_end(e_simulation_abort_reason abort_reason);
extern bool __cdecl simulation_film_record_update(const struct simulation_update* update);
extern void __cdecl simulation_fatal_error();
extern bool __cdecl simulation_film_retrieve_updates(int32 maximum_updates_to_read, int32* updates_read_out);
extern bool __cdecl simulation_film_start_recording();
extern void __cdecl simulation_film_stop_recording();
extern bool __cdecl simulation_format_player_netdebug_data(int32 player_index, const s_simulation_player_netdebug_data* netdebug_data, int32* filled_bar_count);
extern const char* __cdecl simulation_get_abort_reason_string();
extern e_simulation_abort_reason __cdecl simulation_get_aborted_reason();
extern void __cdecl simulation_get_game_description(c_static_string<260>* game_description);
extern bool __cdecl simulation_get_machine_active_in_game(const s_machine_identifier* machine);
extern bool __cdecl simulation_get_machine_bandwidth_events(const s_machine_identifier* machine, int32 bandwidth_event_type_count, int32* bandwidth_event_counters);
extern bool __cdecl simulation_get_machine_connectivity(const s_machine_identifier* machine);
extern bool __cdecl simulation_get_machine_is_host(const s_machine_identifier* machine);
extern uns32 __cdecl simulation_get_network_time_since_abort();
extern int32 __cdecl simulation_get_player_netdebug_filled_bar_count(int32 player_index);
extern c_simulation_view* __cdecl simulation_get_remote_view_by_channel(c_network_channel* channel);
extern const char* simulation_get_starting_up_description();
extern int32 __cdecl simulation_get_status();
extern c_simulation_type_collection* __cdecl simulation_get_type_collection();
extern bool __cdecl simulation_get_view_netdebug_data(const c_simulation_view* view, int32* rtt_msec, int32* packet_rate, int32* bandwidth_bps, int32* packet_loss);
extern c_simulation_world* __cdecl simulation_get_world();
extern c_simulation_world* __cdecl simulation_get_world_if_exists();
extern bool __cdecl simulation_in_progress();
extern void __cdecl simulation_initialize();
extern void __cdecl simulation_initialize_for_new_map();
extern void __cdecl simulation_initialize_for_saved_game(int32 game_state_proc_flags);
extern void __cdecl simulation_invalidate();
extern void __cdecl simulation_must_close_saved_film();
extern void __cdecl simulation_notify_channel_closure(void* simulation_context);
extern void __cdecl simulation_notify_core_save();
extern void __cdecl simulation_notify_going_active();
extern void __cdecl simulation_notify_initial_core_load(int32 update_number);
extern void __cdecl simulation_notify_players_created();
extern void __cdecl simulation_notify_reset_complete();
extern void __cdecl simulation_notify_reset_initiate();
extern void __cdecl simulation_notify_revert();
extern void __cdecl simulation_notify_saved_film_ended();
extern void __cdecl simulation_notify_saved_film_revert(int32 history_record_index, int32 next_update_number);
extern bool __cdecl simulation_performed_main_save_and_exit_campaign_immediately_this_map();
extern void __cdecl simulation_player_joined_game(int32 player_index);
extern void __cdecl simulation_player_left_game(int32 player_index);
extern void __cdecl simulation_prepare_to_load_saved_game(int32 game_state_proc_flags);
extern void __cdecl simulation_prepare_to_send();
extern void __cdecl simulation_process_actor_control(int32 simulation_actor_index, const unit_control_data* actor_control);
extern void __cdecl simulation_process_input(uns32 user_action_mask, const player_action* user_actions);
extern void __cdecl simulation_record_update(const struct simulation_update* update);
extern void __cdecl simulation_remove_view_from_world(c_simulation_view* view);
extern void __cdecl simulation_reset();
extern bool __cdecl simulation_reset_in_progress();
extern void __cdecl simulation_saved_film_revert(int32 history_record_index, int32 next_update_number);
extern void __cdecl simulation_set_performed_main_save_and_exit_campaign_immediately_this_map(bool save_and_quit_performed);
extern bool __cdecl simulation_should_transmit_simulation_data();
extern void __cdecl simulation_start();
extern bool __cdecl simulation_starting_up();
extern void __cdecl simulation_status_lines_update();
extern void __cdecl simulation_stop();
extern void __cdecl simulation_test_update();
extern int32 __cdecl simulation_time_get_maximum_available(bool* match_remote_time);
extern void __cdecl simulation_update();
extern void __cdecl simulation_update_aftermath(const struct simulation_update* update, s_simulation_update_metadata* metadata);
extern void __cdecl simulation_update_out_of_sync();
extern bool __cdecl simulation_update_player_netdebug_data(int32 player_index, s_simulation_player_netdebug_data* netdebug_data);
extern void __cdecl simulation_update_pregame();
extern bool __cdecl simulation_update_read_from_buffer(struct simulation_update* update, int32 buffer_size, const uns8* buffer);
extern bool __cdecl simulation_update_write_to_buffer(const struct simulation_update* update, int32 buffer_size, uns8* buffer, int32* out_update_length);

extern void simulation_debug_render();

