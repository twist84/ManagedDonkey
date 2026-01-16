#pragma once

#include "cseries/cseries.hpp"
#include "camera/observer.hpp"
#include "saved_games/saved_film.hpp"
#include "simulation/simulation.hpp"

enum e_saved_film_snippet_state;
struct game_options;
struct s_saved_film_hud_interface_state;
struct s_simulation_update_metadata;

enum e_saved_film_file_path_creation_purpose
{
	_file_path_for_creation = 0,
	_file_path_for_creation_final = 1,
	_file_path_for_reading = 2,
};

enum e_saved_film_game_state_load_source
{
	_saved_film_game_state_load_source_core = 0,
	_saved_film_game_state_load_source_storage,

	k_saved_film_game_state_load_source
};

enum e_saved_film_playback_abort_reason
{
	_saved_film_playback_abort_reason_none = 0,
	_saved_film_playback_abort_simulation_failed_to_read,
	_saved_film_playback_abort_snippet_failed_to_update,
	_saved_film_playback_abort_snippet_failed_to_update_after_simulation,
	_saved_film_playback_abort_snippet_failed_to_start_recording,
	_saved_film_playback_abort_snippet_failed_to_stop_recording,
	_saved_film_playback_abort_snippet_failed_to_start_preview,
	_saved_film_playback_abort_snippet_failed_to_stop_preview,
	_saved_film_playback_abort_snippet_failed_to_delete,
	_saved_film_playback_abort_snippet_failed_to_commit,
	_saved_film_playback_history_failed_to_write,
	_saved_film_playback_history_failed_to_write_record,
	_saved_film_playback_history_failed_to_revert,
	_saved_film_playback_history_failed_to_revert_by_index,
	_saved_film_playback_failed_to_load_gamestate_for_party,
	_saved_film_playback_map_signature_failed,

	k_saved_film_playback_abort_reason_count
};

enum e_saved_film_revert_type
{
	_saved_film_revert_none = 0,
	_saved_film_revert_backwards = 1,
	_saved_film_revert_forwards = 2,
};

struct s_saved_film_manager_user_director_state
{
	s_observer_result observer_result;
	e_camera_mode camera_mode;
	e_camera_mode desired_camera_mode;
	int32 camera_target_player_absolute_index;
	bool valid;
};
COMPILE_ASSERT(sizeof(s_saved_film_manager_user_director_state) == 0x80);

struct s_saved_film_manager_director_state
{
	c_static_array<s_saved_film_manager_user_director_state, 4> user_director_states;
};
COMPILE_ASSERT(sizeof(s_saved_film_manager_director_state) == sizeof(s_saved_film_manager_user_director_state) * 4);

class c_saved_film;
struct s_saved_film_manager_globals
{
	c_static_string<64> saved_film_name;
	c_saved_film saved_film;
	bool pending_gamestate_load;
	int32 gamestate_file_position;
	bool film_close_in_progress;
	bool reproduction_mode_enabled;
	real32 playback_game_speed;
	bool playback_locked;
	bool show_timestamp;
	bool film_ended;
	bool film_ended_sound_disabled;
	int32 film_ended_system_milliseconds;
	int32 seek_film_tick;
	bool seek_and_stop;
	e_saved_film_revert_type desired_revert_type;
	int32 desired_revert_index;
	int32 snippet_start_tick;
	c_static_flags_no_init<4> authored_cam_set_for_user;
	uns32 valid_camera_update_mask;
	c_static_array<s_simulation_camera_update, 1> camera_updates;
	bool playback_aborted;
	e_saved_film_playback_abort_reason playback_abort_reason;
	bool disable_version_checking;
	bool automatic_debug_saving_enabled;
	bool ui_screen_active;
	bool screensaver_enabled;
	bool initialized;
};

static const real32 k_saved_film_ended_fade_time_seconds = 4.0f;

namespace
{
	extern bool saved_film_manager_should_record_film_default;
}

extern c_interlocked_long g_universal_saved_film_tick;
extern s_saved_film_manager_globals saved_film_manager_globals;

extern void saved_film_manager_abort_playback(e_saved_film_playback_abort_reason abort_reason);
extern bool saved_film_manager_authored_camera_locked_for_snippet();
extern bool saved_film_manager_automatic_debug_saving_enabled();
extern void saved_film_manager_build_file_path_from_name(const char* film_name, e_saved_film_file_path_creation_purpose purpose, c_static_string<128>* film_path_out);
extern bool saved_film_manager_can_revert(e_saved_film_revert_type desired_revert_type);
extern bool saved_film_manager_can_set_playback_control();
extern void saved_film_manager_clear_playback_state();
extern void saved_film_manager_close();
extern void saved_film_manager_commit_snippet_autoname(e_controller_index controller_index);
extern void saved_film_manager_commit_snippet_keyboard(e_controller_index controller_index);
extern void saved_film_manager_copy_film_to_debug_path();
extern void saved_film_manager_create_film_directory();
extern void saved_film_manager_delete_current_snippet();
extern void saved_film_manager_delete_on_level_load(bool delete_on_level_load);
extern void saved_film_manager_disable_version_checking(bool disable);
extern void saved_film_manager_dispose_from_old_map();
extern void saved_film_manager_dispose();
extern void saved_film_manager_end_film_internal();
extern bool saved_film_manager_film_is_ended(real32* out_seconds_ago);
extern bool saved_film_manager_film_valid(e_controller_index controller, const char* film_name);
extern bool saved_film_manager_get_current_film_name(c_static_string<64>* film_name_out);
extern const game_options* saved_film_manager_get_current_game_options();
extern const s_saved_game_item_metadata* saved_film_manager_get_current_metadata();
extern int32 saved_film_manager_get_current_tick_estimate();
extern int32 saved_film_manager_get_current_tick();
extern void saved_film_manager_get_director_state(s_saved_film_manager_director_state* director_state_out);
extern void saved_film_manager_get_hud_interface_state(s_saved_film_hud_interface_state* hud_state);
extern bool saved_film_manager_get_last_recorded_film(char* filepath, int32 maximum_characters, s_saved_game_item_metadata* out_optional_metadata);
extern int32 saved_film_manager_get_length_in_ticks();
extern real32 saved_film_manager_get_pending_playback_game_speed();
extern real32 saved_film_manager_get_playback_game_speed();
extern int32 saved_film_manager_get_position();
extern const wchar_t* saved_film_manager_get_recording_directory();
extern bool saved_film_manager_get_reproduction_enabled();
extern uns32 saved_film_manager_get_simulation_camera_update_mask();
extern bool saved_film_manager_get_simulation_camera_updates(int32 camera_index, s_simulation_camera_update* simulation_camera_update_out);
extern int32 saved_film_manager_get_snippet_start_tick();
extern e_saved_film_snippet_state saved_film_manager_get_snippet_state();
extern int32 saved_film_manager_get_ticks_remaining();
extern void saved_film_manager_handle_camera_update(uns32 valid_camera_update_mask, const s_simulation_camera_update* camera_updates);
extern bool saved_film_manager_handle_revert(int32 saved_film_file_position, int32 film_tick);
extern bool saved_film_manager_has_pending_global_state_change();
extern void saved_film_manager_initialize_for_new_map();
extern void saved_film_manager_initialize();
extern bool saved_film_manager_is_reading();
extern bool saved_film_manager_is_writing();
extern void saved_film_manager_load_pending_gamestate();
extern bool saved_film_manager_load_pending_gamestate_to_compressor();
extern void saved_film_manager_memory_dispose();
extern void saved_film_manager_memory_initialize(e_map_memory_configuration memory_configuration);
extern void saved_film_manager_notify_gamestate_decompression_after_load_procs();
extern void saved_film_manager_notify_gamestate_decompression_before_load_procs();
extern void saved_film_manager_notify_gamestate_load(e_saved_film_game_state_load_source game_state_load_source);
extern void saved_film_manager_notify_out_of_sync();
extern void saved_film_manager_notify_remote_end_film();
extern void saved_film_manager_notify_reverted_gamestate_loaded(int32 history_record_index, int32 update_number, void* gamestate, int32 gamestate_size);
extern void saved_film_manager_notify_snippet_preview_complete();
extern bool saved_film_manager_open_film_for_reading(e_controller_index controller_index, const char* film_name);
extern bool saved_film_manager_open_film_for_writing(const char* film_name, const game_options* options);
extern void saved_film_manager_perform_global_state_change();
extern void saved_film_manager_perform_revert(bool* set_director_state_out);
extern void saved_film_manager_play_hs(int16 controller_index, const char* film_name);
extern void saved_film_manager_play_last_hs();
extern void saved_film_manager_play(e_controller_index controller_index, const char* film_name);
extern bool saved_film_manager_playback_aborted();
extern void saved_film_manager_playback_lock_set(real32 playback_game_speed, bool locked);
extern void saved_film_manager_preview_snippet_start();
extern void saved_film_manager_preview_snippet_stop();
extern bool saved_film_manager_read_simulation_update(const s_saved_film_update* update, struct simulation_update* simulation_update_out);
extern bool saved_film_manager_read_update(s_saved_film_update* update_out);
extern void saved_film_manager_render_debug();
extern void saved_film_manager_replay_film();
extern void saved_film_manager_request_end_film();
extern void saved_film_manager_request_revert_by_index(int32 revert_index);
extern void saved_film_manager_request_revert(e_saved_film_revert_type desired_revert_type);
extern bool saved_film_manager_revert_desired();
extern bool saved_film_manager_rewind_and_seek_to_film_tick(int32 film_tick, bool seek_and_stop);
extern void saved_film_manager_seek_to_film_tick_hs(int32 film_tick);
extern bool saved_film_manager_seeking(int32* seek_time_available_out);
extern void saved_film_manager_set_director_state(const s_saved_film_manager_director_state* director_state);
extern bool saved_film_manager_set_pending_playback_game_speed(real32 game_speed);
extern bool saved_film_manager_set_playback_game_speed(real32 game_speed);
extern bool saved_film_manager_set_position(int32 position);
extern void saved_film_manager_set_reproduction_enabled(bool reproduction_enabled);
extern void saved_film_manager_should_record_film_default_set(bool b);
extern bool saved_film_manager_should_record_film(const game_options* options);
extern void saved_film_manager_show_timestamp(bool show_timestamp);
extern bool saved_film_manager_snippets_available();
extern void saved_film_manager_start_recording_snippet();
extern void saved_film_manager_stop_recording_snippet();
extern bool saved_film_manager_timestamp_enabled_internal();
extern void saved_film_manager_toggle_automatic_debug_saving(bool enable);
extern void saved_film_manager_update_after_simulation_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata);
extern void saved_film_manager_update_before_simulation_update();
extern void saved_film_manager_update_seeking(int32 current_film_tick);
extern void saved_film_manager_update_snippet_authored_cameras();
extern void saved_film_manager_update_ui_screens();
extern void saved_film_manager_update();
extern int32 saved_film_manager_upload_start(int32 maximum_file_count, s_file_reference* out_file_list);
extern bool saved_film_manager_write_simulation_update(const struct simulation_update* update);

