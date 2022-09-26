#pragma once

extern void __cdecl user_interface_messaging_post(void* message);
extern void* load_screen_message_ctor(void* message, long screen_name, long controller, long window, long layered_position);