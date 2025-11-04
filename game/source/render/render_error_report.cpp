#include "render/render_error_report.hpp"

#include "rasterizer/rasterizer.hpp"
#include "render/render_debug.hpp"

bool error_geometry_object_enabled = false;
bool render_debug_safe_frame_bounds = false;

void __cdecl render_report_render_debug(int32 user_index, bool draw_environment_error)
{
	//if (render_debug_drawing_active())
	//{
	//	error_report_begin_frame();
	//	if (error_geometry_object_enabled)
	//	{
	//
	//	}
	//	if (draw_environment_error)
	//	{
	//		render_report_draw_environment_error_report();
	//	}
	//	error_report_end_frame();
	//}

	if (render_debug_safe_frame_bounds)
	{
		rectangle2d resolution{};
		c_rasterizer::get_fullscreen_render_title_safe_pixel_bounds(&resolution);

		real_point2d safe_frame_bounds[4]{};
		set_real_point2d(&safe_frame_bounds[0], (real32)resolution.x0, (real32)resolution.y0);
		set_real_point2d(&safe_frame_bounds[1], (real32)resolution.x1, (real32)resolution.y0);
		set_real_point2d(&safe_frame_bounds[2], (real32)resolution.x1, (real32)resolution.y1);
		set_real_point2d(&safe_frame_bounds[3], (real32)resolution.x0, (real32)resolution.y1);

		render_debug_line2d(&safe_frame_bounds[0], &safe_frame_bounds[1], global_real_argb_white);
		render_debug_line2d(&safe_frame_bounds[1], &safe_frame_bounds[2], global_real_argb_white);
		render_debug_line2d(&safe_frame_bounds[2], &safe_frame_bounds[3], global_real_argb_white);
		render_debug_line2d(&safe_frame_bounds[3], &safe_frame_bounds[0], global_real_argb_white);
	}
}

bool __cdecl render_debug_drawing_active()
{
	return false;
}

void __cdecl error_report_begin_frame()
{
}

void __cdecl error_report_end_frame()
{
}

void __cdecl render_report_draw_environment_error_report()
{
}

