#include "main/main_game.hpp"

#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "networking/logic/network_life_cycle.hpp"

#include <windows.h>

REFERENCE_DECLARE(0x023916D8, s_main_game_globals, main_game_globals);
REFERENCE_DECLARE(0x023DAE90, bool, debug_load_panic_to_main_menu);

// void main_game_initialize();
// void main_game_launch_initialize();
// void main_game_launch_default();
// void main_game_launch_default_editor();
// void main_game_reset_map(bool reset_map);
// bool main_game_reset_in_progress();
// bool main_game_change_in_progress();

void main_game_change(game_options const* options)
{
	// main_halt_and_display_errors
	// c_life_cycle_state_handler_in_game::begin_load_map
	if (options)
	{
		assert_game_options_verify(options);
		memcpy(&main_game_globals.game_loaded_options, options, sizeof(game_options));
	}
	main_game_globals.change_in_progress = 1;
	main_game_globals.game_load_pending = options == nullptr;
	main_game_globals.game_loaded_time = system_milliseconds();
	if (!options)
	{
		if (network_life_cycle_get_state() != _life_cycle_state_none)
			network_life_cycle_request_leave(false);
		else
			network_life_cycle_end();
	}
}

// void main_game_change_abort();
// void main_game_change_update();
// bool main_game_change_immediate(game_options const* options);
// void main_game_goto_next_level();
// bool main_game_load_map(game_options const* options);
// void main_game_unload_and_prepare_for_next_game(game_options const* options);
// bool main_game_loaded_pregame();

bool main_game_loaded_map()
{
	return main_game_globals.game_loaded_status == _game_loaded_status_map_loaded;
}

// char const* main_game_loaded_map_name();
// void main_game_load_panic();
// void main_game_load_from_core_name(char const* core_name);
// void main_game_load_from_core();

void __cdecl main_menu_launch()
{
	INVOKE(0x005680E0, main_menu_launch);
}

void __cdecl main_menu_launch_force()
{
	INVOKE(0x00568190, main_menu_launch_force);
}

void main_game_notify_language_change(e_language language)
{
	INVOKE(0x00567BF0, main_game_notify_language_change, language);
}