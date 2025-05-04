#include "interface/interface_constants.hpp"

void __cdecl calculate_aspect_ratio_scaling()
{
	INVOKE(0x00A9F6F0, calculate_aspect_ratio_scaling);
}

//00A9F800 ; void __cdecl sub_A9F800()

real_vector2d __cdecl interface_get_aspect_ratio_scaling()
{
	return INVOKE(0x00A9F900, interface_get_aspect_ratio_scaling);
}

real_vector2d __cdecl interface_get_authoring_aspect_conversion_scaling(int32 destination_aspect)
{
	return INVOKE(0x00A9F920, interface_get_authoring_aspect_conversion_scaling, destination_aspect);
}

real_vector2d __cdecl interface_get_authoring_to_target_dimensions_scaling()
{
	return INVOKE(0x00A9F960, interface_get_aspect_ratio_scaling);
}

int32 __cdecl interface_get_closest_authored_ui_aspect_ratio()
{
	return INVOKE(0x00A9F980, interface_get_closest_authored_ui_aspect_ratio);
}

int32 __cdecl interface_get_closest_authoring_screen_dimensions()
{
	return INVOKE(0x00A9F9D0, interface_get_closest_authoring_screen_dimensions);
}

void __cdecl interface_get_current_display_or_window_settings(rectangle2d* out_render_bounds, rectangle2d* out_render_title_safe_bounds, rectangle2d* out_display_bounds, rectangle2d* out_display_title_safe_bounds)
{
	INVOKE(0x00A9FA40, interface_get_current_display_or_window_settings, out_render_bounds, out_render_title_safe_bounds, out_display_bounds, out_display_title_safe_bounds);
}

int32 __cdecl interface_get_current_display_settings(rectangle2d* out_render_bounds, rectangle2d* out_render_title_safe_bounds, rectangle2d* out_display_bounds, rectangle2d* out_display_title_safe_bounds)
{
	return INVOKE(0x00A9FAA0, interface_get_current_display_settings, out_render_bounds, out_render_title_safe_bounds, out_display_bounds, out_display_title_safe_bounds);

	//if (out_render_bounds)
	//	c_rasterizer::get_fullscreen_render_pixel_bounds(out_render_bounds);
	//
	//if (out_render_title_safe_bounds)
	//	c_rasterizer::get_fullscreen_render_title_safe_pixel_bounds(out_render_title_safe_bounds);
	//
	//if (out_display_bounds)
	//	c_rasterizer::get_display_pixel_bounds(out_display_bounds);
	//
	//if (out_display_title_safe_bounds)
	//	c_rasterizer::get_display_title_safe_pixel_bounds(out_display_title_safe_bounds);
}

void __cdecl interface_get_current_window_settings(rectangle2d* out_render_window_bounds, rectangle2d* out_render_window_title_safe_bounds, rectangle2d* out_display_window_bounds, rectangle2d* out_display_window_title_safe_bounds)
{
	INVOKE(0x00A9FAF0, interface_get_current_window_settings, out_render_window_bounds, out_render_window_title_safe_bounds, out_display_window_bounds, out_display_window_title_safe_bounds);
}

void __cdecl interface_scale_rectangle2d_for_xenon_scaler(rectangle2d* rectangle, real_vector2d const* aspect_ratio_scale)
{
	//INVOKE(0x00A9FE30, interface_scale_rectangle2d_for_xenon_scaler, rectangle, aspect_ratio_scale);

	rectangle->y0 = static_cast<int16>(rectangle->y0 * aspect_ratio_scale->i);
	rectangle->y1 = static_cast<int16>(rectangle->y1 * aspect_ratio_scale->i);
	rectangle->x0 = static_cast<int16>(rectangle->x0 * aspect_ratio_scale->j);
	rectangle->x1 = static_cast<int16>(rectangle->x1 * aspect_ratio_scale->j);
}

void __cdecl interface_scale_screenspace_vertices_for_xenon_scaler(rasterizer_vertex_screen* vertices, int32 vertex_count, real_vector2d const* aspect_ratio_scale)
{
	INVOKE(0x00A9FEB0, interface_scale_screenspace_vertices_for_xenon_scaler, vertices, vertex_count, aspect_ratio_scale);
}

