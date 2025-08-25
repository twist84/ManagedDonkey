#include "main/loading.hpp"

#include "bink/bink_playback.hpp"
#include "cache/cache_files.hpp"
#include "cache/cache_files_windows.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
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
REFERENCE_DECLARE(0x0471AA58, int32, loaded_resource_bytes);
REFERENCE_DECLARE(0x0471AA5C, int32, total_resource_bytes);

c_static_string<256> loading_globals_definition::loading_progress{};
c_static_string<256> loading_globals_definition::copy_progress{};

HOOK_DECLARE(0x0052F180, main_load_map);
HOOK_DECLARE(0x0052FA00, main_loading_idle);
HOOK_DECLARE(0x0052FB60, main_loading_progress_done);
HOOK_DECLARE(0x0052FB70, main_loading_progress_new);
HOOK_DECLARE(0x0052FB80, main_loading_progress_update);

const wchar_t* __cdecl loading_get_text()
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

void __cdecl loading_basic_progress_enable(const char* scenario_path, int32 insertion_point)
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

real32 __cdecl loading_basic_progress_get()
{
	return INVOKE(0x0052EEB0, loading_basic_progress_get);

	//ASSERT(loading_globals.basic_progress_enabled);
	//return loading_globals.progress;
}

void __cdecl loading_basic_progress_phase_begin(int32 phase, uns32 update_size)
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

void __cdecl loading_basic_progress_update_fraction(real32 progress_fraction)
{
	INVOKE(0x0052EF10, loading_basic_progress_update_fraction, progress_fraction);

	//if (loading_globals.basic_progress_enabled && VALID_INDEX(loading_globals.basic_progress_phase, k_basic_loading_phase_count))
	//{
	//	ASSERT(IN_RANGE_INCLUSIVE(progress_fraction, 0.0f, 1.0f));
	//	loading_basic_progress_update_size(uns32((1.0f - progress_fraction) * loading_globals.progress_sizes0[loading_globals.basic_progress_phase]));
	//}
}

void __cdecl loading_basic_progress_update_phase_sizes()
{
	INVOKE(0x0052EF70, loading_basic_progress_update_phase_sizes);

	// $IMPLEMENT
}

void __cdecl loading_basic_progress_update_size(uns32 update_size)
{
	INVOKE(0x0052F010, loading_basic_progress_update_size, update_size);

	// $IMPLEMENT
}

bool __cdecl main_blocking_load_in_progress(real32* out_progress)
{
	return INVOKE(0x0052F130, main_blocking_load_in_progress, out_progress);
}

//bool __cdecl main_load_map(const char*,enum e_map_load_type)
bool __cdecl main_load_map(const char* scenario_path, int32 map_load_type)
{
	//return INVOKE(0x0052F180, main_load_map, scenario_path, map_load_type);

	return main_load_map_with_insertion_point(-1, scenario_path, map_load_type);
}

// a2 is possibly insertion_point
real32 __cdecl main_load_map_loading_progress(int32 scenario_type, int16 a2, const char* scenario_path)
{
	return INVOKE(0x0052F1A0, main_load_map_loading_progress, scenario_type, a2, scenario_path);
}

//e_map_load_status __cdecl main_load_map_status(const char* scenario_path)
int32 __cdecl main_load_map_status(const char* scenario_path)
{
	//return INVOKE(0x0052F250, main_load_map_status, scenario_path);

	return main_load_map_status_with_insertion_point(NONE, scenario_path);
}

//e_map_load_status __cdecl main_load_map_status_with_insertion_point(int16 insertion_point, const char* scenario_path)
int32 __cdecl main_load_map_status_with_insertion_point(int16 insertion_point, const char* scenario_path)
{
	return INVOKE(0x0052F270, main_load_map_status_with_insertion_point, insertion_point, scenario_path);
}

//bool __cdecl main_load_map_with_insertion_point(int16, const char*, enum e_map_load_type)
bool __cdecl main_load_map_with_insertion_point(int16 insertion_point, const char* scenario_path, int32 map_load_type)
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

void __cdecl main_loading_enable_spinner(bool enable)
{
	INVOKE(0x0052F4F0, main_loading_enable_spinner, enable);

	//loading_globals.spinner_enabled = enable;
}

bool __cdecl main_loading_get_action(struct s_main_loading_action* out_action)
{
	return INVOKE(0x0052F500, main_loading_get_action, out_action);
}

//enum e_gui_game_mode __cdecl main_loading_get_gui_game_mode(void)
int32 __cdecl main_loading_get_gui_game_mode()
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

e_main_pregame_frame __cdecl main_loading_get_loading_status(c_static_wchar_string<12288>* loading_progress_string)
{
	//return INVOKE(0x0052F930, main_loading_get_loading_status, loading_progress_string);

	if (bink_playback_active())
		return _main_pregame_frame_normal;

	if (disable_progress_screen)
		return _main_pregame_frame_none;

	if (loading_globals.tag_load_in_progress || loading_globals.loading_in_progress || loading_globals.spinner_enabled)
	{
		if (game_in_progress() && !loading_globals.tag_load_in_progress)
			return _main_pregame_frame_none;

		static const wchar_t* spinner_states[] = { L"/", L"-", L"\\" };
		static int32 spinner_state_index = 8 * system_milliseconds() / 1000 % NUMBEROF(spinner_states);

		if (loading_globals.tag_load_in_progress)
		{
			static uns32 last_time = system_milliseconds();
			uns32 time = system_milliseconds();
			if (time < last_time + 250)
				return _main_pregame_frame_none;

			last_time = time;
			spinner_state_index = (spinner_state_index + 1) % NUMBEROF(spinner_states);
		}

		if (game_in_progress() && !loading_globals.tag_sync_in_progress && !loading_globals.tag_load_in_progress)
			return _main_pregame_frame_none;

		if (loading_progress_string)
			loading_progress_string->append_print(L"%s|n", spinner_states[spinner_state_index]);

		if (string_is_not_empty(loading_globals.scenario_path))
		{
			if (loading_progress_string)
			{
				//if (loading_globals.zone_set_index == NONE)
				loading_progress_string->append_print(L"loading scenario %S...", loading_globals.scenario_path);
				//else
				//	loading_progress_string->append_print(L"loading ZONE SET #%d %S...", loading_globals.zone_set_index, loading_globals.scenario_path);

				loading_progress_string->append_print(L"|n");
			}
		}

		if (loading_globals.loading_in_progress && !loading_globals.loading_progress.is_empty())
		{
			if (loading_progress_string)
				loading_progress_string->append_print(L"%S", loading_globals.loading_progress.get_string());
		}
		else
		{
			if (loading_progress_string)
				loading_progress_string->append_print(L"|n");
		}

		if (loading_progress_string)
			loading_progress_string->append_print(L"|n|n%S", events_get());

		return _main_pregame_frame_loading_debug;
	}

	if (loading_globals.basic_progress_enabled)
	{
		int32 loading_progress = 0;
		if (loaded_resource_bytes > 0 && total_resource_bytes > 0)
			loading_progress = int32((loaded_resource_bytes * 100.0f) / total_resource_bytes);
	
		if (loading_progress_string)
			loading_progress_string->append_print(L"basic loading progress: %%%d|n", loading_progress);
	
		if (loading_globals.insertion_point)
		{
			if (loading_progress_string)
				loading_progress_string->append_print(L"|n%S", loading_globals.insertion_point);
		}
	
		return _main_pregame_frame_cache_loading;
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
			cache_file_tag_resources_start_map_prefetch(static_cast<int16>(loading_action.campaign_id), loading_action.scenario_path);
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

void __cdecl main_loading_progress_done(const char* description, void* userdata)
{
	//INVOKE(0x0052FB60, main_loading_progress_done, description, userdata);

	ASSERT(loading_globals.loading_in_progress, "someone called main_loading_progress_done without calling main_loading_progress_start!");
	loading_globals.loading_progress.print("%s... done|n", description);
	main_loop_pregame();
	loading_globals.loading_in_progress = false;
}

void __cdecl main_loading_progress_new(const char* description, void* userdata)
{
	//INVOKE(0x0052FB70, main_loading_progress_new, description, userdata);

	loading_globals.loading_progress.print("%s...|n", description);
	loading_globals.loading_in_progress = true;
	main_loop_pregame();
}

void __cdecl main_loading_progress_update(const char* description, const char* scenario_path, int32 progress, void* userdata)
{
	//INVOKE(0x0052FB80, main_loading_progress_update, description, scenario_path, progress, userdata);

	loading_globals.loading_progress.print("%s... %2d%c|n", description, progress, '%');
	if (scenario_path)
		loading_globals.loading_progress.append_print("    %s", scenario_path);
	loading_basic_progress_update_fraction(progress / 100.0f);
	main_loop_pregame();
}

