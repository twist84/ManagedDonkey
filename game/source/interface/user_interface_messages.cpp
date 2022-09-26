#include "interface/user_interface_messages.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "interface/user_interface_memory.hpp"
#include "tag_files/string_ids.hpp"

#include <windows.h>
#include <assert.h>

long c_message::get_screen_name()
{
	FUNCTION_BEGIN(true);

	return m_screen_name;
}

e_controller_index c_message::get_controller()
{
	FUNCTION_BEGIN(true);

	return m_controller;
}

e_window_index c_message::get_window()
{
	FUNCTION_BEGIN(true);

	return m_window;
}

c_message_globals& g_message_globals = *reinterpret_cast<c_message_globals*>(0x052600D0);

c_message_globals::c_message_globals()
{
	FUNCTION_BEGIN(true);

	memset(m_queue, 0, sizeof(m_queue));
	m_next_read = nullptr;
	m_prev_read = nullptr;
	m_xbox_guide_is_active = false;
	//m_system_message_notification_handle = nullptr;
}

c_message_globals::~c_message_globals()
{
	FUNCTION_BEGIN(true);
}

void c_message_globals::initialize()
{
	FUNCTION_BEGIN(true);

	//m_system_message_notification_handle = XNotifyCreateListener(0x6F);
	//XCustomRegisterDynamicActions();
}

void c_message_globals::dispose()
{
	FUNCTION_BEGIN(true);

	//XCustomUnregisterDynamicActions();
	//if (m_system_message_notification_handle && m_system_message_notification_handle != INVALID_HANDLE_VALUE)
	//{
	//	CloseHandle(m_system_message_notification_handle);
	//	m_system_message_notification_handle = nullptr;
	//}
}

void c_message_globals::initialize_for_new_map()
{
	FUNCTION_BEGIN(true);

	for (long i = 0; i < NUMBEROF(m_queue); ++i)
	{
		s_message_queue_node* next = nullptr;
		if (i == NUMBEROF(m_queue) - 1)
			next = m_queue;
		else
			next = &m_queue[i + 1];
		m_queue[i].m_next = next;

		s_message_queue_node* prev = nullptr;
		if (i)
			prev = &m_queue[i - 1];
		else
			prev = &m_queue[NUMBEROF(m_queue) - 1];

		m_queue[i].m_prev = prev;
		m_queue[i].m_message = nullptr;
	}
	this->m_prev_read = m_queue;
	this->m_next_read = m_queue;
}

void c_message_globals::dispose_from_old_map()
{
	FUNCTION_BEGIN(true);

	empty_queue();
}

void c_message_globals::empty_queue()
{
	FUNCTION_BEGIN(true);

	while (can_read())
	{
		c_message* message = dequeue_node(m_next_read, true);

		//ui_track_delete<c_message const>(message);
		message->destructor(0);
		user_interface_free(message);
	}
}

c_message* c_message_globals::dequeue_node(s_message_queue_node* node, bool unknown)
{
	FUNCTION_BEGIN(true);

	assert(&m_queue[0] <= node && node <= &m_queue[NUMBEROF(m_queue) - 1]);
	assert(node->m_message != nullptr);

	c_message* message = node->m_message;

	if (unknown)
	{
		if (node == m_next_read)
			m_next_read = m_next_read->m_next;

		s_message_queue_node* prev = node->m_prev;
		node->m_next->m_prev = prev;
		prev->m_next = node->m_next;
		node->m_message = 0;

		s_message_queue_node* next = m_prev_read->m_next;
		next->m_prev = node;
		m_prev_read->m_next = node;
		node->m_prev = m_prev_read;
		node->m_next = next;
	}

	return nullptr;
}

bool c_message_globals::can_read()
{
	FUNCTION_BEGIN(true);

	return m_next_read->m_message == 0;
}

void c_message_globals::queue(c_message* message)
{
	FUNCTION_BEGIN(true);

	assert(message != nullptr);
	assert(can_write());

	g_message_globals.m_prev_read->m_message = message;
	g_message_globals.m_prev_read = g_message_globals.m_prev_read->m_next;
}

bool c_message_globals::can_write()
{
	FUNCTION_BEGIN(true);

	return m_prev_read->m_message == 0;
}

void c_message_globals::get_next_message(long screen_name, e_controller_index controller, e_window_index window, c_message** message_reference)
{
	FUNCTION_BEGIN(true);

	assert(message_reference != nullptr);

	s_message_queue_node* next_node = 0;
	if (*message_reference)
	{
		for (s_message_queue_node* next = m_next_read; next && next != m_prev_read; next = next->m_next)
		{
			if (next->m_message && next->m_message == *message_reference)
			{
				next_node = next->m_next;
				break;
			}
		}
	}
	else
	{
		assert(m_next_read != nullptr);
		next_node = m_next_read;
	}

	c_message* message = 0;
	while (next_node && next_node != m_prev_read)
	{
		if (next_node->m_message && message_match(next_node->m_message, screen_name, controller, window))
		{
			message = next_node->m_message;
			break;
		}

		next_node = next_node->m_next;
	}

	*message_reference = message;
}

bool c_message_globals::message_match(c_message* message, long screen_name, e_controller_index controller, e_window_index window)
{
	FUNCTION_BEGIN(true);

	bool result = false;
	if ((screen_name == _string_id_invalid || message->get_screen_name() == screen_name)
		&& (controller == k_any_controller || message->get_controller() == controller)
		&& (window == 0xFF || window == -1 || message->get_window() == window))
	{
		result = true;
	}

	return result;
}

void c_message_globals::dequeue(c_message* message)
{
	FUNCTION_BEGIN(true);

	for (s_message_queue_node* node = m_next_read; node && node != m_prev_read; node = node->m_next)
	{
		if (node->m_message && node->m_message == message)
		{
			dequeue_node(node, true);
			break;
		}
	}
}

bool c_message_globals::get_xbox_guide_is_active()
{
	FUNCTION_BEGIN(true);

	return m_xbox_guide_is_active;
}

void c_message_globals::set_xbox_guide_is_active(bool xbox_guide_is_active)
{
	FUNCTION_BEGIN(true);

	m_xbox_guide_is_active = xbox_guide_is_active;
}

void* c_message_globals::get_system_message_notification_handle()
{
	FUNCTION_BEGIN(true);

	//return m_system_message_notification_handle;
	return nullptr;
}

void __cdecl user_interface_messaging_initialize()
{
	FUNCTION_BEGIN(true);

	g_message_globals.initialize();
}

void __cdecl user_interface_messaging_dispose()
{
	FUNCTION_BEGIN(true);

	g_message_globals.dispose();
}

void __cdecl user_interface_messaging_initialize_for_new_map()
{
	FUNCTION_BEGIN(true);

	g_message_globals.initialize_for_new_map();
}

void __cdecl user_interface_messaging_dispose_from_old_map()
{
	FUNCTION_BEGIN(true);

	g_message_globals.dispose_from_old_map();
}

// TODO
void __cdecl user_interface_messaging_update()
{
	FUNCTION_BEGIN(true);
}

void __cdecl user_interface_messaging_post(c_message* message)
{
	FUNCTION_BEGIN(true);

	assert(message != nullptr);

	//INVOKE(0x00A93450, user_interface_messaging_post, message);

	bool message_queued = false;
	if (game_in_progress())
	{
		if (g_message_globals.can_write())
		{
			g_message_globals.queue(message);
			message_queued = true;
		}
		else
		{
			c_console::write_line("ui: failed to post message because message queue is not writeable!");
		}
	}
	else
	{
		c_console::write_line("ui: failed to post message because game is not in progress!");
	}

	if (!message_queued)
	{
		//ui_track_delete<c_message const>(message);
		message->destructor(0);
		user_interface_free(message);
	}
}

bool __cdecl user_interface_messaging_get_next_message(long screen_name, e_controller_index controller, e_window_index window, c_message** message_reference)
{
	FUNCTION_BEGIN(true);

	//return INVOKE(0x00A933D0, user_interface_messaging_get_next_message, screen_name, controller, window, message_reference);

	g_message_globals.get_next_message(screen_name, controller, window, message_reference);
	return message_reference == nullptr;
}

void __cdecl user_interface_messaging_pop(c_message* message)
{
	FUNCTION_BEGIN(true);

	//INVOKE(0x00A93430, user_interface_messaging_pop, message);

	if (game_in_progress())
		g_message_globals.dequeue(message);
}

bool __cdecl user_interface_message_queue_is_empty()
{
	FUNCTION_BEGIN(true);

	if (game_in_progress())
		return !g_message_globals.can_read();

	return true;
}

// c_load_screen_message::c_load_screen_message
c_load_screen_message* load_screen_message_ctor(c_load_screen_message* message, long screen_name, e_controller_index controller, e_window_index window, long layered_position)
{
	FUNCTION_BEGIN(true);

	return DECLFUNC(0x00A92780, c_load_screen_message*, __thiscall, c_load_screen_message*, long, e_controller_index, e_window_index, long)(message, screen_name, controller, window, layered_position);
}
