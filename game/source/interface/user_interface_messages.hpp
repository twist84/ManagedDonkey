#pragma once

#include "cseries/cseries.hpp"

struct c_message
{
	struct c_message_vtbl* __vftable /*VFT*/;
	dword m_time;
	long m_message_type;
	long m_message_id;
	long m_controller_index;
	long m_window_index;
};
static_assert(sizeof(c_message) == 0x18);

struct c_load_screen_message : c_message
{
	dword __unknown18;
	bool __unknown1C;
	dword __unknown20;
	dword __unknown24;
	dword __unknown28;
	dword __unknown2C;
	dword __unknown30;
	long m_layered_position;
	bool __unknown38;
};
static_assert(sizeof(c_load_screen_message) == 0x3C);

struct c_message_globals
{
	struct s_message_entry
	{
		dword __unknown0;
		dword __unknown4;
		dword __unknown8;
	};
	
	s_message_entry m_queue[32];
	s_message_entry* m_queue_entry0;
	s_message_entry* m_queue_entry1;

	bool m_xbox_guide_is_active;
};
static_assert(sizeof(c_message_globals) == 0x18C);

extern c_message_globals& g_message_globals;

extern void __cdecl user_interface_messaging_pop(c_message* message);
extern void __cdecl user_interface_messaging_post(c_message* message);

extern c_load_screen_message* load_screen_message_ctor(c_load_screen_message* message, long message_id, long controller, long window, long layered_position);

extern void user_interface_messaging_pop_load_screen(c_load_screen_message* message);
extern c_load_screen_message* user_interface_messaging_post_load_screen(long message_id, long controller, long window, long layered_position);