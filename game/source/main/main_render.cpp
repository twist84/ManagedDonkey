#include "main/main_render.hpp"

#include "bink/bink_playback.hpp"
#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries_events.hpp"
#include "cutscene/cinematics.hpp"
#include "game/player_mapping.hpp"
#include "interface/c_controller.hpp"
#include "interface/terminal.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_globals.hpp"
#include "render/render.hpp"
#include "render/views/render_view.hpp"
#include "simulation/simulation.hpp"
#include "interface/terminal.hpp"

HOOK_DECLARE(0x00604D70, main_render_view); // paired with `main_render_view_inline_hook`

bool debug_render_horizontal_splitscreen = false;
bool debug_force_all_player_views_to_default_player = false;

c_player_render_camera_iterator::c_player_render_camera_iterator() :
	m_window_count(),
	m_window_arrangement(),
	m_next(0),
	m_user_index(-1),
	m_current_observer_result(nullptr)
{
	if (cinematic_in_progress()/* || player_is_reading_terminal()*/)
	{
		m_window_count = 1;
	}
	else
	{
		long output_user_active_count = player_mapping_output_user_active_count();

		long player_count = 1;
		if (output_user_active_count > 1)
			player_count = output_user_active_count;

		long window_count = 1;
		if (player_count > MAXIMUM_PLAYER_WINDOWS)
			window_count = MAXIMUM_PLAYER_WINDOWS;
		m_window_count = window_count;
	}

	ASSERT(m_window_count <= MAXIMUM_PLAYER_WINDOWS);

	if (m_window_count <= 1)
	{
		m_window_arrangement = 0;
	}
	else
	{
		bool is_widescreen = rasterizer_get_is_widescreen();
		if (debug_render_horizontal_splitscreen)
			is_widescreen = true;

		if (is_widescreen)
			m_window_arrangement = 2;
		else
			m_window_arrangement = 1;
	}
}

bool c_player_render_camera_iterator::next()
{
	bool result = m_next < m_window_count;
	if (result)
	{
		m_next++;

		if (debug_force_all_player_views_to_default_player)
		{
			m_user_index = player_mapping_first_active_output_user();
		}
		else
		{
			m_user_index++;
			while (m_user_index < MAXIMUM_PLAYER_WINDOWS && !players_user_is_active(m_user_index))
				m_user_index++;

			if (m_user_index >= MAXIMUM_PLAYER_WINDOWS)
			{
				m_user_index = -1;
				result = false;
			}
		}

		if (m_user_index != -1)
		{
			m_current_observer_result = observer_get_camera(m_user_index);
			ASSERT(m_current_observer_result != NULL);
		}
	}

	return result;
}

long c_player_render_camera_iterator::get_window_count() const
{
	return m_window_count;
}

long c_player_render_camera_iterator::get_window_arrangement() const
{
	return m_window_arrangement;
}

long c_player_render_camera_iterator::get_user_index() const
{
	return m_user_index;
}

s_observer_result const* c_player_render_camera_iterator::get_observer_result() const
{
	return m_current_observer_result;
}

void __cdecl main_render()
{
	INVOKE(0x006042C0, main_render);
}

void __cdecl main_render_assert_no_pending_messages()
{
	INVOKE(0x00604420, main_render_assert_no_pending_messages);
}

void __cdecl main_render_frame_begin()
{
	INVOKE(0x00604430, main_render_frame_begin);
}

void __cdecl main_render_game()
{
	INVOKE(0x00604440, main_render_game);
}

bool __cdecl sub_42E5D0()
{
	return INVOKE(0x0042E5D0, sub_42E5D0);
}

void __cdecl main_render_pregame(long pregame_frame_type, char const* text)
{
	//INVOKE(0x00604860, main_render_pregame, pregame_frame_type, text);

	if (!sub_42E5D0())
	{
		c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);

		c_view::abort_current_view_stack();

		c_fullscreen_view fullscreen_view;
		fullscreen_view.setup_camera(nullptr);
		fullscreen_view.begin(&fullscreen_view);
		fullscreen_view.render_blank_frame(&pregame_frame_colors[pregame_frame_type].blank_frame);

		s_render_fullscreen_text_context context;
		context.text = text;
		context.color = &pregame_frame_colors[pregame_frame_type].text_color;
		context.shadow_color = &pregame_frame_colors[pregame_frame_type].text_shadow_color;
		context.scale = pregame_frame_scales[pregame_frame_type];

		bool simple_font = true;
		if (pregame_frame_type == 1 || (pregame_frame_type - 4) <= 1)
		{
			//context.text = events_get();
			simple_font = false;
		}

		render_fullscreen_text(&context, simple_font);
		overlapped_render();
		controllers_render();

		// this isn't actually supposed to be here
		terminal_draw();

		if (pregame_frame_type == 1)
		{
			fullscreen_view.render();

			if (bink_playback_active())
			{
				bink_playback_update();
				bink_playback_check_for_terminate();
				bink_playback_render();
			}
		}

		c_view::end();
	}
}

void __cdecl main_render_pregame_loading_screen()
{
	INVOKE(0x00604990, main_render_pregame_loading_screen);
}

// 00604A20
// calls `c_rasterizer::end_frame`

void __cdecl main_render_process_messages()
{
	INVOKE(0x00604A60, main_render_process_messages);
}

void __cdecl main_render_purge_pending_messages()
{
	INVOKE(0x00604AA0, main_render_purge_pending_messages);
}

// 00604AD0
// sets a bool related to D3D

void __cdecl main_render_start_blocking_frame()
{
	INVOKE(0x00604AE0, main_render_start_blocking_frame);
}

void __cdecl main_render_status_message(wchar_t const* loading_status)
{
	INVOKE(0x00604AF0, main_render_status_message, loading_status);
}

void __cdecl main_render_update_loading_screen()
{
	INVOKE(0x00604C70, main_render_update_loading_screen);
}

// I don't like this at all but for the moment
// I don't perticularly want to reimplement `main_render_game` as its quite large
__declspec(naked) void main_render_view_inline()
{
	__asm
	{
        // main_render_view(player_view, player_view->get_player_view_user_index())
        push dword ptr[esi + 0x26A4]
        push esi
        call main_render_view

        // jump out after the inlined `main_render_view`
		// we push the return address to the stack for the jump,
		// the stack will be cleaned up outside of this hook
		// this is **very** non-standard and bad, *Wait, That's Illegal*
        push 0x00604747
        jmp dword ptr[esp]
	}
}
HOOK_DECLARE(0x006046EB, main_render_view_inline);

void __cdecl main_render_view(c_player_view* player_view, long player_index)
{
	//INVOKE(0x00604D70, main_render_view, player_view, player_index);

	c_player_view::set_global_player_view(player_view);
	c_view::begin(player_view);
	render_window_reset(player_view->get_player_view_user_index());
	player_view->create_frame_textures(player_index);

	// don't need this, because `bink_texture_view_update` is this and its bad
	//c_bink_texture_view::update(player_index);

	render_prepare_for_window(player_index, player_view->get_player_view_user_index());
	player_view->compute_visibility();
	player_view->render_submit_visibility();
	player_view->render();

	// all this logic just to call little ol' me
	render_debug_window_render(player_view->get_player_view_user_index());

	c_view::end();
	c_player_view::set_global_player_view(0);
}

