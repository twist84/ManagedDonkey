#include "saved_games/saved_film_snippet.hpp"

#include "cache/physical_memory_map.hpp"
#include "camera/director.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "interface/c_controller.hpp"

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
	// $TODO: implement me
}

void saved_film_snippet_build_auto_name(e_controller_index controller_index, c_static_wchar_string<16>* snippet_name_out)
{
	//const s_saved_game_item_metadata* metadata = saved_film_manager_get_current_metadata();
	//ASSERT(metadata);
	//
	//wchar_t auto_name[16]{};
	//ustrnzcpy(auto_name, metadata->display_name, NUMBEROF(auto_name));
	//saved_game_files_uniquify_name(controller_index, _saved_game_film_clip, auto_name, NUMBEROF(auto_name));
	//snippet_name_out->set(auto_name);
}

bool saved_film_snippet_close_camera_file()
{
	// $TODO: implement me
	return false;

	//if (saved_film_snippet_globals.camera_file.m_state == _async_buffer_state_writing && 
	//	!saved_film_snippet_pad_camera_file_for_write_completion())
	//{
	//	event(_event_warning, "networking:saved_film:snippet: failed to finalize camera file before close");
	//}
	//
	//if (!saved_film_snippet_globals.camera_file.close_file())
	//{
	//	event(_event_warning, "networking:saved_film:snippet: failed to close camera file");
	//	return false;
	//}
	//
	//return true;
}

bool saved_film_snippet_commit_by_autoname(e_controller_index controller_index)
{
	// $TODO: implement me
	return false;
}

bool saved_film_snippet_commit_by_keyboard(e_controller_index controller_index)
{
	// $TODO: implement me
	return false;
}

bool saved_film_snippet_delete()
{
	// $TODO: implement me
	return false;
}

void saved_film_snippet_dispose_from_saved_film_playback()
{
}

void saved_film_snippet_dispose_virtual_keyboard_task()
{
	//if (saved_film_snippet_globals.virtual_keyboard_task)
	//{
	//	overlapped_task_block_until_finished(saved_film_snippet_globals.virtual_keyboard_task);
	//	saved_film_snippet_globals.virtual_keyboard_task = NULL;
	//}
}

bool saved_film_snippet_finished_revert_for_seek(int32 update_number, void* gamestate, int32 gamestate_size)
{
	// $TODO: implement me
	return false;
}

void saved_film_snippet_get_camera_for_simulation_update(s_simulation_camera_update* camera_update_out)
{
	// $TODO: implement me
}

bool saved_film_snippet_get_current_start_tick(int32* start_tick_out)
{
	// $TODO: implement me
	return false;
}

e_saved_film_snippet_state saved_film_snippet_get_current_state()
{
	return saved_film_snippet_globals.current_state;
}

void saved_film_snippet_get_hud_interface_state(s_saved_film_hud_interface_state* state)
{
	// $TODO: implement me
}

void saved_film_snippet_initialize()
{
	saved_film_snippet_initialize_state();
}

void saved_film_snippet_initialize_for_saved_film_playback()
{
	saved_film_snippet_initialize_state();
}

void saved_film_snippet_initialize_state()
{
	// $TODO: implement me
}

void saved_film_snippet_memory_dispose()
{
	//saved_film_snippet_close_camera_file();
	//saved_film_snippet_globals.camera_file.release_storage(&g_physical_memory_allocation);
	//saved_film_snippet_globals.destination_film.close();
	//saved_film_snippet_globals.destination_film.dispose(&g_physical_memory_allocation);
	//saved_film_snippet_initialize_state();
}

void saved_film_snippet_memory_initialize()
{
	//if (!saved_film_snippet_globals.camera_file.allocate_storage(&g_physical_memory_allocation, 0x1000))
	//{
	//	event(_event_critical, "networking:saved_film:snippet: failed to allocate storage for camera file");
	//	return;
	//}
	//
	//if (!saved_film_snippet_globals.destination_film.initialize(&g_physical_memory_allocation))
	//{
	//	event(_event_critical, "networking:saved_film:snippet: failed to allocate storage for snippet film file");
	//	return;
	//}
}

bool saved_film_snippet_pad_camera_file_for_write_completion()
{
	// $TODO: implement me
	return false;
}

bool saved_film_snippet_preview_start()
{
	// $TODO: implement me
	return false;

	//ASSERT(saved_film_snippet_globals.recording_start_tick != NONE);
	//ASSERT(saved_film_snippet_globals.recording_stop_tick != NONE);
	//ASSERT(saved_film_snippet_globals.recording_start_tick <= saved_film_snippet_globals.recording_stop_tick);
	//ASSERT(saved_film_snippet_globals.current_state == _saved_film_snippet_state_recorded_and_ready);
	//
	//if (!saved_film_snippet_globals.camera_file.set_position(0))
	//{
	//	event(_event_error, "networking:saved_film:snippets: failed to set camera file position for preview");
	//	return false;
	//}
	//
	//if (!saved_film_manager_rewind_and_seek_to_film_tick(saved_film_snippet_globals.recording_start_tick, 0))
	//{
	//	event(_event_error, "networking:saved_film:snippets: failed to request seek for preview [%d]",
	//		saved_film_snippet_globals.recording_start_tick);
	//	return false;
	//}
	//
	//for (int32 user_index = player_mapping_first_active_output_user();
	//	user_index != NONE;
	//	user_index = player_mapping_next_active_output_user(user_index))
	//{
	//	director_get(user_index)->set_camera_mode(_camera_mode_authored, 0.0f);
	//}
	//
	//saved_film_snippet_globals.current_state = _saved_film_snippet_state_previewing_waiting_for_seek;
	//return true;
}

bool saved_film_snippet_preview_stop()
{
	// $TODO: implement me
	return false;

	//ASSERT(saved_film_snippet_globals.recording_start_tick != NONE);
	//ASSERT(saved_film_snippet_globals.recording_stop_tick != NONE);
	//ASSERT(saved_film_snippet_globals.recording_start_tick <= saved_film_snippet_globals.recording_stop_tick);
	//ASSERT(saved_film_snippet_globals.current_state == _saved_film_snippet_state_previewing);
	//
	//saved_film_snippet_globals.current_state = _saved_film_snippet_state_recorded_and_ready;
	//return true;
}

bool saved_film_snippet_recording_or_previewing()
{
	// $TODO: implement me
	return false;
}

bool saved_film_snippet_start_recording()
{
	// $TODO: implement me
	return false;

	//ASSERT(saved_film_snippet_globals.current_state == _saved_film_snippet_state_none);
	//
	//int32 current_tick_estimate = saved_film_manager_get_current_tick_estimate();
	//if (current_tick_estimate < 0)
	//{
	//	event(_event_error, "networking:saved_film:snippets: failed to get recording start tick [%d]",
	//		current_tick_estimate);
	//}
	//
	//if (!saved_film_snippet_globals.camera_file.open_file(k_camera_file_name, _async_buffer_file_access_read_write, _async_buffer_disposition_create_always))
	//{
	//	event(_event_warning, "networking:saved_film:snippet: failed to open camera file for writing");
	//	return false;
	//}
	//
	//ASSERT(saved_film_snippet_globals.recording_start_tick == NONE);
	//ASSERT(saved_film_snippet_globals.recording_stop_tick == NONE);
	//
	//if (!saved_film_snippet_globals.destination_film.open_for_write(
	//	k_snippet_file_name,
	//	game_options_get(),
	//	controller_get_first_non_guest_signed_in_controller()))
	//{
	//	event(_event_warning, "networking:saved_film:snippet: failed to open film");
	//	return false;
	//}
	//
	//if (!saved_film_manager_rewind_and_seek_to_film_tick(current_tick_estimate, false))
	//{
	//	event(_event_warning, "networking:saved_film:snippet: failed to request seek to tick %d",
	//		current_tick_estimate);
	//	return false;
	//}
	//
	//saved_film_manager_get_director_state(&saved_film_snippet_globals.start_recording_director_state);
	//saved_film_snippet_globals.recording_start_tick = current_tick_estimate;
	//saved_film_snippet_globals.current_state = _saved_film_snippet_state_recording_waiting_for_seek;
	//return true;
}

bool saved_film_snippet_stop_recording()
{
	// $TODO: implement me
	return false;

	//ASSERT(saved_film_snippet_globals.current_state == _saved_film_snippet_state_recording);
	//ASSERT(saved_film_snippet_globals.recording_start_tick >= 0);
	//
	//int32 current_tick_estimate = saved_film_manager_get_current_tick_estimate();
	//if (current_tick_estimate < 0)
	//{
	//	event(_event_error, "networking:saved_film:snippets: failed to get stop tick [%d]",
	//		current_tick_estimate);
	//	return false;
	//}
	//
	//if (current_tick_estimate < saved_film_snippet_globals.recording_start_tick)
	//{
	//	event(_event_error, "networking:saved_film:snippets: stop tick is less than start tick? [%d/%d]",
	//		saved_film_snippet_globals.recording_start_tick,
	//		current_tick_estimate);
	//	return false;
	//}
	//
	//int32 start_tick = saved_film_snippet_globals.destination_film.get_current_tick()
	//	+ saved_film_snippet_globals.recording_start_tick
	//	- current_tick_estimate;
	//if (start_tick < 0)
	//{
	//	event(_event_error, "networking:saved_film:snippets: failed to compute valid start tick? [%d]",
	//		start_tick);
	//	return false;
	//}
	//
	//saved_film_snippet_globals.destination_film.mark_film_as_snippet(start_tick);
	//saved_film_snippet_globals.recording_stop_tick = current_tick_estimate;
	//
	//if (!saved_film_snippet_globals.destination_film.close())
	//{
	//	event(_event_error, "networking:saved_film:snippets: failed to close snippet film");
	//	return false;
	//}
	//
	//if (!saved_film_snippet_pad_camera_file_for_write_completion())
	//{
	//	event(_event_error, "networking:saved_film:snippets: failed to finalize camera file for preview");
	//	return false;
	//}
	//
	//saved_film_snippet_globals.current_state = _saved_film_snippet_state_recorded_and_ready;
	//return true;
}

bool saved_film_snippet_update()
{
	// $TODO: implement me
	return false;
}

bool saved_film_snippet_update_after_simulation_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata)
{
	// $TODO: implement me
	return false;
}

void saved_film_snippets_notify_reverted_for_seek(bool* set_director_state_out)
{
	// $TODO: implement me
}

