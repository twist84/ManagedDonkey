#include "interface/interface_constants.hpp"

void __cdecl calculate_aspect_ratio_scaling()
{
	INVOKE(0x00A9F6F0, calculate_aspect_ratio_scaling);
}

//00A9F800 ; void __cdecl sub_A9F800()

vector2d __cdecl interface_get_aspect_ratio_scaling()
{
	return INVOKE(0x00A9F900, interface_get_aspect_ratio_scaling);
}

vector2d __cdecl interface_get_authoring_aspect_conversion_scaling(long authored_ui_aspect_ratio)
{
	return INVOKE(0x00A9F920, interface_get_authoring_aspect_conversion_scaling, authored_ui_aspect_ratio);
}

vector2d __cdecl interface_get_authoring_to_target_dimensions_scaling()
{
	return INVOKE(0x00A9F960, interface_get_aspect_ratio_scaling);
}

long __cdecl interface_get_closest_authored_ui_aspect_ratio()
{
	return INVOKE(0x00A9F980, interface_get_closest_authored_ui_aspect_ratio);
}

long __cdecl interface_get_closest_authoring_screen_dimensions()
{
	return INVOKE(0x00A9F9D0, interface_get_closest_authoring_screen_dimensions);
}

void __cdecl interface_get_current_display_or_window_settings(short_rectangle2d* fullscreen_render_pixel_bounds, short_rectangle2d* fullscreen_render_title_safe_pixel_bounds, short_rectangle2d* display_pixel_bounds, short_rectangle2d* display_title_safe_pixel_bounds)
{
	INVOKE(0x00A9FA40, interface_get_current_display_or_window_settings, fullscreen_render_pixel_bounds, fullscreen_render_title_safe_pixel_bounds, display_pixel_bounds, display_title_safe_pixel_bounds);
}

long __cdecl interface_get_current_display_settings(short_rectangle2d* fullscreen_render_pixel_bounds, short_rectangle2d* fullscreen_render_title_safe_pixel_bounds, short_rectangle2d* display_pixel_bounds, short_rectangle2d* display_title_safe_pixel_bounds)
{
	return INVOKE(0x00A9FAA0, interface_get_current_display_settings, fullscreen_render_pixel_bounds, fullscreen_render_title_safe_pixel_bounds, display_pixel_bounds, display_title_safe_pixel_bounds);
}

void __cdecl interface_get_current_window_settings(short_rectangle2d* fullscreen_render_pixel_bounds, short_rectangle2d* fullscreen_render_title_safe_pixel_bounds, short_rectangle2d* display_pixel_bounds, short_rectangle2d* display_title_safe_pixel_bounds)
{
	INVOKE(0x00A9FAF0, interface_get_current_window_settings, fullscreen_render_pixel_bounds, fullscreen_render_title_safe_pixel_bounds, display_pixel_bounds, display_title_safe_pixel_bounds);
}

void __cdecl interface_scale_rectangle2d_for_xenon_scaler(short_rectangle2d* rectangle, vector2d const* vector)
{
	//INVOKE(0x00A9FE30, interface_scale_rectangle2d_for_xenon_scaler, rectangle, vector);

	rectangle->y0 = static_cast<short>(rectangle->y0 * vector->i);
	rectangle->y1 = static_cast<short>(rectangle->y1 * vector->i);
	rectangle->x0 = static_cast<short>(rectangle->x0 * vector->j);
	rectangle->x1 = static_cast<short>(rectangle->x1 * vector->j);
}

