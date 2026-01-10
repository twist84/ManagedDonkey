#pragma once

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

class c_start_menu_game_multiplayer :
	public c_start_menu_pane_screen_widget
{
public:
	void __thiscall submenu_invoked_(c_gui_list_widget* submenu_widget);

public:
	virtual ~c_start_menu_game_multiplayer() = default;
	virtual void initialize() override;
	virtual c_gui_bitmap_widget* create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition) override;
	virtual void update(uns32 current_milliseconds) override;
	virtual void initialize_datasource() override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual bool handle_dialog_result(const c_dialog_result_message* message) override;
	virtual void submenu_invoked(c_gui_list_widget* submenu_widget) override;
	virtual bool allow_pane_tab_change() override;

public:
	c_start_menu_game_multiplayer(int32 name);
	static void handle_leave_game_choice(c_start_menu_pane_screen_widget* screen, const c_controller_input_message* message);
	static void handle_leave_game_choice(c_start_menu_pane_screen_widget* screen, e_controller_index controller_index);
	static bool handle_leave_game_response(c_start_menu_pane_screen_widget* screen, const c_dialog_result_message* dialog_result_message);

public:
	wchar_t m_gametype[32];
	wchar_t m_gametype_description[256];
};
static_assert(sizeof(c_start_menu_game_multiplayer) == 0x23A8);
static_assert(sizeof(c_start_menu_game_multiplayer) == sizeof(c_start_menu_pane_screen_widget) + 0x240);

extern bool __cdecl parse_game_description(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_game_type(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl should_display_editor_elements();

