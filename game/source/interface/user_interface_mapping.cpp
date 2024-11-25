#include "interface/user_interface_mapping.hpp"

#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "interface/c_controller.hpp"

REFERENCE_DECLARE_ARRAY(0x05253D68, e_window_index, controller_game_render_windows, k_number_of_controllers);

long __cdecl user_interface_get_local_user_window_count()
{
	return INVOKE(0x00AB52F0, user_interface_get_local_user_window_count);
}

e_window_index __cdecl user_interface_get_window_for_controller(e_controller_index controller_index)
{
	return INVOKE(0x00AB5350, user_interface_get_window_for_controller, controller_index);

	//long local_user_window_count = user_interface_get_local_user_window_count();
	//e_window_index game_render_window = _console_window;
	//
	//if (!game_is_ui_shell()
	//	&& player_mapping_output_user_active_count() > 0
	//	&& controller_index != k_no_controller
	//	&& controller_index != k_any_controller)
	//{
	//	game_render_window = controller_get_game_render_window(controller_index);
	//	if (game_render_window < _window0 || game_render_window >= local_user_window_count)
	//		game_render_window = _console_window;
	//}
	//
	//return game_render_window;
}

