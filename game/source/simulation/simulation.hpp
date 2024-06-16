#pragma once

#include "cseries/cseries.hpp"
#include "game/player_control.hpp"
#include "game/players.hpp"
#include "replication/replication_encoding.hpp"
#include "simulation/simulation_queue.hpp"
#include "simulation/simulation_view.hpp"
#include "units/units.hpp"

class c_wait_for_render_thread
{
public:
	c_wait_for_render_thread(char const* file, long line);
	~c_wait_for_render_thread();

protected:
	dword m_flags;
};

enum e_simulation_abort_reason
{
	_simulation_abort_reason_exiting_in_game = 0,
	_simulation_abort_reason_exiting_in_match,
	_simulation_abort_reason_failed_to_find_next_map,
	_simulation_abort_reason_playing_film,
	_simulation_abort_reason_reset_failed,
	_simulation_abort_reason_fatal_error,
	_simulation_abort_reason_lost_connection,
	_simulation_abort_reason_out_of_sync,
	_simulation_abort_reason_film_ended,
	_simulation_abort_reason_film_playback_error,

	k_simulation_abort_reason_count
};

struct simulation_machine_update
{
	dword machine_valid_mask;
	c_static_array<s_machine_identifier, 17> machine_identifiers;
};
static_assert(sizeof(simulation_machine_update) == 0x114);

struct s_simulation_camera_update
{
	long camera_type;
	long player_absolute_index;
	real_point3d position;
	vector3d forward;
	bool zoomed;
};
static_assert(sizeof(s_simulation_camera_update) == 0x24);

enum e_simulation_update_high_level_flags
{
	// set in 'c_simulation_world::build_update'
	_simulation_update_high_level_simulation_in_progress_bit = 0,

	// set in 'c_simulation_world::build_update', `is_authority() && m_notify_gamestate_flushed`
	_simulation_update_high_level_unknown_bit1, // _simulation_update_high_level_gamestate_flushed_bit?

	// set in 'c_simulation_world::build_update', `m_notify_gamestate_flushed_outside_game_tick`
	_simulation_update_high_level_unknown_bit2, // _simulation_update_high_level_gamestate_flushed_outside_game_tick_bit

	// set in 'c_simulation_world::attach_simulation_queues_to_update'
	_simulation_update_high_level_game_simulation_queue_requires_application_bit,

	k_simulation_update_high_level_flags
};

enum e_simulation_update_metadata_flags
{
	// set in `c_simulation_world::build_update`
	_simulation_update_from_local_simulation_bit = 0,

	// set in `c_simulation_world::handle_synchronous_update`
	_simulation_update_unknown_bit1, // _simulation_update_from_remote_simulation_bit?

	// set in `simulation_film_retrieve_updates`
	_simulation_update_from_saved_film_bit,

	k_simulation_update_metadata_flags
};

struct simulation_update
{
	long update_number;

	c_flags<e_simulation_update_high_level_flags, word, k_simulation_update_high_level_flags> high_level_flags;

	dword_flags player_flags;

	dword_flags action_test_flags;
	c_static_array<s_player_action, 16> player_actions;

	dword valid_actor_mask;
	c_static_array<long, 16> actor_unit_indices;
	c_static_array<unit_control_data, 16> actor_control;

	bool machine_update_exists;
	simulation_machine_update machine_update;

	dword valid_player_prediction_mask;
	c_static_array<s_player_prediction, 16> player_prediction;

	long verify_game_time;
	long verify_random;

	//s_determinism_verification determinism_verification;

	dword valid_camera_mask;
	c_static_array<s_simulation_camera_update, 1> camera_updates;

	c_simulation_queue bookkeeping_simulation_queue;
	c_simulation_queue game_simulation_queue;
};
static_assert(sizeof(struct simulation_update) == 0x1658);

struct s_simulation_update_metadata
{
	c_flags<e_simulation_update_metadata_flags, word, k_simulation_update_metadata_flags> flags;

	long saved_film_position;
	long saved_film_tick;
};
static_assert(sizeof(s_simulation_update_metadata) == 0xC);

enum e_update_queue_node
{
	_update_queue_node_update = 0,
	_update_queue_node_playback_event,

	k_update_queue_node_count,
	k_update_queue_node_invalid = -1
};

struct s_simulation_update_playback_event
{
	byte __data0[0x4];
	long event_playback_event; // e_simulation_playback_event
	dword event_identifier;
	long event_update_number;
};
static_assert(sizeof(s_simulation_update_playback_event) == 0x10);

// unknown struct name using `s_simulation_update_node` for now
struct s_simulation_update_node
{
	c_enum<e_update_queue_node, long, _update_queue_node_update, k_update_queue_node_count> node_type;

	byte __data0[0x4];

	struct simulation_update update;
	s_simulation_update_metadata metadata;
	s_simulation_update_playback_event playback_event;
	s_simulation_update_node* next;
};
static_assert(sizeof(s_simulation_update_node) == 0x1680);

struct c_simulation_world;
struct c_simulation_watcher;
struct c_simulation_type_collection;
struct s_simulation_globals
{
	bool initialized;
	bool fatal_error;
	bool saved_film_revert;
	bool aborted;

	dword network_time_since_abort;

	c_enum<e_simulation_abort_reason, long, _simulation_abort_reason_exiting_in_game, k_simulation_abort_reason_count> abort_reason;

	bool reset;
	bool reset_in_progress;

	byte __unknownE;

	bool prepare_to_load_saved_game;
	bool recording_film;
	bool must_close_saved_film;
	bool performed_main_save_and_exit_campaign_immediately_this_map;

	byte __unknown13;

	c_simulation_world* world;
	c_simulation_watcher* watcher;
	c_simulation_type_collection* type_collection;

	dword __unknown20;

	c_static_string<256> status;

	bool paused;

	byte __unknown125;
	byte __unknown126;
	byte __unknown127;

	// used in `simulation_update_write_to_buffer`
	struct simulation_update update;
	dword_flags update_flags;
};
static_assert(sizeof(s_simulation_globals) == 0x1784);

extern s_simulation_globals& simulation_globals;

struct s_simulation_player_netdebug_data;

extern void patch_simulation();

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
extern bool __cdecl simulation_format_player_netdebug_data(long a1, s_simulation_player_netdebug_data const* netdebug_data, long* a3);
extern char const* __cdecl simulation_get_abort_reason_string();
extern e_simulation_abort_reason __cdecl simulation_get_aborted_reason();
extern void __cdecl simulation_get_game_description(c_static_string<260>* game_description);
extern bool __cdecl simulation_get_machine_active_in_game(s_machine_identifier const* machine);
extern bool __cdecl simulation_get_machine_bandwidth_events(s_machine_identifier const* machine, long bandwidth_event_type_count, long* bandwidth_event_counters);
extern bool __cdecl simulation_get_machine_connectivity(s_machine_identifier const* machine);
extern bool __cdecl simulation_get_machine_is_host(s_machine_identifier const* machine);
extern dword __cdecl simulation_get_network_time_since_abort();
extern c_simulation_view* __cdecl simulation_get_remote_view_by_channel(c_network_channel* channel);
extern char const* simulation_get_starting_up_description();
extern long __cdecl simulation_get_status();
extern c_simulation_type_collection* __cdecl simulation_get_type_collection();
extern bool __cdecl simulation_get_view_network_metrics(c_simulation_view const* view, long* rtt_msec, long* packet_rate, long* bandwidth_bps, long* packet_loss);
extern c_simulation_world* __cdecl simulation_get_world();
extern c_simulation_world* __cdecl simulation_get_world_if_exists();
extern bool __cdecl simulation_in_progress();
extern void __cdecl simulation_initialize();
extern void __cdecl simulation_initialize_for_new_map();
extern void __cdecl simulation_initialize_for_saved_game(long flags);
extern void __cdecl simulation_invalidate();
extern void __cdecl simulation_must_close_saved_film();
extern void __cdecl simulation_notify_channel_closure(void* closure_callback);
extern void __cdecl simulation_notify_core_save();
extern void __cdecl simulation_notify_going_active();
extern void __cdecl simulation_notify_initial_core_load(long next_update_number);
extern void __cdecl simulation_notify_players_created();
extern void __cdecl simulation_notify_reset_complete();
extern void __cdecl simulation_notify_reset_initiate();
extern void __cdecl simulation_notify_revert();
extern void __cdecl simulation_notify_saved_film_ended();
extern void __cdecl simulation_notify_saved_film_revert(long a1, long a2);
extern bool __cdecl simulation_performed_main_save_and_exit_campaign_immediately_this_map();
extern void __cdecl simulation_player_joined_game(long player_index);
extern void __cdecl simulation_player_left_game(long player_index);
extern void __cdecl simulation_prepare_to_load_saved_game(long a1);
extern void __cdecl simulation_prepare_to_send();
extern void __cdecl simulation_process_actor_control(long actor_index, unit_control_data const* control);
extern void __cdecl simulation_process_input(dword player_mask, c_static_array<s_player_action, 4> const& actions);
extern void __cdecl simulation_record_update(struct simulation_update const* update);
extern void __cdecl simulation_remove_view_from_world(c_simulation_view* view);
extern void __cdecl simulation_reset();
extern bool __cdecl simulation_reset_in_progress();
extern void __cdecl simulation_saved_film_revert();
extern void __cdecl simulation_set_performed_main_save_and_exit_campaign_immediately_this_map(bool performed_main_save_and_exit_campaign_immediately_this_map);
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
extern bool __cdecl simulation_update_read_from_buffer(struct simulation_update* update, long buffer_size, byte const* buffer);
extern bool __cdecl simulation_update_write_to_buffer(struct simulation_update const* update, long buffer_size, byte* buffer, long* out_update_length);

extern void simulation_debug_render();

