#include "saved_games/saved_film_snippet.hpp"

#include "cache/physical_memory_map.hpp"
#include "camera/director.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/chud/chud.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "main/levels.hpp"

static REFERENCE_DECLARE(0x019428EC, c_physical_memory_allocation, g_physical_memory_allocation); // only use in this translation unit

s_saved_film_snippet_globals saved_film_snippet_globals{};

const char* const k_saved_film_snippet_state_string[k_number_of_saved_film_snippet_states]
{
	"none",
	"recording_waiting_for_seek",
	"recording_waiting_for_start",
	"recording",
	"recorded_and_ready",
	"previewing_waiting_for_seek",
	"previewing",
	"commiting-invoke-title-keyboard",
	"commiting-wait-title-keyboard",
	"commiting-invoke-desc-keyboard",
	"commiting-wait-desc-keyboard",
	"commiting-initiate-creation",
	"commiting-wait-for-creation",
	"commiting-initiate-metadata-update",
	"commiting-wait-for-metadata-update",
	"resetting",
};

const wchar_t* const k_camera_file_name = L"sf_camera.blob";
const char* const k_snippet_file_name = "sf_snippet.film";

s_saved_film_snippet_globals::s_saved_film_snippet_globals() :
	camera_file(),
	destination_film(),
	snippet_name(),
	snippet_description(),
	snippet_save_done(0),
	snippet_save_success(0)
{

}

//s_saved_film_snippet_globals::~s_saved_film_snippet_globals();
//s_saved_film_snippet_globals& s_saved_film_snippet_globals::operator=(const s_saved_film_snippet_globals& __that);

void saved_film_snippet_build_auto_description(e_controller_index controller_index, c_static_wchar_string<128>* snippet_description_out)
{
	const s_saved_game_item_metadata* metadata = saved_film_manager_get_current_metadata();
	const game_options* options = saved_film_manager_get_current_game_options();
	
	ASSERT(metadata);
	ASSERT(options);
	
	wchar_t map_name_buffer[128]{};
	wchar_t auto_description[128]{};
	if (!saved_game_files_get_default_film_save_description(
		controller_index,
		metadata->get_gui_game_mode(),
		levels_get_active_session_map_name(map_name_buffer, NUMBEROF(map_name_buffer)),
		options->multiplayer_variant.get_active_variant()->get_metadata()->display_name,
		metadata->campaign_difficulty,
		auto_description, NUMBEROF(auto_description)))
	{
		event(_event_warning, "networking:saved_film:snippet: failed to build auto description, using current film description");
		snippet_description_out->set_char(metadata->description);
		return;
	}
	
	snippet_description_out->set(auto_description);
}

void saved_film_snippet_build_auto_name(e_controller_index controller_index, c_static_wchar_string<16>* snippet_name_out)
{
	const s_saved_game_item_metadata* metadata = saved_film_manager_get_current_metadata();
	ASSERT(metadata);
	
	wchar_t auto_name[16]{};
	ustrnzcpy(auto_name, metadata->display_name, NUMBEROF(auto_name));
	saved_game_files_uniquify_name(controller_index, _saved_game_film_clip, auto_name, NUMBEROF(auto_name));
	snippet_name_out->set(auto_name);
}

bool saved_film_snippet_close_camera_file()
{
	if (saved_film_snippet_globals.camera_file.get_state() == _async_buffer_state_writing &&
		!saved_film_snippet_pad_camera_file_for_write_completion())
	{
		event(_event_warning, "networking:saved_film:snippet: failed to finalize camera file before close");
	}
	
	if (!saved_film_snippet_globals.camera_file.close_file())
	{
		event(_event_warning, "networking:saved_film:snippet: failed to close camera file");
		return false;
	}
	
	return true;
}

bool saved_film_snippet_commit_by_autoname(e_controller_index controller_index)
{
	ASSERT(saved_film_snippet_globals.recording_start_tick != NONE);
	ASSERT(saved_film_snippet_globals.recording_stop_tick != NONE);
	ASSERT(saved_film_snippet_globals.recording_start_tick <= saved_film_snippet_globals.recording_stop_tick);
	ASSERT(saved_film_snippet_globals.current_state == _saved_film_snippet_state_recorded_and_ready);
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	
	if (!saved_film_snippet_close_camera_file())
	{
		event(_event_warning, "networking:saved_film:snippet: failed to close camera file for commit");
		return false;
	}
	
	saved_film_snippet_globals.controller_index = controller_index;
	saved_film_snippet_build_auto_name(saved_film_snippet_globals.controller_index, &saved_film_snippet_globals.snippet_name);
	saved_film_snippet_build_auto_description(saved_film_snippet_globals.controller_index, &saved_film_snippet_globals.snippet_description);
	saved_film_snippet_globals.current_state = _saved_film_snippet_state_commiting_initiate_creation;
	return true;
}

bool saved_film_snippet_commit_by_keyboard(e_controller_index controller_index)
{
	ASSERT(saved_film_snippet_globals.recording_start_tick != NONE);
	ASSERT(saved_film_snippet_globals.recording_stop_tick != NONE);
	ASSERT(saved_film_snippet_globals.recording_start_tick <= saved_film_snippet_globals.recording_stop_tick);
	ASSERT(saved_film_snippet_globals.current_state == _saved_film_snippet_state_recorded_and_ready);
	ASSERT(saved_film_snippet_globals.virtual_keyboard_task == NULL);
	
	if (!saved_film_snippet_close_camera_file())
	{
		event(_event_warning, "networking:saved_film:snippet: failed to close camera file for commit by keyboard");
		return false;
	}
	
	if (saved_film_snippet_globals.virtual_keyboard_task = c_virtual_keyboard_task::get_instance(__FILE__, __LINE__,
		controller_index,
		NULL, NULL, NULL, 256, 0, false))
	{
		event(_event_error, "networking:saved_film:snippet: failed to get virtual keyboard task interface");
		return false;
	}
	
	saved_film_snippet_globals.controller_index = controller_index;
	saved_film_snippet_globals.current_state = _saved_film_snippet_state_commiting_invoking_title_keyboard;
	return true;
}

bool saved_film_snippet_delete()
{
	ASSERT(saved_film_snippet_globals.recording_start_tick != NONE);
	ASSERT(saved_film_snippet_globals.recording_stop_tick != NONE);
	ASSERT(saved_film_snippet_globals.recording_start_tick <= saved_film_snippet_globals.recording_stop_tick);
	ASSERT(saved_film_snippet_globals.current_state == _saved_film_snippet_state_recorded_and_ready);
	
	if (!saved_film_manager_rewind_and_seek_to_film_tick(saved_film_snippet_globals.recording_start_tick, true))
	{
		event(_event_warning, "networking:saved_film:snippet: failed to request seek to tick %d for delete",
			saved_film_snippet_globals.recording_start_tick);
		return false;
	}
	
	if (!saved_film_snippet_close_camera_file())
	{
		event(_event_warning, "networking:saved_film:snippet: failed to close camera file for delete");
		return false;
	}
	
	saved_film_snippet_globals.current_state = _saved_film_snippet_state_resetting;
	return true;
}

void saved_film_snippet_dispose_from_saved_film_playback()
{
}

void saved_film_snippet_dispose_virtual_keyboard_task()
{
	if (saved_film_snippet_globals.virtual_keyboard_task)
	{
		overlapped_task_block_until_finished(saved_film_snippet_globals.virtual_keyboard_task);
		saved_film_snippet_globals.virtual_keyboard_task = NULL;
	}
}

bool saved_film_snippet_finished_revert_for_seek(int32 update_number, void* gamestate, int32 gamestate_size)
{
	if (saved_film_snippet_globals.current_state != _saved_film_snippet_state_recording_waiting_for_seek)
	{
		return false;
	}
	
	ASSERT(saved_film_snippet_globals.recording_start_tick >= 0);
	ASSERT(saved_film_snippet_globals.recording_stop_tick == NONE);
	ASSERT(saved_film_snippet_globals.camera_file.get_position() == 0);
	
	game_state_security_write_signature(false, NULL);
	if (!saved_film_snippet_globals.destination_film.write_gamestate_from_buffer(update_number, gamestate, gamestate_size))
	{
		event(_event_warning, "networking:saved_film:snippets: failed to write gamestate for preview");
		return false;
	}
	
	saved_film_snippet_globals.current_state = _saved_film_snippet_state_recording_waiting_for_start;
	return true;
}

void saved_film_snippet_get_camera_for_simulation_update(s_simulation_camera_update* camera_update_out)
{
	// $IMPLEMENT
}

bool saved_film_snippet_get_current_start_tick(int32* start_tick_out)
{
	if (!IN_RANGE_INCLUSIVE(saved_film_snippet_globals.current_state, _saved_film_snippet_state_recording_waiting_for_seek, _saved_film_snippet_state_previewing) &&
		saved_film_snippet_globals.current_state != _saved_film_snippet_state_resetting)
	{
		return false;
	}
	
	ASSERT(saved_film_snippet_globals.recording_start_tick != NONE);
	*start_tick_out = saved_film_snippet_globals.recording_start_tick;
	return true;
}

e_saved_film_snippet_state saved_film_snippet_get_current_state()
{
	return saved_film_snippet_globals.current_state;
}

void saved_film_snippet_get_hud_interface_state(s_saved_film_hud_interface_state* state)
{
	if (!IN_RANGE_INCLUSIVE(saved_film_snippet_globals.current_state, _saved_film_snippet_state_recording, _saved_film_snippet_state_resetting))
	{
		return;
	}
	
	real32 recording_start_seconds = game_ticks_to_seconds((real32)saved_film_snippet_globals.recording_start_tick);
	real32 duration_in_seconds = state->duration_in_seconds;
	state->recording = true;
	if (duration_in_seconds >= k_real_epsilon)
	{
		state->recording_start_theta = recording_start_seconds / duration_in_seconds;
	}
	else
	{
		state->recording_start_theta = 0;
	}
}

void saved_film_snippet_initialize()
{
	saved_film_snippet_initialize_state();
}

void saved_film_snippet_initialize_for_saved_film_playback()
{
	saved_film_snippet_initialize_state();
}

void dispose_progress_spinner_if_active()
{
	if (!saved_film_snippet_globals.progress_spinner_active)
	{
		return;
	}

	c_gui_screen_widget* in_progress_mini_screen = window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, in_progress_mini));
	if (in_progress_mini_screen)
	{
		in_progress_mini_screen->transition_out(_transition_out_normal);
	}
	saved_film_snippet_globals.progress_spinner_active = false;
}

void saved_film_snippet_initialize_state()
{
	csmemset(&saved_film_snippet_globals.start_recording_director_state, 0, sizeof(s_saved_film_manager_director_state));
	saved_film_snippet_globals.recording_start_tick = NONE;
	saved_film_snippet_globals.recording_stop_tick = NONE;
	saved_film_snippet_globals.controller_index = k_no_controller;
	saved_film_snippet_globals.snippet_save_task_id = NONE;
	saved_film_snippet_globals.snippet_content_item_index = NONE;
	saved_film_snippet_globals.snippet_save_done.set(0);
	saved_film_snippet_globals.snippet_save_success.set(0);
	saved_film_snippet_globals.current_state = _saved_film_snippet_state_none;
	saved_film_snippet_globals.snippet_name.clear();
	dispose_progress_spinner_if_active();
	saved_film_snippet_dispose_virtual_keyboard_task();
}

void saved_film_snippet_memory_dispose()
{
	saved_film_snippet_close_camera_file();
	saved_film_snippet_globals.camera_file.release_storage(&g_physical_memory_allocation);
	saved_film_snippet_globals.destination_film.close();
	saved_film_snippet_globals.destination_film.dispose(&g_physical_memory_allocation);
	saved_film_snippet_initialize_state();
}

void saved_film_snippet_memory_initialize()
{
	if (!saved_film_snippet_globals.camera_file.allocate_storage(&g_physical_memory_allocation, 0x1000))
	{
		event(_event_critical, "networking:saved_film:snippet: failed to allocate storage for camera file");
		return;
	}
	
	if (!saved_film_snippet_globals.destination_film.initialize(&g_physical_memory_allocation))
	{
		event(_event_critical, "networking:saved_film:snippet: failed to allocate storage for snippet film file");
		return;
	}
}

bool saved_film_snippet_pad_camera_file_for_write_completion()
{
	// $IMPLEMENT
	return false;
}

bool saved_film_snippet_preview_start()
{
	ASSERT(saved_film_snippet_globals.recording_start_tick != NONE);
	ASSERT(saved_film_snippet_globals.recording_stop_tick != NONE);
	ASSERT(saved_film_snippet_globals.recording_start_tick <= saved_film_snippet_globals.recording_stop_tick);
	ASSERT(saved_film_snippet_globals.current_state == _saved_film_snippet_state_recorded_and_ready);
	
	if (!saved_film_snippet_globals.camera_file.set_position(0))
	{
		event(_event_error, "networking:saved_film:snippets: failed to set camera file position for preview");
		return false;
	}
	
	if (!saved_film_manager_rewind_and_seek_to_film_tick(saved_film_snippet_globals.recording_start_tick, 0))
	{
		event(_event_error, "networking:saved_film:snippets: failed to request seek for preview [%d]",
			saved_film_snippet_globals.recording_start_tick);
		return false;
	}
	
	for (int32 user_index = player_mapping_first_active_output_user();
		user_index != NONE;
		user_index = player_mapping_next_active_output_user(user_index))
	{
		director_get(user_index)->set_camera_mode(_camera_mode_authored, 0.0f);
	}
	
	saved_film_snippet_globals.current_state = _saved_film_snippet_state_previewing_waiting_for_seek;
	return true;
}

bool saved_film_snippet_preview_stop()
{
	ASSERT(saved_film_snippet_globals.recording_start_tick != NONE);
	ASSERT(saved_film_snippet_globals.recording_stop_tick != NONE);
	ASSERT(saved_film_snippet_globals.recording_start_tick <= saved_film_snippet_globals.recording_stop_tick);
	ASSERT(saved_film_snippet_globals.current_state == _saved_film_snippet_state_previewing);
	
	saved_film_snippet_globals.current_state = _saved_film_snippet_state_recorded_and_ready;
	return true;
}

bool saved_film_snippet_recording_or_previewing()
{
	return IN_RANGE_INCLUSIVE(saved_film_snippet_globals.current_state, _saved_film_snippet_state_recording_waiting_for_seek, _saved_film_snippet_state_previewing);
}

bool saved_film_snippet_start_recording()
{
	ASSERT(saved_film_snippet_globals.current_state == _saved_film_snippet_state_none);
	
	int32 current_tick_estimate = saved_film_manager_get_current_tick_estimate();
	if (current_tick_estimate < 0)
	{
		event(_event_error, "networking:saved_film:snippets: failed to get recording start tick [%d]",
			current_tick_estimate);
	}
	
	if (!saved_film_snippet_globals.camera_file.open_file(k_camera_file_name, _async_buffer_file_access_read_write, _async_buffer_disposition_create_always))
	{
		event(_event_warning, "networking:saved_film:snippet: failed to open camera file for writing");
		return false;
	}
	
	ASSERT(saved_film_snippet_globals.recording_start_tick == NONE);
	ASSERT(saved_film_snippet_globals.recording_stop_tick == NONE);
	
	if (!saved_film_snippet_globals.destination_film.open_for_write(
		k_snippet_file_name,
		game_options_get(),
		controller_get_first_non_guest_signed_in_controller()))
	{
		event(_event_warning, "networking:saved_film:snippet: failed to open film");
		return false;
	}
	
	if (!saved_film_manager_rewind_and_seek_to_film_tick(current_tick_estimate, false))
	{
		event(_event_warning, "networking:saved_film:snippet: failed to request seek to tick %d",
			current_tick_estimate);
		return false;
	}
	
	saved_film_manager_get_director_state(&saved_film_snippet_globals.start_recording_director_state);
	saved_film_snippet_globals.recording_start_tick = current_tick_estimate;
	saved_film_snippet_globals.current_state = _saved_film_snippet_state_recording_waiting_for_seek;
	return true;
}

bool saved_film_snippet_stop_recording()
{
	ASSERT(saved_film_snippet_globals.current_state == _saved_film_snippet_state_recording);
	ASSERT(saved_film_snippet_globals.recording_start_tick >= 0);
	
	int32 current_tick_estimate = saved_film_manager_get_current_tick_estimate();
	if (current_tick_estimate < 0)
	{
		event(_event_error, "networking:saved_film:snippets: failed to get stop tick [%d]",
			current_tick_estimate);
		return false;
	}
	
	if (current_tick_estimate < saved_film_snippet_globals.recording_start_tick)
	{
		event(_event_error, "networking:saved_film:snippets: stop tick is less than start tick? [%d/%d]",
			saved_film_snippet_globals.recording_start_tick,
			current_tick_estimate);
		return false;
	}
	
	int32 start_tick = saved_film_snippet_globals.destination_film.get_current_tick()
		+ saved_film_snippet_globals.recording_start_tick
		- current_tick_estimate;
	if (start_tick < 0)
	{
		event(_event_error, "networking:saved_film:snippets: failed to compute valid start tick? [%d]",
			start_tick);
		return false;
	}
	
	saved_film_snippet_globals.destination_film.mark_film_as_snippet(start_tick);
	saved_film_snippet_globals.recording_stop_tick = current_tick_estimate;
	
	if (!saved_film_snippet_globals.destination_film.close())
	{
		event(_event_error, "networking:saved_film:snippets: failed to close snippet film");
		return false;
	}
	
	if (!saved_film_snippet_pad_camera_file_for_write_completion())
	{
		event(_event_error, "networking:saved_film:snippets: failed to finalize camera file for preview");
		return false;
	}
	
	saved_film_snippet_globals.current_state = _saved_film_snippet_state_recorded_and_ready;
	return true;
}

bool saved_film_snippet_update()
{
	// $IMPLEMENT
	return false;

	//ASSERT(game_in_progress());
	//ASSERT(game_is_playback());
	//
	//switch (saved_film_snippet_globals.current_state)
	//{
	//case _saved_film_snippet_state_recording:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_commiting_invoking_title_keyboard:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_commiting_waiting_title_keyboard:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_commiting_invoking_description_keyboard:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_commiting_waiting_description_keyboard:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_commiting_initiate_creation:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_commiting_wait_for_creation:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_commiting_initiate_metadata_update:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_commiting_wait_for_metadata_update:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//}
}

bool saved_film_snippet_update_after_simulation_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata)
{
	// $IMPLEMENT
	return false;

	//ASSERT(game_in_progress());
	//ASSERT(game_is_playback());
	//
	//switch (saved_film_snippet_globals.current_state)
	//{
	//case _saved_film_snippet_state_none:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_recording_waiting_for_seek:
	//case _saved_film_snippet_state_previewing_waiting_for_seek:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_recording_waiting_for_start:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_recording:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_recorded_and_ready:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_previewing:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//case _saved_film_snippet_state_resetting:
	//{
	//	// $IMPLEMENT
	//}
	//break;
	//default:
	//{
	//	VASSERT("this function should be unreachable");
	//}
	//break;
	//}
	//
	//return false;
}

void saved_film_snippets_notify_reverted_for_seek(bool* set_director_state_out)
{
	if (saved_film_snippet_globals.current_state == _saved_film_snippet_state_previewing_waiting_for_seek)
	{
		ASSERT(saved_film_snippet_globals.recording_start_tick >= 0);
		ASSERT(saved_film_snippet_globals.recording_stop_tick >= 0);
		ASSERT(saved_film_snippet_globals.recording_start_tick <= saved_film_snippet_globals.recording_stop_tick);
		ASSERT(saved_film_snippet_globals.camera_file.get_position() == 0);
		saved_film_snippet_globals.current_state = _saved_film_snippet_state_previewing;
	}
	else if (saved_film_snippet_globals.current_state == _saved_film_snippet_state_resetting)
	{
		ASSERT(saved_film_snippet_globals.recording_start_tick != NONE);
		ASSERT(saved_film_snippet_globals.recording_stop_tick != NONE);
		ASSERT(saved_film_snippet_globals.recording_start_tick <= saved_film_snippet_globals.recording_stop_tick);
		if (saved_film_manager_get_current_tick_estimate() == saved_film_snippet_globals.recording_start_tick)
		{
			saved_film_manager_set_director_state(&saved_film_snippet_globals.start_recording_director_state);
			saved_film_snippet_initialize_state();
			*set_director_state_out = true;
		}
	}
}

