#pragma once

#include "cseries/cseries.hpp"

struct s_chud_curvature_info;
struct s_chud_draw_widget_data;
struct s_chud_runtime_widget_datum;

struct s_chud_render_data
{
	int32 vertex_shader_tag_index;
	int32 pixel_shader_tag_index;
	int16 alpha_blend_mode;
	real_vector4d chud_color_outputs[6];
	real_vector4d chud_scalar_output_abcd;
	real_vector4d chud_scalar_output_ef;
	real_vector4d texture_transform;
	rectangle2d rectangle;
};
static_assert(sizeof(s_chud_render_data) == 0xA4);

class c_chud_point_tracker
{
	bool m_active;
	real_point3d m_position;
	real_point2d m_position_screenspace;
	real_point2d m_position_virtual;
	real32 m_offscreen_amount;
	bool m_behind_camera;
	real32 m_behind_camera_yaw_sign;
};
static_assert(sizeof(c_chud_point_tracker) == 0x2C);

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

	rectangle2d adjusted_camera_window_title_safe_pixel_bounds;

	real32 virtual_width;
	real32 virtual_height;

	s_chud_curvature_info* current_curvature_info;
	real_point2d current_sensor_origin;
	int32 current_resolution;

	real32 global_alpha;
	real32 global_first_person_theta;

	int32 user_index;
	int32 user_skin_index;

	int32 metagame_player_count;
	real_point2d saved_film_bar_position;
};
static_assert(sizeof(s_chud_draw_globals) == 0xBC);

extern bool& chud_enabled;
extern bool& chud_debug_messages;
extern s_chud_draw_globals& g_chud_draw_globals;
extern bool& chud_debug_grid;
extern bool& chud_contain_turbulence;
extern bool& chud_debug_crosshair;

extern bool __cdecl chud_compute_render_data(s_chud_draw_widget_data* draw_widget_data, s_chud_render_data* out_render_data, bool is_draw_turbulence);
extern void __cdecl chud_debug_draw(int32 user_index);
extern void __cdecl chud_debug_draw_reticle(int32 user_index, real32 angle, const real_argb_color* color);
extern void __cdecl chud_debug_draw_reticle_labeled(int32 user_index, real32 angle, const char* label, real32 a4, const real_argb_color* color);
extern void __cdecl chud_debug_render();
extern bool __cdecl chud_draw_begin(int32 user_index, real32 global_hud_alpha, bool setup_cortana_effect, bool drawing_saved_film);
extern void __cdecl chud_draw_bitmap_widget(int32 user_index, void* draw_widget_data, bool is_draw_turbulence);
extern void __cdecl chud_draw_end(int32 user_index, bool resolve_cortana_effect);
extern void __cdecl chud_draw_text_widget(int32 user_index, void* draw_widget_data, bool is_draw_turbulence);
extern void __cdecl chud_draw_widget(int32 user_index, s_chud_runtime_widget_datum* widget, int32 chud_definition_index, int32 is_draw_turbulence);

