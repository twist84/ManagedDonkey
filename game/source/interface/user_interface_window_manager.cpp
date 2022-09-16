#include "interface/user_interface_window_manager.hpp"

#include "cseries/cseries.hpp"

void __cdecl window_manager_reset_screens()
{
	INVOKE(0x00AADAC0, window_manager_reset_screens);
}