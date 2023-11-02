#include "interface/user_interface_mapping.hpp"

#include "cseries/cseries.hpp"

e_window_index __cdecl user_interface_get_window_for_controller(e_controller_index controller_index)
{
	return INVOKE(0x00AB5350, user_interface_get_window_for_controller, controller_index);
}

