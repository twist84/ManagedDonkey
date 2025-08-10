#pragma once

#include "cseries/async_buffer.hpp"
#include "multithreading/synchronized_value.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "text/unicode.hpp"

class c_virtual_keyboard_task;

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
//static_assert(sizeof(s_saved_film_snippet_globals) == 0x26100);

