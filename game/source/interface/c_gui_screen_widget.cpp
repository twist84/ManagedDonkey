#include "interface/c_gui_screen_widget.hpp"

#include "interface/user_interface_data.hpp"
#include "interface/user_interface_text_parser.hpp"

//.text:00AAB260 ; protected: virtual e_render_data_size c_gui_screen_widget::get_render_data_size()
//.text:00AAB270 ; public: s_window_manager_screen_render_data* c_gui_screen_widget::get_render_state()

c_gui_screen_widget::c_gui_screen_widget(int32 name)
{
	DECLFUNC(0x00AB02B0, c_gui_screen_widget*, __thiscall, c_gui_screen_widget*, int32)(this, name);
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

//.text:00AB0850 ; public: void s_window_manager_screen_render_data::clear()
//.text:00AB0870 ; 
//.text:00AB08B0 ; private: void c_gui_screen_widget::create_and_add_child_widgets_from_definition(s_runtime_screen_widget_definition*)

void c_gui_screen_widget::dispose()
{
	INVOKE_CLASS_MEMBER(0x00AB0980, c_gui_screen_widget, dispose);
}

//.text:00AB0A10 ; 
//.text:00AB0A20 ; public: void c_gui_screen_widget::dispose_child_screens()
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

	if (datasource_index)
		*datasource_index = NONE;

	if (m_datasource_count <= 0)
		return nullptr;

	int32 index = 0;
	for (c_gui_data** datasource = m_datasource; (*datasource)->get_name().get_value() != name; datasource++)
	{
		if (++index >= m_datasource_count)
			return nullptr;
	}

	if (datasource_index)
		*datasource_index = index;

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
	return INVOKE_CLASS_MEMBER(0x00AB0FF0, c_gui_screen_widget, get_render_window);
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
	INVOKE_CLASS_MEMBER(0x00AB18E0, c_gui_screen_widget, post_initialize);
}

//.text:00AB1910 ; public: void c_gui_screen_widget::post_update()
//.text:00AB19E0 ; public: void c_gui_screen_widget::pre_update()
//.text:00AB1A60 ; 

void c_gui_screen_widget::predict_immediate_resources(int32 a1)
{
	INVOKE_CLASS_MEMBER(0x00AB1AE0, c_gui_screen_widget, predict_immediate_resources, a1);
}

//.text:00AB1B00 ; 

void c_gui_screen_widget::predict_pending_resources(int32 a1)
{
	INVOKE_CLASS_MEMBER(0x00AB1B80, c_gui_screen_widget, predict_pending_resources, a1);
}

bool c_gui_screen_widget::process_message(c_message const* message)
{
	return INVOKE_CLASS_MEMBER(0x00AB1BA0, c_gui_screen_widget, process_message, message);
}

void c_gui_screen_widget::reconstruct()
{
	INVOKE_CLASS_MEMBER(0x00AB1C10, c_gui_screen_widget, reconstruct);
}

//.text:00AB1C80 ; private: void c_gui_screen_widget::recover_from_tag_changes()

void c_gui_screen_widget::reload_assets()
{
	INVOKE_CLASS_MEMBER(0x00AB1CE0, c_gui_screen_widget, reload_assets);
}

void c_gui_screen_widget::reload_next_frame()
{
	INVOKE_CLASS_MEMBER(0x00AB1DF0, c_gui_screen_widget, reload_next_frame);
}

//.text:00AB1E00 ; protected: void c_gui_screen_widget::remove_datasource(c_gui_data*)
//.text:00AB1E80 ; protected: void c_gui_screen_widget::reposition_for_splitscreen()

bool c_gui_screen_widget::running_in_codeless_mode()
{
	return m_running_in_codeless_mode;
}

//.text:00AB2030 ; 
//.text:00AB2100 ; 
//.text:00AB21E0 ; private: void c_gui_screen_widget::set_initial_focused_widget()
//.text:00AB22D0 ; public: void c_gui_screen_widget::set_text_widget_string_id(int32, int32)
//.text:00AB2300 ; public: void c_gui_screen_widget::set_text_widget_string_id_from_datasource(int32, int32, int32, int32)
//.text:00AB23A0 ; public: void c_gui_screen_widget::set_text_widget_string_id_from_focused_list_item(int32, int32, int32)
//.text:00AB23E0 ; public: void c_gui_screen_widget::set_text_widget_text(int32, wchar_t const*)
//.text:00AB2400 ; public: void c_gui_screen_widget::set_text_widget_text_from_datasource(int32, int32, int32, int32)
//.text:00AB24D0 ; public: void c_gui_screen_widget::set_text_widget_text_from_focused_list_item(int32, int32, int32)

void c_gui_screen_widget::submenu_invoked(c_gui_list_widget* submenu_widget)
{
	INVOKE_CLASS_MEMBER(0x00AB2510, c_gui_screen_widget, submenu_invoked, submenu_widget);
}

//.text:00AB2520 ; private: void c_gui_screen_widget::swap_display_group_block(c_gui_screen_widget::e_display_group_type, int32)

void c_gui_screen_widget::transfer_focus(c_gui_widget* widget)
{
	INVOKE_CLASS_MEMBER(0x00AB25F0, c_gui_screen_widget, transfer_focus, widget);
}

void c_gui_screen_widget::transfer_focus_to_list(c_gui_list_widget* list_widget, int32 element_handle, bool play_received_animation, bool play_lost_animation)
{
	INVOKE_CLASS_MEMBER(0x00AB2640, c_gui_screen_widget, transfer_focus_to_list, list_widget, element_handle, play_received_animation, play_lost_animation);
}

//.text:00AB2740 ; 
//.text:00AB27D0 ; public: void c_gui_screen_widget::transfer_focus_without_animations(c_gui_widget*, bool, bool)
//.text:00AB2830 ; public: void c_gui_screen_widget::transition_out(e_transition_out_type)

void c_gui_screen_widget::transition_out_with_transition_type(e_transition_out_type transition_out, e_screen_transition_type transition_type)
{
	INVOKE_CLASS_MEMBER(0x00AB2850, c_gui_screen_widget, transition_out_with_transition_type, transition_out, transition_type);
}

//.text:00AB2870 ; private: void c_gui_screen_scoreboard::translate_widget_recursive(c_gui_widget*, int32, int32)

bool c_gui_screen_widget::try_and_get_render_data_emblem_info(c_gui_bitmap_widget* bitmap_widget, s_emblem_info* emblem_info)
{
	return INVOKE_CLASS_MEMBER(0x00AB2930, c_gui_screen_widget, try_and_get_render_data_emblem_info, bitmap_widget, emblem_info);
}

void c_gui_screen_widget::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00AB2940, c_gui_screen_widget, update, current_milliseconds);
}

void c_gui_screen_widget::update_render()
{
	INVOKE_CLASS_MEMBER(0x00AB29A0, c_gui_screen_widget, update_render);
}

//.text:00AB29B0 ; public: bool s_window_manager_screen_render_data::valid()

