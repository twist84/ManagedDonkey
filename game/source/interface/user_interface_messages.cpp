#include "interface/user_interface_messages.hpp"

#include "cseries/cseries.hpp"
#include "interface/user_interface_memory.hpp"

long c_message::get_screen_name()
{
	return m_screen_name;
}

e_controller_index c_message::get_controller()
{
	return m_controller;
}

e_window_index c_message::get_window()
{
	return m_window;
}

c_message_globals& g_message_globals = *reinterpret_cast<c_message_globals*>(0x052600D0);

void __cdecl user_interface_messaging_pop(c_message* message)
{
	INVOKE(0x00A93430, user_interface_messaging_pop, message);
}

void __cdecl user_interface_messaging_post(c_message* message)
{
	INVOKE(0x00A93450, user_interface_messaging_post, message);
}

// c_load_screen_message::c_load_screen_message
c_load_screen_message* load_screen_message_ctor(c_load_screen_message* message, long message_id, long controller, long window, long layered_position)
{
	return DECLFUNC(0x00A92780, c_load_screen_message*, __thiscall, c_load_screen_message*, long, long, long, long)(message, message_id, controller, window, layered_position);
}

void user_interface_messaging_pop_load_screen(c_load_screen_message* message)
{
	if (message)
	{
		user_interface_messaging_pop(message);
		user_interface_free(message);
	}
}

c_load_screen_message* user_interface_messaging_post_load_screen(long message_id, long controller, long window, long layered_position)
{
	c_load_screen_message* message = (c_load_screen_message*)user_interface_malloc_tracked(sizeof(c_load_screen_message), __FILE__, __LINE__);
	if (load_screen_message_ctor(message, message_id, controller, window, layered_position))
		user_interface_messaging_post(message);

	return message;
}