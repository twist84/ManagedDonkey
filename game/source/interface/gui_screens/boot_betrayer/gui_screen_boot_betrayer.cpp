#include "interface/gui_screens/boot_betrayer/gui_screen_boot_betrayer.hpp"

#include "interface/user_interface_mapping.hpp"
#include "tag_files/string_ids.hpp"

c_load_boot_betrayer_screen_message::c_load_boot_betrayer_screen_message(e_controller_index controller, e_window_index window, long layered_position, s_player_identifier const* current_player_id, s_player_identifier const* target_player_id) :
	c_load_screen_message(STRING_ID(gui, boot_betrayer) + (user_interface_get_local_user_window_count() > 1), controller, window, layered_position),
	m_current_player_id(*current_player_id),
	m_target_player_id(*target_player_id)
{
}

c_load_boot_betrayer_screen_message::~c_load_boot_betrayer_screen_message()
{
}

