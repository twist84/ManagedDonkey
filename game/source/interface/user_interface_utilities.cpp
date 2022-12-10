#include "interface/user_interface_utilities.hpp"

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"

void tint_widget_to_change_color(c_gui_widget* widget, long color_list_index, bool team_game)
{
	INVOKE(0x00AA4C80, tint_widget_to_change_color, widget, color_list_index, team_game);
}

