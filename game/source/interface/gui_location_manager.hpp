#pragma once

#include "cseries/cseries.hpp"

struct c_gui_screen_widget;
struct c_gui_location_manager
{
public:
	c_gui_location_manager();

private:
	static bool __fastcall can_change_location(c_gui_location_manager* _this);
	static long __fastcall get_pregame_lobby_name(c_gui_location_manager* _this, long gui_game_mode);

public:
	static void __fastcall set_running(c_gui_location_manager* _this, bool running);
	static void __fastcall update(c_gui_location_manager* _this);
	static void __fastcall change_location(c_gui_location_manager* _this, long gui_location, long screen_name);
	static long __fastcall get_current_ui_location(c_gui_location_manager* _this);
	static c_gui_screen_widget* __fastcall get_location_screen(c_gui_location_manager* _this);
	static long __fastcall get_location_screen_name(c_gui_location_manager* _this, long gui_location);

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
extern void __cdecl window_manager_load_screen_hs(long screen);