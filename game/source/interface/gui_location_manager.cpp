#include "interface/gui_location_manager.hpp"

#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"
#include "shell/shell.hpp"
#include "tag_files/string_ids.hpp"

c_hook_call change_location_hook(0x00ADFB73, { .pointer = c_gui_location_manager::change_location });
c_hook_call get_current_ui_location_hook(0x00ADFA61, { .pointer = c_gui_location_manager::get_current_ui_location });
HOOK_DECLARE(0x00ADF9D0, location_manager_get);
HOOK_DECLARE(0x00ADF9E0, location_manager_start);
HOOK_DECLARE(0x00ADF9F0, location_manager_stop);

c_gui_location_manager& g_location_manager = *reinterpret_cast<c_gui_location_manager*>(0x01944204);

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
bool __fastcall c_gui_location_manager::can_change_location(c_gui_location_manager* _this)
{
	return _this->m_change_location >= 30;
}

// 00ADF6E0
void __fastcall c_gui_location_manager::change_location(c_gui_location_manager* _this, long gui_location, long screen_name)
{
	bool can_change_location = _this->can_change_location(_this);

	DECLFUNC(0x00ADF6E0, void, __thiscall, c_gui_location_manager*, long)(_this, screen_name);

	if (can_change_location)
	{
		void* load_screen_message = user_interface_malloc_tracked(0x3C, __FILE__, __LINE__);
		void* message = load_screen_message_ctor(load_screen_message, screen_name, k_any_controller, _render_window4, _gui_string_id_bottom_most);
		user_interface_messaging_post(message);
	}
}

// 00ADF870
long __fastcall c_gui_location_manager::get_current_ui_location(c_gui_location_manager* _this)
{
	return DECLFUNC(0x00ADF870, long, __thiscall, c_gui_location_manager*)(_this);
}

// 00ADF8A0
c_gui_screen_widget* __fastcall c_gui_location_manager::get_location_screen(c_gui_location_manager* _this)
{
	c_gui_screen_widget* result = DECLFUNC(0x00ADF8A0, c_gui_screen_widget*, __thiscall, c_gui_location_manager*)(_this);

	return result;
}

// 00ADF8F0
long __fastcall c_gui_location_manager::get_location_screen_name(c_gui_location_manager* _this, long gui_location)
{
	long result = DECLFUNC(0x00ADF8F0, long, __thiscall, c_gui_location_manager*, long)(_this, gui_location);

	return result;
}

// 00ADF960
long __fastcall c_gui_location_manager::get_pregame_lobby_name(c_gui_location_manager* _this, long gui_game_mode)
{
	long result = DECLFUNC(0x00ADF960, long, __thiscall, c_gui_location_manager*, long)(_this, gui_game_mode);

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
void __fastcall c_gui_location_manager::set_running(c_gui_location_manager* _this, bool running)
{
	_this->m_running = running;
}

// 00ADFA10
void __fastcall c_gui_location_manager::update(c_gui_location_manager* _this)
{
	DECLFUNC(0x00ADFA10, void, __thiscall, c_gui_location_manager*)(_this);
}
