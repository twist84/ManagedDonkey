#include "interface/user_interface_window_manager.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

void* c_window_manager::this_ptr = reinterpret_cast<void*>(0x05260F34);

void __cdecl window_manager_reset_screens()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00AADAC0, window_manager_reset_screens);
}