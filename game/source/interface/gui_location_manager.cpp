#include "interface/gui_location_manager.hpp"

#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"
#include "tag_files/string_ids.hpp"


REFERENCE_DECLARE(0x01944204, c_gui_location_manager, g_location_manager);

HOOK_DECLARE_CLASS_MEMBER(0x00ADF6E0, c_gui_location_manager, change_location);
HOOK_DECLARE_CLASS_MEMBER(0x00ADF870, c_gui_location_manager, get_current_ui_location);
HOOK_DECLARE(0x00ADF9D0, location_manager_get);
HOOK_DECLARE(0x00ADF9E0, location_manager_start);
HOOK_DECLARE(0x00ADF9F0, location_manager_stop);

// 00ADF680
c_gui_location_manager::c_gui_location_manager() :
	m_running(true),
	__unknown4(0),
	m_show_postgame_stats_upon_lobby_entrance(false),
	m_change_time(0xFFFFFFFF),
	m_change_location(30)
{
}

// 00ADF6C0
bool c_gui_location_manager::can_change_location()
{
	return m_change_location >= 30;
}

// 00ADF6E0
void __thiscall c_gui_location_manager::change_location(int32 screen_name)
{
	bool can_change_location_ = can_change_location();

	HOOK_INVOKE_CLASS_MEMBER(, c_gui_location_manager, change_location, screen_name);

	if (can_change_location_)
	{
		if (c_load_screen_message* message = new c_load_screen_message(screen_name, k_any_controller, _console_window, STRING_ID(gui, bottom_most)))
			user_interface_messaging_post(message);
	}
}

// 00ADF870
int32 __thiscall c_gui_location_manager::get_current_ui_location()
{
	int32 result = 0;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_location_manager, get_current_ui_location);
	return result;
}

// 00ADF8A0
c_gui_screen_widget* c_gui_location_manager::get_location_screen()
{
	c_gui_screen_widget* result = INVOKE_CLASS_MEMBER(0x00ADF8A0, c_gui_location_manager, get_location_screen);

	return result;
}

// 00ADF8F0
int32 c_gui_location_manager::get_location_screen_name(int32 gui_location)
{
	int32 result = INVOKE_CLASS_MEMBER(0x00ADF8F0, c_gui_location_manager, get_location_screen_name, gui_location);

	return result;
}

// 00ADF960
int32 c_gui_location_manager::get_pregame_lobby_name(int32 gui_game_mode)
{
	int32 result = INVOKE_CLASS_MEMBER(0x00ADF960, c_gui_location_manager, get_pregame_lobby_name, gui_game_mode);

	return result;
}

// 00ADF9D0
c_gui_location_manager* __cdecl location_manager_get()
{
	return &g_location_manager;
}

// 00ADF9E0
void __cdecl location_manager_start()
{
	g_location_manager.m_running = true;
}

// 00ADF9F0
void __cdecl location_manager_stop()
{
	g_location_manager.m_running = false;
}

// 00ADFA00
void c_gui_location_manager::set_running(bool running)
{
	m_running = running;
}

// 00ADFA10
void c_gui_location_manager::update()
{
	INVOKE_CLASS_MEMBER(0x00ADFA10, c_gui_location_manager, update);
}

