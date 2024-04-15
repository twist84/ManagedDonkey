#include "game/game_engine_display.hpp"

#include "cseries/cseries.hpp"
#include "cutscene/cinematics.hpp"
#include "game/player_mapping.hpp"
#include "interface/interface_constants.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "simulation/simulation.hpp"

REFERENCE_DECLARE_ARRAY(0x0471AA0C, byte, unknown_bytes0471AA0C, 4);

HOOK_DECLARE(0x006E5040, game_engine_render_fade_to_black);

// add a command/global
bool g_fade_to_black_enabled = false;

//.text:006E47D0 ; 

void __cdecl game_engine_get_place_string(long place, c_static_wchar_string<256>* place_string)
{
	INVOKE(0x006E48C0, game_engine_get_place_string, place, place_string);
}

void __cdecl game_engine_get_score_string(long score, class c_static_wchar_string<256>* score_string)
{
	INVOKE(0x006E4960, game_engine_get_place_string, score, score_string);
}

void __cdecl game_engine_get_team_name(long team, c_static_wchar_string<256>* team_name)
{
	INVOKE(0x006E49A0, game_engine_get_team_name, team, team_name);
}

//real __cdecl game_engine_get_user_fade_to_black_amount(e_output_user_index user_index)
real __cdecl game_engine_get_user_fade_to_black_amount(long user_index)
{
	//return INVOKE(0x006E49F0, game_engine_get_user_fade_to_black_amount, user_index);

	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	if (user_index == NONE || !simulation_in_progress())
		return 1.0f;

	return game_engine_globals->user_fade_to_black_amounts[user_index];
}

//.text:006E4A40 ; real __cdecl game_engine_hud_get_fade(long)

//.text:006E4AA0 ; bool __cdecl game_engine_hud_get_state_message(long, wchar_t*, long, bool)
bool __cdecl game_engine_hud_get_state_message(long user_index, wchar_t* message_buffer, long message_buffer_length, bool a4)
{
	// #TODO: implement this
	if (INVOKE(0x006E4AA0, game_engine_hud_get_state_message, user_index, message_buffer, message_buffer_length, a4))
		return true;

	TLS_DATA_GET_VALUE_REFERENCE(player_data);
	TLS_DATA_GET_VALUE_REFERENCE(local_game_engine_globals);

	s_player_state_data& player_state = local_game_engine_globals->player_states[user_index];
	e_game_engine_status state_index = player_state.state_index;
	switch (state_index)
	{
	case _game_engine_status_respawning_soon:
	{
		if (!game_engine_in_round())
			return false;

		long player_index = player_mapping_get_player_by_output_user((e_output_user_index)user_index);
		player_datum* player = static_cast<player_datum*>(datum_try_and_get(*player_data, player_index));
		if (!player)
			return false;

		if (player->unit_index != NONE)
			return false;

		if (player->respawn_timer_countdown <= 0)
			return false;

		if (TEST_BIT(player->flags, _player_unknown_bit3)) // affected by `game_engine_get_pre_round_ticks`
			usnzprintf(message_buffer, message_buffer_length, L"Spawn in %d", player->respawn_timer_countdown);
		else
			usnzprintf(message_buffer, message_buffer_length, L"Respawn in %d", player->respawn_timer_countdown);

		return true;
	}
	break;
	}

	return false;
}
HOOK_DECLARE_CALL(0x00A963C6, game_engine_hud_get_state_message);

//.text:006E4D90 ; bool __cdecl chud_should_render_motion_sensor(long)
//.text:006E4DE0 ; void __cdecl game_engine_parse_utf_character(e_utf32, wchar_t*, long)
//.text:006E4E50 ; void __cdecl game_engine_render(e_output_user_index)
//.text:006E4E90 ; void __cdecl game_engine_render_all_multiplayer_object_boundaries()
//.text:006E4FE0 ; void __cdecl game_engine_render_debug(long)

//void __cdecl game_engine_render_fade_to_black(e_output_user_index  user_index)
void __cdecl game_engine_render_fade_to_black(long user_index)
{
	//INVOKE(0x006E5040, game_engine_render_fade_to_black, user_index);

	if (!g_fade_to_black_enabled)
		return;

	if (!VALID_INDEX(user_index, 4))
		return;

	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	if (TEST_BIT(game_engine_globals->user_fade_to_black_flags, user_index))
	{
		long ticks = 0;
		if (ticks = game_seconds_to_ticks_round(3.0f), ++unknown_bytes0471AA0C[user_index], unknown_bytes0471AA0C[user_index] > ticks)
		{
			post_game_engine_globals_message(0, static_cast<char>(user_index), 0);
		}
	}
	else
	{
		unknown_bytes0471AA0C[user_index] = 0;
	}

	//if (director_debug_supress_black_screen(player_mapping_first_active_output_user()))
	//	return;

	real user_fade_to_black_amount = game_engine_get_user_fade_to_black_amount(user_index);
	if (user_fade_to_black_amount <= 0.0f)
		return;

	short_rectangle2d display_pixel_bounds{};
	interface_get_current_display_settings(nullptr, nullptr, &display_pixel_bounds, nullptr);

	real_argb_color fade_color{};
	fade_color.color = *global_real_rgb_black;
	fade_color.alpha = user_fade_to_black_amount;

	short_rectangle2d quad_bounds{};
	set_rectangle2d(&quad_bounds, 0, 0, display_pixel_bounds.x1 - display_pixel_bounds.x0, display_pixel_bounds.y1 - display_pixel_bounds.y0);
	draw_quad(&quad_bounds, real_argb_color_to_pixel32(&fade_color));
}

//.text:006E5160 ; void __cdecl game_engine_render_watermarks()
//.text:006E54D0 ; void __cdecl game_engine_submit_nav_points(long, long)
//.text:006E5510 ; bool __cdecl game_engine_suppress_render_scene(long)

