#include "interface/user_interface.hpp"

#include "cseries/cseries.hpp"
#include "data_mining/data_mine_management.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "interface/attract_mode.hpp"
#include "interface/c_controller.hpp"
#include "interface/damaged_media.hpp"
#include "interface/gui_pregame_setup_manager.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/user_interface_mouse.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_player_model_camera.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "profiler/profiler.hpp"
#include "saved_games/saved_game_files.hpp"

#include <windows.h>

real g_ui_time_scale = 1.0f;
real g_ui_time_step = 0.0f;

REFERENCE_DECLARE(0x0191CC20, long, g_user_interface_alpha_locked_down_state); // e_alpha_configuration_ui_level
REFERENCE_DECLARE(0x0191CC24, bool, g_eula_accepted);
REFERENCE_DECLARE(0x052559E0, bool, g_user_interface_is_alpha);
REFERENCE_DECLARE(0x052559E4, s_user_interface_globals, g_user_interface_globals);

HOOK_DECLARE(0x00A84EE0, user_interface_update);

//.text:00A84030 ; void __cdecl debug_render_title_safe_bounds(bool)

bool __cdecl get_alpha_custom_games_disabled()
{
	return INVOKE(0x00A84050, get_alpha_custom_games_disabled);
}

bool __cdecl get_alpha_eula_accepted()
{
	return INVOKE(0x00A840C0, get_alpha_eula_accepted);
}

bool __cdecl get_alpha_is_external_beta()
{
	return INVOKE(0x00A840E0, get_alpha_is_external_beta);
}

bool __cdecl get_alpha_is_internal_beta()
{
	return INVOKE(0x00A84120, get_alpha_is_internal_beta);
}

bool __cdecl get_alpha_is_locked_down()
{
	return INVOKE(0x00A84160, get_alpha_is_locked_down);
}

//e_alpha_configuration_ui_level __cdecl get_alpha_locked_down_state()
long __cdecl get_alpha_locked_down_state()
{
	return INVOKE(0x00A84190, get_alpha_locked_down_state);
}

//.text:00A841D0 ; bool __cdecl get_debug_frame_element_bounds()

bool __cdecl get_is_alpha_version()
{
	return INVOKE(0x00A841E0, get_is_alpha_version);
}

//.text:00A841F0 ; 
//.text:00A84200 ; c_gui_data* __cdecl interface_get_global_datasource(long)
//.text:00A84240 ; void __cdecl set_debug_frame_element_bounds(bool)
//.text:00A84260 ; void __cdecl ui_handle_script_verification()

void __cdecl user_interface_dispose()
{
	INVOKE(0x00A84270, user_interface_dispose);
}

void __cdecl user_interface_dispose_from_old_map()
{
	INVOKE(0x00A842A0, user_interface_dispose_from_old_map);
}

//.text:00A842F0 ; void __cdecl user_interface_dispose_global_datasources()

void __cdecl user_interface_enter_game_shell()
{
	INVOKE(0x00A84330, user_interface_enter_game_shell);

	//g_user_interface_globals.m_last_event_handled_milliseconds = g_user_interface_globals.milliseconds.peek();
}

//.text:00A84350 ; bool __cdecl user_interface_get_bad_download_flag()
//.text:00A84360 ; long __cdecl user_interface_get_main_menu_music_fade_time_milliseconds()

void __cdecl user_interface_get_number_of_render_windows(long user_index, long* horizontal_window_count, long* vertical_window_count)
{
	INVOKE(0x00A84370, user_interface_get_number_of_render_windows, user_index, horizontal_window_count, vertical_window_count);
}

//.text:00A84420 ; qword __cdecl user_interface_get_player_hash_bits(e_controller_index, long)
//.text:00A84450 ; void __cdecl user_interface_get_projection_plane_distances(real*, real*, real*)
//.text:00A844D0 ; e_controller_index __cdecl user_interface_get_reload_from_persistent_storage()
//.text:00A844E0 ; 
//.text:00A844F0 ; long __cdecl user_interface_get_selected_campaign_difficulty()
//.text:00A84500 ; e_campaign_id __cdecl user_interface_get_selected_campaign_id()
//.text:00A84510 ; e_map_id __cdecl user_interface_get_selected_campaign_map_id()
//.text:00A84520 ; char* __cdecl user_interface_get_selected_campaign_path(char*, long)
//.text:00A84570 ; bool __cdecl user_interface_get_storage_device_selection_deferred()

void __cdecl user_interface_initialize()
{
	INVOKE(0x00A84580, user_interface_initialize);
}

void __cdecl user_interface_initialize_for_new_map()
{
	INVOKE(0x00A84650, user_interface_initialize_for_new_map);
}

//.text:00A84710 ; void __cdecl user_interface_initialize_global_datasources()

bool __cdecl user_interface_is_active()
{
	return INVOKE(0x00A84840, user_interface_is_active);

	//return window_manager_get()->any_ui_active();
}

//.text:00A84850 ; bool __cdecl user_interface_is_shutting_down()
//.text:00A84860 ; 
//.text:00A848B0 ; bool __cdecl user_interface_main_menu_music_done_fading_out()
//.text:00A848C0 ; double __cdecl user_interface_main_menu_music_fade_out_progress()
//.text:00A848D0 ; void __cdecl user_interface_main_menu_music_initialize_for_saved_game()

dword __cdecl user_interface_milliseconds()
{
	return INVOKE(0x00A848E0, user_interface_milliseconds);
}

//.text:00A848F0 ; dword __cdecl user_interface_milliseconds_at_last_event()
//.text:00A84900 ; bool __cdecl user_interface_non_dead_cam_active_for_game_player_controller(e_controller_index)

void __cdecl user_interface_non_idle_event_occured()
{
	INVOKE(0x00A84920, user_interface_non_idle_event_occured);
}

//.text:00A84940 ; 
//.text:00A84960 ; 
//.text:00A84970 ; 
//.text:00A84990 ; 
//.text:00A849A0 ; 

//void __cdecl user_interface_render(e_controller_index controller, long user_index, e_window_index window, rectangle2d const* viewport_bounds, c_rasterizer::e_surface rasterizer_render_surface, bool is_screenshot)
void __cdecl user_interface_render(e_controller_index controller, long user_index, long window, rectangle2d const* viewport_bounds, long rasterizer_render_surface, bool is_screenshot)
{
	INVOKE(0x00A849B0, user_interface_render, controller, user_index, window, viewport_bounds, rasterizer_render_surface, is_screenshot);
}

//.text:00A84B40 ; void __cdecl user_interface_render_begin(c_rasterizer::e_surface)
//.text:00A84B70 ; void __cdecl user_interface_render_end()
//.text:00A84B80 ; void __cdecl user_interface_render_quad_in_viewport(rectangle2d const*, dword)
//.text:00A84BC0 ; void __cdecl user_interface_render_quad_in_window(rectangle2d const*, dword)

bool __cdecl user_interface_requests_unlocked_framerate()
{
	return INVOKE(0x00A84BF0, user_interface_requests_unlocked_framerate);
}

void __cdecl user_interface_scoreboard_update()
{
	//INVOKE(0x00A84C00, user_interface_scoreboard_update);

	for (long i = 0; i < k_number_of_controllers; i++)
	{
		e_controller_index controller_index = static_cast<e_controller_index>(i);

		c_controller_interface* controller = controller_get(controller_index);
		if (!controller->in_use() && !user_interface_should_show_console_scoreboard(0))
			c_gui_screen_scoreboard::hide_scoreboard(controller_index);
	}
}

//.text:00A84C50 ; void __cdecl user_interface_set_bad_download_flag(bool)

void __cdecl user_interface_set_reload_from_persistent_storage(e_controller_index controller_index)
{
	INVOKE(0x00A84C70, user_interface_set_reload_from_persistent_storage, controller_index);
}

//.text:00A84C90 ; void __cdecl user_interface_set_selected_campaign_difficulty(long)
//.text:00A84CC0 ; void __cdecl user_interface_set_selected_campaign_id(e_campaign_id)
//.text:00A84CE0 ; void __cdecl user_interface_set_selected_campaign_map_id(e_map_id)
//.text:00A84D00 ; void __cdecl user_interface_set_storage_device_selection_deferred(bool)

s_user_interface_shared_globals const* user_interface_shared_tag_globals_try_and_get()
{
	return INVOKE(0x00A84D20, user_interface_shared_tag_globals_try_and_get);
}

bool __cdecl user_interface_should_render_at_origin()
{
	return INVOKE(0x00A84D40, user_interface_should_render_at_origin);
}

bool __cdecl user_interface_should_render_fancy()
{
	return INVOKE(0x00A84D50, user_interface_should_render_fancy);
}

bool __cdecl user_interface_should_show_console_scoreboard(long* user_interface_show_busy_state)
{
	return INVOKE(0x00A84D60, user_interface_should_show_console_scoreboard, user_interface_show_busy_state);
}

//.text:00A84E40 ; void __cdecl user_interface_show_campaign_custom_music_warning_if_needed(bool*)
//.text:00A84E50 ; void __cdecl user_interface_shut_down()

s_user_interface_tag_globals const* __cdecl user_interface_tag_globals_try_and_get()
{
	return INVOKE(0x00A84E80, user_interface_tag_globals_try_and_get);
}

void __cdecl user_interface_update(real shell_seconds_elapsed)
{
	//INVOKE(0x00A84EE0, user_interface_update, shell_seconds_elapsed);

	PROFILER(user_interface_update)
	{
		real ui_time = (shell_seconds_elapsed * g_ui_time_scale) + g_ui_time_step;
		g_ui_time_step = 0.0f;
	
		g_user_interface_globals.shell_seconds_elapsed = ui_time;
		g_user_interface_globals.m_current_milliseconds.add(long(ui_time * 1000));
		long milliseconds = g_user_interface_globals.m_current_milliseconds.peek();
	
		long output_user_active_count = player_mapping_output_user_active_count();
		bool v3 = g_user_interface_globals.m_active_output_user_count == output_user_active_count;
		g_user_interface_globals.m_active_output_user_count = output_user_active_count;
		bool update_toast_position = !v3;

		damaged_media_update();
		//user_interface_memory_update();
	
		if (game_in_progress())
		{
			if (!debugging_system_has_focus())
				user_interface_controller_update();

			saved_game_files_update();
			user_interface_messaging_update();
			user_interface_mouse_update();
			user_interface_update_console_scoreboard();
			user_interface_scoreboard_update();
			window_manager_get()->update(milliseconds);
			user_interface_error_manager_get()->update(milliseconds);
			c_gui_pregame_setup_manager::get()->update();
			user_interface_networking_update();
			//user_interface_process_magic_key_combos();
	
			if (user_interface_xbox_guide_is_active())
				user_interface_non_idle_event_occured();
	
			if (attract_mode_should_start())
				attract_mode_start();
	
			g_user_interface_globals.m_music_manager.update();
	
			if (game_is_ui_shell() && user_interface_should_render_fancy())
				g_user_interface_globals.m_music_manager.__unknown18 = milliseconds;
	
			if (game_is_ui_shell())
			{
				if (update_toast_position)
					user_interface_update_toast_position();

				//if (user_interface_squad_exists() && user_interface_squad_get_ui_game_mode() == _ui_game_mode_survival)
				//	user_interface_show_campaign_custom_music_warning_if_needed(NULL);

				user_interface_player_model_camera_update();
				//user_interface_hs_update();
			}
			else if (game_is_campaign())
			{
				//user_interface_show_campaign_custom_music_warning_if_needed(NULL);
			}
	
			//sub_635120(shell_seconds_elapsed); // saber function
		}
	
		if (g_user_interface_globals.m_user_interface_shutdown_start_time != NONE &&
			system_milliseconds() > dword(g_user_interface_globals.m_user_interface_shutdown_start_time + 2000))
		{
			data_mine_flush();
			exit(0); // relaunch
		}
	}
}

void __cdecl user_interface_update_console_scoreboard()
{
	//INVOKE(0x00A85040, user_interface_update_console_scoreboard);

	c_gui_screen_scoreboard::update_scoreboard_alpha(k_any_controller);
	if (user_interface_should_show_console_scoreboard(NULL))
	{
		bool scoreboard_screen_is_above = false;
		c_gui_screen_scoreboard* scoreboard_screen = c_gui_screen_scoreboard::get_scoreboard_screen(k_any_controller);
		if (scoreboard_screen)
		{
			c_window_manager* window_manager = window_manager_get();
			if (window_manager->get_screen_above(_console_window, scoreboard_screen))
				scoreboard_screen_is_above = true;
		}

		if (!scoreboard_screen_is_above)
			c_gui_screen_scoreboard::show_scoreboard(k_any_controller, false);
	}
	else
	{
		c_gui_screen_scoreboard::hide_scoreboard(k_any_controller);
	}
}

void __cdecl user_interface_update_toast_position()
{
	INVOKE(0x00A85050, user_interface_update_toast_position);
}

