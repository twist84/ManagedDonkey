#pragma once

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

class c_start_menu_game_campaign :
	public c_start_menu_pane_screen_widget
{
public:
	virtual ~c_start_menu_game_campaign() = default;
	virtual void initialize() override;
	virtual void post_initialize() override;
	virtual c_gui_bitmap_widget* create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition) override;
	virtual void update(uns32 current_milliseconds) override;
	virtual void initialize_datasource() override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual bool handle_dialog_result(const c_dialog_result_message* message) override;
	virtual bool allow_pane_tab_change() override;

public:
	c_start_menu_game_campaign(int32 name);
};
COMPILE_ASSERT(sizeof(c_start_menu_game_campaign) == 0x2168);
COMPILE_ASSERT(sizeof(c_start_menu_game_campaign) == sizeof(c_start_menu_pane_screen_widget));

extern bool __cdecl parse_xml_primary_mission_objectives(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_secondary_mission_objectives(void* this_ptr, wchar_t* buffer, int32 buffer_length);

