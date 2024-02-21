#include "interface/gui_screens/boot_betrayer/gui_screen_boot_betrayer.hpp"

// c_load_boot_betrayer_screen_message::c_load_boot_betrayer_screen_message
c_load_boot_betrayer_screen_message* load_boot_betrayer_screen_message_ctor(c_load_boot_betrayer_screen_message* message, e_controller_index controller, e_window_index window, long layered_position, s_player_identifier const* current_player_id, s_player_identifier const* target_player_id)
{
	return DECLFUNC(0x00AB5450, c_load_boot_betrayer_screen_message*, __thiscall, c_load_boot_betrayer_screen_message*, e_controller_index, e_window_index, long, s_player_identifier const*, s_player_identifier const*)(message, controller, window, layered_position, current_player_id, target_player_id);
}

