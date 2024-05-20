#pragma once

#include "camera/observer.hpp"
#include "shell/shell.hpp"

#define MAXIMUM_PLAYER_WINDOWS 4
#define RENDER_ENABLED(true_false) if (sub_42E5D0() == true_false)

struct c_player_render_camera_iterator
{
public:
	c_player_render_camera_iterator();
	bool next();
	long get_window_count() const;
	long get_window_arrangement() const;
	e_output_user_index get_output_user_index() const;
	s_observer_result const* get_observer_result() const;

protected:
	long m_window_count;
	long m_window_arrangement;
	long m_next;
	e_output_user_index m_output_user_index;
	s_observer_result const* m_current_observer_result;
};
static_assert(sizeof(c_player_render_camera_iterator) == 0x14);

enum e_main_pregame_frame
{
	_main_pregame_frame_none = 0,
	_main_pregame_frame_normal,
	_main_pregame_frame_progress_report,
	_main_pregame_frame_status_message,
	_main_pregame_frame_minidump_upload_waiting,
	_main_pregame_frame_minidump_upload_completed_successfully,
	_main_pregame_frame_unknown6,
	_main_pregame_frame_notify_out_of_sync,
	_main_pregame_frame_loading_screen,

	k_main_pregame_frame_count
};

struct s_render_fullscreen_text_context_colors
{
	real_rgb_color blank_frame;
	real_rgb_color text_color;
	real_rgb_color text_shadow_color;
};

struct s_render_fullscreen_text_context
{
	char const* text;
	real_rgb_color* color;
	real_rgb_color* shadow_color;
	real scale;
};
static_assert(sizeof(s_render_fullscreen_text_context) == 0x10);

extern real pregame_frame_scales[k_main_pregame_frame_count];
extern s_render_fullscreen_text_context_colors pregame_frame_colors[k_main_pregame_frame_count];

struct c_player_view;

extern bool __cdecl sub_42E5D0();
extern void __cdecl main_render();
extern void __cdecl main_render_assert_no_pending_messages();
extern void __cdecl main_render_frame_begin();
extern void __cdecl main_render_game();
extern void __cdecl main_render_pregame(e_main_pregame_frame main_pregame_frame, char const* loading_status);
extern void __cdecl main_render_pregame_loading_screen();
extern void __cdecl sub_604A20();
extern void __cdecl main_render_process_messages();
extern void __cdecl main_render_purge_pending_messages();
extern void __cdecl main_render_sapien();
extern void __cdecl main_render_start_blocking_frame();
extern void __cdecl main_render_status_message(wchar_t const* loading_status);
extern void __cdecl main_render_update_loading_screen();
extern void __cdecl main_render_view(c_player_view* player_view, long player_index);

