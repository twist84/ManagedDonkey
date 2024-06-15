#include "main/loading.hpp"

#include "bink/bink_playback.hpp"
#include "cache/cache_files.hpp"
#include "cache/cache_files_windows.hpp"
#include "cseries/cseries.hpp"
#include "cseries/progress.hpp"
#include "game/game.hpp"
#include "interface/user_interface_networking.hpp"
#include "main/main.hpp"
#include "main/main_game.hpp"
#include "main/main_render.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "scenario/scenario.hpp"

REFERENCE_DECLARE(0x02390D00, bool, disable_progress_screen);
REFERENCE_DECLARE(0x02390D04, loading_globals_definition, loading_globals);
REFERENCE_DECLARE(0x0471AA58, long, loaded_resource_bytes);
REFERENCE_DECLARE(0x0471AA5C, long, total_resource_bytes);

c_static_string<256> loading_globals_definition::loading_progress{};
c_static_string<256> loading_globals_definition::copy_progress{};

HOOK_DECLARE(0x0052F180, main_load_map);
HOOK_DECLARE(0x0052FA00, main_loading_idle);
HOOK_DECLARE(0x0052FB60, main_loading_progress_done);
HOOK_DECLARE(0x0052FB70, main_loading_progress_new);
HOOK_DECLARE(0x0052FB80, main_loading_progress_update);

wchar_t const* __cdecl loading_get_text()
{
	return INVOKE(0x0052EBC0, loading_get_text);
}

void __cdecl loading_basic_progress_complete()
{
	INVOKE(0x0052EDF0, loading_basic_progress_complete);

	//ASSERT(loading_globals.basic_progress_enabled);
	//loading_globals.basic_progress_phase = _basic_loading_phase_none;
	//loading_globals.progress_sizes1[0] = 0.0f;
	//loading_globals.progress_sizes1[1] = 0.0f;
	//loading_globals.progress_sizes1[2] = 0.0f;
	//loading_globals.progress = 1.0f;
}

void __cdecl loading_basic_progress_disable()
{
	INVOKE(0x0052EE20, loading_basic_progress_disable);

	//ASSERT(loading_globals.basic_progress_enabled);
	//loading_globals.basic_progress_enabled = false;
	//loading_globals.scenario_path = NULL;
}

void __cdecl loading_basic_progress_enable(char const* scenario_path, long insertion_point)
{
	INVOKE(0x0052EE40, loading_basic_progress_enable, scenario_path, insertion_point);

	//ASSERT(!loading_globals.basic_progress_enabled);
	//loading_globals.scenario_path = scenario_path;
	//loading_globals.insertion_point = insertion_point;
	//loading_globals.basic_progress_enabled = true;
	//loading_globals.has_progress_sizes = 0;
	//loading_globals.basic_progress_phase = _basic_loading_phase_none;
	//csmemset(loading_globals.progress_sizes0, 0, sizeof(loading_globals.progress_sizes0));
	//csmemset(loading_globals.progress_sizes1, 0, sizeof(loading_globals.progress_sizes1));
	//loading_globals.progress = 0.0f;
}

bool __cdecl loading_basic_progress_enabled()
{
	return INVOKE(0x0052EEA0, loading_basic_progress_enabled);

	//return loading_globals.basic_progress_enabled;
}

real __cdecl loading_basic_progress_get()
{
	return INVOKE(0x0052EEB0, loading_basic_progress_get);

	//ASSERT(loading_globals.basic_progress_enabled);
	//return loading_globals.progress;
}

void __cdecl loading_basic_progress_phase_begin(long phase, dword update_size)
{
	INVOKE(0x0052EEC0, loading_basic_progress_phase_begin, phase, update_size);

	//if (loading_globals.basic_progress_enabled)
	//{
	//	ASSERT(loading_globals.basic_progress_phase == _basic_loading_phase_none);
	//	ASSERT(VALID_INDEX(phase, k_basic_loading_phase_count));
	//	loading_globals.basic_progress_phase = phase;
	//	loading_basic_progress_update_size(update_size);
	//}
}

void __cdecl loading_basic_progress_phase_end()
{
	INVOKE(0x0052EEF0, loading_basic_progress_phase_end);

	//if (loading_globals.basic_progress_enabled)
	//{
	//	ASSERT(VALID_INDEX(loading_globals.basic_progress_phase, k_basic_loading_phase_count));
	//	loading_basic_progress_update_size(0);
	//	loading_globals.basic_progress_phase = _basic_loading_phase_none;
	//}
}

void __cdecl loading_basic_progress_update_fraction(real progress_fraction)
{
	INVOKE(0x0052EF10, loading_basic_progress_update_fraction, progress_fraction);

	//if (loading_globals.basic_progress_enabled && VALID_INDEX(loading_globals.basic_progress_phase, k_basic_loading_phase_count))
	//{
	//	ASSERT(IN_RANGE_INCLUSIVE(progress_fraction, 0.0f, 1.0f));
	//	loading_basic_progress_update_size(dword((1.0f - progress_fraction) * loading_globals.progress_sizes0[loading_globals.basic_progress_phase]));
	//}
}

void __cdecl loading_basic_progress_update_phase_sizes()
{
	INVOKE(0x0052EF70, loading_basic_progress_update_phase_sizes);

	// #TODO: implement
}

void __cdecl loading_basic_progress_update_size(dword update_size)
{
	INVOKE(0x0052F010, loading_basic_progress_update_size, update_size);

	// #TODO: implement
}

bool __cdecl main_blocking_load_in_progress(real* out_progress)
{
	return INVOKE(0x0052F130, main_blocking_load_in_progress, out_progress);
}

//bool __cdecl main_load_map(char const *,enum e_map_load_type)
bool __cdecl main_load_map(char const* scenario_path, long map_load_type)
{
	//return INVOKE(0x0052F180, main_load_map, scenario_path, map_load_type);

	return main_load_map_with_insertion_point(-1, scenario_path, map_load_type);
}

// a2 is possibly insertion_point
real __cdecl main_load_map_loading_progress(long scenario_type, short a2, char const* scenario_path)
{
	return INVOKE(0x0052F1A0, main_load_map_loading_progress, scenario_type, a2, scenario_path);
}

//e_map_load_status __cdecl main_load_map_status(char const* scenario_path)
long __cdecl main_load_map_status(char const* scenario_path)
{
	//return INVOKE(0x0052F250, main_load_map_status, scenario_path);

	return main_load_map_status_with_insertion_point(NONE, scenario_path);
}

//e_map_load_status __cdecl main_load_map_status_with_insertion_point(short insertion_point, char const* scenario_path)
long __cdecl main_load_map_status_with_insertion_point(short insertion_point, char const* scenario_path)
{
	return INVOKE(0x0052F270, main_load_map_status_with_insertion_point, insertion_point, scenario_path);
}

//bool __cdecl main_load_map_with_insertion_point(short, char const*, enum e_map_load_type)
bool __cdecl main_load_map_with_insertion_point(short insertion_point, char const* scenario_path, long map_load_type)
{
	return INVOKE(0x0052F2E0, main_load_map_with_insertion_point, insertion_point, scenario_path, map_load_type);
}

bool __cdecl main_load_next_map_loading()
{
	return INVOKE(0x0052F480, main_load_next_map_loading);
}

void __cdecl main_loading_dispose()
{
	INVOKE(0x0052F4E0, main_loading_dispose);

	//loading_globals.loading_in_progress = false;
	//progress_set_default_callbacks(NULL);
}

void __cdecl main_loading_enable_spinner(bool enable_spinner)
{
	INVOKE(0x0052F4F0, main_loading_enable_spinner, enable_spinner);

	//loading_globals.spinner_enabled = enable_spinner;
}

bool __cdecl main_loading_get_action(struct s_main_loading_action* out_loading_action)
{
	return INVOKE(0x0052F500, main_loading_get_action, out_loading_action);
}

//enum e_gui_game_mode __cdecl main_loading_get_gui_game_mode(void)
long __cdecl main_loading_get_gui_game_mode()
{
	return INVOKE(0x0052F8F0, main_loading_get_gui_game_mode);

	//if (user_interface_squad_exists())
	//	return user_interface_squad_get_ui_game_mode();
	//
	//e_gui_game_mode game_mode = _ui_game_mode_none;
	//if (c_gui_pregame_setup_manager::get()->try_to_get_last_set_game_mode(&game_mode))
	//	return game_mode;
	//
	//return _ui_game_mode_none;
}

e_main_pregame_frame __cdecl main_loading_get_loading_status(c_static_wchar_string<12288>* loading_status)
{
	//return INVOKE(0x0052F930, main_loading_get_loading_status, loading_status);

#if defined(_DEBUG)
	main_loading_enable_spinner(true);
#endif // _DEBUG

	if (bink_playback_active())
		return _main_pregame_frame_normal;

	if (loading_globals.basic_progress_enabled)
	{
		if (disable_progress_screen)
			return _main_pregame_frame_loading_screen;

		long loading_progress = 0;
		if (loaded_resource_bytes > 0 && total_resource_bytes > 0)
			loading_progress = long((loaded_resource_bytes * 100.0f) / total_resource_bytes);

		if (loading_globals.spinner_enabled)
		{
			if (!g_active_designer_zone_mask)
			{
				static wchar_t const* spinner_states[] = { L"/", L"-", L"\\" };
				static long spinner_state_index = 8 * system_milliseconds() / 1000 % NUMBEROF(spinner_states);

				static dword last_time = system_milliseconds();
				dword time = system_milliseconds();
				if (time < last_time + 250)
					return _main_pregame_frame_none;

				last_time = time;
				spinner_state_index = (spinner_state_index + 1) % NUMBEROF(spinner_states);

				if (game_in_progress() && !loading_globals.tag_load_in_progress)
					return _main_pregame_frame_none;

				if (loading_status)
					loading_status->print(L"%s %d%%|n", spinner_states[spinner_state_index], loading_progress);

				if (!loading_globals.copy_progress.is_empty())
				{
					if (loading_status)
						loading_status->append_print(L"%S|n", loading_globals.copy_progress.get_string());
				}

				if (loading_globals.loading_in_progress && !loading_globals.loading_progress.is_empty())
				{
					if (loading_status)
						loading_status->append_print(L"%S", loading_globals.loading_progress.get_string());
				}

				if (string_is_not_empty(loading_globals.scenario_path))
				{
					if (loading_status)
						loading_status->append_print(L"loading scenario %S...", loading_globals.scenario_path);
				}

				if (loading_status)
					loading_status->append(L"|n");

				return _main_pregame_frame_progress_report;
			}
		}

		if (loaded_resource_bytes > 0 && total_resource_bytes > 0)
		{
			REFERENCE_DECLARE(0x0189DEE4, dword, game_language);

			dword temp_game_language = game_language;
			game_language = 0;
			wchar_t const* loading_text = loading_get_text();
			game_language = temp_game_language;

			if (loading_status)
			{
				loading_status->print(L"%s %d%%", loading_text, loading_progress);
				loading_status->append(L"|n|nhttps://github.com/twist84/ManagedDonkey/");
			}
		}

		return _main_pregame_frame_status_message;
	}

	return _main_pregame_frame_none;
}

void __cdecl main_loading_idle()
{
	//INVOKE(0x0052FA00, main_loading_idle);

	s_main_loading_action loading_action{};
	if (!main_loading_get_action(&loading_action) || network_session_interface_wants_main_menu_to_load())
		return;

	if (string_is_not_empty(loading_action.scenario_path))
	{
		if (loading_action.copy_stop)
		{
			cache_files_copy_stop(cache_file_get_canonical_path(k_single_player_shared_scenario_tag).get_string());
			cache_files_copy_stop(cache_file_get_canonical_path(k_introduction_scenario_tag).get_string());
		}
		else if (loading_action.copy_map_start_only)
		{
			cache_files_copy_map_start_only(loading_action.scenario_path, loading_action.load_action);
		}
		else
		{
			cache_files_copy_map(loading_action.scenario_path, loading_action.load_action);
		}

		if (loading_action.map_has_progression)
		{
			cache_file_tag_resources_start_map_prefetch(static_cast<short>(loading_action.campaign_id), loading_action.scenario_path);
		}
		else if (loading_action.stop_map_prefetch)
		{
			cache_file_tag_resources_stop_map_prefetch();
		}
	}
	else
	{
		if (!loading_action.map_has_progression && loading_action.stop_map_prefetch)
			cache_file_tag_resources_stop_map_prefetch();
	}
}

void __cdecl main_loading_initialize()
{
	INVOKE(0x0052FAD0, main_loading_initialize);

	//progress_callbacks callbacks
	//{
	//	.progress_new_proc = main_loading_progress_new,
	//	.progress_update_proc = main_loading_progress_update,
	//	.progress_done_proc = main_loading_progress_done,
	//	.progress_data = NULL
	//};
	//progress_set_default_callbacks(&callbacks);
	//loading_globals.loading_in_progress = false;
}

bool __cdecl main_loading_is_idle()
{
	return INVOKE(0x0052FB40, main_loading_is_idle);

	//return cache_files_copy_in_progress() == 0;
}

void __cdecl main_loading_progress_done(char const* description, void* userdata)
{
	//INVOKE(0x0052FB60, main_loading_progress_done, description, userdata);

	ASSERT(loading_globals.loading_in_progress, "someone called main_loading_progress_done without calling main_loading_progress_start!");
	loading_globals.loading_progress.print("%s... done|n", description);
	main_loop_pregame();
	loading_globals.loading_in_progress = false;
}

void __cdecl main_loading_progress_new(char const* description, void* userdata)
{
	//INVOKE(0x0052FB70, main_loading_progress_new, description, userdata);

	loading_globals.loading_progress.print("%s...|n", description);
	loading_globals.loading_in_progress = true;
	main_loop_pregame();
}

void __cdecl main_loading_progress_update(char const* description, char const* scenario_path, long progress, void* userdata)
{
	//INVOKE(0x0052FB80, main_loading_progress_update, description, scenario_path, progress, userdata);

	loading_globals.loading_progress.print("%s... %2d%c|n", description, progress, '%');
	if (scenario_path)
		loading_globals.loading_progress.append_print("    %s", scenario_path);
	loading_basic_progress_update_fraction(progress / 100.0f);
	main_loop_pregame();
}

