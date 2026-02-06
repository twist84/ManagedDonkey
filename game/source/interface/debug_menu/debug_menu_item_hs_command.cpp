#include "interface/debug_menu/debug_menu_item_hs_command.hpp"

#include "cseries/cseries_events.hpp"
#include "hs/hs_compile.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "main/console.hpp"

#include <string.h>

void c_debug_menu_item_hs_command::notify_selected()
{
	if (m_command)
	{
		console_process_command(m_command, true);
		hs_compile_and_evaluate(_event_message, "debug_menu", m_command, true);
	}
}

const real_argb_color* c_debug_menu_item_hs_command::get_enabled_color()
{
	return debug_real_argb_tv_orange;
}

c_debug_menu_item_hs_command::c_debug_menu_item_hs_command(c_debug_menu* menu, const char* name, const char* command) :
	c_debug_menu_item_numbered(menu, name, nullptr)
{
	ASSERT(name != nullptr && command != nullptr && menu != nullptr);

	int32 command_length = strlen_debug(command) + 1;
	ASSERT(command_length > 0);

	m_command = static_cast<char*>(debug_menu_malloc(command_length));
	if (m_command)
		csstrnzcpy(m_command, command, command_length);
}

