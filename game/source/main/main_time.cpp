#include "main/main_time.hpp"

#include "game/game.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_globals.hpp"
#include "text/draw_string.hpp"

//#include <windows.h>

REFERENCE_DECLARE(0x022B47FC, bool, display_framerate) = false;

HOOK_DECLARE(0x00507FF0, main_time_is_throttled);
HOOK_DECLARE(0x00508160, main_time_throttle);

bool debug_disable_frame_rate_throttle = false;
bool debug_frame_rate_based_on_system_time = false;
bool debug_frame_rate_stabilization = false;
bool display_frame_deltas = false;

void __cdecl __tls_set_g_main_time_globals_allocator(void* address)
{
	//INVOKE(0x00507A30, __tls_set_g_main_time_globals_allocator, address);

	g_main_time_globals = (s_main_time_globals*)address;
}

//.text:00507A50 ; 
//.text:00507AA0 ; 
//.text:00507AD0 ; 
//.text:00507B00 ; 
//.text:00507B20 ; real64 __cdecl main_time_compute_frame_rate_average()

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

	bounds.x0 = int16(bounds.x1 - (50.0f * draw_string_get_glyph_scaling_for_display_settings()));
	bounds.y0 = int16(bounds.y1 - (75.0f * draw_string_get_glyph_scaling_for_display_settings()));

	for (int32 flip_delta_index = (rasterizer_globals.next_flip_delta_index + 14) % 15;
		flip_delta_index != rasterizer_globals.next_flip_delta_index;
		flip_delta_index = (flip_delta_index + 14) % 15)
	{
		bounds.y0 -= 20;
		bounds.y1 -= 20;

		char str[8]{};
		csnzprintf(str, 4, "%d", rasterizer_globals.flip_deltas[flip_delta_index]);

		const real_argb_color* color = global_real_argb_green;
		if (rasterizer_globals.flip_deltas[flip_delta_index] > 2)
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

//.text:00507D70 ; int64 sub_507D70() // return g_main_time_globals->last_milliseconds;

int64 __cdecl main_time_get_absolute_milliseconds()
{
	return INVOKE(0x00507D90, main_time_get_absolute_milliseconds);
}

//.text:00507DD0
//.text:00507DE0

int64 __cdecl main_time_get_input_collection_time()
{
	return INVOKE(0x00507DF0, main_time_get_input_collection_time);

	//return g_main_time_globals->last_input_timestamp;
}

int32 __cdecl main_time_get_native_tick_rate()
{
	return INVOKE(0x00507E10, main_time_get_native_tick_rate);
}

int64 __cdecl main_time_get_publishing_end_time()
{
	return INVOKE(0x00507E40, main_time_get_publishing_end_time);

	//return g_main_time_globals->last_publish_end_timestamp;
}

int64 __cdecl main_time_get_publishing_start_time()
{
	return INVOKE(0x00507E60, main_time_get_publishing_start_time);

	//return g_main_time_globals->last_publish_start_timestamp;
}

int64 __cdecl main_time_get_target_display_vblank_index()
{
	return INVOKE(0x00507E80, main_time_get_target_display_vblank_index);

	//return g_main_time_globals->target_display_vblank_index;
}

int32 __cdecl main_time_get_time_since_input_collection(int64 timestamp)
{
	return INVOKE(0x00507EA0, main_time_get_time_since_input_collection, timestamp);

	//return timestamp - g_main_time_globals->last_input_timestamp;
}

int32 __cdecl main_time_get_vblank_rate()
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

	//g_main_time_globals = (s_main_time_globals*)g_main_time_globals_allocator.allocate(sizeof(s_main_time_globals), "main_timing");
	//
	//g_main_time_globals->last_milliseconds = system_milliseconds();
	//g_main_time_globals->last_vblank_index = 0LL;
	//QueryPerformanceCounter((LARGE_INTEGER*)&g_main_time_globals->last_vblank_index);
	//g_main_time_globals->last_initial_vblank_index = 0LL;
	//g_main_time_globals->target_display_vblank_index = 0LL;
	//g_main_time_globals->last_input_timestamp = 0LL;
	//
	//rasterizer_globals_initialize();
	//
	//g_frame_rate_data.starting_film_tick = saved_film_manager_get_current_tick_estimate();
	//g_frame_rate_data.min = 0;
	//g_frame_rate_data.max = 0;
	//g_frame_rate_data.measurement_sum = 0;
	//g_frame_rate_data.measurement_count = 0;
}

bool __cdecl main_time_is_throttled()
{
	//return INVOKE(0x00507FF0, main_time_is_throttled);

	bool throttle = true;
	if (!game_in_progress() || debug_disable_frame_rate_throttle)
	{
		throttle = false;
	}
	else
	{
		throttle = !g_main_time_globals->temporary_throttle_control || g_main_time_globals->temporary_throttle;
	}
	return throttle;
}

void __cdecl main_time_mark_input_collection_time()
{
	INVOKE(0x00508030, main_time_mark_input_collection_time);

	//g_main_time_globals->last_input_timestamp = main_time_get_absolute_milliseconds();
}

void __cdecl main_time_mark_publishing_end_time()
{
	INVOKE(0x00508070, main_time_mark_publishing_end_time);

	//g_main_time_globals->last_publish_end_timestamp = main_time_get_absolute_milliseconds();
}

void __cdecl main_time_mark_publishing_start_time()
{
	INVOKE(0x005080B0, main_time_mark_publishing_start_time);

	//g_main_time_globals->last_publish_start_timestamp = main_time_get_absolute_milliseconds();
}

void __cdecl main_time_remove_temporary_throttle()
{
	INVOKE(0x005080F0, main_time_remove_temporary_throttle);

	//g_main_time_globals->temporary_throttle_control = false;
}

void __cdecl main_time_reset()
{
	INVOKE(0x00508110, main_time_reset);

	//LARGE_INTEGER vblank_snapshot;
	//QueryPerformanceCounter(&vblank_snapshot);
	//
	//g_main_time_globals->last_vblank_index = vblank_snapshot.QuadPart;
	//g_main_time_globals->last_initial_vblank_index = vblank_snapshot.QuadPart;
	//g_main_time_globals->target_display_vblank_index = vblank_snapshot.QuadPart;
	//g_main_time_globals->last_input_timestamp = main_time_get_absolute_milliseconds();
}

void __cdecl main_time_restore(int32 game_state_flags)
{
	INVOKE(0x00508120, main_time_restore, game_state_flags);

	//main_time_reset();
}

void __cdecl main_time_set_temporary_throttle(bool throttle)
{
	INVOKE(0x00508130, main_time_set_temporary_throttle, throttle);

	//g_main_time_globals->temporary_throttle_control = true;
	//g_main_time_globals->temporary_throttle = throttle;
}

void __cdecl main_time_throttle(int64 target_display_vblank_index)
{
	//INVOKE(0x00508160, main_time_throttle, target_display_vblank_index);

	//if (!debug_frame_rate_based_on_system_time)
	//{
	//	int64 target_update_vblank = 0;
	//	if (target_display_vblank_index)
	//		target_update_vblank = target_display_vblank_index - 2;
	//
	//	g_main_time_throttle_debug.target_update_vblank = target_update_vblank;
	//	g_main_time_throttle_debug.initial_vblank_index = rasterizer_get_vblank_index();
	//	g_main_time_throttle_debug.initial_swap_index = rasterizer_get_most_recent_swap_index();
	//	g_main_time_throttle_debug.previous_swap_index = g_previous_swap_index;
	//
	//	if (main_time_is_throttled())
	//	{
	//		volatile int64 current_vblank_index = rasterizer_get_vblank_index();
	//		for (uns32 previous_swap_index = g_previous_swap_index; g_previous_swap_index; previous_swap_index = g_previous_swap_index)
	//		{
	//			if (current_vblank_index != previous_swap_index)
	//				break;
	//			internal_event_wait(k_event_main_time_throttle_vblank);
	//			current_vblank_index = rasterizer_get_vblank_index();
	//		}
	//		g_previous_swap_index = rasterizer_get_most_recent_swap_index();
	//		if (target_update_vblank > 0 &&
	//			target_update_vblank - current_vblank_index <= 8 &&
	//			current_vblank_index < target_update_vblank)
	//		{
	//			do
	//			{
	//				internal_event_wait(k_event_main_time_throttle_vblank);
	//			} while (rasterizer_get_vblank_index() < target_update_vblank);
	//		}
	//	}
	//	g_main_time_throttle_debug.final_vblank_index = rasterizer_get_vblank_index();
	//	uns32 most_recent_swap_index = rasterizer_get_most_recent_swap_index();
	//	g_main_time_throttle_debug.count++;
	//	g_main_time_throttle_debug.final_swap_index = most_recent_swap_index;
	//	int64 time_ms = system_milliseconds();
	//	int64 time_delta_ms = time_ms - g_main_time_throttle_debug.time_ms;
	//	g_main_time_throttle_debug.time_ms = time_ms;
	//	g_main_time_throttle_debug.time_delta_ms = time_delta_ms;
	//	rasterizer_lag_timing_mark_render_present(target_display_vblank_index);
	//}
	//
	//main_time_update_framerate_datamining();
}

real32 __cdecl main_time_update()
{
	return INVOKE(0x00508170, main_time_update);
}

void __cdecl main_time_update_framerate_datamining()
{

}

__declspec(naked) void main_time_is_throttled_inline()
{
	// original instructions
	//      call    game_in_progress
	//      test    al, al
	//      jz      short loc_508287

	// all this for the following
	//if (main_time_is_throttled())

	ASM_ADDR(0x00508252, addr_508252);
	ASM_ADDR(0x00508287, addr_508287);

	__asm
	{
		// execute the original instructions
		call    game_in_progress
		test    al, al
		jz      short loc_508287

		// execute our instructions
		cmp     debug_disable_frame_rate_throttle, 0
		jnz     short loc_508287

		// jump out to after our hook
		jmp     addr_508252

	loc_508287:
		jmp     addr_508287
	}
}
HOOK_DECLARE(0x00508249, main_time_is_throttled_inline);

