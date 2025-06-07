#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "networking/online/online_service_record.hpp"

class c_start_menu_pane_screen_widget :
	public c_gui_screen_widget
{
public:
	c_start_menu_pane_screen_widget(int32 name) :
		c_gui_screen_widget(name)
	{
		DECLFUNC(0x00B1F280, c_start_menu_pane_screen_widget*, __thiscall, c_start_menu_pane_screen_widget*, int32)(this, name);
	}

	void close_current_subpane();

//protected:
	s_player_identifier m_owner_player_id;
	s_player_identifier m_target_player_id;
	uns64 m_target_player_xuid;
	s_service_record_identity m_identity;
	real32 m_appearance_camera_zoom;
	real32 m_appearance_camera_yaw;
	int32 m_automatic_button_key;
	byte __data2164[0x4];
};
static_assert(sizeof(c_start_menu_pane_screen_widget) == 0x2168);
static_assert(sizeof(c_start_menu_pane_screen_widget) == sizeof(c_gui_screen_widget) + 0x6C8);

