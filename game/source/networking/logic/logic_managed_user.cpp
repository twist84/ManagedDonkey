#include "networking/logic/logic_managed_user.hpp"

#include "cseries/cseries.hpp"

void __cdecl managed_user_clear(e_controller_index controller_index)
{
	INVOKE(0x004D25F0, managed_user_clear, controller_index);
}

