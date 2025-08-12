#include "saved_games/saved_film_manager.hpp"

#include "cache/cache_files.hpp"
#include "cache/physical_memory_map.hpp"
#include "camera/director.hpp"
#include "camera/saved_film_director.hpp"
#include "cseries/async.hpp"
#include "cseries/async_xoverlapped_memory.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "main/levels.hpp"
#include "main/main.hpp"
#include "main/main_game.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "multithreading/synchronized_value.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "saved_games/autosave_queue.hpp"
#include "saved_games/saved_film_history.hpp"
#include "saved_games/saved_film_scratch_memory.hpp"
#include "saved_games/saved_film_snippet.hpp"
#include "simulation/simulation.hpp"

namespace
{
	bool saved_film_manager_should_record_film_default = false;
}

c_interlocked_long g_universal_saved_film_tick;
s_saved_film_manager_globals saved_film_manager_globals{};

static int32 saved_film_pending_seek_film_tick = NONE;
static REFERENCE_DECLARE(0x019428EC, c_physical_memory_allocation, g_physical_memory_allocation); // only use in this translation unit

static real32 saved_film_set_pending_playback_game_speed = -1.0f;

const char* k_playback_abort_reason_names[k_saved_film_playback_abort_reason_count]
{
	"none",
	"simulation_failed_to_read",
	"failed_to_update",
	"failed_to_update_after_simulation",
	"failed_to_start_recording",
	"failed_to_stop_recording",
	"failed_to_start_preview",
	"failed_to_stop_preview",
	"failed_to_delete",
	"failed_to_commit",
	"failed_to_write",
	"failed_to_write_record",
	"failed_to_revert",
	"failed_to_revert_by_index",
	"failed_to_load_gamestate_for_party",
	"map_signature_failed",
};

c_string_builder cache_file_signature_summary(int32 signature_size, const byte* signature_bytes)
{
	return c_string_builder("%d bytes, %02X:%02X:%02X:%02X:...",
		signature_size,
		signature_size > 0 ? signature_bytes[0] : 0,
		signature_size > 1 ? signature_bytes[1] : 0,
		signature_size > 2 ? signature_bytes[2] : 0,
		signature_size > 3 ? signature_bytes[3] : 0);
}

void saved_film_manager_abort_playback(e_saved_film_playback_abort_reason abort_reason)
{
	if (saved_film_manager_globals.playback_aborted)
	{
		return;
	}

	ASSERT(game_is_playback());
	event(_event_warning, "networking:saved_film:manager: playback aborted due to error '%s'",
		k_playback_abort_reason_names[abort_reason]);
	saved_film_manager_globals.playback_aborted = true;
	saved_film_manager_globals.playback_abort_reason = abort_reason;
}

bool saved_film_manager_authored_camera_locked_for_snippet()
{
	return game_in_progress()
		&& game_playback_get() == _game_playback_film
		&& saved_film_manager_globals.saved_film.m_film_state == _saved_film_open_for_read
		&& !saved_film_manager_globals.film_ended
		&& saved_film_snippet_recording_or_previewing();
}

bool saved_film_manager_automatic_debug_saving_enabled()
{
	return saved_film_manager_globals.automatic_debug_saving_enabled;
}

void saved_film_manager_build_file_path_from_name(const char* film_name, e_saved_film_file_path_creation_purpose purpose, c_static_string<128>* film_path_out)
{
	ASSERT(film_name);
	ASSERT(film_path_out);

	switch (purpose)
	{
	case _file_path_for_creation:
	{
		const wchar_t* filename_prefix = L"";
		const wchar_t* directory_path = autosave_queue_get_directory_path();
		if (autosave_queue_get_filename_prefix())
		{
			filename_prefix = autosave_queue_get_filename_prefix();
		}
		film_path_out->print("%ls\\%ls_%s.film", directory_path, filename_prefix, film_name);
	}
	break;
	case _file_path_for_creation_final:
	{
		const wchar_t* filename_prefix = L"";
		const wchar_t* directory_path = autosave_queue_get_directory_path();
		if (autosave_queue_get_filename_prefix())
		{
			filename_prefix = autosave_queue_get_filename_prefix();
		}
		film_path_out->print("%ls\\%ls_%s.film", directory_path, filename_prefix, film_name);
	}
	break;
	case _file_path_for_reading:
	{
		film_path_out->print("%s.film", film_name);
	}
	break;
	default:
	{
		throw "unreachable";
	}
	break;
	}
}

bool saved_film_manager_can_revert(e_saved_film_revert_type desired_revert_type)
{
	return saved_film_history_ready_for_revert_or_reset() && saved_film_history_can_revert_by_type(desired_revert_type);
}

bool saved_film_manager_can_set_playback_control()
{
	if (saved_film_manager_globals.playback_locked)
	{
		return false;
	}

	if (!game_is_authoritative_playback())
	{
		return false;
	}

	if (saved_film_manager_globals.film_ended)
	{
		return false;
	}

	if (saved_film_manager_get_snippet_state() != _saved_film_snippet_state_none)
	{
		return false;
	}

	if (saved_film_manager_globals.snippet_start_tick != NONE &&
		g_universal_saved_film_tick.peek() < saved_film_manager_globals.snippet_start_tick)
	{
		return false;
	}

	return true;
}

void saved_film_manager_clear_playback_state()
{
	saved_film_manager_globals.authored_cam_set_for_user.clear();
	saved_film_manager_globals.valid_camera_update_mask = 0;
	saved_film_manager_globals.desired_revert_index = NONE;
	saved_film_manager_globals.snippet_start_tick = NONE;
	saved_film_manager_globals.playback_game_speed = 1.0f;
	saved_film_manager_globals.camera_updates.clear();
	saved_film_manager_globals.film_ended = false;
	saved_film_manager_globals.film_ended_sound_disabled = false;
	saved_film_manager_globals.playback_locked = false;
	saved_film_manager_globals.desired_revert_type = _saved_film_revert_none;
	saved_film_manager_globals.seek_film_tick = NONE;
	saved_film_manager_globals.seek_and_stop = false;
	g_universal_saved_film_tick.set(0);
	saved_film_manager_globals.playback_aborted = false;
	saved_film_manager_globals.playback_abort_reason = _saved_film_playback_abort_reason_none;
}

void saved_film_manager_close()
{
	if (saved_film_manager_globals.film_close_in_progress)
	{
		event(_event_warning, "networking:saved_film:manager: film close in progress, not closing again");
		return;
	}

	saved_film_manager_globals.film_close_in_progress = true;
	saved_film_manager_globals.saved_film.close();
	saved_film_manager_clear_playback_state();
	saved_film_manager_globals.pending_gamestate_load = false;
	saved_film_manager_globals.gamestate_file_position = NONE;
	saved_film_manager_globals.film_close_in_progress = false;
}

void saved_film_manager_commit_snippet_autoname(e_controller_index controller_index)
{
	return;

	e_saved_film_snippet_state snippet_state = saved_film_manager_get_snippet_state();
	if (snippet_state == _saved_film_snippet_state_none)
	{
		event(_event_warning, "networking:saved_film:manager: snippts not available (can't commit by autoname)");
		return;
	}

	if (snippet_state != _saved_film_snippet_state_recorded_and_ready)
	{
		event(_event_warning, "networking:saved_film:manager: can't commit snippet by autoname [current in state %d]",
			snippet_state);
		return;
	}

	if (!saved_film_snippet_commit_by_autoname(controller_index))
	{
		saved_film_manager_abort_playback(_saved_film_playback_abort_snippet_failed_to_commit);
		return;
	}

	event(_event_message, "networking:saved_film:manager: committing snippet by autoname");
}

void saved_film_manager_commit_snippet_keyboard(e_controller_index controller_index)
{
	return;

	e_saved_film_snippet_state snippet_state = saved_film_manager_get_snippet_state();
	if (snippet_state == _saved_film_snippet_state_none)
	{
		event(_event_warning, "networking:saved_film:manager: snippts not available (can't commit by keyboard)");
		return;
	}

	if (snippet_state != _saved_film_snippet_state_recorded_and_ready)
	{
		event(_event_warning, "networking:saved_film:manager: can't commit snippet by keyboard [current in state %d]",
			snippet_state);
		return;
	}

	if (!saved_film_snippet_commit_by_keyboard(controller_index))
	{
		saved_film_manager_abort_playback(_saved_film_playback_abort_snippet_failed_to_commit);
		return;
	}

	event(_event_message, "networking:saved_film:manager: committing snippet by keyboard");
}

void saved_film_manager_copy_film_to_debug_path()
{
	ASSERT(saved_film_manager_globals.initialized);

	if (!saved_film_manager_globals.automatic_debug_saving_enabled || global_scenario->type > _scenario_type_multiplayer)
	{
		return;
	}

	void* blf_saved_film = overlapped_malloc(sizeof(s_blf_saved_film));
	if (!blf_saved_film)
	{
		event(_event_warning, "networking:saved_film:manager: failed to allocate copy buffer to save last saved film to the xbox dev kit drive!");
		return;
	}

	c_synchronized_long save_film_success = 0;
	c_synchronized_long save_film_complete = 0;
	if (saved_game_files_save_last_film_to_debugging_hard_drive(
		_controller0,
		blf_saved_film,
		sizeof(s_blf_saved_film),
		&save_film_success,
		&save_film_complete) == NONE)
	{
		event(_event_warning, "networking:saved_film:manager: failed to allocate async copy task to save last saved film to the xbox dev kit drive!");
	}
	else
	{
		internal_async_yield_until_done(&save_film_complete, false, false, __FILE__, __LINE__);
		if (save_film_success.peek() != 1)
		{
			event(_event_warning, "networking:saved_film:manager: failed to save last saved film to the xbox dev kit drive!");
		}
	}

	overlapped_free(blf_saved_film);
}

void saved_film_manager_create_film_directory()
{
	s_file_reference film_directory{};
	const wchar_t* directory_path = autosave_queue_get_directory_path();

	if (!file_reference_create_from_path_wide(&film_directory, directory_path, true))
	{
		return;
	}

	if (file_exists(&film_directory))
	{
		return;
	}

	file_create_parent_directories_if_not_present(&film_directory);
}

void saved_film_manager_delete_current_snippet()
{
	return;

	if (!saved_film_manager_snippets_available())
	{
		event(_event_warning, "networking:saved_film:manager: snippts not available (can't delete)");
		return;
	}

	e_saved_film_snippet_state current_state = saved_film_snippet_get_current_state();
	if (current_state != _saved_film_snippet_state_recorded_and_ready)
	{
		event(_event_warning, "networking:saved_film:manager: can't delete snippet [current in state %d]",
			current_state);
		return;
	}
	
	if (!saved_film_snippet_delete())
	{
		saved_film_manager_abort_playback(_saved_film_playback_abort_snippet_failed_to_delete);
		return;
	}

	event(_event_message, "networking:saved_film:manager: deleted snippet");
}

void saved_film_manager_delete_on_level_load(bool delete_on_level_load)
{
	event(_event_error, "networking:saved_film:manager: this no longer does anything");
}

void saved_film_manager_disable_version_checking(bool disable)
{
	saved_film_manager_globals.disable_version_checking = disable;
}

void saved_film_manager_dispose_from_old_map()
{
	if (game_is_ui_shell() || main_game_reset_in_progress())
	{
		return;
	}

	saved_film_manager_close();
	saved_film_history_dispose_from_saved_film_playback();
	saved_film_snippet_dispose_from_saved_film_playback();
	saved_film_manager_globals.screensaver_enabled = true;
}

void saved_film_manager_dispose()
{
	ASSERT(saved_film_manager_globals.initialized);

	saved_film_manager_globals.saved_film.dispose(&g_physical_memory_allocation);
	saved_film_manager_globals.saved_film_name.clear();
	saved_film_manager_globals.initialized = false;
	c_saved_film_scratch_memory::get()->dispose();
}

void saved_film_manager_end_film_internal()
{
	if (saved_film_manager_globals.film_ended)
	{
		event(_event_warning, "networking:saved_film:manager: film ended already set, not ending the film again");
		return;
	}

	saved_film_manager_globals.film_ended = true;
	saved_film_manager_globals.film_ended_system_milliseconds = system_milliseconds();

	if (game_is_authoritative_playback())
	{
		simulation_notify_saved_film_ended();
	}
}

bool saved_film_manager_film_is_ended(real32* out_seconds_ago)
{
	if (!game_in_progress() || !game_is_playback() || !saved_film_manager_globals.film_ended)
	{
		return false;
	}

	if (out_seconds_ago)
	{
		*out_seconds_ago = (float)system_milliseconds() * 0.001f;
	}

	return true;
}

bool saved_film_manager_film_valid(e_controller_index controller, const char* film_name)
{
	ASSERT(saved_film_manager_globals.initialized);

	if (saved_film_manager_globals.saved_film.get_film_state() != k_saved_film_state_none)
	{
		event(_event_warning, "networking:saved_film:manager: can't validate film, as we have one open already [state %d]",
			saved_film_manager_globals.saved_film.get_film_state());
		return false;
	}

	bool result = saved_film_manager_open_film_for_reading(controller, film_name);
	saved_film_manager_close();
	return result;
}

bool saved_film_manager_get_current_film_name(c_static_string<64>* film_name_out)
{
	ASSERT(film_name_out);

	film_name_out->set(saved_film_manager_globals.saved_film_name.get_string());
	return true;
}

const game_options* saved_film_manager_get_current_game_options()
{
	if (saved_film_manager_globals.saved_film.get_film_state() != _saved_film_open_for_read)
	{
		return NULL;
	}

	return saved_film_manager_globals.saved_film.get_game_options();
}

const s_saved_game_item_metadata* saved_film_manager_get_current_metadata()
{
	if (saved_film_manager_globals.saved_film.get_film_state() != _saved_film_open_for_read)
	{
		return NULL;
	}

	const s_blf_chunk_content_header* content_header = saved_film_manager_globals.saved_film.get_film_content_header();
	if (!content_header->metadata.is_valid())
	{
		return NULL;
	}

	return &content_header->metadata;
}

int32 saved_film_manager_get_current_tick_estimate()
{
	return g_universal_saved_film_tick.peek();
}

int32 saved_film_manager_get_current_tick()
{
	ASSERT(saved_film_manager_globals.initialized);

	return saved_film_manager_globals.saved_film.get_current_tick();
}

void saved_film_manager_get_director_state(s_saved_film_manager_director_state* director_state_out)
{
	csmemset(director_state_out, 0, sizeof(s_saved_film_manager_director_state));

	for (int32 user_index = 0; user_index < 4; user_index++)
	{
		if (!player_mapping_output_user_is_active(user_index))
		{
			continue;
		}

		c_director* director = director_get(user_index);
		if (director->get_type() != _director_mode_saved_film)
		{
			continue;
		}

		s_saved_film_manager_user_director_state* user_director_state = &director_state_out->user_director_states[user_index];
		c_saved_film_director* saved_film_director = (c_saved_film_director*)director;
		const c_camera* camera = saved_film_director->get_camera();

		user_director_state->observer_result = *observer_get_camera(user_index);
		user_director_state->camera_mode = camera->get_type();
		user_director_state->desired_camera_mode = saved_film_director->m_desired_camera_mode;
		user_director_state->valid = true;
		user_director_state->camera_target_player_absolute_index = saved_film_director->get_watched_player();
	}
}

__declspec(naked) void saved_film_manager_get_hud_interface_state_inline()
{
	// original instructions
	//      call    game_is_multiplayer
	//      test    al, al
	//      jz      short loc_A86C64

	ASM_ADDR(0x00A86C38, addr_A86C38);
	ASM_ADDR(0x00A86C64, addr_A86C64);

	__asm
	{
		// execute our instructions
		lea     eax, [edi + 0x8BC]
		push    eax
		call    saved_film_manager_get_hud_interface_state

		// execute the original instructions
		call    game_is_multiplayer
		test    al, al
		jz      short loc_A86C64

		// jump out to after our hook
		jmp     addr_A86C38

		loc_A86C64:
		jmp     addr_A86C64
	}
}
HOOK_DECLARE(0x00A86C2F, saved_film_manager_get_hud_interface_state_inline);

void saved_film_manager_get_hud_interface_state(s_saved_film_hud_interface_state* hud_state)
{
	csmemset(hud_state, 0, sizeof(s_saved_film_hud_interface_state));

	saved_film_history_get_hud_interface_state(hud_state);
	saved_film_snippet_get_hud_interface_state(hud_state);
}

// $TODO: check my work
bool saved_film_manager_get_last_recorded_film(char* filepath, int32 maximum_characters, s_saved_game_item_metadata* out_optional_metadata)
{
	s_file_reference directory{};
	file_reference_create_from_path_wide(&directory, autosave_queue_get_directory_path(), true);

	s_find_file_data file_data{};
	find_files_start_with_search_spec(&file_data, 0, &directory, "*.film");

	s_file_reference current_file{};
	s_file_last_modification_date mod_date{};

	bool v7 = false;
	bool v8 = true;

	s_file_reference newest_file{};
	s_file_last_modification_date newest_file_mod_date{};

	while (find_files_next(&file_data, &current_file, &mod_date))
	{
		if (!v8 && file_compare_last_modification_dates(&mod_date, &newest_file_mod_date) <= 0)
		{
			continue;
		}

		newest_file = current_file;
		v7 = true;
		v8 = false;
	}
	find_files_end(&file_data);

	if (!v7)
	{
		return false;
	}

	file_reference_get_fullpath(&newest_file, filepath, maximum_characters);
	if (out_optional_metadata)
	{
		return saved_game_read_metadata_from_file(&newest_file, out_optional_metadata);
	}

	return true;
}

int32 saved_film_manager_get_length_in_ticks()
{
	ASSERT(saved_film_manager_globals.initialized);

	return saved_film_manager_globals.saved_film.get_length_in_ticks();
}

real32 saved_film_manager_get_pending_playback_game_speed()
{
	return saved_film_set_pending_playback_game_speed;
}

real32 saved_film_manager_get_playback_game_speed()
{
	return saved_film_manager_globals.playback_game_speed;
}

int32 saved_film_manager_get_position()
{
	ASSERT(saved_film_manager_globals.initialized);

	return saved_film_manager_globals.saved_film.get_position();
}

const wchar_t* saved_film_manager_get_recording_directory()
{
	return autosave_queue_get_directory_path();
}

bool saved_film_manager_get_reproduction_enabled()
{
	return saved_film_manager_globals.reproduction_mode_enabled;
}

uns32 saved_film_manager_get_simulation_camera_update_mask()
{
	return saved_film_manager_globals.valid_camera_update_mask;
}

bool saved_film_manager_get_simulation_camera_updates(int32 camera_index, s_simulation_camera_update* simulation_camera_update_out)
{
	ASSERT(VALID_INDEX(camera_index, saved_film_manager_globals.camera_updates.get_count()));

	if (!TEST_BIT(saved_film_manager_globals.valid_camera_update_mask, camera_index))
	{
		return false;
	}

	*simulation_camera_update_out = saved_film_manager_globals.camera_updates[camera_index];
	return true;
}

int32 saved_film_manager_get_snippet_start_tick()
{
	return NONE;

	if (!game_in_progress() || !game_is_playback())
	{
		return NONE;
	}

	int32 snippet_start_tick = saved_film_manager_globals.snippet_start_tick;

	if (!saved_film_manager_snippets_available())
	{
		return snippet_start_tick;
	}

	if (saved_film_snippet_get_current_state() == _saved_film_snippet_state_none)
	{
		return snippet_start_tick;
	}

	int32 current_snippet_start_tick = NONE;
	if (!saved_film_snippet_get_current_start_tick(&current_snippet_start_tick))
	{
		return snippet_start_tick;
	}

	return current_snippet_start_tick;
}

e_saved_film_snippet_state saved_film_manager_get_snippet_state()
{
	if (!saved_film_manager_snippets_available())
	{
		return _saved_film_snippet_state_none;
	}

	return saved_film_snippet_get_current_state();
}

int32 saved_film_manager_get_ticks_remaining()
{
	ASSERT(saved_film_manager_globals.initialized);

	if (saved_film_manager_globals.saved_film.get_film_state() != _saved_film_open_for_read)
	{
		return NULL;
	}

	return saved_film_manager_globals.saved_film.get_ticks_remaining();
}

void saved_film_manager_handle_camera_update(uns32 valid_camera_update_mask, const s_simulation_camera_update* camera_updates)
{
	saved_film_manager_globals.valid_camera_update_mask = valid_camera_update_mask;
	saved_film_manager_globals.camera_updates.clear();

	if (TEST_BIT(valid_camera_update_mask, 0))
	{
		saved_film_manager_globals.camera_updates[0] = *camera_updates;
	}
}

bool saved_film_manager_handle_revert(int32 saved_film_file_position, int32 film_tick)
{
	ASSERT(saved_film_manager_globals.initialized);

	g_universal_saved_film_tick.set(film_tick);
	if (!game_is_authoritative_playback())
	{
		return true;
	}
	return saved_film_manager_globals.saved_film.handle_revert(saved_film_file_position, film_tick);
}

bool saved_film_manager_has_pending_global_state_change()
{
	if (!game_in_progress() || !game_is_playback())
	{
		return false;
	}

	if (saved_film_manager_globals.pending_gamestate_load)
	{
		return true;
	}

	return saved_film_manager_revert_desired();
}

void saved_film_manager_initialize_for_new_map()
{
	if (game_is_ui_shell() || main_game_reset_in_progress())
	{
		return;
	}

	saved_film_manager_clear_playback_state();

	if (!game_is_playback())
	{
		return;
	}

	if (game_is_authoritative_playback())
	{
		int32 local_signature_size = 0;
		const byte* local_signature_bytes = NULL;

		if (!cache_file_get_content_signature(&local_signature_size, &local_signature_bytes))
		{
			event(_event_warning, "networking:saved_film:manager: signature MISMATCH, failed to get local signature");
			saved_film_manager_abort_playback(_saved_film_playback_map_signature_failed);
		}
		else
		{
			int32 map_signature_size = saved_film_manager_globals.saved_film.m_film_header.film_header.build_compatibility.map_signature_size;
			const byte* map_signature_bytes = saved_film_manager_globals.saved_film.m_film_header.film_header.build_compatibility.map_signature_bytes;

			bool use_full_language_dependent_signature = !game_options_valid() || !game_is_campaign();

			if (!cache_file_content_signatures_match(local_signature_size, local_signature_bytes, map_signature_size, map_signature_bytes, use_full_language_dependent_signature))
			{
				event(_event_error, "networking:saved_film:manager: signature MISMATCH (%s), film signature (%s) does not match our local one (%s)",
					use_full_language_dependent_signature ? "full language dependent" : "language neutral only",
					cache_file_signature_summary(map_signature_size, map_signature_bytes).get_string(),
					cache_file_signature_summary(local_signature_size, local_signature_bytes).get_string());

				saved_film_manager_abort_playback(_saved_film_playback_map_signature_failed);
			}
			else
			{
				event(_event_message, "networking:saved_film:manager: signature match (%s), film signature matches our local one (%s)",
					use_full_language_dependent_signature ? "full language dependent" : "language neutral only",
					cache_file_signature_summary(local_signature_size, local_signature_bytes).get_string());
			}
		}
	}

	saved_film_manager_globals.snippet_start_tick = game_options_get()->playback_start_tick;
	saved_film_history_initialize_for_saved_film_playback();
	saved_film_snippet_initialize_for_saved_film_playback();
	saved_film_manager_globals.screensaver_enabled = false;
}

void saved_film_manager_initialize()
{
	ASSERT(!saved_film_manager_globals.initialized);

	saved_film_manager_globals.saved_film.initialize(&g_physical_memory_allocation);
	saved_film_manager_globals.saved_film_name.clear();
	saved_film_manager_globals.show_timestamp = true;
	saved_film_manager_globals.disable_version_checking = false;
	saved_film_manager_globals.automatic_debug_saving_enabled = false;
	saved_film_manager_globals.pending_gamestate_load = false;
	saved_film_manager_globals.gamestate_file_position = NONE;
	saved_film_manager_globals.film_close_in_progress = false;
	saved_film_manager_globals.ui_screen_active = false;
	saved_film_manager_clear_playback_state();
	saved_film_manager_create_film_directory();
	saved_film_manager_globals.initialized = true;
	c_saved_film_scratch_memory::get()->initialize();
	saved_film_history_initialize();
	saved_film_snippet_initialize();
}

bool saved_film_manager_is_reading()
{
	ASSERT(saved_film_manager_globals.initialized);

	return saved_film_manager_globals.saved_film.get_film_state() == _saved_film_open_for_read;
}

bool saved_film_manager_is_writing()
{
	ASSERT(saved_film_manager_globals.initialized);

	return saved_film_manager_globals.saved_film.get_film_state() == _saved_film_open_for_write;
}

void saved_film_manager_load_pending_gamestate()
{
	// $TODO: implement me
}

bool saved_film_manager_load_pending_gamestate_to_compressor()
{
	// $TODO: implement me

	return false;
}

void saved_film_manager_memory_dispose()
{
	saved_film_snippet_memory_dispose();
	saved_film_history_memory_dispose();
	c_saved_film_scratch_memory::get()->memory_dispose();
}

void saved_film_manager_memory_initialize(e_map_memory_configuration memory_configuration)
{
	if (!map_memory_configuration_is_saved_film(memory_configuration))
	{
		return;
	}

	c_saved_film_scratch_memory::get()->memory_initialize();
	saved_film_history_memory_initialize();
	saved_film_snippet_memory_initialize();
}

void saved_film_manager_notify_gamestate_decompression_after_load_procs()
{
	if (!game_in_progress() || !game_is_playback())
	{
		return;
	}

	if (game_is_campaign())
	{
		game_state_save();
	}

	//determinism_debug_manager_set_file_position(0, 0);

	{
		LOCAL_TAG_RESOURCE_SCOPE_LOCK;
		players_finish_creation();
	}

	saved_film_manager_globals.authored_cam_set_for_user.clear();

	for (int32 user_index = 0; user_index < 4; user_index++)
	{
		if (!player_mapping_output_user_is_active(user_index))
		{
			continue;
		}

		c_director* director = director_get(user_index);
		if (director->get_type() != _director_mode_saved_film)
		{
			continue;
		}

		c_saved_film_director* saved_film_director = (c_saved_film_director*)director;
		saved_film_director->notify_revert();
	}
}

void saved_film_manager_notify_gamestate_decompression_before_load_procs()
{
	if (!game_in_progress() || !game_is_playback() || !game_is_multiplayer())
	{
		return;
	}

	if (saved_film_manager_is_reading())
	{
		ASSERT(saved_film_manager_globals.pending_gamestate_load);
	}

	//determinism_debug_manager_reset_for_core_load();
	saved_film_history_notify_initial_gamestate_loaded();
}

void saved_film_manager_notify_gamestate_load(e_saved_film_game_state_load_source game_state_load_source)
{
	if (saved_film_manager_globals.saved_film.get_film_state() != _saved_film_open_for_write)
	{
		return;
	}

	if (game_state_load_source == _saved_film_game_state_load_source_core && saved_film_manager_globals.saved_film.get_current_tick() > 0)
	{
		saved_film_manager_close();
		saved_film_manager_open_film_for_writing(saved_film_manager_globals.saved_film_name.get_string(), game_options_get());
	}

	//determinism_debug_manager_reset_for_core_load();

	if (saved_film_manager_globals.saved_film.get_current_tick())
	{
		event(_event_error, "networking:saved_film:manager: you are loading a core while a film is in the middle of being recorded, things are going to be bad!");
	}
	else
	{
		saved_film_manager_globals.saved_film.write_gamestate();
	}
}

void saved_film_manager_notify_out_of_sync()
{
}

void saved_film_manager_notify_remote_end_film()
{
	if (!game_in_progress() || !game_is_playback())
	{
		event(_event_error, "networking:saved_film:manager: remote machine tried to end film but we are not running playback");
		return;
	}

	if (game_is_authoritative_playback())
	{
		event(_event_error, "networking:saved_film:manager: remote machine tried to end film but we are the authority!");
		return;
	}

	event(_event_message, "networking:saved_film:manager: remote machine ended film");
	saved_film_manager_end_film_internal();
}

void saved_film_manager_notify_reverted_gamestate_loaded(int32 history_record_index, int32 update_number, void* gamestate, int32 gamestate_size)
{
	event(_event_message, "networking:saved_film:manager: notified reverted [history %d update %d]",
		history_record_index,
		update_number);

	if (saved_film_manager_globals.seek_film_tick != NONE && saved_film_snippet_get_current_state())
	{
		saved_film_snippet_finished_revert_for_seek(update_number, gamestate, gamestate_size);
	}

	simulation_notify_saved_film_revert(history_record_index, update_number);
}

void saved_film_manager_notify_snippet_preview_complete()
{
	return;

	event(_event_message, "networking:saved_film:manager: notified film snippet complete");
	ASSERT(game_is_authoritative_playback());

	saved_film_manager_globals.playback_locked = true;
	saved_film_manager_globals.playback_game_speed = 0.0f;
}

bool saved_film_manager_open_film_for_reading(e_controller_index controller_index, const char* film_name)
{
	e_saved_film_state film_state = saved_film_manager_globals.saved_film.m_film_state;
	if (film_state != k_saved_film_state_none)
	{
		event(_event_error, "networking:saved_film:manager: can't open film for reading, current state is %d",
			film_state);
		return false;
	}

	c_static_string<128> saved_film_file_path{};
	ASSERT(saved_film_manager_globals.initialized);

	event(_event_message, "networking:saved_film:manager: opening film %s for reading",
		film_name);

	if (csstrstr(film_name, "\\"))
	{
		saved_film_file_path.set(film_name);
	}
	else
	{
		saved_film_manager_build_file_path_from_name(film_name, _file_path_for_reading, &saved_film_file_path);
	}

	// $TODO: clean this up
	c_debug_output_path debug_output_path{};
	bool valid = saved_film_manager_globals.saved_film.open_for_read(saved_film_file_path.get_string(), controller_index, saved_film_manager_globals.disable_version_checking);
	if (valid || !csstrstr(film_name, "\\")
		&& (saved_film_file_path.print("%s%s%s.film", debug_output_path.get_root(), "saved_films\\", film_name),
			valid = saved_film_manager_globals.saved_film.open_for_read(saved_film_file_path.get_string(), controller_index, saved_film_manager_globals.disable_version_checking)))
	{
		saved_film_manager_globals.saved_film_name.set(film_name);
		saved_film_manager_globals.pending_gamestate_load = saved_film_manager_globals.saved_film.m_film_header.film_header.contains_gamestate;
		saved_film_manager_globals.gamestate_file_position = saved_film_manager_globals.saved_film.get_position();
	}
	else
	{
		saved_film_manager_globals.saved_film_name.clear();
		saved_film_manager_globals.pending_gamestate_load = false;
		saved_film_manager_globals.gamestate_file_position = NONE;
	}

	return valid;
}

bool saved_film_manager_open_film_for_writing(const char* film_name, const game_options* options)
{
	e_saved_film_state film_state = saved_film_manager_globals.saved_film.get_film_state();
	if (film_state != k_saved_film_state_none)
	{
		event(_event_error, "networking:saved_film:manager: can't open film for writing, current state is %d",
			film_state);
		return false;
	}

	c_static_string<128> saved_film_path{};
	ASSERT(saved_film_manager_globals.initialized);

	event(_event_message, "networking:saved_film:manager: opening film %s for writing",
		film_name);

	saved_film_manager_build_file_path_from_name(film_name, _file_path_for_creation, &saved_film_path);
	bool valid = saved_film_manager_globals.saved_film.open_for_write(saved_film_path.get_string(), options, controller_get_first_non_guest_signed_in_controller());
	if (valid)
	{
		saved_film_manager_globals.saved_film_name.set(film_name);
	}
	else
	{
		saved_film_manager_globals.saved_film_name.clear();
	}

	return valid;
}

void saved_film_manager_perform_global_state_change()
{
	ASSERT(saved_film_manager_has_pending_global_state_change());
	event(_event_message, "networking:saved_film:manager: performing global state change");

	s_saved_film_manager_director_state director_state{};
	saved_film_manager_get_director_state(&director_state);

	if (saved_film_manager_globals.pending_gamestate_load)
	{
		saved_film_manager_load_pending_gamestate();
		saved_film_manager_globals.pending_gamestate_load = false;
	}

	bool director_state_set = false;
	if (saved_film_manager_revert_desired())
	{
		saved_film_manager_perform_revert(&director_state_set);
	}
	if (!director_state_set)
	{
		saved_film_manager_set_director_state(&director_state);
	}
}

void saved_film_manager_perform_revert(bool* set_director_state_out)
{
	ASSERT(saved_film_manager_revert_desired());

	int32 current_tick_estimate = saved_film_manager_get_current_tick_estimate();
	if (saved_film_manager_globals.seek_film_tick == NONE || saved_film_manager_globals.seek_film_tick > current_tick_estimate)
	{
		if (saved_film_manager_globals.desired_revert_type)
		{
			event(_event_message, "networking:saved_film:manager: reverting by type %d",
				saved_film_manager_globals.desired_revert_type);
			saved_film_history_revert_by_type(saved_film_manager_globals.desired_revert_type);
		}
		else if (saved_film_manager_globals.desired_revert_index != NONE)
		{
			event(_event_message, "networking:saved_film:manager: reverting by index %d",
				saved_film_manager_globals.desired_revert_index);

			if (saved_film_manager_globals.desired_revert_index == 0)
			{
				saved_film_manager_globals.authored_cam_set_for_user.clear();
			}

			if (!saved_film_history_revert_by_index(saved_film_manager_globals.desired_revert_index))
			{
				saved_film_manager_abort_playback(_saved_film_playback_history_failed_to_revert_by_index);
			}
		}
	}
	else
	{
		event(_event_message, "networking:saved_film:manager: reverting by film tick %d",
			saved_film_manager_globals.seek_film_tick);

		saved_film_history_revert_by_film_tick(saved_film_manager_globals.seek_film_tick);
		if (saved_film_manager_snippets_available())
		{
			saved_film_snippets_notify_reverted_for_seek(set_director_state_out);
		}
		saved_film_manager_update_seeking(current_tick_estimate);
	}

	for (int32 user_index = 0; user_index < 4; user_index++)
	{
		if (!player_mapping_output_user_is_active(user_index))
		{
			continue;
		}

		c_director* director = director_get(user_index);
		if (director->get_type() != _director_mode_saved_film)
		{
			continue;
		}

		c_saved_film_director* saved_film_director = (c_saved_film_director*)director;
		saved_film_director->notify_revert();
	}

	saved_film_manager_globals.desired_revert_type = _saved_film_revert_none;
	saved_film_manager_globals.desired_revert_index = NONE;
}

void saved_film_manager_play_hs(int16 controller_index, const char* film_name)
{
	saved_film_manager_play((e_controller_index)controller_index, film_name);
}

void saved_film_manager_play_last_hs()
{
	char filename[64]{};
	if (saved_film_manager_get_last_recorded_film(filename, sizeof(filename), NULL))
	{
		saved_film_manager_play(k_no_controller, filename);
	}
}

void saved_film_manager_play(e_controller_index controller_index, const char* film_name)
{
	ASSERT(saved_film_manager_globals.initialized);

	network_life_cycle_end();
	simulation_end(_simulation_abort_reason_preparing_to_play_film);

	saved_film_manager_set_playback_game_speed(1.0f);
	saved_film_manager_close();

	if (!saved_film_manager_open_film_for_reading(controller_index, film_name))
	{
		event(_event_error, "networking:saved_film:manager: unable to read header from saved film '%s', film is invalid",
			film_name);
		return;
	}

	event(_event_message, "networking:saved_film:manager: playing saved film '%s'",
		film_name);

	saved_film_manager_globals.saved_film_name.set(film_name);

	game_options options{};
	csmemcpy(&options, saved_film_manager_globals.saved_film.get_game_options(), sizeof(game_options));
	options.game_playback = _game_playback_film;
	options.playback_length_in_ticks = saved_film_manager_get_length_in_ticks();

	int32 snippet_start_tick = NONE;
	if (saved_film_manager_globals.saved_film.m_film_header.film_header.is_snippet)
	{
		snippet_start_tick = saved_film_manager_globals.saved_film.m_film_header.film_header.snippet_start_tick;
	}
	options.playback_start_tick = snippet_start_tick;

	options.record_saved_film = false;

	game_options_validate(&options);
	main_game_change(&options);
}

bool saved_film_manager_playback_aborted()
{
	return saved_film_manager_globals.playback_aborted;
}

void saved_film_manager_playback_lock_set(real32 playback_game_speed, bool locked)
{
	ASSERT(game_is_authoritative_playback());

	saved_film_manager_globals.playback_locked = locked;
	saved_film_manager_globals.playback_game_speed = MAX(0.0f, MIN(30.0f, playback_game_speed));
}

void saved_film_manager_preview_snippet_start()
{
	return;

	e_saved_film_snippet_state snippet_state = saved_film_manager_get_snippet_state();
	if (snippet_state == _saved_film_snippet_state_none)
	{
		event(_event_warning, "networking:saved_film:manager: snippts not available (can't start preview)");
		return;
	}

	if (snippet_state != _saved_film_snippet_state_recorded_and_ready)
	{
		event(_event_warning, "networking:saved_film:manager: can't preview snippet [current in state %d]",
			snippet_state);
		return;
	}

	if (!saved_film_snippet_preview_start())
	{
		saved_film_manager_abort_playback(_saved_film_playback_abort_snippet_failed_to_start_preview);
		return;
	}

	event(_event_message, "networking:saved_film:manager: starting snippet preview");
	saved_film_manager_playback_lock_set(0.0f, true);
}

void saved_film_manager_preview_snippet_stop()
{
	return;

	e_saved_film_snippet_state snippet_state = saved_film_manager_get_snippet_state();
	if (snippet_state == _saved_film_snippet_state_none)
	{
		event(_event_warning, "networking:saved_film:manager: snippts not available (can't stop preview)");
		return;
	}

	if (snippet_state != _saved_film_snippet_state_previewing)
	{
		event(_event_warning, "networking:saved_film:manager: can't stop snippet preview [current in state %d]",
			snippet_state);
		return;
	}

	if (saved_film_manager_get_current_tick_estimate() <= saved_film_manager_get_snippet_start_tick())
	{
		event(_event_warning, "networking:saved_film:manager: can't stop snippet preview [snippet has not actually started previewing]");
		return;
	}

	if (!saved_film_snippet_preview_stop())
	{
		saved_film_manager_abort_playback(_saved_film_playback_abort_snippet_failed_to_stop_preview);
		return;
	}

	event(_event_message, "networking:saved_film:manager: stopping snippet preview");
	saved_film_manager_playback_lock_set(0.0f, true);
}

bool saved_film_manager_read_simulation_update(const s_saved_film_update* update, struct simulation_update* simulation_update_out)
{
	ASSERT(saved_film_manager_globals.initialized);

	return saved_film_manager_globals.saved_film.read_simulation_update(update, simulation_update_out);
}

bool saved_film_manager_read_update(s_saved_film_update* update_out)
{
	ASSERT(saved_film_manager_globals.initialized);

	return saved_film_manager_globals.saved_film.read_update(update_out);
}

void saved_film_manager_render_debug()
{
	if (game_in_progress() && game_is_playback() && g_saved_films_show_timestamp)
	{
		// $TODO: implement
	}
}

void saved_film_manager_replay_film()
{
	if (!game_in_progress() || !game_is_playback())
	{
		event(_event_error, "networking:saved_film:manager: not running playback, can't replay film");
		return;
	}

	if (!game_is_authoritative_playback())
	{
		event(_event_error, "networking:saved_film:manager: not the authority, can't request replay film!");
		return;
	}

	if (!saved_film_history_ready_for_revert_or_reset())
	{
		event(_event_warning, "networking:saved_film:manager: history not ready for revert or reset, can't replay film");
		return;
	}

	event(_event_message, "networking:saved_film:manager: replaying film by reverting to index 0");
	saved_film_manager_globals.desired_revert_index = 0;
	saved_film_manager_playback_lock_set(1.0f, false);
}

void saved_film_manager_request_end_film()
{
	if (!game_in_progress() || !game_is_playback())
	{
		event(_event_error, "networking:saved_film:manager: tried to end film but we are not running playback");
		return;
	}

	if (!game_is_authoritative_playback())
	{
		event(_event_error, "networking:saved_film:manager: tried to end film but we are running remote playback and cannot end authoritatively");
		return;
	}

	event(_event_message, "networking:saved_film:manager: local authority ended film");
	saved_film_manager_end_film_internal();
}

void saved_film_manager_request_revert_by_index(int32 revert_index)
{
	event(_event_message, "networking:saved_film:manager: requesting revert by index %d", revert_index);

	ASSERT(!saved_film_manager_globals.playback_locked);
	ASSERT(game_is_playback());
	ASSERT(!game_is_authoritative_playback());

	saved_film_manager_globals.desired_revert_index = revert_index;
}

void saved_film_manager_request_revert(e_saved_film_revert_type desired_revert_type)
{
	ASSERT(desired_revert_type != _saved_film_revert_none);

	if (!saved_film_manager_can_set_playback_control())
	{
		event(_event_warning, "networking:saved_film:manager: can't set playback control, can't request revert");
		return;
	}

	if (!saved_film_history_can_revert_by_type(desired_revert_type))
	{
		event(_event_warning, "networking:saved_film:manager: revert type %d unavailable",
			desired_revert_type);
		return;
	}

	event(_event_message, "networking:saved_film:manager: requesting revert by type (%d)",
		desired_revert_type);

	saved_film_manager_globals.desired_revert_type = desired_revert_type;
}

bool saved_film_manager_revert_desired()
{
	if (saved_film_manager_globals.film_ended || !simulation_in_progress())
	{
		return false;
	}

	if ((saved_film_manager_globals.seek_film_tick == NONE || saved_film_manager_globals.seek_film_tick > saved_film_manager_get_current_tick_estimate())
		&& saved_film_manager_globals.desired_revert_type == _saved_film_revert_none)
	{
		return saved_film_manager_globals.desired_revert_index != NONE;
	}
	return true;
}

bool saved_film_manager_rewind_and_seek_to_film_tick(int32 film_tick, bool seek_and_stop)
{
	ASSERT(film_tick >= 0);

	if (!saved_film_manager_snippets_available())
	{
		event(_event_warning, "networking:saved_film:manager: film snippets unavailable, can't queue seek");
		return false;
	}

	if (saved_film_manager_has_pending_global_state_change())
	{
		event(_event_warning, "networking:saved_film:manager: already have global state change pending, can't queue seek");
		return false;
	}

	int32 current_tick_estimate = saved_film_manager_get_current_tick_estimate();
	if (film_tick > current_tick_estimate)
	{
		event(_event_warning, "networking:saved_film:manager: attempting to rewind and seek to a tick in the future? [%d > %d]",
			film_tick,
			current_tick_estimate);
		return false;
	}

	event(_event_message, "networking:saved_film:manager: seeking to film tick %d (seek and stop %s)",
		film_tick,
		seek_and_stop ? "TRUE" : "FALSE");

	saved_film_manager_playback_lock_set(1.0f, true);
	saved_film_manager_globals.seek_film_tick = film_tick;
	saved_film_manager_globals.seek_and_stop = seek_and_stop;
	return true;
}

void saved_film_manager_seek_to_film_tick_hs(int32 film_tick)
{
	if (film_tick < 0)
	{
		event(_event_warning, "networking:saved_film:manager: invalid film tick %d, can't seek",
			film_tick);
		return;
	}

	if (saved_film_manager_has_pending_global_state_change())
	{
		event(_event_warning, "networking:saved_film:manager: already have global state change queued, can't seek");
		return;
	}

	saved_film_pending_seek_film_tick = film_tick;
}

bool saved_film_manager_seeking(int32* seek_time_available_out)
{
	ASSERT(seek_time_available_out);

	if (!game_in_progress()
		|| !game_is_playback()
		|| saved_film_manager_globals.saved_film.m_film_state != _saved_film_open_for_read
		|| saved_film_manager_globals.seek_film_tick == NONE
		|| saved_film_manager_globals.seek_film_tick < saved_film_manager_get_current_tick_estimate())
	{
		return false;
	}

	*seek_time_available_out = saved_film_manager_globals.seek_film_tick - saved_film_manager_get_current_tick_estimate();
	return true;
}

// $TODO: check my work
void saved_film_manager_set_director_state(const s_saved_film_manager_director_state* director_state)
{
	LOCAL_TAG_RESOURCE_SCOPE_LOCK;

	for (int32 user_index = 0; user_index < 4; user_index++)
	{
		if (!player_mapping_output_user_is_active(user_index))
		{
			continue;
		}

		c_director* director = director_get(user_index);
		if (director->get_type() != _director_mode_saved_film)
		{
			continue;
		}

		const s_saved_film_manager_user_director_state* user_director_state = &director_state->user_director_states[user_index];
		if (!user_director_state->valid)
		{
			continue;
		}

		c_saved_film_director* saved_film_director = (c_saved_film_director*)director;
		observer_obsolete_position(user_index);
		saved_film_director->force_set_camera_mode(user_director_state->camera_mode, 0.0f);
		saved_film_director->m_desired_camera_mode = user_director_state->desired_camera_mode;
		saved_film_director->set_watched_player(NONE);

		c_camera* camera = saved_film_director->get_camera();
		camera->set_target(NONE);
		if (user_director_state->camera_target_player_absolute_index != NONE)
		{
			player_datum* player = DATUM_GET(player_data, player_datum, user_director_state->camera_target_player_absolute_index);
			if (player && !TEST_BIT(player->flags, _player_left_game_bit))
			{
				int32 player_index = player_index_from_absolute_player_index(user_director_state->camera_target_player_absolute_index);
				saved_film_director->set_watched_player(player_index);
				if (!saved_film_director->in_free_camera_mode())
				{
					int32 unit_index = player->unit_index;
					if (unit_index == NONE)
					{
						unit_index = player->dead_unit_index;
					}
					camera->set_target(unit_index);
				}
			}
		}

		camera->set_position(&user_director_state->observer_result.position);
		camera->set_forward(&user_director_state->observer_result.forward);
	}
}

bool saved_film_manager_set_pending_playback_game_speed(real32 game_speed)
{
	saved_film_set_pending_playback_game_speed = game_speed;
	return true;
}

bool saved_film_manager_set_playback_game_speed(real32 game_speed)
{
	if (!saved_film_manager_can_set_playback_control())
	{
		event(_event_warning, "networking:saved_film:manager: can't set playback control, can't set game speed");
		return false;
	}

	saved_film_manager_globals.playback_game_speed = MAX(0.0f, MIN(30.0f, game_speed));
	return true;
}

bool saved_film_manager_set_position(int32 position)
{
	ASSERT(saved_film_manager_globals.initialized);

	return saved_film_manager_globals.saved_film.set_position(position);
}

void saved_film_manager_set_reproduction_enabled(bool reproduction_enabled)
{
	if (saved_film_manager_globals.reproduction_mode_enabled == reproduction_enabled)
	{
		return;
	}

	saved_film_manager_globals.reproduction_mode_enabled = reproduction_enabled;
	event(_event_message, "networking:saved_film:manager: set reproduction mode (%s)",
		reproduction_enabled ? "enabled" : "disabled");

	if (game_in_progress() && game_is_playback())
	{
		event(_event_message, "networking:saved_film:manager: reproduction mode changed, forcing recreation of players' output users");
		for (int32 user_index = 0; user_index < 4; user_index++)
		{
			player_mapping_attach_output_user(user_index, NONE);
		}
		players_finish_creation();
	}
}

void saved_film_manager_should_record_film_default_set(bool b)
{
	saved_film_manager_should_record_film_default = b;
}

bool __cdecl game_options_verify_for_begin_load_map(game_options* options, char* error_string, int32 error_string_length)
{
	if (!network_squad_session_get_film())
	{
		options->record_saved_film = saved_film_manager_should_record_film(options);
	}

	return game_options_verify(options, error_string, error_string_length);
}
HOOK_DECLARE_CALL(0x00491F77, game_options_verify_for_begin_load_map);

bool saved_film_manager_should_record_film(const game_options* options)
{
	ASSERT(options);

	if (options->game_mode == _game_mode_campaign)
	{
		s_level_datum level_data{};
		if (levels_try_and_get_campaign_map(options->map_id, &level_data))
		{
			return level_data.flags.test(_level_allows_saved_films);
		}
	}
	else if (options->game_mode == _game_mode_multiplayer)
	{
		return true;
	}

	return saved_film_manager_should_record_film_default;
}

void saved_film_manager_show_timestamp(bool show_timestamp)
{
	saved_film_manager_globals.show_timestamp = show_timestamp;
}

bool saved_film_manager_snippets_available()
{
	return game_in_progress()
		&& game_playback_get() == _game_playback_film
		&& saved_film_manager_globals.saved_film.m_film_state == _saved_film_open_for_read
		&& !saved_film_manager_globals.film_ended;
}

void saved_film_manager_start_recording_snippet()
{
	if (!saved_film_manager_snippets_available())
	{
		event(_event_warning, "networking:saved_film:manager: snippts not available (can't start recording)");
		return;
	}

	e_saved_film_snippet_state snippet_state = saved_film_snippet_get_current_state();
	if (snippet_state)
	{
		event(_event_warning, "networking:saved_film:manager: can't start recording snippet [current in state %d]",
			snippet_state);
		return;
	}

	if (saved_film_manager_get_snippet_start_tick() != NONE)
	{
		event(_event_warning, "networking:saved_film:manager: can't start recording snippet [can't record snippet of snippet]");
		return;
	}

	if (player_mapping_output_user_active_count() != 1)
	{
		event(_event_warning, "networking:saved_film:manager: can't start recording snippet [active user count != 1]");
		return;
	}

	if (controller_get_first_non_guest_signed_in_controller() == k_no_controller)
	{
		event(_event_warning, "networking:saved_film:manager: can't start recording snippet [can't get non-guest signed in controller]");
		return;
	}

	if (saved_film_history_ready_for_revert_or_reset())
	{
		event(_event_warning, "networking:saved_film:manager: can't start recording snippet [saved film history not ready]");
		return;
	}

	if (!saved_film_snippet_start_recording())
	{
		saved_film_manager_abort_playback(_saved_film_playback_abort_snippet_failed_to_start_recording);
		return;
	}

	event(_event_message, "networking:saved_film:manager: starting to record snippet");
	saved_film_manager_playback_lock_set(1.0f, true);
}

void saved_film_manager_stop_recording_snippet()
{
	if (!saved_film_manager_snippets_available())
	{
		event(_event_warning, "networking:saved_film:manager: snippts not available (can't stop recording)");
		return;
	}

	e_saved_film_snippet_state snippet_state = saved_film_snippet_get_current_state();
	if (snippet_state != _saved_film_snippet_state_recording)
	{
		event(_event_warning, "networking:saved_film:manager: can't stop recording snippet [current in state %d]",
			snippet_state);
		return;
	}

	if (!saved_film_snippet_stop_recording())
	{
		saved_film_manager_abort_playback(_saved_film_playback_abort_snippet_failed_to_stop_recording);
		return;
	}

	event(_event_message, "networking:saved_film:manager: stopping snppet recording");
	saved_film_manager_playback_lock_set(0.0f, true);
}

bool saved_film_manager_timestamp_enabled_internal()
{
	return saved_film_manager_globals.show_timestamp;
}

void saved_film_manager_toggle_automatic_debug_saving(bool enable)
{
	saved_film_manager_globals.automatic_debug_saving_enabled = enable;
}

void saved_film_manager_update_after_simulation_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata)
{
	saved_film_history_update_after_simulation_update(update, metadata);

	if (!game_in_progress() || !game_is_playback())
	{
		return;
	}

	g_universal_saved_film_tick.set(metadata->saved_film_tick);

	saved_film_manager_globals.valid_camera_update_mask = update->valid_camera_update_mask;
	saved_film_manager_globals.camera_updates.clear();

	if (TEST_BIT(update->valid_camera_update_mask, 0))
	{
		saved_film_manager_globals.camera_updates[0] = update->camera_updates[0];
	}

	if (game_is_authoritative_playback())
	{
		saved_film_manager_update_seeking(metadata->saved_film_tick);
	}

	if (!saved_film_manager_snippets_available())
	{
		return;
	}

	if (!saved_film_snippet_update_after_simulation_update(update, metadata))
	{
		saved_film_manager_abort_playback(_saved_film_playback_abort_snippet_failed_to_update_after_simulation);
	}
}

void saved_film_manager_update_before_simulation_update()
{
	saved_film_history_update_before_simulation_update(saved_film_manager_get_snippet_state() != _saved_film_snippet_state_none);
}

void saved_film_manager_update_seeking(int32 current_film_tick)
{
	if (!game_in_progress()
		|| !game_is_playback()
		|| saved_film_manager_globals.saved_film.m_film_state != _saved_film_open_for_read
		|| saved_film_manager_globals.seek_film_tick == NONE)
	{
		return;
	}

	if (current_film_tick < saved_film_manager_globals.seek_film_tick)
	{
		e_game_playback_type game_playback = game_playback_get();
		if (game_playback == _game_playback_film)
		{
			saved_film_manager_playback_lock_set(3.0f, game_playback);
			return;
		}

		saved_film_manager_playback_lock_set(2.0f, true);
		return;
	}

	if (current_film_tick != saved_film_manager_globals.seek_film_tick
		&& current_film_tick != saved_film_manager_globals.seek_film_tick - 1)
	{
		return;
	}

	saved_film_manager_playback_lock_set(saved_film_manager_globals.seek_and_stop ? 0.0f : 1.0f, false);
	saved_film_manager_globals.seek_film_tick = NONE;
	saved_film_manager_globals.seek_and_stop = false;
}

void saved_film_manager_update_snippet_authored_cameras()
{
	if (saved_film_manager_globals.snippet_start_tick == NONE)
	{
		return;
	}

	for (int32 user_index = 0; user_index < 4; user_index++)
	{
		if (!player_mapping_output_user_is_active(user_index))
		{
			continue;
		}

		c_director* director = director_get(user_index);
		if (director->get_type() != _director_mode_saved_film)
		{
			continue;
		}

		if (saved_film_manager_globals.authored_cam_set_for_user.test(user_index))
		{
			continue;
		}

		event(_event_message, "networking:saved_film:manager: setting user %d camera to authored for snippet playback",
			user_index);

		director->set_camera_mode(_camera_mode_authored, 0.0f);
		saved_film_manager_globals.authored_cam_set_for_user.set(user_index, true);
	}
}

void saved_film_manager_update_ui_screens()
{
	//if (!game_is_playback())
	//{
	//	return;
	//}
	//
	//const int32 snippet_start_tick = saved_film_manager_get_snippet_start_tick();
	//if (snippet_start_tick == NONE || g_universal_saved_film_tick.peek() >= snippet_start_tick)
	//{
	//	if (!saved_film_manager_globals.ui_screen_active)
	//	{
	//		return;
	//	}
	//
	//	c_gui_screen_widget* screen_widget = window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, in_progress_mini_me));
	//	if (screen_widget)
	//	{
	//		screen_widget->transition_out(_transition_out_normal);
	//	}
	//
	//	saved_film_manager_globals.ui_screen_active = false;
	//	return;
	//}
	//
	//if (!saved_film_manager_globals.ui_screen_active)
	//{
	//	c_gui_screen_widget* screen_widget = window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, in_progress_mini_me));
	//	if (screen_widget)
	//	{
	//		return;
	//	}
	//
	//	if (c_load_in_progress_screen_message* screen_message = new c_load_in_progress_screen_message(
	//		screen_message,
	//		STRING_ID(gui, in_progress_mini_me),
	//		k_any_controller,
	//		k_number_of_player_windows,
	//		STRING_ID(gui, film_snippet_preparing_title),
	//		STRING_ID(gui, film_snippet_preparing_message),
	//		false,
	//		true))
	//	{
	//		user_interface_messaging_post(screen_message);
	//		saved_film_manager_globals.ui_screen_active = true;
	//	}
	//}
}

void saved_film_manager_update()
{
	saved_film_manager_update_ui_screens();

	if (!game_in_progress())
	{
		return;
	}

	if (game_is_playback())
	{
		// $TODO: implement this

	}

	if (!game_is_playback() && saved_film_manager_globals.saved_film.m_film_state > _saved_film_open_for_write)
	{
		g_universal_saved_film_tick.set(game_time_get());

		return;
	}
}

int32 saved_film_manager_upload_start(int32 maximum_file_count, s_file_reference* out_file_list)
{
	return NONE;
}

bool saved_film_manager_write_simulation_update(const struct simulation_update* update)
{
	ASSERT(saved_film_manager_globals.initialized);

	if (saved_film_manager_globals.saved_film.m_film_state != _saved_film_open_for_write)
	{
		event(_event_error, "networking:saved_film:manager: film not open for write, can't write simulation update");
		return false;
	}

	if (!saved_film_manager_globals.saved_film.write_simulation_update(update))
	{
		event(_event_warning, "networking:saved_film:manager: failed to write simulation update %d to film", update->update_number);
		return false;
	}

	g_universal_saved_film_tick.set(saved_film_manager_get_current_tick());
	return true;
}

