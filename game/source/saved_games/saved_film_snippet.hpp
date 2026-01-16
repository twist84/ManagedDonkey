#pragma once

#include "cseries/async_buffer.hpp"
#include "multithreading/synchronized_value.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "text/unicode.hpp"

class c_virtual_keyboard_task;
struct s_saved_film_hud_interface_state;
struct s_simulation_camera_update;
struct s_simulation_update_metadata;

enum e_saved_film_snippet_state
{
	_saved_film_snippet_state_none = 0,
	_saved_film_snippet_state_recording_waiting_for_seek,
	_saved_film_snippet_state_recording_waiting_for_start,
	_saved_film_snippet_state_recording,
	_saved_film_snippet_state_recorded_and_ready,
	_saved_film_snippet_state_previewing_waiting_for_seek,
	_saved_film_snippet_state_previewing,
	_saved_film_snippet_state_commiting_invoking_title_keyboard,
	_saved_film_snippet_state_commiting_waiting_title_keyboard,
	_saved_film_snippet_state_commiting_invoking_description_keyboard,
	_saved_film_snippet_state_commiting_waiting_description_keyboard,
	_saved_film_snippet_state_commiting_initiate_creation,
	_saved_film_snippet_state_commiting_wait_for_creation,
	_saved_film_snippet_state_commiting_initiate_metadata_update,
	_saved_film_snippet_state_commiting_wait_for_metadata_update,
	_saved_film_snippet_state_resetting,

	k_number_of_saved_film_snippet_states
};

struct s_saved_film_snippet_globals
{
	s_saved_film_snippet_globals();
	//~s_saved_film_snippet_globals();
	//s_saved_film_snippet_globals& operator=(const s_saved_film_snippet_globals& __that);

	c_async_stored_buffer_set<2> camera_file;
	c_saved_film destination_film;
	s_saved_film_manager_director_state start_recording_director_state;
	int32 recording_start_tick;
	int32 recording_stop_tick;
	c_static_wchar_string<16> snippet_name;
	c_static_wchar_string<128> snippet_description;
	c_virtual_keyboard_task* virtual_keyboard_task;
	e_controller_index controller_index;
	int32 snippet_save_task_id;
	int32 snippet_content_item_index;
	c_synchronized_long snippet_save_done;
	c_synchronized_long snippet_save_success;
	bool progress_spinner_active;
	e_saved_film_snippet_state current_state;
};
//COMPILE_ASSERT(sizeof(s_saved_film_snippet_globals) == 0x26100);

extern void saved_film_snippet_build_auto_description(e_controller_index controller_index, c_static_wchar_string<128>* snippet_description_out);
extern void saved_film_snippet_build_auto_name(e_controller_index controller_index, c_static_wchar_string<16>* snippet_name_out);
extern bool saved_film_snippet_close_camera_file();
extern bool saved_film_snippet_commit_by_autoname(e_controller_index controller_index);
extern bool saved_film_snippet_commit_by_keyboard(e_controller_index controller_index);
extern bool saved_film_snippet_delete();
extern void saved_film_snippet_dispose_from_saved_film_playback();
extern void saved_film_snippet_dispose_virtual_keyboard_task();
extern bool saved_film_snippet_finished_revert_for_seek(int32 update_number, void* gamestate, int32 gamestate_size);
extern void saved_film_snippet_get_camera_for_simulation_update(s_simulation_camera_update* camera_update_out);
extern bool saved_film_snippet_get_current_start_tick(int32* start_tick_out);
extern e_saved_film_snippet_state saved_film_snippet_get_current_state();
extern void saved_film_snippet_get_hud_interface_state(s_saved_film_hud_interface_state* state);
extern void saved_film_snippet_initialize();
extern void saved_film_snippet_initialize_for_saved_film_playback();
extern void saved_film_snippet_initialize_state();
extern void saved_film_snippet_memory_dispose();
extern void saved_film_snippet_memory_initialize();
extern bool saved_film_snippet_pad_camera_file_for_write_completion();
extern bool saved_film_snippet_preview_start();
extern bool saved_film_snippet_preview_stop();
extern bool saved_film_snippet_recording_or_previewing();
extern bool saved_film_snippet_start_recording();
extern bool saved_film_snippet_stop_recording();
extern bool saved_film_snippet_update();
extern bool saved_film_snippet_update_after_simulation_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata);
extern void saved_film_snippets_notify_reverted_for_seek(bool* set_director_state_out);

