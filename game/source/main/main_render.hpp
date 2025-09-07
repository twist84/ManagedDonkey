#pragma once

#include "camera/observer.hpp"

#define RENDER_ENABLED(true_false) if (sub_42E5D0() == true_false)

class c_player_view;

enum
{
	_render_player_window_arrangement_single = 0,
	_render_player_window_arrangement_multiple_vertical,
	_render_player_window_arrangement_multiple_horizontal,

	k_render_player_window_arrangement_count,
};

class c_player_render_camera_iterator
{
public:
	c_player_render_camera_iterator();
	const s_observer_result* get_observer_result() const;
	int32 get_user_index() const;
	int32 get_window_arrangement() const;
	int32 get_window_count() const;
	bool next();

protected:
	int32 m_window_count;
	int32 m_window_arrangement;
	int32 m_next_window_index;
	int32 m_current_user_index;
	const s_observer_result* m_current_observer_result;
};
static_assert(sizeof(c_player_render_camera_iterator) == 0x14);

struct s_render_fullscreen_text_context_colors
{
	real_rgb_color blank_frame;
	real_rgb_color text_color;
	real_rgb_color text_shadow_color;
};

struct s_render_fullscreen_text_context
{
	const char* string;
	real_rgb_color* text_color;
	real_rgb_color* shadow_color;
	real32 font_scale;
};
static_assert(sizeof(s_render_fullscreen_text_context) == 0x10);

extern real_vector4d& g_player_window_constants;

extern bool debug_force_all_player_views_to_default_player;
extern bool debug_render_horizontal_splitscreen;
extern int32 g_watermark_enabled;

extern real32 pregame_frame_scales[k_main_pregame_frame_count];
extern s_render_fullscreen_text_context_colors pregame_frame_colors[k_main_pregame_frame_count];

extern void __cdecl main_render();
extern void __cdecl main_render_assert_no_pending_messages();
extern void __cdecl main_render_frame_begin();
extern void __cdecl main_render_game();
extern void __cdecl game_engine_render_window_watermarks(int32 user_index);
extern void __cdecl game_engine_render_frame_watermarks_for_controller(e_controller_index controller_index);
extern void __cdecl game_engine_render_frame_watermarks(bool pregame);
extern void __cdecl main_render_pregame(e_main_pregame_frame main_pregame_frame, const char* pregame_frame_text);
extern void __cdecl main_render_pregame_loading_screen();
extern void __cdecl sub_604A20();
extern void __cdecl main_render_process_messages();
extern void __cdecl main_render_purge_pending_messages();
extern void __cdecl main_render_sapien();
extern void __cdecl main_render_sub_604AD0();
extern void __cdecl main_render_start_blocking_frame();
extern void __cdecl main_render_status_message(const wchar_t* loading_status);
extern void __cdecl main_render_update_loading_screen();
extern void __cdecl main_render_view(c_player_view* player_view, int32 player_index);

