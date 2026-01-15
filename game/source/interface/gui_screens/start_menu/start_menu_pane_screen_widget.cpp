#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

#include "interface/c_gui_list_widget.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_window_manager.hpp"

c_start_menu_pane_screen_widget::c_start_menu_pane_screen_widget(int32 name) :
	c_gui_screen_widget(name),
	m_owner_player_id(),
	m_target_player_id(),
	m_target_player_xuid(),
	m_identity(),
	m_appearance_camera_zoom(0.0f),
	m_appearance_camera_yaw(0.0f),
	m_automatic_button_key(_string_id_invalid)
{
	//DECLFUNC(0x00B1F280, void, __thiscall, c_start_menu_pane_screen_widget*, int32)(this, name);
}

//.text:00B1F2F0 ; public: virtual c_start_menu_custom_message::~c_start_menu_custom_message()
//.text:00B1F300 ; public: virtual void* c_start_menu_custom_message::`scalar deleting destructor'(unsigned int)
//.text:00B1F330 ; public: virtual void* c_start_menu_pane_screen_widget::`vector deleting destructor'(unsigned int)

void c_start_menu_pane_screen_widget::close_current_subpane()
{
	INVOKE_CLASS_MEMBER(0x00B1F360, c_start_menu_pane_screen_widget, close_current_subpane);

	//if (c_start_menu_custom_message* message = new (_ui_allocation_marker_dummy) c_start_menu_custom_message(
	//	_message_type_close_current_pane,
	//	STRING_ID(gui, start_menu),
	//	c_gui_widget::get_single_responding_controller(),
	//	c_gui_screen_widget::get_render_window()))
	//{
	//	user_interface_messaging_post(message);
	//}
}

void c_start_menu_pane_screen_widget::close_start_menu1()
{
	INVOKE_CLASS_MEMBER(0x00B1F400, c_start_menu_pane_screen_widget, close_start_menu1);

	//if (c_start_menu_custom_message* message = new (_ui_allocation_marker_dummy) c_start_menu_custom_message(
	//	_message_type_close_start_menu,
	//	STRING_ID(gui, start_menu),
	//	c_gui_widget::get_single_responding_controller(),
	//	c_gui_screen_widget::get_render_window(),
	//	true))
	//{
	//	user_interface_messaging_post(message);
	//}
}

void c_start_menu_pane_screen_widget::close_start_menu0()
{
	INVOKE_CLASS_MEMBER(0x00B1F4A0, c_start_menu_pane_screen_widget, close_start_menu0);

	//if (c_start_menu_custom_message* message = new (_ui_allocation_marker_dummy) c_start_menu_custom_message(
	//	_message_type_close_start_menu,
	//	STRING_ID(gui, start_menu),
	//	c_gui_widget::get_single_responding_controller(),
	//	c_gui_screen_widget::get_render_window(),
	//	false))
	//{
	//	user_interface_messaging_post(message);
	//}
}

//.text:00B1F540 ; protected: bool c_start_menu_pane_screen_widget::controller_event_should_go_to_parent(const c_controller_input_message*)

bool __cdecl gui_screen_widget_is_start_menu_pane_screen(c_gui_screen_widget* screen)
{
	//return INVOKE(0x00B1F5D0, gui_screen_widget_is_start_menu_pane_screen, screen);

	bool start_menu_pane = false;
	if (screen)
	{
		switch (screen->m_name)
		{
		case STRING_ID(gui, start_menu_hq):
		case STRING_ID(gui, start_menu_hq_service_record):
		case STRING_ID(gui, start_menu_hq_service_record_file_share):
		case STRING_ID(gui, start_menu_hq_service_record_file_share_bungie):
		case STRING_ID(gui, start_menu_hq_screenshots):
		case STRING_ID(gui, start_menu_hq_transfers):
		case STRING_ID(gui, start_menu_settings):
		case STRING_ID(gui, start_menu_settings_controls):
		case STRING_ID(gui, start_menu_settings_controls_button):
		case STRING_ID(gui, start_menu_settings_controls_thumbstick):
		case STRING_ID(gui, start_menu_settings_appearance):
		case STRING_ID(gui, start_menu_settings_appearance_model):
		case STRING_ID(gui, start_menu_settings_appearance_emblem):
		case STRING_ID(gui, start_menu_settings_appearance_colors):
		case STRING_ID(gui, start_menu_settings_camera):
		case STRING_ID(gui, start_menu_settings_film_autosave):
		case STRING_ID(gui, start_menu_settings_display):
			start_menu_pane = true;
		}
	}
	return start_menu_pane;
}

bool c_start_menu_pane_screen_widget::handle_controller_input_message(const c_controller_input_message* message)
{
	return INVOKE_CLASS_MEMBER(0x00B1F620, c_start_menu_pane_screen_widget, handle_controller_input_message, message);
}

bool c_start_menu_pane_screen_widget::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return INVOKE_CLASS_MEMBER(0x00B1F670, c_start_menu_pane_screen_widget, handle_list_item_chosen, message, list_name, list_item_widget, datasource);
}

bool c_start_menu_pane_screen_widget::__funcs53()
{
	return INVOKE_CLASS_MEMBER(0x00B1F800, c_start_menu_pane_screen_widget, __funcs53);

	//bool result = false;
	//
	//c_window_manager* window_manager = window_manager_get();
	//e_window_index render_window = c_gui_screen_widget::get_render_window();
	//for (c_gui_screen_widget* screen_above = window_manager->get_screen_above(render_window, this);
	//	screen_above;
	//	screen_above = window_manager->get_screen_above(render_window, screen_above))
	//{
	//	if (!TEST_BIT(screen_above->m_definition.flags, 4) && !screen_above->transitioning_out())
	//	{
	//		if (!gui_screen_widget_is_start_menu_pane_screen(screen_above))
	//		{
	//			result = true;
	//			break;
	//		}
	//	}
	//}
	//
	//return result;
}

//.text:00B1F8B0 ; public: void c_start_menu_pane_screen_widget::replace_current_subpane(int32)

//protected:
bool c_start_menu_pane_screen_widget::set_focused_datasource_element_from_value(c_gui_list_widget* list_widget, int32 value_name, int32 value_to_set, bool play_animations_on_focus_change)
{
	//return INVOKE_CLASS_MEMBER(0x00B1F950, c_start_menu_pane_screen_widget, set_focused_datasource_element_from_value, list_widget, value_name, value_to_set, play_animations_on_focus_change);

	ASSERT(list_widget != NULL);
	return list_widget->set_focused_datasource_element_from_value(value_name, value_to_set, play_animations_on_focus_change);
}

void c_start_menu_pane_screen_widget::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B1F970, c_start_menu_pane_screen_widget, update, current_milliseconds);
}

