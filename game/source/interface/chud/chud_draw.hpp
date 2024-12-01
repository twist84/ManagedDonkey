#pragma once

#include "cseries/cseries.hpp"

struct s_chud_curvature_info;
struct s_chud_draw_globals
{
	real_rectangle2d displayed_global_bounds;
	real_rectangle2d displayed_global_safe_bounds;

	real_rectangle2d displayed_window_bounds;
	real_rectangle2d displayed_window_safe_bounds;

	real_rectangle2d rendered_global_bounds;
	real_rectangle2d rendered_global_safe_bounds;

	real_rectangle2d rendered_window_bounds;
	real_rectangle2d rendered_window_safe_bounds;

	short_rectangle2d adjusted_camera_window_title_safe_pixel_bounds;

	real virtual_width;
	real virtual_height;

	s_chud_curvature_info* current_curvature_info;
	real_point2d current_sensor_origin;
	long current_resolution;

	real global_alpha;
	real global_first_person_theta;

	long user_index;
	long user_skin_index;

	long metagame_player_count;
	real_point2d saved_film_bar_position;
};
static_assert(sizeof(s_chud_draw_globals) == 0xBC);

extern bool& chud_enabled;
extern bool& chud_debug_messages;
extern s_chud_draw_globals& g_chud_draw_globals;
extern bool& chud_debug_grid;
extern bool& chud_debug_crosshair;


extern bool __cdecl chud_compute_render_data(void* draw_widget_data, void* out_render_data, bool is_draw_turbulence);
extern void __cdecl chud_debug_draw(long user_index);
extern void __cdecl chud_debug_draw_reticle(long user_index, real angle, real_argb_color const* color);
extern void __cdecl chud_debug_draw_reticle_labeled(long user_index, real angle, char const* label, real a4, real_argb_color const* color);
extern void __cdecl chud_debug_render();
extern void __cdecl chud_draw_bitmap_widget(long user_index, void* draw_widget_data, bool is_draw_turbulence);
extern void __cdecl chud_draw_text_widget(long user_index, void* draw_widget_data, bool is_draw_turbulence);

