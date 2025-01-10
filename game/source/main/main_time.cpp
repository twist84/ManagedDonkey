#include "main/main_time.hpp"

#include "game/game.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_globals.hpp"
#include "text/draw_string.hpp"

REFERENCE_DECLARE(0x022B47FC, bool, display_framerate);

bool debug_disable_frame_rate_throttle = false;
bool display_frame_deltas = false;

void __cdecl __tls_set_g_main_time_globals_allocator(void* address)
{
	//INVOKE(0x00507A30, __tls_set_g_main_time_globals_allocator, address);

	TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	g_main_time_globals = (s_main_time_globals*)address;
}

//.text:00507A50 ; 
//.text:00507AA0 ; 
//.text:00507AD0 ; 
//.text:00507B00 ; 
//.text:00507B20 ; double __cdecl main_time_compute_frame_rate_average()

void __cdecl main_time_frame_rate_debug()
{
	INVOKE(0x00507B30, main_time_frame_rate_debug);
}

void rasterizer_render_debug_frame_deltas()
{
	c_rasterizer_draw_string draw_string;
	c_font_cache_mt_safe font_cache;

	rectangle2d bounds{};
	c_rasterizer::get_fullscreen_render_pixel_bounds(&bounds);

	bounds.x0 = short(bounds.x1 - (50.0f * draw_string_get_glyph_scaling_for_display_settings()));
	bounds.y0 = short(bounds.y1 - (75.0f * draw_string_get_glyph_scaling_for_display_settings()));

	for (long i = (rasterizer_globals.frame_delta_index + 14) % 15; i != rasterizer_globals.frame_delta_index; i = (i + 14) % 15)
	{
		bounds.y0 -= 20;
		bounds.y1 -= 20;

		char str[8]{};
		csnzprintf(str, 4, "%d", rasterizer_globals.frame_deltas[i]);

		real_argb_color const* color = global_real_argb_green;
		if (rasterizer_globals.frame_deltas[i] > 2)
			color = global_real_argb_red;

		draw_string.set_justification(_text_justification_right);
		draw_string.set_color(color);
		draw_string.set_bounds(&bounds);
		draw_string.draw(&font_cache, str);
	}
}

void __cdecl main_time_frame_rate_display()
{
	INVOKE(0x00507B40, main_time_frame_rate_display);

	if (display_frame_deltas)
		rasterizer_render_debug_frame_deltas();
}

//.text:00507D70 ; __int64 sub_507D70() // return g_main_time_globals->last_milliseconds;

__int64 __cdecl main_time_get_absolute_milliseconds()
{
	return INVOKE(0x00507D90, main_time_get_absolute_milliseconds);
}

//.text:00507DD0
//.text:00507DE0

__int64 __cdecl main_time_get_input_collection_time()
{
	return INVOKE(0x00507DF0, main_time_get_input_collection_time);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//return g_main_time_globals->last_input_timestamp;
}

long __cdecl main_time_get_native_tick_rate()
{
	return INVOKE(0x00507E10, main_time_get_native_tick_rate);
}

__int64 __cdecl main_time_get_publishing_end_time()
{
	return INVOKE(0x00507E40, main_time_get_publishing_end_time);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//return g_main_time_globals->last_publish_end_timestamp;
}

__int64 __cdecl main_time_get_publishing_start_time()
{
	return INVOKE(0x00507E60, main_time_get_publishing_start_time);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//return g_main_time_globals->last_publish_start_timestamp;
}

__int64 __cdecl main_time_get_target_display_vblank_index()
{
	return INVOKE(0x00507E80, main_time_get_target_display_vblank_index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//return g_main_time_globals->target_display_vblank_index;
}

long __cdecl main_time_get_time_since_input_collection(__int64 timestamp)
{
	return INVOKE(0x00507EA0, main_time_get_time_since_input_collection, timestamp);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//return timestamp - g_main_time_globals->last_input_timestamp;
}

long __cdecl main_time_get_vblank_rate()
{
	return INVOKE(0x00507EC0, main_time_get_vblank_rate);
}

void __cdecl main_time_globals_post_copy_update(void* new_address)
{
	INVOKE(0x00507EF0, main_time_globals_post_copy_update, new_address);

	//((s_main_time_globals*)new_address)->last_publish_end_timestamp = system_milliseconds();
}

void __cdecl main_time_initialize()
{
	INVOKE(0x00507F10, main_time_initialize);
}

bool __cdecl main_time_is_throttled()
{
	return INVOKE(0x00507FF0, main_time_is_throttled);

	//if (!game_in_progress() || debug_disable_frame_rate_throttle)
	//	return false;
	//
	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//return !g_main_time_globals->temporary_throttle_control || g_main_time_globals->temporary_throttle;
}

void __cdecl main_time_mark_input_collection_time()
{
	INVOKE(0x00508030, main_time_mark_input_collection_time);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//g_main_time_globals->last_input_timestamp = main_time_get_absolute_milliseconds();
}

void __cdecl main_time_mark_publishing_end_time()
{
	INVOKE(0x00508070, main_time_mark_publishing_end_time);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//g_main_time_globals->last_publish_end_timestamp = main_time_get_absolute_milliseconds();
}

void __cdecl main_time_mark_publishing_start_time()
{
	INVOKE(0x005080B0, main_time_mark_publishing_start_time);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//g_main_time_globals->last_publish_start_timestamp = main_time_get_absolute_milliseconds();
}

void __cdecl main_time_remove_temporary_throttle()
{
	INVOKE(0x005080F0, main_time_remove_temporary_throttle);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//g_main_time_globals->temporary_throttle_control = false;
}

void __cdecl main_time_reset()
{
	INVOKE(0x00508110, main_time_reset);

	//LARGE_INTEGER vblank_snapshot;
	//QueryPerformanceCounter(&vblank_snapshot);
	//
	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//g_main_time_globals->last_vblank_index = vblank_snapshot.QuadPart;
	//g_main_time_globals->last_initial_vblank_index = vblank_snapshot.QuadPart;
	//g_main_time_globals->target_display_vblank_index = vblank_snapshot.QuadPart;
	//g_main_time_globals->last_input_timestamp = main_time_get_absolute_milliseconds();
}

void __cdecl main_time_restore(long game_state_flags)
{
	INVOKE(0x00508120, main_time_restore, game_state_flags);

	//main_time_reset();
}

void __cdecl main_time_set_temporary_throttle(bool throttle)
{
	INVOKE(0x00508130, main_time_set_temporary_throttle, throttle);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//g_main_time_globals->temporary_throttle_control = true;
	//g_main_time_globals->temporary_throttle = throttle;
}

void __cdecl main_time_throttle(__int64 target_display_vblank_index)
{
	INVOKE(0x00508160, main_time_throttle, target_display_vblank_index);
}

real __cdecl main_time_update()
{
	return INVOKE(0x00508170, main_time_update);
}

