#pragma once

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

class c_start_menu_game_saved_film :
	public c_start_menu_pane_screen_widget
{
public:
	virtual ~c_start_menu_game_saved_film() = default;
	virtual void initialize() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual void initialize_datasource() override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual bool handle_dialog_result(const c_dialog_result_message* dialog_result_message) override;
	virtual bool allow_pane_tab_change() override;

public:
	c_start_menu_game_saved_film(int32 name);

public:
	e_controller_index m_controller_index;
};
COMPILE_ASSERT(sizeof(c_start_menu_game_saved_film) == 0x2170);
COMPILE_ASSERT(sizeof(c_start_menu_game_saved_film) == sizeof(c_start_menu_pane_screen_widget) + 0x8);

