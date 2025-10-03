#include "game/game_engine_display.hpp"

#include "cseries/cseries.hpp"
#include "cutscene/cinematics.hpp"
#include "game/player_mapping.hpp"
#include "interface/interface_constants.hpp"
#include "main/main_render.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "simulation/simulation.hpp"

REFERENCE_DECLARE(0x0471AA0C, s_game_engine_render_globals, g_game_engine_render_globals);

HOOK_DECLARE(0x006E4E50, game_engine_render);
HOOK_DECLARE(0x006E5040, game_engine_render_fade_to_black);
HOOK_DECLARE(0x006E5160, game_engine_render_watermarks);

// game_engine_hud_get_state_message
// skip over `respawning_soon`, and `game_over_{ won, tied, lost, you_lost_but_game_tied }` checks
byte const game_engine_status_skip_patch_bytes[]
{
	//.text:006E4CF4                 jmp     short loc_6E4D0D
	//.text:006E4CF6                 nop

	0xEB, 0x17,
	0x90
};
DATA_PATCH_DECLARE(0x006E4CF4, game_engine_status_skip, game_engine_status_skip_patch_bytes);

// add a command/global
bool g_fade_to_black_enabled = false;

//.text:006E47D0 ; 

void __cdecl game_engine_get_place_string(int32 place, c_static_wchar_string<256>* place_string)
{
	INVOKE(0x006E48C0, game_engine_get_place_string, place, place_string);
}

void __cdecl game_engine_get_score_string(int32 score, class c_static_wchar_string<256>* score_string)
{
	INVOKE(0x006E4960, game_engine_get_place_string, score, score_string);
}

void __cdecl game_engine_get_team_name(int32 team, c_static_wchar_string<256>* team_name)
{
	INVOKE(0x006E49A0, game_engine_get_team_name, team, team_name);
}

//real32 __cdecl game_engine_get_user_fade_to_black_amount(int32 user_index)
real32 __cdecl game_engine_get_user_fade_to_black_amount(int32 user_index)
{
	//return INVOKE(0x006E49F0, game_engine_get_user_fade_to_black_amount, user_index);

	if (user_index == NONE || !simulation_in_progress())
		return 1.0f;

	return game_engine_globals->fade_to_black_amount[user_index];
}

real32 __cdecl game_engine_hud_get_fade(int32 user_index)
{
	return INVOKE(0x006E4A40, game_engine_hud_get_fade, user_index);

	//if (!game_in_progress() || game_is_ui_shell())
	//	return 1.0f;
	//
	//e_controller_index controller_index = user_interface_get_controller_for_local_user(user_index);
	//if (controller_index == k_no_controller)
	//	return 1.0f;
	//
	//real32 scoreboard_alpha = c_gui_screen_scoreboard::get_scoreboard_alpha(controller_index);
	//return 1.0f - scoreboard_alpha;
}

bool __cdecl game_engine_hud_get_state_message(int32 user_index, wchar_t* message_buffer, int32 message_buffer_length, bool a4)
{
	return INVOKE(0x006E4AA0, game_engine_hud_get_state_message, user_index, message_buffer, message_buffer_length, a4);
}

//.text:006E4D90 ; bool __cdecl chud_should_render_motion_sensor(int32)
//.text:006E4DE0 ; void __cdecl game_engine_parse_utf_character(e_utf32, wchar_t*, int32)

void __cdecl game_engine_render(int32 user_index)
{
	//INVOKE(0x006E4E50, game_engine_render, user_index);

	HOOK_INVOKE(, game_engine_render, user_index);
}

//.text:006E4E90 ; void __cdecl game_engine_render_all_multiplayer_object_boundaries()

void __cdecl game_engine_render_debug(int32 user_index)
{
	INVOKE(0x006E4FE0, game_engine_render_debug, user_index);
}

void __cdecl game_engine_render_fade_to_black(int32 user_index)
{
	//INVOKE(0x006E5040, game_engine_render_fade_to_black, user_index);

	if (!g_fade_to_black_enabled)
		return;

	if (!VALID_INDEX(user_index, k_number_of_users))
		return;

	if (TEST_BIT(game_engine_globals->fade_to_black_cache_latch, user_index))
	{
		int32 ticks = 0;
		if (ticks = game_seconds_to_ticks_round(3.0f), ++g_game_engine_render_globals.fade_to_black_cache_latch_faliure_count[user_index], g_game_engine_render_globals.fade_to_black_cache_latch_faliure_count[user_index] > ticks)
		{
			post_game_engine_globals_message(0, static_cast<int8>(user_index), 0);
		}
	}
	else
	{
		g_game_engine_render_globals.fade_to_black_cache_latch_faliure_count[user_index] = 0;
	}

	//if (director_debug_supress_black_screen(player_mapping_first_active_output_user()))
	//	return;

	real32 user_fade_to_black_amount = game_engine_get_user_fade_to_black_amount(user_index);
	if (user_fade_to_black_amount <= 0.0f)
		return;

	rectangle2d display_pixel_bounds{};
	interface_get_current_display_settings(nullptr, nullptr, &display_pixel_bounds, nullptr);

	real_argb_color fade_color{};
	fade_color.rgb = *global_real_rgb_black;
	fade_color.alpha = user_fade_to_black_amount;

	rectangle2d quad_bounds{};
	set_rectangle2d(&quad_bounds, 0, 0, rectangle2d_width(&display_pixel_bounds), rectangle2d_height(&display_pixel_bounds));
	draw_quad(&quad_bounds, real_argb_color_to_pixel32(&fade_color));
}

void __cdecl game_engine_render_watermarks()
{
	//INVOKE(0x006E5160, game_engine_render_watermarks);

	game_engine_render_frame_watermarks(false);
}

//.text:006E54D0 ; void __cdecl game_engine_submit_nav_points(int32, int32)

bool __cdecl game_engine_suppress_render_scene(int32 user_index)
{
	return INVOKE(0x006E5510, game_engine_suppress_render_scene, user_index);
}

