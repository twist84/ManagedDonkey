#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "networking/online/online_service_record.hpp"

class c_start_menu_pane_screen_widget :
	public c_gui_screen_widget
{
public:
	virtual ~c_start_menu_pane_screen_widget() = default;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual bool __funcs53() override;
	virtual bool allow_pane_tab_change();

public:
	c_start_menu_pane_screen_widget(int32 name);
	void close_current_subpane();
	void close_start_menu1();
	void close_start_menu0();
	static bool __cdecl is_change_teams_allowed(e_controller_index controller_index);

protected:
	void set_automatic_button_key(int32 value);
	bool set_focused_datasource_element_from_value(c_gui_list_widget* list_widget, int32 value_name, int32 value_to_set, bool play_animations_on_focus_change);

public:
//protected:
	s_player_identifier m_owner_player_id;
	s_player_identifier m_target_player_id;
	uns64 m_target_player_xuid;
	s_service_record_identity m_identity;
	real32 m_appearance_camera_zoom;
	real32 m_appearance_camera_yaw;
	int32 m_automatic_button_key;
};
static_assert(sizeof(c_start_menu_pane_screen_widget) == 0x2168);
static_assert(sizeof(c_start_menu_pane_screen_widget) == sizeof(c_gui_screen_widget) + 0x6C8);

extern bool __cdecl gui_screen_widget_is_start_menu_pane_screen(c_gui_screen_widget* screen);

