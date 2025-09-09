#pragma once

#include "cseries/cseries.hpp"
#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

class c_player_profile_interface;

class c_start_menu_settings_appearance_model :
	public c_start_menu_pane_screen_widget
{
public:
	virtual ~c_start_menu_settings_appearance_model() = default;
	virtual void initialize() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;
	virtual void initialize_datasource() override;
	virtual void update_render_state(uns32 current_milliseconds) override;
	virtual void submenu_invoked(c_gui_list_widget* submenu_widget) override;

public:
	c_start_menu_settings_appearance_model(int32 name);

private: 
	void add_model_customization_selections_datasource(int32 area_name, e_controller_index controller_index);
	void update_model_widget(bool force_update);

public:
	c_string_id m_model_customizations[k_player_model_choice_count];
};
static_assert(sizeof(c_start_menu_settings_appearance_model) == 0x2170);
static_assert(sizeof(c_start_menu_settings_appearance_model) == sizeof(c_start_menu_pane_screen_widget) + 0x8);

extern int32 get_current_model_customization_value(const c_player_profile_interface* player_profile, int32 area_name);
extern bool get_current_model_selection_area_and_item_index(c_gui_screen_widget* screen_widget, int32* area_name, int32* current_selection_index);

extern bool __cdecl parse_xml_part_count_current(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_part_count_total(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_part_description(void* this_ptr, wchar_t* buffer, int32 buffer_length);

