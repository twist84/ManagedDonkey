#pragma once

#include "cseries/cseries.hpp"

struct c_render_globals
{
public:
	static void __cdecl advance_frame_time(real32 seconds_elapsed);

	static bool __cdecl get_depth_fade_active();
	static bool __cdecl get_distortion_active();
	static bool __cdecl get_distortion_history();
	static bool __cdecl get_distortion_visible();
	static int32 __cdecl get_frame_index();
	static real32 __cdecl get_frame_time();

	static void __cdecl increment_frame_index();

	static void __cdecl set_depth_fade_active(bool depth_fade_active);
	static void __cdecl set_distortion_active(bool distortion_active);
	static void __cdecl set_distortion_history(bool distortion_history);
	static void __cdecl set_distortion_visible(bool distortion_visible);
	static void __cdecl set_frame_index(int32 frame_index);
	static void __cdecl set_frame_time(real32 frame_time);

	static void __cdecl set_weather_occlusion_available(bool weather_occlusion_available);
	static bool __cdecl get_weather_occlusion_available();

//private:
	static uns32& m_frame_index;
	static real32& m_frame_time;
	static bool& m_distortion_active;
	static bool& m_distortion_visible;
	static bool& m_distortion_history;
	static bool& m_depth_fade_active;
	static bool& m_weather_occlusion_available;
	static bool& m_render_pc_albedo_lighting;
};

extern bool& render_pc_specular;


struct c_draw_string;
struct c_font_cache_base;

struct s_render_fullscreen_text_context;
struct render_camera;
struct render_projection;

extern void __cdecl combine_projection_and_view_matrix(const real_matrix4x3* view_matrix, real32 const (* const projection_matrix)[4], real32* const projection_and_view_matrix);
extern void __cdecl render_dispose();
extern void __cdecl render_dispose_from_old_map();
extern void __cdecl render_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl render_frame_begin();
extern void __cdecl render_fullscreen_text(const s_render_fullscreen_text_context* context, bool simple_font);
extern void __cdecl render_fullscreen_text_internal(const s_render_fullscreen_text_context* context, c_draw_string* draw_string, c_font_cache_base* font_cache);
extern void __cdecl render_initialize();
extern void __cdecl render_initialize_for_new_map();
extern void __cdecl render_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern void __cdecl render_prepare_for_window(int32 player_window_index, int32 user_index);
extern void __cdecl render_setup_window(render_camera* camera, render_projection* projection);
extern void __cdecl render_window_reset(int32 user_index);

