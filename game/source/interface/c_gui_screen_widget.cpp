#include "interface/c_gui_screen_widget.hpp"

#include "cseries/cseries_events.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "interface/user_interface_window_manager.hpp"

//.text:00AAB260 ; protected: virtual e_render_data_size c_gui_screen_widget::get_render_data_size()
//.text:00AAB270 ; public: s_window_manager_screen_render_data* c_gui_screen_widget::get_render_state()

c_gui_screen_widget::c_gui_screen_widget(int32 name) :
	c_gui_widget(_gui_screen),
	m_screen_index(NONE),
	m_creation_time_milliseconds(user_interface_milliseconds()),
	m_disposal_time_milliseconds(-1),
	m_last_focus_change_time_milliseconds(-1),
	m_current_focused_widget(NULL),
	m_suppress_focus(false),
	m_render_in_screenshot(false),
	m_reload_next_frame(true),
	m_responds_to_controller_events(true),
	m_initial_focused_widget(NONE),
	m_initial_focused_widget_element_handle(NONE),
	m_initial_focused_widget_column_name(NONE),
	m_initial_focused_widget_column_value(NONE),
	m_definition(),
	m_datasource_count(0),
	m_game_tag_parser_count(0),
	m_render_data(),
	m_running_in_codeless_mode()
{
	//DECLFUNC(0x00AB02B0, c_gui_screen_widget*, __thiscall, c_gui_screen_widget*, int32)(this, name);

	m_definition.widget_identifier = name;
	for (int32 datasource_index = 0; datasource_index < NUMBEROF(m_datasource); datasource_index++)
	{
		m_datasource[datasource_index] = NULL;
	}

	for (int32 datasource_index = 0; datasource_index < NUMBEROF(m_game_tag_parsers); datasource_index++)
	{
		m_game_tag_parsers[datasource_index] = NULL;
	}

	clear_display_groups();
}

//.text:00AB0500 ; 
//.text:00AB0550 ; public: s_runtime_screen_widget_definition::s_runtime_screen_widget_definition()
//.text:00AB0630 ; 
//.text:00AB0660 ; 
//.text:00AB0670 ; public: virtual void* c_gui_screen_codeless_widget::`scalar deleting destructor'(unsigned int)

//.text:00AB06A0 ; public: virtual void* c_gui_screen_widget::`scalar deleting destructor'(unsigned int)
c_gui_screen_widget::~c_gui_screen_widget()
{
	DECLFUNC(0x00AB06A0, void, __thiscall, c_gui_screen_widget*)(this);
}

void c_gui_screen_widget::add_datasource(c_gui_data* datasource)
{
	//INVOKE_CLASS_MEMBER(0x00AB06D0, c_gui_screen_widget, add_datasource, datasource);

	m_datasource[m_datasource_count++] = datasource;
}

//.text:00AB06F0 ; public: void c_gui_screen_widget::add_definition_fields(s_screen_widget_definition const*, bool)

void c_gui_screen_widget::add_game_tag_parser(c_game_tag_parser* parser)
{
	//INVOKE_CLASS_MEMBER(0x00AB0800, c_gui_screen_widget, add_game_tag_parser, parser);

	if (parser)
	{
		if (m_game_tag_parser_count < NUMBEROF(m_game_tag_parsers))
		{
			user_interface_text_parser_register(parser, m_screen_index);
			m_game_tag_parsers[m_game_tag_parser_count++] = parser;
		}
	}
}

bool c_gui_screen_widget::can_receive_focus() const
{
	return INVOKE_CLASS_MEMBER(0x00AB0840, c_gui_screen_widget, can_receive_focus);
}

void s_window_manager_screen_render_data::clear()
{
	//INVOKE_CLASS_MEMBER(0x00AB0850, s_window_manager_screen_render_data, clear);

	render_data_buffer_count = 0;
	current_count = 0;
}

void c_gui_screen_widget::clear_display_groups()
{
	INVOKE_CLASS_MEMBER(0x00AB0870, c_gui_screen_widget, clear_display_groups);
}
//.text:00AB08B0 ; private: void c_gui_screen_widget::create_and_add_child_widgets_from_definition(s_runtime_screen_widget_definition*)

void c_gui_screen_widget::dispose()
{
	INVOKE_CLASS_MEMBER(0x00AB0980, c_gui_screen_widget, dispose);
}

//.text:00AB0A10 ; 

void c_gui_screen_widget::dispose_child_screens()
{
	INVOKE_CLASS_MEMBER(0x00AB0A20, c_gui_screen_widget, dispose_child_screens);
}

//.text:00AB0A90 ; private: void c_gui_screen_widget::dispose_datasource()
//.text:00AB0B00 ; private: void c_gui_screen_widget::dispose_display_group(c_gui_group_widget*)
//.text:00AB0BC0 ; protected: c_gui_list_item_widget* c_gui_screen_widget::find_list_item(int32, int32, int32)
//.text:00AB0C50 ; 
//.text:00AB0C60 ; 
//.text:00AB0CD0 ; public: int32 c_gui_screen_widget::get_constants_datasource_integer(int32)

s_runtime_core_widget_definition* c_gui_screen_widget::get_core_definition()
{
	return INVOKE_CLASS_MEMBER(0x00AB0DA0, c_gui_screen_widget, get_core_definition);
}

c_gui_data* c_gui_screen_widget::get_data(int32 name, int32* datasource_index)
{
	//return INVOKE_CLASS_MEMBER(0x00AB0DB0, c_gui_screen_widget, get_data, name, datasource_index);

	// $TODO: cleanup this function

	if (datasource_index)
	{
		*datasource_index = NONE;
	}

	if (m_datasource_count <= 0)
	{
		return NULL;
	}

	int32 index = 0;
	for (c_gui_data** datasource = m_datasource; (*datasource)->get_name() != name; datasource++)
	{
		if (++index >= m_datasource_count)
		{
			return NULL;
		}
	}

	if (datasource_index)
	{
		*datasource_index = index;
	}

	return m_datasource[index];
}

//.text:00AB0E20 ; 
//.text:00AB0E30 ; 
//.text:00AB0E40 ; 
//.text:00AB0E90 ; private: int32 c_gui_screen_widget::get_display_group_index(c_gui_screen_widget::e_display_group_type, int32)
//.text:00AB0F30 ; 

c_gui_widget* c_gui_screen_widget::get_focused_widget()
{
	return INVOKE_CLASS_MEMBER(0x00AB0F40, c_gui_screen_widget, get_focused_widget);
}

//.text:00AB0F60 ; 
//.text:00AB0F70 ; 
//.text:00AB0F80 ; 

e_window_index c_gui_screen_widget::get_render_window()
{
	//return INVOKE_CLASS_MEMBER(0x00AB0FF0, c_gui_screen_widget, get_render_window);

	return window_manager_get()->get_render_window_for_screen(this);
}

bool c_gui_screen_widget::get_string_by_string_id(int32 string_identifier, c_static_wchar_string<1024>* buffer)
{
	return INVOKE_CLASS_MEMBER(0x00AB1000, c_gui_screen_widget, get_string_by_string_id, string_identifier, buffer);
}

//.text:00AB1040 ; 
//.text:00AB1110 ; 
//.text:00AB11D0 ; private: bool c_gui_screen_widget::handle_controller_alt_stick_attempt(c_controller_input_message const*)
//.text:00AB1240 ; private: bool c_gui_screen_widget::handle_controller_alt_tab_attempt(c_controller_input_message const*)

bool c_gui_screen_widget::handle_controller_input_message(c_controller_input_message const* message)
{
	return INVOKE_CLASS_MEMBER(0x00AB1270, c_gui_screen_widget, handle_controller_input_message, message);
}

//.text:00AB1370 ; private: bool c_gui_screen_widget::handle_controller_selection_attempt(c_controller_input_message const*)
//.text:00AB13C0 ; private: bool c_gui_screen_widget::handle_controller_tab_attempt(c_controller_input_message const*)

bool c_gui_screen_widget::handle_dialog_result(c_dialog_result_message const* message)
{
	return INVOKE_CLASS_MEMBER(0x00AB13F0, c_gui_screen_widget, handle_dialog_result, message);
}

bool c_gui_screen_widget::handle_focused_widget_selected(c_controller_input_message const* message, c_gui_widget* widget)
{
	return INVOKE_CLASS_MEMBER(0x00AB1420, c_gui_screen_widget, handle_focused_widget_selected, message, widget);
}

bool c_gui_screen_widget::handle_list_item_chosen(c_controller_input_message const* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00AB14C0, c_gui_screen_widget, handle_list_item_chosen, message, list_name, list_item_widget, datasource);
	//return INVOKE_CLASS_MEMBER((uns32)__vftable->__funcs[49], c_gui_screen_widget, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	return false;
}

void c_gui_screen_widget::initialize()
{
	INVOKE_CLASS_MEMBER(0x00AB14D0, c_gui_screen_widget, initialize);
}

//.text:00AB1560 ; public: virtual void c_gui_screen_codeless_widget::initialize_datasource()

void c_gui_screen_widget::initialize_datasource()
{
	INVOKE_CLASS_MEMBER(0x00AB1650, c_gui_screen_widget, initialize_datasource);
}

bool c_gui_screen_widget::__funcs53()
{
	return INVOKE_CLASS_MEMBER(0x00AB1760, c_gui_screen_widget, __funcs53);

	//c_window_manager* window_manager = window_manager_get();
	//e_window_index window_index = window_manager->get_render_window_for_screen(this);
	//c_gui_screen_widget* screen_above = window_manager->get_screen_above(window_index, this);
	//if (!screen_above)
	//{
	//	return false;
	//}
	//while (!TEST_BIT(screen_above->m_definition.flags, 4) || screen_above->transitioning_out())
	//{
	//	screen_above = window_manager->get_screen_above(window_index, screen_above);
	//	if (!screen_above)
	//	{
	//		return false;
	//	}
	//}
	//return true;
}

//.text:00AB17D0 ; private: c_gui_group_widget* c_gui_screen_widget::load_display_group(s_runtime_screen_widget_definition const*, c_gui_screen_widget::e_display_group_type, int32)
//.text:00AB1830 ; public: void c_gui_screen_widget::load_display_group_by_name(c_gui_screen_widget::e_display_group_type, int32)
//.text:00AB1860 ; 
//.text:00AB1890 ; 

void c_gui_screen_widget::play_sound(e_user_interface_sound_effect sound_effect)
{
	INVOKE_CLASS_MEMBER(0x00AB18A0, c_gui_screen_widget, play_sound, sound_effect);

	//user_interface_play_sound(sound_effect, m_definition.sound_overrides_index);
}

//.text:00AB18C0 ; 

void c_gui_screen_widget::post_initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AB18E0, c_gui_screen_widget, post_initialize);

	c_gui_screen_widget::set_initial_focused_widget();
	if (m_definition.script_index != 0xFFFFi16)
	{
		window_manager_get()->run_screen_hs_script(m_definition.script_index);
	}

	//if (s_screen_widget_definition const* screen_definition = get_screen_definition())
	//{
	//	if (s_mouse_cursor_definition const* mouse_cursor_definition = mouse_cursor_definition_get(screen_definition->mouse_cursor_tag.index))
	//	{
	//		input_set_mouse_cursors_from_tag(mouse_cursor_definition);
	//	}
	//}

	c_gui_widget::post_initialize();
}

//.text:00AB1910 ; public: void c_gui_screen_widget::post_update()
//.text:00AB19E0 ; public: void c_gui_screen_widget::pre_update()

void __cdecl sub_AB1A60(c_gui_screen_widget* screen_widget, int32 a2)
{
	INVOKE(0x00AB1A60, sub_AB1A60, screen_widget, a2);
}

void c_gui_screen_widget::predict_immediate_resources(int32 a1)
{
	//INVOKE_CLASS_MEMBER(0x00AB1AE0, c_gui_screen_widget, predict_immediate_resources, a1);

	sub_AB1A60(this, a1);
}

void __cdecl sub_AB1B00(c_gui_screen_widget* screen_widget, int32 a2)
{
	INVOKE(0x00AB1B00, sub_AB1B00, screen_widget, a2);
}

void c_gui_screen_widget::predict_pending_resources(int32 a1)
{
	//INVOKE_CLASS_MEMBER(0x00AB1B80, c_gui_screen_widget, predict_pending_resources, a1);

	sub_AB1B00(this, a1);
}

bool c_gui_screen_widget::process_message(c_message const* message)
{
	return INVOKE_CLASS_MEMBER(0x00AB1BA0, c_gui_screen_widget, process_message, message);

	//bool result = false;
	//switch (message->get_type())
	//{
	//case _message_type_controller_input:
	//{
	//	c_controller_input_message const* controller_input_message = (c_controller_input_message const*)message;
	//	if (result = handle_controller_input_message(controller_input_message))
	//	{
	//		c_window_manager::play_controller_sound(
	//			m_definition.sound_override_reference_index,
	//			controller_input_message->get_event_type(),
	//			controller_input_message->get_component());
	//	}
	//}
	//break;
	//case _message_type_dialog_result:
	//{
	//	c_dialog_result_message const* dialog_result_message = (c_dialog_result_message const*)message;
	//	result = handle_dialog_result(dialog_result_message);
	//}
	//break;
	//}
	//
	//return result;
}

void c_gui_screen_widget::reconstruct()
{
	//INVOKE_CLASS_MEMBER(0x00AB1C10, c_gui_screen_widget, reconstruct);

	csmemset(&m_animated_state, 0, sizeof(m_animated_state));
	m_current_focused_widget = NULL;
	c_gui_screen_widget::clear_display_groups();
	c_gui_screen_widget::dispose_child_screens();
	c_gui_widget::delete_all_children();
}

//.text:00AB1C80 ; private: void c_gui_screen_widget::recover_from_tag_changes()

void c_gui_screen_widget::reload_assets()
{
	INVOKE_CLASS_MEMBER(0x00AB1CE0, c_gui_screen_widget, reload_assets);
}

void c_gui_screen_widget::reload_next_frame()
{
	//INVOKE_CLASS_MEMBER(0x00AB1DF0, c_gui_screen_widget, reload_next_frame);

	m_reload_next_frame = true;
}

//.text:00AB1E00 ; protected: void c_gui_screen_widget::remove_datasource(c_gui_data*)
//.text:00AB1E80 ; protected: void c_gui_screen_widget::reposition_for_splitscreen()

bool c_gui_screen_widget::running_in_codeless_mode()
{
	return m_running_in_codeless_mode;
}

//.text:00AB2030 ; 
//.text:00AB2100 ; 

void c_gui_screen_widget::set_initial_focused_widget()
{
	INVOKE_CLASS_MEMBER(0x00AB21E0, c_gui_screen_widget, set_initial_focused_widget);
}

void c_gui_screen_widget::set_text_widget_string_id(int32 widget_name, int32 widget_text)
{
	//INVOKE_CLASS_MEMBER(0x00AB22D0, c_gui_screen_widget, set_text_widget_string_id, widget_name, widget_text);

	if (c_gui_text_widget* child_text_widget = get_child_text_widget(widget_name))
	{
		child_text_widget->set_text_from_string_id(this, widget_text);
	}
}

//.text:00AB2300 ; public: void c_gui_screen_widget::set_text_widget_string_id_from_datasource(int32, int32, int32, int32)
//.text:00AB23A0 ; public: void c_gui_screen_widget::set_text_widget_string_id_from_focused_list_item(int32, int32, int32)

void c_gui_screen_widget::set_text_widget_text(int32 widget_name, wchar_t const* widget_text)
{
	//INVOKE_CLASS_MEMBER(0x00AB23E0, c_gui_screen_widget, set_text_widget_text, widget_name, widget_text);

	if (c_gui_text_widget* child_text_widget = get_child_text_widget(widget_name))
	{
		child_text_widget->set_text(widget_text);
	}
}

//.text:00AB2400 ; public: void c_gui_screen_widget::set_text_widget_text_from_datasource(int32, int32, int32, int32)
//.text:00AB24D0 ; public: void c_gui_screen_widget::set_text_widget_text_from_focused_list_item(int32, int32, int32)

void c_gui_screen_widget::submenu_invoked(c_gui_list_widget* submenu_widget)
{
	//INVOKE_CLASS_MEMBER(0x00AB2510, c_gui_screen_widget, submenu_invoked, submenu_widget);
}

//.text:00AB2520 ; private: void c_gui_screen_widget::swap_display_group_block(c_gui_screen_widget::e_display_group_type, int32)

void c_gui_screen_widget::transfer_focus(c_gui_widget* new_focused_widget)
{
	//INVOKE_CLASS_MEMBER(0x00AB25F0, c_gui_screen_widget, transfer_focus, new_focused_widget);

	c_gui_screen_widget::transfer_focus_without_animations(new_focused_widget, true, true);
}

void c_gui_screen_widget::transfer_focus_to_list(c_gui_list_widget* list_widget, int32 element_handle, bool play_received_animation, bool play_lost_animation)
{
	INVOKE_CLASS_MEMBER(0x00AB2640, c_gui_screen_widget, transfer_focus_to_list, list_widget, element_handle, play_received_animation, play_lost_animation);

	c_gui_data* data = list_widget->get_data();
	if (!data)
	{
		return;
	}
	
	if (data->contains(element_handle))
	{
		element_handle = data->get_first_element_handle();
	}
	
	if (element_handle == NONE)
	{
		return;
	}
}

//.text:00AB2740 ; 

void c_gui_screen_widget::transfer_focus_without_animations(c_gui_widget* new_focused_widget, bool play_received_animation, bool play_lost_animation)
{
	INVOKE_CLASS_MEMBER(0x00AB27D0, c_gui_screen_widget, transfer_focus_without_animations, new_focused_widget, play_received_animation, play_lost_animation);

	ASSERT(new_focused_widget != NULL);

	//if (!new_focused_widget->can_receive_focus())
	//{
	//	event(_event_verbose, "ui: tried to give focus to an invalid child widget (type= %s, name= %s)",
	//		gui_widget_type_to_string(new_focused_widget->m_type),
	//		c_gui_widget::widget_name_to_string(new_focused_widget->m_name));
	//	return;
	//}
	//if (play_received_animation)
	//{
	//	new_focused_widget->start_animation(_control_received_focus, true);
	//}
	//if (play_lost_animation)
	//{
	//	m_current_focused_widget->start_animation(_control_lost_focus, true);
	//}
	//m_current_focused_widget = new_focused_widget;
}

void c_gui_screen_widget::transition_out(e_transition_out_type transition_out)
{
	//INVOKE_CLASS_MEMBER(0x00AB2830, c_gui_screen_widget, transition_out, transition_out);

	transition_out_with_transition_type(transition_out, _screen_transition_type_normal);
}

void c_gui_screen_widget::transition_out_with_transition_type(e_transition_out_type transition_out, e_screen_transition_type transition_type)
{
	//INVOKE_CLASS_MEMBER(0x00AB2850, c_gui_screen_widget, transition_out_with_transition_type, transition_out, transition_type);

	window_manager_get()->begin_transition_out(this, transition_type);
}

//.text:00AB2870 ; private: void c_gui_screen_scoreboard::translate_widget_recursive(c_gui_widget*, int32, int32)

bool c_gui_screen_widget::try_and_get_render_data_emblem_info(c_gui_bitmap_widget* bitmap_widget, s_emblem_info* emblem_info)
{
	//return INVOKE_CLASS_MEMBER(0x00AB2930, c_gui_screen_widget, try_and_get_render_data_emblem_info, bitmap_widget, emblem_info);

	return NULL;
}

void c_gui_screen_widget::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00AB2940, c_gui_screen_widget, update, current_milliseconds);

	c_gui_widget::update(current_milliseconds);
	
	e_controller_index driving_controller = c_gui_widget::get_driving_controller();
	if (driving_controller == k_no_controller || driving_controller == k_any_controller)
	{
		return;
	}
	
	if (!controller_get(driving_controller)->in_use())
	{
		return;
	}
	
	transition_out_with_transition_type(_transition_out_normal, _screen_transition_type_normal);
}

void c_gui_screen_widget::update_render()
{
	//INVOKE_CLASS_MEMBER(0x00AB29A0, c_gui_screen_widget, update_render);
}

//.text:00AB29B0 ; public: bool s_window_manager_screen_render_data::valid()

