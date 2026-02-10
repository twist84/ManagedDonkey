#pragma once

#include "cseries/cseries.hpp"

class c_gui_screen_widget;
class c_gui_location_manager
{
public:
	enum e_location_mode
	{
		_location_mode_unset = 0,
		_location_mode_normal,
		_location_mode_editor_or_game_start,
	};

public:
	void __thiscall update_();

public:
	c_gui_location_manager();

private:
	static void __cdecl begin_enter_location(e_gui_location old_location, e_gui_location new_location);
	bool can_change_location();
	int32 get_pregame_lobby_name(int32 game_mode);

public:
	void change_location(int32 new_location_name);
	e_gui_location get_current_ui_location();
	c_gui_screen_widget* get_location_screen();
	int32 get_location_screen_name(int32 gui_location);
	bool get_show_postgame_stats_upon_lobby_entrance() const;
	void set_running(bool running);
	void set_show_postgame_stats_upon_lobby_entrance(bool value);
	void update();
	void update_change_location();

//protected:
	bool m_running;
	e_location_mode m_location_mode;
	bool m_show_postgame_stats;
	uns32 m_location_change_throttle_time;
	int32 m_location_change_throttle_count;
};
COMPILE_ASSERT(sizeof(c_gui_location_manager) == 0x14);

extern c_gui_location_manager& g_location_manager;

extern c_gui_location_manager* __cdecl location_manager_get();
extern void __cdecl location_manager_start();
extern void __cdecl location_manager_stop();

