#pragma once

#include "cseries/cseries.hpp"

struct c_gui_screen_widget;
struct c_gui_location_manager
{
public:
	c_gui_location_manager();

private:
	bool can_change_location();
	long get_pregame_lobby_name(long gui_game_mode);

public:
	void set_running(bool running);
	void update();
	void __thiscall change_location(long screen_name);
	long __thiscall get_current_ui_location();
	c_gui_screen_widget* get_location_screen();
	long get_location_screen_name(long gui_location);

//protected:
	bool m_running;
	dword __unknown4;
	bool m_show_postgame_stats_upon_lobby_entrance;
	dword m_change_time;
	long m_change_location;
};
static_assert(sizeof(c_gui_location_manager) == 0x14);

extern c_gui_location_manager& g_location_manager;

extern c_gui_location_manager* __cdecl location_manager_get();
extern void __cdecl location_manager_start();
extern void __cdecl location_manager_stop();

