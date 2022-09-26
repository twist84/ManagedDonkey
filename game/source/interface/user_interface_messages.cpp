#include "interface/user_interface_messages.hpp"

#include "cseries/cseries.hpp"

void __cdecl user_interface_messaging_post(void* message)
{
	INVOKE(0x00A93450, user_interface_messaging_post, message);
}

// c_load_screen_message::c_load_screen_message
void* load_screen_message_ctor(void* message, long screen_name, long controller, long window, long layered_position)
{
	return DECLFUNC(0x00A92780, void*, __thiscall, void*, long, long, long, long)(message, screen_name, controller, window, layered_position);
}