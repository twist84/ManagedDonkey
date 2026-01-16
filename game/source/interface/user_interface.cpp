#include "interface/user_interface.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "cutscene/cinematics.hpp"
#include "data_mining/data_mine_management.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "interface/attract_mode.hpp"
#include "interface/c_controller.hpp"
#include "interface/damaged_media.hpp"
#include "interface/gui_pregame_setup_manager.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/interface_constants.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_mouse.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_player_model_camera.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "profiler/profiler.hpp"
#include "rasterizer/rasterizer.hpp"
#include "saved_games/content_catalogue.hpp"
#include "saved_games/saved_game_files.hpp"

#include <windows.h>

real32 g_ui_time_scale = 1.0f;
real32 g_ui_time_step = 0.0f;

REFERENCE_DECLARE(0x0191CC20, int32, g_user_interface_alpha_locked_down_state) = NONE; // e_alpha_configuration_ui_level
REFERENCE_DECLARE(0x0191CC24, bool, g_eula_accepted) = true;
REFERENCE_DECLARE(0x052559E0, bool, g_user_interface_is_alpha) = false;
REFERENCE_DECLARE(0x052559E4, s_user_interface_globals, g_user_interface_globals);

HOOK_DECLARE(0x00A849B0, user_interface_render);
HOOK_DECLARE(0x00A84EE0, user_interface_update);

void __cdecl debug_enable_text_bounds(bool enable)
{
	INVOKE(0x00A84020, debug_enable_text_bounds, enable);
}

void __cdecl debug_render_title_safe_bounds(bool enable)
{
	INVOKE(0x00A84030, debug_render_title_safe_bounds, enable);
}

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
int32 __cdecl get_alpha_locked_down_state()
{
	return INVOKE(0x00A84190, get_alpha_locked_down_state);
}

bool __cdecl get_debug_frame_element_bounds()
{
	return INVOKE(0x00A841D0, get_debug_frame_element_bounds);
}

bool __cdecl get_is_alpha_version()
{
	return INVOKE(0x00A841E0, get_is_alpha_version);
}

bool __cdecl get_is_blue_disk()
{
	// true:  halo3_blue
	// false: halo3_black
	return false;
}

//.text:00A841F0 ; 
//.text:00A84200 ; c_gui_data* __cdecl interface_get_global_datasource(int32)

void __cdecl set_debug_frame_element_bounds(bool enable)
{
	INVOKE(0x00A84240, set_debug_frame_element_bounds, enable);
}

void __cdecl ui_handle_script_verification()
{
	//INVOKE(0x00A84260, ui_handle_script_verification);

	tag_iterator iterator{};
	tag_iterator_new(&iterator, GUI_SCREEN_WIDGET_DEFINITION_TAG);
	for (int32 tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
	{
		int16 script_index = NONE;
		s_screen_widget_definition* definition = TAG_GET(GUI_SCREEN_WIDGET_DEFINITION_TAG, s_screen_widget_definition, tag_index);
		if (definition->script_name[0])
		{
			script_index = hs_find_script_by_name(definition->script_name, 0);
		}
		definition->script_index = script_index;
	}
}

void __cdecl user_interface_dispose()
{
	INVOKE(0x00A84270, user_interface_dispose);

	//user_interface_networking_dispose();
	//user_interface_controller_dispose();
	//user_interface_error_manager_get()->dispose();
	//window_manager_get()->dispose();
	//user_interface_messaging_dispose();
	//user_interface_text_parser_dispose();
	//user_interface_memory_dispose();
}

void __cdecl user_interface_dispose_from_old_map()
{
	//INVOKE(0x00A842A0, user_interface_dispose_from_old_map);

	content_catalogue_dispose_from_old_map();
	user_interface_messaging_dispose_from_old_map();
	user_interface_error_manager_get()->dispose_from_old_map();
	window_manager_get()->dispose_from_old_map();

	//user_interface_available_squads_dispose();

	g_user_interface_globals.m_music_manager.reset();
	user_interface_dispose_global_datasources();
	user_interface_networking_dispose_from_old_map();
}

void __cdecl user_interface_dispose_global_datasources()
{
	INVOKE(0x00A842F0, user_interface_dispose_global_datasources);
}

void __cdecl user_interface_enter_game_shell()
{
	INVOKE(0x00A84330, user_interface_enter_game_shell);

	//g_user_interface_globals.m_last_event_handled_milliseconds = g_user_interface_globals.milliseconds.peek();
}

//.text:00A84350 ; bool __cdecl user_interface_get_bad_download_flag()
//.text:00A84360 ; int32 __cdecl user_interface_get_main_menu_music_fade_time_milliseconds()

void __cdecl user_interface_get_number_of_render_windows(int32 user_index, int32* horizontal_window_count, int32* vertical_window_count)
{
	INVOKE(0x00A84370, user_interface_get_number_of_render_windows, user_index, horizontal_window_count, vertical_window_count);
}

//.text:00A84420 ; uns64 __cdecl user_interface_get_player_hash_bits(e_controller_index, int32)

void __cdecl user_interface_get_projection_plane_distances(real32* near_clip_plane_distance, real32* projection_plane_distance, real32* far_clip_plane_distance)
{
	INVOKE(0x00A84450, user_interface_get_projection_plane_distances, near_clip_plane_distance, projection_plane_distance, far_clip_plane_distance);
}

e_controller_index __cdecl user_interface_get_reload_from_persistent_storage()
{
	return INVOKE(0x00A844D0, user_interface_get_reload_from_persistent_storage);
}

//.text:00A844E0 ; 
//.text:00A844F0 ; int32 __cdecl user_interface_get_selected_campaign_difficulty()
//.text:00A84500 ; e_campaign_id __cdecl user_interface_get_selected_campaign_id()
//.text:00A84510 ; e_map_id __cdecl user_interface_get_selected_campaign_map_id()
//.text:00A84520 ; char* __cdecl user_interface_get_selected_campaign_path(char*, int32)
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
//.text:00A848C0 ; real64 __cdecl user_interface_main_menu_music_fade_out_progress()
//.text:00A848D0 ; void __cdecl user_interface_main_menu_music_initialize_for_saved_game()

uns32 __cdecl user_interface_milliseconds()
{
	return INVOKE(0x00A848E0, user_interface_milliseconds);
}

//.text:00A848F0 ; uns32 __cdecl user_interface_milliseconds_at_last_event()

void __cdecl user_interface_music_manager_toggle_debug_render_music_state(bool rendering_active)
{
	g_user_interface_globals.m_music_manager.m_debug_render_music_state = rendering_active;
}

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

//void __cdecl user_interface_render(e_controller_index controller, int32 user_index, e_window_index window, const rectangle2d* viewport_bounds, c_rasterizer::e_surface rasterizer_render_surface, bool is_screenshot)
void __cdecl user_interface_render(e_controller_index controller, int32 user_index, e_window_index window, const rectangle2d* viewport_bounds, int32 rasterizer_render_surface, bool is_screenshot)
{
	//INVOKE(0x00A849B0, user_interface_render, controller, user_index, window, viewport_bounds, rasterizer_render_surface, is_screenshot);

	user_interface_render_begin((c_rasterizer::e_surface)rasterizer_render_surface);

	if (window <= k_number_of_player_windows)
	{
		window_manager_get()->render(window, user_index, viewport_bounds, is_screenshot);
	}

	if (window == k_number_of_player_windows)
	{
		g_user_interface_globals.m_music_manager.render();
		if (g_user_interface_globals.m_flags.test_bit(s_user_interface_globals::_debug_render_title_safe_bounds))
		{
			struct s_safe_bounds
			{
				real32 fraction;
				uns32 color;
			};
			COMPILE_ASSERT(sizeof(s_safe_bounds) == 0x8);

			s_safe_bounds k_safe_bounds[4]
			{
				{ .fraction = 0.0f,   .color = 0xFFFF0000 },
				{ .fraction = 0.05f,  .color = 0xFFFF7F00 },
				{ .fraction = 0.075f, .color = 0xFFFFFF00 },
				{ .fraction = 0.1f,   .color = 0xFF00FF00 }
			};

			rectangle2d frame_bounds{};
			interface_get_current_display_settings(&frame_bounds, NULL, NULL, NULL);
			real32 rect_height = rectangle2d_height(&frame_bounds);
			real32 rect_width = rectangle2d_width(&frame_bounds);

			for (int32 safe_bound_index = 0; safe_bound_index < NUMBEROF(k_safe_bounds); safe_bound_index++)
			{
				real32 fraction = k_safe_bounds[safe_bound_index].fraction;
				uns32 color = k_safe_bounds[safe_bound_index].color;

				point2d points[5]{};
				points[0].x = (int16)(fraction * rect_width);
				points[0].y = (int16)(fraction * rect_height);
				points[1].x = points[0].x;
				points[2].x = (int16)(((1.0f - fraction) * rect_width) - 1.0f);
				points[3].x = points[2].x;
				points[1].y = (int16)(((1.0f - fraction) * rect_height) - 1.0f);
				points[2].y = points[1].y;
				points[3].y = points[0].y;
				points[4] = points[0];
				c_rasterizer::draw_debug_linestrip2d(points, NUMBEROF(points), color);
			}
		}
	}

	user_interface_render_end();
}

//void __cdecl user_interface_render_begin(c_rasterizer::e_surface render_surface)
void __cdecl user_interface_render_begin(int32 render_surface)
{
	//INVOKE(0x00A84B40, user_interface_render_begin, render_surface);

	c_rasterizer::begin_high_quality_blend();
	c_rasterizer::set_render_target(0, (c_rasterizer::e_surface)render_surface, 0xFFFFFFFF);
	c_rasterizer::set_render_target(1, c_rasterizer::_surface_none, 0xFFFFFFFF);
}

void __cdecl user_interface_render_end()
{
	//INVOKE(0x00A84B70, user_interface_render_end);
}

void __cdecl user_interface_render_quad_in_viewport(const rectangle2d* viewport_bounds, uns32 color)
{
	//INVOKE(0x00A84B80, user_interface_render_quad_in_viewport, viewport_bounds, color);

	rectangle2d rect = *viewport_bounds;
	offset_rectangle2d(&rect, -rect.x0, -rect.y0);
	draw_quad(&rect, color);
}

void __cdecl user_interface_render_quad_in_window(const rectangle2d* bounds, uns32 color)
{
	//INVOKE(0x00A84BC0, user_interface_render_quad_in_window, bounds, color);

	rectangle2d rect = *bounds;
	draw_quad(&rect, color);
}

bool __cdecl user_interface_requests_unlocked_framerate()
{
	return INVOKE(0x00A84BF0, user_interface_requests_unlocked_framerate);
}

void __cdecl user_interface_scoreboard_update()
{
	//INVOKE(0x00A84C00, user_interface_scoreboard_update);

	for (e_controller_index controller_index = _controller0; controller_index < k_number_of_controllers; controller_index++)
	{
		c_controller_interface* controller = controller_get(controller_index);
		if (!controller->in_use() && !user_interface_should_show_console_scoreboard(0))
		{
			c_gui_screen_scoreboard::hide_scoreboard(controller_index);
		}
	}
}

//.text:00A84C50 ; void __cdecl user_interface_set_bad_download_flag(bool)

void __cdecl user_interface_set_reload_from_persistent_storage(e_controller_index controller_index)
{
	INVOKE(0x00A84C70, user_interface_set_reload_from_persistent_storage, controller_index);
}

//.text:00A84C90 ; void __cdecl user_interface_set_selected_campaign_difficulty(int32)
//.text:00A84CC0 ; void __cdecl user_interface_set_selected_campaign_id(e_campaign_id)
//.text:00A84CE0 ; void __cdecl user_interface_set_selected_campaign_map_id(e_map_id)
//.text:00A84D00 ; void __cdecl user_interface_set_storage_device_selection_deferred(bool)

const s_user_interface_shared_globals* user_interface_shared_tag_globals_try_and_get()
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

bool __cdecl user_interface_should_show_console_scoreboard(int32* user_interface_show_busy_state)
{
	return INVOKE(0x00A84D60, user_interface_should_show_console_scoreboard, user_interface_show_busy_state);
}

//.text:00A84E40 ; void __cdecl user_interface_show_campaign_custom_music_warning_if_needed(bool*)
//.text:00A84E50 ; void __cdecl user_interface_shut_down()

const s_user_interface_tag_globals* __cdecl user_interface_tag_globals_try_and_get()
{
	return INVOKE(0x00A84E80, user_interface_tag_globals_try_and_get);
}

void __cdecl user_interface_time_step(real32 time_step)
{
	g_ui_time_step = time_step;
}

void __cdecl user_interface_update(real32 shell_seconds_elapsed)
{
	//INVOKE(0x00A84EE0, user_interface_update, shell_seconds_elapsed);

	PROFILER(user_interface_update)
	{
		real32 ui_time = (shell_seconds_elapsed * g_ui_time_scale) + g_ui_time_step;
		g_ui_time_step = 0.0f;
	
		g_user_interface_globals.shell_seconds_elapsed = ui_time;
		g_user_interface_globals.m_current_milliseconds.add(int32(ui_time * 1000));
		int32 current_ui_milliseconds = g_user_interface_globals.m_current_milliseconds.peek();
	
		int32 active = player_mapping_output_user_active_count();
		bool active_same = g_user_interface_globals.m_active_user_count == active;
		g_user_interface_globals.m_active_user_count = active;
		bool update_toast_position = !active_same;

		damaged_media_update();
		//user_interface_memory_update();
	
		if (game_in_progress())
		{
			if (!debugging_system_has_focus())
			{
				user_interface_controller_update();
			}

			saved_game_files_update();
			user_interface_messaging_update();
			user_interface_mouse_update();
			user_interface_update_console_scoreboard();
			user_interface_scoreboard_update();
			window_manager_get()->update(current_ui_milliseconds);
			user_interface_error_manager_get()->update(current_ui_milliseconds);
			c_gui_pregame_setup_manager::get()->update();
			user_interface_networking_update();
			//user_interface_process_magic_key_combos();
	
			if (user_interface_xbox_guide_is_active())
			{
				user_interface_non_idle_event_occured();
			}
	
			if (attract_mode_should_start())
			{
				attract_mode_start();
			}
	
			g_user_interface_globals.m_music_manager.update();
	
			if (game_is_ui_shell() && user_interface_should_render_fancy())
			{
				g_user_interface_globals.m_last_fancy_render_time = current_ui_milliseconds;
			}
	
			if (game_is_ui_shell())
			{
				if (update_toast_position)
				{
					user_interface_update_toast_position();
				}

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
			system_milliseconds() > uns32(g_user_interface_globals.m_user_interface_shutdown_start_time + 2000))
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
			{
				scoreboard_screen_is_above = true;
			}
		}

		if (!scoreboard_screen_is_above)
		{
			c_gui_screen_scoreboard::show_scoreboard(k_any_controller, false);
		}
	}
	else
	{
		c_gui_screen_scoreboard::hide_scoreboard(k_any_controller);
	}
}

void __cdecl user_interface_update_toast_position()
{
	INVOKE(0x00A85050, user_interface_update_toast_position);

#if 0
	e_online_guide_toast_position toast_position = k_toast_position_default;
	if (!game_is_ui_shell())
	{
		if (cinematic_in_progress())
		{
			toast_position = _toast_position_top_right;
		}
		else if (player_mapping_output_user_active_count() > 1)
		{
			toast_position = _toast_position_top_right;
		}
	}
	online_guide_set_toast_position(toast_position);
#endif
}

