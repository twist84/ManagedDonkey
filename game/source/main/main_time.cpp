#include "main/main_time.hpp"

#include "interface/interface_constants.hpp"
#include "text/draw_string.hpp"
#include "rasterizer/rasterizer_globals.hpp"

REFERENCE_DECLARE(0x022B47FC, bool, display_framerate);

bool display_frame_deltas = false;

void rasterizer_render_debug_frame_deltas()
{
	c_rasterizer_draw_string draw_string;
	c_font_cache_mt_safe font_cache;

	short_rectangle2d bounds{};
	interface_get_current_display_settings(nullptr, nullptr, &bounds, nullptr);

	bounds.x0 = short(bounds.x1 - (50.0f * draw_string_get_glyph_scaling_for_display_settings()));
	bounds.y0 = short(bounds.y1 - (75.0f * draw_string_get_glyph_scaling_for_display_settings()));

	char string[8]{};
	for (long i = (rasterizer_globals.frame_delta_index + 14) % 15; i != rasterizer_globals.frame_delta_index; i = (i + 14) % 15)
	{
		bounds.y0 -= 20;
		bounds.y1 -= 20;

		csnzprintf(string, 4, "%d", rasterizer_globals.frame_deltas[i]);

		real_argb_color const* color = global_real_argb_green;
		if (rasterizer_globals.frame_deltas[i] > 2)
			color = global_real_argb_red;

		draw_string.set_justification(1);
		draw_string.set_color(color);
		draw_string.set_bounds(&bounds);
		draw_string.draw(&font_cache, string);
	}
}

void __cdecl main_time_frame_rate_display()
{
	INVOKE(0x00507B40, main_time_frame_rate_display);

	if (display_frame_deltas)
		rasterizer_render_debug_frame_deltas();
}

void __cdecl main_time_reset()
{
	INVOKE(0x00508110, main_time_reset);
}

void __cdecl main_time_throttle(__int64 a1)
{
	INVOKE(0x00508160, main_time_throttle, a1);
}

real __cdecl main_time_update()
{
	return INVOKE(0x00508170, main_time_update);
}

