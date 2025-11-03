#pragma once

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

class c_start_menu_settings_appearance_emblem :
	public c_start_menu_pane_screen_widget
{
public:
	virtual ~c_start_menu_settings_appearance_emblem() = default;
	virtual void post_initialize() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual void update_render_state(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;

public:
	c_start_menu_settings_appearance_emblem(int32 name);
};
static_assert(sizeof(c_start_menu_settings_appearance_emblem) == 0x2168);
static_assert(sizeof(c_start_menu_settings_appearance_emblem) == sizeof(c_start_menu_pane_screen_widget));

extern void player_emblem_toggle_alternate_emblem_foreground(e_controller_index controller_index);
extern void player_emblem_toggle_flip_emblem_foreground(e_controller_index controller_index);
extern void player_emblem_toggle_flip_emblem_background(e_controller_index controller_index);

