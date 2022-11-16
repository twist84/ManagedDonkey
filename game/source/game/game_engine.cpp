#include "game/game_engine.hpp"

#include "cseries/cseries.hpp"
#include "cseries/console.hpp"
#include "game/game.hpp"
#include "game/game_engine_util.hpp"
#include "game/game_time.hpp"
#include "input/input_abstraction.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/user_interface.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"

HOOK_DECLARE(0x00551780, game_engine_interface_update);
//HOOK_DECLARE(0x005521D0, game_engine_get_pre_round_ticks);

long __cdecl game_engine_get_player_place(long absolute_player_index)
{
	return INVOKE(0x005506F0, game_engine_get_player_place, absolute_player_index);
}

long __cdecl game_engine_get_player_score(long absolute_player_index)
{
	return INVOKE(0x00550720, game_engine_get_player_score, absolute_player_index);
}

long __cdecl game_engine_get_player_score_for_display(long absolute_player_index, bool final_score)
{
	return INVOKE(0x005507D0, game_engine_get_player_score_for_display, absolute_player_index, final_score);
}

long __cdecl game_engine_get_team_place(long team)
{
	return INVOKE(0x00550BF0, game_engine_get_team_place, team);
}

long __cdecl game_engine_get_team_score_for_display(long team, bool final_score)
{
	return INVOKE(0x00550D10, game_engine_get_team_score_for_display, team, final_score);
}

bool __cdecl game_engine_in_round()
{
	return INVOKE(0x00550F90, game_engine_in_round);
}

// this isn't the correct implementation
// skipping standard player iteration and boot betrayer screen
void __cdecl game_engine_interface_update(float world_seconds_elapsed)
{
	if (game_in_progress() && !game_is_ui_shell())
	{
		for (long i = 0; i < k_number_of_controllers; i++)
		{
			e_controller_index controller_index = static_cast<e_controller_index>(i);

			s_game_input_state* input_state;
			input_abstraction_get_input_state(controller_index, &input_state);
			byte down_frames = input_state->abstract_buttons[_gamepad_button_back].down_frames();

			s_local_game_engine_globals* local_game_engine_globals = get_tls()->local_game_engine_globals;
			if (!current_game_engine() || game_engine_in_round())
			{
				local_game_engine_globals->__time0 = 0x7FFFFFFF;
			}
			else if (local_game_engine_globals->__time0 == 0x7FFFFFFF)
			{
				local_game_engine_globals->__time0 = game_time_get() + game_seconds_integer_to_ticks(1);
			}

			if (user_interface_should_show_console_scoreboard(0) || down_frames != 0)
				c_gui_screen_scoreboard::show_scoreboard(controller_index, true);
			else
				c_gui_screen_scoreboard::hide_scoreboard(controller_index);

			c_gui_screen_scoreboard::update_scoreboard_alpha(controller_index);
		}
	}

	DECLFUNC(0x00552D40, void, __cdecl, real)(world_seconds_elapsed);
}

long __cdecl game_engine_get_pre_round_ticks()
{
	FUNCTION_BEGIN(false);

	return game_seconds_integer_to_ticks(8);
}

