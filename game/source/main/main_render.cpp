#include "main/main_render.hpp"

#include "bink/bink_playback.hpp"
#include "cache/pc_geometry_cache.hpp"
#include "cache/pc_texture_cache.hpp"
#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries_events.hpp"
#include "cutscene/cinematics.hpp"
#include "editor/editor_stubs.hpp"
#include "game/game.hpp"
#include "game/game_engine_display.hpp"
#include "game/player_mapping.hpp"
#include "interface/c_controller.hpp"
#include "interface/closed_caption.hpp"
#include "interface/interface.hpp"
#include "interface/interface_constants.hpp"
#include "interface/terminal.hpp"
#include "main/console.hpp"
#include "main/main.hpp"
#include "main/main_game.hpp"
#include "main/main_time.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "networking/tools/network_debug_dump.hpp"
#include "profiler/profiler.hpp"
#include "profiler/profiler_stopwatch.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_globals.hpp"
#include "rasterizer/rasterizer_loading_screen.hpp"
#include "rasterizer/rasterizer_performance_throttles.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "rasterizer/rasterizer_synchronization.hpp"
#include "render/old_render_debug.hpp"
#include "render/render.hpp"
#include "render/screen_postprocess.hpp"
#include "render/views/render_view.hpp"
#include "shell/shell.hpp"
#include "simulation/simulation.hpp"
#include "text/draw_string.hpp"
#include "visibility/visibility_collection.hpp"

REFERENCE_DECLARE(0x02446520, real_vector4d, g_player_window_constants);

HOOK_DECLARE(0x006042C0, main_render);
HOOK_DECLARE(0x00604440, main_render_game);
HOOK_DECLARE(0x00604860, main_render_pregame);
//HOOK_DECLARE(0x00604AE0, main_render_start_blocking_frame);
//HOOK_DECLARE(0x00604D70, main_render_view); // paired with `main_render_view_inline_hook`

bool debug_force_all_player_views_to_default_player = false;
bool debug_render_horizontal_splitscreen = false;
int32 g_watermark_enabled = 0;

//c_stop_watch g_main_render_block_watch = c_stop_watch(true);

c_player_render_camera_iterator::c_player_render_camera_iterator() :
	m_window_count(),
	m_window_arrangement(),
	m_next_window_index(0),
	m_current_user_index(NONE),
	m_current_observer_result(nullptr)
{
	if (cinematic_in_progress() || player_is_reading_terminal())
	{
		m_window_count = 1;
	}
	else
	{
		int32 output_user_active_count = player_mapping_output_user_active_count();

		int32 player_count = 1;
		if (output_user_active_count > 1)
		{
			player_count = output_user_active_count;
		}

		int32 window_count = MAXIMUM_PLAYER_WINDOWS;
		if (player_count < window_count)
		{
			window_count = player_count;
		}

		m_window_count = window_count;
	}

	ASSERT(m_window_count <= MAXIMUM_PLAYER_WINDOWS);

	if (m_window_count <= 1)
	{
		m_window_arrangement = _render_player_window_arrangement_single;
	}
	else
	{
		bool is_widescreen = c_rasterizer::get_is_widescreen();
		if (debug_render_horizontal_splitscreen)
		{
			is_widescreen = true;
		}

		if (is_widescreen)
		{
			m_window_arrangement = _render_player_window_arrangement_multiple_horizontal;
		}
		else
		{
			m_window_arrangement = _render_player_window_arrangement_multiple_vertical;
		}
	}
}

const s_observer_result* c_player_render_camera_iterator::get_observer_result() const
{
	return m_current_observer_result;
}

int32 c_player_render_camera_iterator::get_user_index() const
{
	return m_current_user_index;
}

int32 c_player_render_camera_iterator::get_window_arrangement() const
{
	return m_window_arrangement;
}

int32 c_player_render_camera_iterator::get_window_count() const
{
	return m_window_count;
}

bool c_player_render_camera_iterator::next()
{
	bool success = m_next_window_index < m_window_count;
	if (success)
	{
		m_next_window_index++;

		if (debug_force_all_player_views_to_default_player)
		{
			m_current_user_index = player_mapping_first_active_output_user();
		}
		else
		{
			for (int32 user_index = ++m_current_user_index; user_index < MAXIMUM_PLAYER_WINDOWS; user_index++)
			{
				if (player_mapping_output_user_is_active(m_current_user_index))
				{
					break;
				}
			}

			if (m_current_user_index >= k_number_of_users)
			{
				m_current_user_index = NONE;
				success = false;
			}
		}

		if (m_current_user_index != NONE)
		{
			m_current_observer_result = observer_get_camera(m_current_user_index);
			ASSERT(m_current_observer_result != NULL);
		}
	}

	return success;
}

void __cdecl main_render()
{
	//INVOKE(0x006042C0, main_render);

	REFERENCE_DECLARE(0x02446778, int32, frames_of_consecutive_delay);

	PROFILER(main_render)
	{
		bool should_draw = !sub_42E5D0() || !debug_no_drawing;

		{
			RENDER_THREAD_LOCK;
			rasterizer_lag_timing_mark_render_start();
			main_render_process_messages();

			if (frames_of_consecutive_delay > 50)
			{
				should_draw = false;
				frames_of_consecutive_delay = 0;
			}

			if (rasterizer_lag_timing_get_gamestate_delay() > 10)
				frames_of_consecutive_delay++;
			else
				frames_of_consecutive_delay = 0;

			if (should_draw)
			{
				bool render_game = true;
				bool render_sapien = false;

				if (c_rasterizer::begin_frame())
				{
					PROFILER(render)
					{
						PROFILER(general)
						{
							if (game_in_progress() && game_get_active_structure_bsp_mask())
							{
								if (simulation_starting_up())
									render_game = false;
							}
							else
							{
								render_game = false;
							}

							if (render_game)
							{
								render_sapien = game_in_editor();
								render_game = !render_sapien;
							}

							main_render_frame_begin();

							if (render_game)
							{
								main_render_game();
							}
							else if (render_sapien)
							{
								main_render_sapien();
							}
							else
							{
								main_render_pregame(_main_pregame_frame_normal, NULL);
							}

							should_draw = !texture_cache_is_blocking() && !geometry_cache_is_blocking();
						}
					}
				}
				else
				{
					should_draw = false;
				}

				c_render_globals::increment_frame_index();
			}

			//if (should_draw)
			//{
			//	if (int64 blocking_cycles = g_main_render_block_watch.stop())
			//		status_printf("blocking time: %.2f ms", 1000.0f * c_stop_watch::cycles_to_seconds(blocking_cycles));
			//}

			rasterizer_lag_timing_mark_render_end();
		}

		if (should_draw)
		{
			int64 target_display_vblank_index = main_time_get_target_display_vblank_index();
			restricted_region_mirror_locked_for_current_thread(k_game_state_shared_region);
			main_time_throttle(target_display_vblank_index);

			sub_604A20();
		}
	}
}

void __cdecl main_render_assert_no_pending_messages()
{
	INVOKE(0x00604420, main_render_assert_no_pending_messages);

	//effects_assert_no_pending_messages();
}

void __cdecl main_render_frame_begin()
{
	//INVOKE(0x00604430, main_render_frame_begin);

	render_frame_begin();
}

#define PLAYER_VIEW_RENDER_BEGIN \
{ \
	c_player_view::set_global_player_view(player_view); \
	c_view::begin(player_view); \
}

#define PLAYER_VIEW_RENDER_PREPARE \
{ \
	render_window_reset(player_view->get_player_view_user_index()); \
	player_view->create_frame_textures(view_index); \
	render_prepare_for_window(view_index, player_view->get_player_view_user_index()); \
	player_view->compute_visibility(); \
	player_view->render_submit_visibility(); \
}

#define PLAYER_VIEW_RENDER_END \
{ \
	c_view::end(); \
	c_player_view::set_global_player_view(NULL); \
}

void __cdecl main_render_game()
{
	//INVOKE(0x00604440, main_render_game);

	PROFILER(main_render_game)
	{
		RENDER_THREAD_LOCK;

		if (!cinematic_in_progress())
		{
			observer_adopt_global_update_list();
		}

		main_render_update_loading_screen();

		c_player_render_camera_iterator iterator{};
		int32 window_count = iterator.get_window_count();
		int32 window_arrangement = iterator.get_window_arrangement();

		bool render_freeze = debug_render_freeze;

		main_render_process_messages();
		main_render_frame_begin();

		c_cpu_gpu_synchronizer::insert_fence_at_beginning_of_frame();

		texture_cache_update_for_render();
		geometry_cache_update_for_render();

		for (int32 window_index = 0; window_index < window_count; window_index++)
		{
			c_player_view* player_view = c_player_view::get_current(window_index);

			int32 user_index = NONE;
			const s_observer_result* observer_result = NULL;
			if (iterator.next())
			{
				user_index = iterator.get_user_index();
				observer_result = iterator.get_observer_result();
			}

			player_view->setup_camera(
				window_index,
				window_count,
				window_arrangement,
				user_index,
				observer_result,
				render_freeze);
		}

		{
			c_rasterizer_profile_scope _main_render(_rasterizer_profile_element_total, L"main_render");

			c_visible_items::clear_all();
			c_player_view::frame_advance();

			if (!c_rasterizer_loading_screen::suppress_render_scene())
			{
				c_performance_throttles::update_current_performance_throttles();

				c_screen_postprocess::accept_edited_settings();

				c_static_wchar_string<32> pix_name;
				bool is_widescreen = c_rasterizer::get_is_widescreen();

				for (int32 window_index = 0; window_index < window_count; window_index++)
				{
					c_rasterizer_profile_scope _player_view(_rasterizer_profile_element_total, pix_name.print(L"player_view %d", window_index));
					c_player_view* player_view = c_player_view::get_current(window_index);

					c_water_renderer::set_player_window(window_index, window_count, is_widescreen);
					player_view->m_stall_cpu_to_wait_for_gpu = window_index == window_count - 1;
					main_render_view(player_view, window_index);
				}

				c_ui_view ui_view{};
				ui_view.setup_camera(NULL, c_rasterizer::get_display_surface());
				{
					c_rasterizer_profile_scope _fullscreen_view_render(_rasterizer_profile_element_total, L"fullscreen_view_render");

					c_ui_view::begin(&ui_view);

					static bool restore = true;
					if (restore && window_count > 1)
					{
						for (int32 window_index = window_count - 1; window_index >= 0; window_index--)
						{
							c_player_view* player_view = c_player_view::get_current(window_index);
							player_view->restore_to_display_surface();
						}
					}

					c_rasterizer::begin_high_quality_blend();

					if (bink_playback_in_progress())
					{
						c_rasterizer_profile_scope _bink_playback_render(_rasterizer_profile_element_total, L"bink_playback_render");
						bink_playback_render();
					}

					{
						c_rasterizer_profile_scope _interface_draw_fullscreen_overlays(_rasterizer_profile_element_total, L"interface_draw_fullscreen_overlays");
						interface_draw_fullscreen_overlays();
					}

					{
						c_rasterizer_profile_scope _director_render(_rasterizer_profile_element_total, L"director_render");
						director_render();
					}

					{
						c_rasterizer_profile_scope _cinematic_render(_rasterizer_profile_element_total, L"cinematic_render");
						cinematic_render(true, true);
					}

					{
						c_rasterizer_profile_scope _ui_view_render(_rasterizer_profile_element_total, L"ui_view_render");
						ui_view.render();
					}

					{
						c_rasterizer_profile_scope _closed_caption_render(_rasterizer_profile_element_total, L"closed_caption_render");
						closed_caption_render();
					}

					{
						c_rasterizer_profile_scope _render_debug_frame_render(_rasterizer_profile_element_total, L"render_debug_frame_render");
						render_debug_frame_render();
					}

					game_engine_render_watermarks();
					c_view::end();
				}

				// $IMPLEMENT these?
				// `screenshot_post_render` is a nullsub, so skip `screenshot_render` logic
				//if (screenshot_render(c_player_view::get_current(), get_render_player_window_game_state(0)->m_camera_fx_values, iterator.get_window_count()))
				//{
				//	screenshot_post_render();
				//}

				c_render_globals::set_depth_fade_active(false);
				c_render_globals::set_distortion_active(false);
			}

			{
				c_rasterizer_profile_scope _loading_screen(_rasterizer_profile_element_total, L"loading_screen");
				if (c_rasterizer_loading_screen::active())
				{
					c_rasterizer_loading_screen::render();
				}
			}
		}
	}
}

void __cdecl game_engine_render_window_watermarks(int32 user_index)
{
}

void __cdecl game_engine_render_frame_watermarks_for_controller_halo3_alpha(e_controller_index controller_index)
{
	static bool first_run = true;
	static c_static_string<128> strings[3]{};
	static c_static_string<20> player_xuid_rand{};
	static c_static_string<16> display_name{};

	if (first_run)
	{
		first_run = false;

		strings[0].clear();
		strings[1].clear();
		strings[2].clear();
		player_xuid_rand.clear();
		display_name.clear();
	}

	int32 player_xuid_upper32 = 0;
	if (controller_index != k_no_controller)
	{
		c_controller_interface* controller = controller_get(controller_index);
		wchar_string_to_ascii_string(controller->get_display_name(), display_name.get_buffer(), display_name.element_count, 0);
		player_xuid_upper32 = controller->get_player_xuid() >> 32;
	}

	strings[0].print("ALPHA BUILD");
	strings[2].print("%s", netdebug_get_sessionid());

	int32 random_value = 0x19660D * system_milliseconds() + 0x3C6EF35F;
	int32 player_xuid_rand_value = player_xuid_upper32 ^ random_value ^ 0xAEA9434D;

	player_xuid_rand.print("%.8x|n%.8x", random_value, player_xuid_rand_value);

	c_font_cache_mt_safe font_cache{};
	c_rasterizer_draw_string draw_string{};

	rectangle2d bounds{};
	interface_get_current_display_or_window_settings(NULL, NULL, NULL, &bounds);

	draw_string.set_font(_body_text_font);
	draw_string.set_color(0xB0FFFFFF);
	draw_string.set_justification(_text_justification_center);
	int16 line_height = draw_string.get_line_height();

	{
		real_rectangle2d rect{};
		set_real_rectangle2d(&rect, bounds.x0, real32(bounds.x1 - 10), real32(bounds.y1 - 4 * line_height), bounds.y1);

		for (int32 i = 0; i < NUMBEROF(strings); i++)
		{
			draw_string.set_bounds(&rect);
			rect.y0 += line_height;

			draw_string.draw(&font_cache, strings[i].get_string());
		}
	}

	draw_string.set_justification(_text_justification_right);

	{
		real_rectangle2d rect{};
		set_real_rectangle2d(&rect, bounds.x0, real32(bounds.x1 - 15), real32(bounds.y1 - 3 * line_height), bounds.y1);

		draw_string.set_bounds(&rect);
		draw_string.draw(&font_cache, player_xuid_rand.get_string());
	}
}

void __cdecl game_engine_render_frame_watermarks_for_controller_halo3_beta(e_controller_index controller_index)
{
	static bool first_run = true;
	static c_static_string<128> strings[3]{};
	static c_static_string<20> player_xuid_rand{};
	static c_static_string<16> display_name{};

	if (first_run)
	{
		first_run = false;

		strings[0].clear();
		strings[1].clear();
		strings[2].clear();
		player_xuid_rand.clear();
		display_name.clear();
	}

	int32 player_xuid_upper32 = 0;
	if (controller_index != k_no_controller)
	{
		c_controller_interface* controller = controller_get(controller_index);
		wchar_string_to_ascii_string(controller->get_display_name(), display_name.get_buffer(), display_name.element_count, 0);
		player_xuid_upper32 = controller->get_player_xuid() >> 32;
	}

	strings[0].print("BETA BUILD");
	strings[1].print("%s", display_name.get_string());
	strings[2].print("%s", netdebug_get_sessionid());

	int32 random_value = 0x19660D * system_milliseconds() + 0x3C6EF35F;
	int32 player_xuid_rand_value = player_xuid_upper32 ^ random_value ^ 0xAEA9434D;

	player_xuid_rand.print("%.8x|n%.8x", random_value, player_xuid_rand_value);

	c_font_cache_mt_safe font_cache{};
	c_rasterizer_draw_string draw_string{};

	rectangle2d bounds{};
	interface_get_current_display_or_window_settings(NULL, NULL, NULL, &bounds);

	draw_string.set_font(_body_text_font);
	draw_string.set_color(0xB0FFFFFF);
	draw_string.set_justification(_text_justification_center);
	int16 line_height = draw_string.get_line_height();

	{
		real_rectangle2d rect{};
		set_real_rectangle2d(&rect, bounds.x0, real32(bounds.x1 - 10), real32(bounds.y1 - 6 * line_height), bounds.y1);

		for (int32 i = 0; i < NUMBEROF(strings); i++)
		{
			draw_string.set_bounds(&rect);
			rect.y0 += line_height;

			draw_string.draw(&font_cache, strings[i].get_string());
		}
	}

	draw_string.set_justification(_text_justification_right);

	{
		real_rectangle2d rect{};
		set_real_rectangle2d(&rect, bounds.x0, real32(bounds.x1 - 15), real32(bounds.y1 - 7 * line_height), bounds.y1);

		draw_string.set_bounds(&rect);
		draw_string.draw(&font_cache, player_xuid_rand.get_string());
	}
}

void __cdecl game_engine_render_frame_watermarks_for_controller_halo4_pre_release(e_controller_index controller_index)
{
	static bool first_run = true;
	static c_static_string<128> strings[3]{};
	static c_static_string<20> player_xuid_rand{};
	static c_static_string<16> display_name{};

	if (first_run)
	{
		first_run = false;

		strings[0].clear();
		strings[1].clear();
		strings[2].clear();
		player_xuid_rand.clear();
		display_name.clear();
	}

	int32 player_xuid_upper32 = 0;
	if (controller_index != k_no_controller)
	{
		c_controller_interface* controller = controller_get(controller_index);
		wchar_string_to_ascii_string(controller->get_display_name(), display_name.get_buffer(), display_name.element_count, 0);
		player_xuid_upper32 = controller->get_player_xuid() >> 32;
	}

	s_date_and_time date_and_time{};
	game_time_get_date_and_time(&date_and_time);

	strings[0].print("PRE-RELEASE BUILD");
	strings[1].print("%s @ %d/%d/%d %02d:%02d:%02d",
		display_name.get_string(),
		date_and_time.month,
		date_and_time.day,
		date_and_time.year,
		date_and_time.hour,
		date_and_time.minute,
		date_and_time.second);
	strings[2].print("%s", netdebug_get_sessionid());

	int32 random_value = 0x181A04 * system_milliseconds() + 0x2D016C86;
	int32 player_xuid_rand_value = player_xuid_upper32 ^ random_value ^ 0xCAF69B89;

	player_xuid_rand.print("%.8x%.8x", random_value, player_xuid_rand_value);

	c_font_cache_mt_safe font_cache{};
	c_rasterizer_draw_string draw_string{};

	rectangle2d bounds{};
	interface_get_current_display_or_window_settings(NULL, NULL, NULL, &bounds);

	draw_string.set_font(_body_text_font);
	draw_string.set_color(0xB0FFFFFF);
	draw_string.set_justification(_text_justification_right);
	int16 line_height = draw_string.get_line_height();

	{
		real_rectangle2d rect{};
		set_real_rectangle2d(&rect, bounds.x0, real32(bounds.x1 - 10), real32(bounds.y1 - 13 * line_height), bounds.y1);

		for (int32 i = 0; i < NUMBEROF(strings); i++)
		{
			draw_string.set_bounds(&rect);
			rect.y0 += line_height;

			draw_string.draw(&font_cache, strings[i].get_string());
		}
	}

	draw_string.set_justification(_text_justification_right);

	{
		real_rectangle2d rect{};
		set_real_rectangle2d(&rect, real32(bounds.x0 + 10), bounds.x1, real32(bounds.y0 + 7 * line_height), bounds.y1);

		draw_string.set_bounds(&rect);
		draw_string.draw(&font_cache, player_xuid_rand.get_string());
	}
}

void __cdecl game_engine_render_frame_watermarks_for_controller(e_controller_index controller_index)
{
	switch (g_watermark_enabled)
	{
	case 1:
		game_engine_render_frame_watermarks_for_controller_halo3_alpha(controller_index);
		break;
	case 2:
		game_engine_render_frame_watermarks_for_controller_halo3_beta(controller_index);
		break;
	case 3:
		game_engine_render_frame_watermarks_for_controller_halo4_pre_release(controller_index);
		break;
	}
}

void __cdecl game_engine_render_frame_watermarks(bool pregame)
{
	if (g_watermark_enabled && (pregame || simulation_starting_up()) && !bink_playback_active())
	{
		c_static_wchar_string<1024> status;
		c_font_cache_mt_safe font_cache;
		c_rasterizer_draw_string draw_string;

		const wchar_t* spinner_states[] = { L"/", L"-", L"\\" };
		int32 spinner_state_index = 8 * system_milliseconds() / 1000 % NUMBEROF(spinner_states);
		status.print(L"Establishing connection... %s|n(please do not turn off your Xbox 360)|n|n%S",
			spinner_states[spinner_state_index],
			simulation_get_starting_up_description());

		draw_string.set_font(_full_screen_hud_message_font);
		draw_string.set_justification(_text_justification_center);
		draw_string.set_color(global_real_argb_white);

		rectangle2d display_title_safe_pixel_bounds{};
		interface_get_current_display_or_window_settings(NULL, NULL, NULL, &display_title_safe_pixel_bounds);
		real_rectangle2d bounds{};
		set_real_rectangle2d(&bounds,
			display_title_safe_pixel_bounds.x0,
			display_title_safe_pixel_bounds.x1,
			// this calculation seems to be correct, could possibly go a little lower
			((display_title_safe_pixel_bounds.y1 - (4.0f * draw_string.get_line_height())) / 2),
			display_title_safe_pixel_bounds.y1
		);
		draw_string.set_bounds(&bounds);
		draw_string.draw(&font_cache, status.get_string());
	}

	if (pregame || !game_in_progress())
		game_engine_render_window_watermarks(NONE);

	//game_engine_render_frame_watermarks_for_controller(controller_get_first_non_guest_signed_in_controller());
	//game_engine_render_frame_watermarks_for_controller((e_controller_index)DECLFUNC(0x00A94930, int16, __cdecl)());
	game_engine_render_frame_watermarks_for_controller(_controller0);
}

void __cdecl main_render_pregame(e_main_pregame_frame pregame_frame_type, const char* pregame_frame_text)
{
	//INVOKE(0x00604860, main_render_pregame, pregame_frame_type, pregame_frame_text);

	if (!sub_42E5D0())
	{
		RENDER_THREAD_LOCK;

		c_view::abort_current_view_stack();

		c_fullscreen_view fullscreen_view;
		fullscreen_view.setup_camera(NULL);
		fullscreen_view.begin(&fullscreen_view);
		ASSERT(pregame_frame_type >= 0 && pregame_frame_type < k_main_pregame_frame_count);
		fullscreen_view.render_blank_frame(&pregame_frame_colors[pregame_frame_type].blank_frame);

		s_render_fullscreen_text_context context;

		context.string = pregame_frame_text;
		context.text_color = &pregame_frame_colors[pregame_frame_type].text_color;
		context.shadow_color = &pregame_frame_colors[pregame_frame_type].text_shadow_color;
		context.font_scale = pregame_frame_scales[pregame_frame_type];

		bool game_has_crashed = false;
		if (pregame_frame_type == _main_pregame_frame_normal)
		{
			if (!main_game_change_in_progress() && main_halted_with_errors())
				context.string = events_get();
		}
		else if (IN_RANGE_INCLUSIVE(pregame_frame_type, _main_pregame_frame_cache_loading, _main_pregame_frame_crash_done))
		{
			game_has_crashed = true;
		}

		render_fullscreen_text(&context, game_has_crashed);
		overlapped_render();
		controllers_render();
		//async_display_current_task_status();

		if (pregame_frame_type == _main_pregame_frame_normal)
		{
			fullscreen_view.render();

			if (bink_playback_active())
			{
				bink_playback_update();
				bink_playback_check_for_terminate_no_lock();
				bink_playback_render();
			}
		}

		if (pregame_frame_type == _main_pregame_frame_normal || pregame_frame_type == _main_pregame_frame_loading_debug)
		{
			game_engine_render_frame_watermarks(true);
		}

		c_view::end();
	}
}

void __cdecl main_render_pregame_loading_screen()
{
	INVOKE(0x00604990, main_render_pregame_loading_screen);
}

void __cdecl sub_604A20()
{
	//INVOKE(0x00604A20, sub_604A20);

	c_rasterizer::end_frame();
	g_main_render_timing_data->reset();
}

void __cdecl main_render_process_messages()
{
	INVOKE(0x00604A60, main_render_process_messages);

	//if (!thread_has_crashed(k_thread_render))
	//{
	//	objects_render_thread_update();
	//	effects_process_render_thread_messages();
	//	c_decal_system::process_render_thread_messages();
	//}
	//main_render_purge_pending_messages();
}

void __cdecl main_render_purge_pending_messages()
{
	INVOKE(0x00604AA0, main_render_purge_pending_messages);

	//reset_object_render_message_queue();
	//reset_effect_render_message_queue(0);
	//c_decal_system::reset_render_message_queue();
}

void __cdecl main_render_sapien()
{
	//INVOKE(0x00604AC0, main_render_sapien);

	main_render_game();
}

// sets a bool related to D3D
void __cdecl main_render_sub_604AD0()
{
	INVOKE(0x00604AD0, main_render_sub_604AD0);
}

void __cdecl main_render_start_blocking_frame()
{
	//INVOKE(0x00604AE0, main_render_start_blocking_frame);

	//g_main_render_block_watch.start();
}

void __cdecl main_render_status_message(const wchar_t* loading_status)
{
	INVOKE(0x00604AF0, main_render_status_message, loading_status);
}

void __cdecl main_render_update_loading_screen()
{
	INVOKE(0x00604C70, main_render_update_loading_screen);
}

void __cdecl main_render_view(c_player_view* player_view, int32 player_index)
{
	//INVOKE(0x00604D70, main_render_view, player_view, player_index);

	c_player_view::set_global_player_view(player_view);
	c_view::begin(player_view);
	render_window_reset(player_view->get_player_view_user_index());
	player_view->create_frame_textures(player_index);
	render_prepare_for_window(player_index, player_view->get_player_view_user_index());
	player_view->compute_visibility();
	player_view->render_submit_visibility();
	player_view->render();
	c_view::end();
	c_player_view::set_global_player_view(NULL);
}

