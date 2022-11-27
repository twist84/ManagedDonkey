#include "main/main_render.hpp"

#include "cutscene/cinematics.hpp"
#include "game/player_mapping.hpp"
#include "rasterizer/rasterizer_globals.hpp"

#include <assert.h>

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

    assert(m_window_count <= MAXIMUM_PLAYER_WINDOWS);

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
            assert(m_current_observer_result != NULL);
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

void __cdecl main_render_pregame(long main_pregame_frame, char const* loading_status)
{
    INVOKE(0x00604860, main_render_pregame, main_pregame_frame, loading_status);

    //if (!sub_42E5D0())
    //{
    //    dword flags = _internal_halt_render_thread_and_lock_resources(__FILE__, __LINE__);
    //
    //    c_view::abort_current_view_stack();
    //
    //    c_fullscreen_view fullscreen_view;
    //    fullscreen_view.setup_camera(nullptr);
    //    fullscreen_view.begin();
    //    fullscreen_view.render_blank_frame(fullscreen_text_context_colors[main_pregame_frame][0]);
    //
    //    s_render_fullscreen_text_context context;
    //    context.text = text;
    //    context.color = &fullscreen_text_context_colors[pregame_frame_type][1];
    //    context.shadow_color = &fullscreen_text_context_colors[pregame_frame_type][2];
    //    context.scale = fullscreen_text_context_scales[pregame_frame_type];
    //
    //    if (pregame_frame_type == 4 || pregame_frame_type == 5)
    //    {
    //        render_fullscreen_text(&context, true);
    //        overlapped_render();
    //        controllers_render();
    //    }
    //
    //    if (pregame_frame_type == 1)
    //    {
    //        c_fullscreen_view::render();
    //        if (bink_playback_active())
    //        {
    //            bink_playback_update();
    //            bink_playback_check_for_terminate();
    //            bink_playback_render();
    //        }
    //    }
    //    c_view::end();
    //
    //    unlock_resources_and_resume_render_thread(flags);
    //}
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

