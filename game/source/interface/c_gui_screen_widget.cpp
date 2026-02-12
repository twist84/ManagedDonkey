#include "interface/c_gui_screen_widget.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_gui_group_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/gui_screens/motd_popup/gui_screen_motd_popup.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "saved_games/content_catalogue.hpp"

template<>
void ui_track_delete<c_gui_data>(const c_gui_data* object)
{
	ASSERT(object != nullptr);

	object->~c_gui_data();
	user_interface_free(object);
}

template<>
void ui_track_delete<c_gui_tag_datasource>(const c_gui_tag_datasource* object)
{
	ASSERT(object != nullptr);

	object->~c_gui_tag_datasource();
	user_interface_free(object);
}

template<>
void ui_track_delete<c_game_tag_parser>(const c_game_tag_parser* object)
{
	ASSERT(object != nullptr);

	object->~c_game_tag_parser();
	user_interface_free(object);
}

template<>
void ui_track_delete<c_gui_data_array_test>(const c_gui_data_array_test* object)
{
	ASSERT(object != nullptr);

	object->~c_gui_data_array_test();
	user_interface_free(object);
}

template<>
void ui_track_delete<c_gui_group_widget>(const c_gui_group_widget* object)
{
	ASSERT(object != nullptr);

	object->~c_gui_group_widget();
	user_interface_free(object);
}

template<>
void ui_track_delete<c_controller_input_message>(const c_controller_input_message* object)
{
	ASSERT(object != nullptr);

	object->~c_controller_input_message();
	user_interface_free(object);
}

c_gui_screen_widget::c_gui_screen_widget(int32 name) :
	c_gui_widget(_gui_screen),
	m_screen_index(NONE),
	m_creation_time_milliseconds(user_interface_milliseconds()),
	m_disposal_time_milliseconds(-1),
	m_last_focus_change_time_milliseconds(-1),
	m_current_focused_widget(nullptr),
	m_suppress_focus(false),
	m_render_in_screenshot(false),
	m_current_display_group_widgets(),
	m_previous_display_group_widgets(),
	m_current_display_group_indicies(),
	m_reload_next_frame(false),
	m_responds_to_controller_events(true),
	m_initial_focused_widget(NONE),
	m_initial_focused_widget_element_handle(NONE),
	m_initial_focused_widget_column_name(_string_id_invalid),
	m_initial_focused_widget_column_value(NONE),
	m_definition(),
	m_datasource_count(0),
	m_game_tag_parser_count(0),
	m_render_data(),
	m_running_in_codeless_mode()
{
	//DECLFUNC(0x00AB02B0, void, __thiscall, c_gui_screen_widget*, int32)(this, name);

	m_definition.widget_identifier = name;
	for (int32 datasource_index = 0; datasource_index < NUMBEROF(m_datasource); datasource_index++)
	{
		m_datasource[datasource_index] = nullptr;
	}

	for (int32 datasource_index = 0; datasource_index < NUMBEROF(m_game_tag_parsers); datasource_index++)
	{
		m_game_tag_parsers[datasource_index] = nullptr;
	}

	clear_display_groups();
}

//.text:00AB0500 ; 

s_runtime_screen_widget_definition::s_runtime_screen_widget_definition()
{
	DECLFUNC(0x00AB0550, void, __thiscall, s_runtime_screen_widget_definition*)(this);
}

s_window_manager_screen_render_data::s_window_manager_screen_render_data() :
	render_data_buffer(nullptr),
	render_data_buffer_length(0),
	render_data_buffer_count(0),
	render_list(),
	current_count(0),
	built_for_viewport_bounds()
{
	//DECLFUNC(0x00AB0630, void, __thiscall, s_window_manager_screen_render_data*)(this);
}

//.text:00AB0660 ; 
//.text:00AB0670 ; public: virtual void* c_gui_screen_codeless_widget::`scalar deleting destructor'(unsigned int)
//.text:00AB06A0 ; public: virtual void* c_gui_screen_widget::`scalar deleting destructor'(unsigned int)

void c_gui_screen_widget::add_datasource(c_gui_data* datasource)
{
	//INVOKE_CLASS_MEMBER(0x00AB06D0, c_gui_screen_widget, add_datasource, datasource);

	m_datasource[m_datasource_count++] = datasource;
}

void c_gui_screen_widget::add_definition_fields(const s_screen_widget_definition* definition, bool was_templated)
{
	//INVOKE_CLASS_MEMBER(0x00AB06F0, c_gui_screen_widget, add_definition_fields, definition, was_templated);

	c_gui_widget::add_definition_fields(m_type, definition, &m_definition, nullptr, was_templated);

	if (definition->initial_button_key_name)
	{
		m_definition.initial_button_key_name = definition->initial_button_key_name;
	}

	if (definition->string_list_tag_reference.index != NONE && definition->string_list_tag_reference.index)
	{
		m_definition.string_list_tag_reference_index = definition->string_list_tag_reference.index;
	}

	if (definition->datasource_blocks.count > 0 && definition->datasource_blocks.address)
	{
		m_definition.datasource_blocks = definition->datasource_blocks;
	}

	if (definition->group_blocks.count > 0 && definition->group_blocks.address)
	{
		m_definition.group_blocks = definition->group_blocks;
	}

	if (definition->button_key_blocks.count > 0 && definition->button_key_blocks.address)
	{
		m_definition.button_key_blocks = definition->button_key_blocks;
	}

	if (definition->sound_override_reference.index != NONE && definition->sound_override_reference.index)
	{
		m_definition.sound_override_reference_index = definition->sound_override_reference.index;
	}

	if (definition->script_name[0])
	{
		csstrnzcpy(m_definition.script_name, definition->script_name, sizeof(m_definition.script_name));
	}
	else
	{
		m_definition.script_name[0] = 0;
	}

	m_definition.script_index = definition->script_index;
}

void c_gui_screen_widget::add_game_tag_parser(c_game_tag_parser* parser)
{
	//INVOKE_CLASS_MEMBER(0x00AB0800, c_gui_screen_widget, add_game_tag_parser, parser);

	if (!parser)
	{
		return;
	}

	if (m_game_tag_parser_count >= NUMBEROF(m_game_tag_parsers))
	{
		return;
	}

	user_interface_text_parser_register(parser, m_screen_index);
	m_game_tag_parsers[m_game_tag_parser_count++] = parser;
}

bool c_gui_screen_widget::button_key_to_controller_component(c_gui_text_widget* widget, e_controller_component* component)
{
	ASSERT(widget);

	switch (*widget->get_text())
	{
	case _unicode_private_font_icon_a_button:
		*component = _controller_component_button_a;
		return true;
	case _unicode_private_font_icon_b_button:
		*component = _controller_component_button_b;
		return true;
	case _unicode_private_font_icon_x_button:
		*component = _controller_component_button_x;
		return true;
	case _unicode_private_font_icon_y_button:
		*component = _controller_component_button_y;
		return true;
	case _unicode_private_font_icon_start_button:
		*component = _controller_component_button_start;
		return true;
	}

	return false;
}

bool c_gui_screen_widget::can_receive_focus() const
{
	//return INVOKE_CLASS_MEMBER(0x00AB0840, c_gui_screen_widget, can_receive_focus);

	return false;
}

void s_window_manager_screen_render_data::clear()
{
	//INVOKE_CLASS_MEMBER(0x00AB0850, s_window_manager_screen_render_data, clear);

	render_data_buffer_count = 0;
	current_count = 0;
}

void c_gui_screen_widget::clear_display_groups()
{
	//INVOKE_CLASS_MEMBER(0x00AB0870, c_gui_screen_widget, clear_display_groups);

	for (int32 display_group_index = 0; display_group_index < k_display_group_type_count; display_group_index++)
	{
		m_current_display_group_widgets[display_group_index] = nullptr;
		m_previous_display_group_widgets[display_group_index] = nullptr;
		m_current_display_group_indicies[display_group_index] = NONE;
	}
}

void c_gui_screen_widget::create_and_add_child_widgets_from_definition(s_runtime_screen_widget_definition* definition)
{
	INVOKE_CLASS_MEMBER(0x00AB08B0, c_gui_screen_widget, create_and_add_child_widgets_from_definition, definition);
}

void c_gui_screen_widget::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00AB0980, c_gui_screen_widget, dispose);

	c_gui_widget::dispose();
	m_render_data.dispose();
	c_gui_screen_widget::dispose_datasource();

	ASSERT(VALID_COUNT(m_game_tag_parser_count, NUMBEROF(m_game_tag_parsers)));
	for (int32 game_tag_index = 0; game_tag_index < m_game_tag_parser_count; game_tag_index++)
	{
		c_game_tag_parser* game_tag_parser = m_game_tag_parsers[game_tag_index];

		user_interface_text_parser_unregister(game_tag_parser, m_screen_index);
		ui_track_delete<c_game_tag_parser>(game_tag_parser);
		game_tag_parser = nullptr;
	}
	m_game_tag_parser_count = 0;
}

void s_window_manager_screen_render_data::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00AB0A10, s_window_manager_screen_render_data, dispose);

	if (render_data_buffer)
	{
		user_interface_free(render_data_buffer);

		// Bungie shouldn't this be nulled out to prevent use after free?
		//render_data_buffer = nullptr;
	}
}

void c_gui_screen_widget::dispose_child_screens()
{
	//INVOKE_CLASS_MEMBER(0x00AB0A20, c_gui_screen_widget, dispose_child_screens);

	for (c_gui_widget* child_widget = c_gui_widget::get_children(); child_widget; child_widget = child_widget->get_next())
	{
		if (child_widget->m_type != _gui_screen)
		{
			continue;
		}

		window_manager_get()->dispose_screen((c_gui_screen_widget*)child_widget);
	}
}

void c_gui_screen_widget::dispose_datasource()
{
	//INVOKE_CLASS_MEMBER(0x00AB0A90, c_gui_screen_widget, dispose_datasource);

	ASSERT(VALID_COUNT(m_datasource_count, NUMBEROF(m_datasource)));
	for (int32 datasource_index = 0; datasource_index < m_datasource_count; datasource_index++)
	{
		c_gui_data* datasource = m_datasource[datasource_index];

		datasource->dispose();
		ui_track_delete<c_gui_data>(datasource);
		datasource = nullptr;
	}
	m_datasource_count = 0;
}

//.text:00AB0B00 ; private: void c_gui_screen_widget::dispose_display_group(c_gui_group_widget*)

c_gui_list_item_widget* c_gui_screen_widget::find_list_item(int32 list_name, int32 list_column, int32 data_to_find)
{
	return INVOKE_CLASS_MEMBER(0x00AB0BC0, c_gui_screen_widget, find_list_item, list_name, list_column, data_to_find);
}

//.text:00AB0C50 ; 

c_gui_group_widget* c_gui_screen_widget::get_button_key_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB0C60, c_gui_screen_widget, get_button_key_widget);

	for (c_gui_widget* child_widget = c_gui_widget::get_children(); child_widget; child_widget = child_widget->get_next())
	{
		if (child_widget->m_type != _gui_button_key || !TEST_BIT(child_widget->get_core_definition()->flags, 6))
		{
			continue;
		}

		return (c_gui_group_widget*)child_widget;
	}
	return nullptr;
}

int32 c_gui_screen_widget::get_constants_datasource_integer(int32 constant_name)
{
	return INVOKE_CLASS_MEMBER(0x00AB0CD0, c_gui_screen_widget, get_constants_datasource_integer, constant_name);
}

s_runtime_core_widget_definition* c_gui_screen_widget::get_core_definition()
{
	//return INVOKE_CLASS_MEMBER(0x00AB0DA0, c_gui_screen_widget, get_core_definition);

	return &m_definition;
}

c_gui_data* c_gui_screen_widget::get_data(int32 name, int32* datasource_index)
{
	//return INVOKE_CLASS_MEMBER(0x00AB0DB0, c_gui_screen_widget, get_data, name, datasource_index);

	// $TODO cleanup this function

	if (datasource_index)
	{
		*datasource_index = NONE;
	}

	if (m_datasource_count <= 0)
	{
		return nullptr;
	}

	int32 index = 0;
	for (c_gui_data** datasource = m_datasource; (*datasource)->get_name() != name; datasource++)
	{
		if (++index >= m_datasource_count)
		{
			return nullptr;
		}
	}

	if (datasource_index)
	{
		*datasource_index = index;
	}

	return m_datasource[index];
}

int32 c_gui_screen_widget::get_datasource_count() const
{
	//return INVOKE_CLASS_MEMBER(0x00AB0E20, c_gui_screen_widget, get_datasource_count);

	return m_datasource_count;
}

//.text:00AB0E30 ; public: int32 c_gui_list_widget::get_datasource_name()
//.text:00AB0E40 ; public: void c_gui_screen_widget::get_datasources(c_gui_data** const, int32*)
//.text:00AB0E90 ; private: int32 c_gui_screen_widget::get_display_group_index(c_gui_screen_widget::e_display_group_type, int32)
//.text:00AB0F30 ; public: bool c_gui_group_widget::get_dispose_as_display_group() const

c_gui_widget* c_gui_screen_widget::get_focused_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00AB0F40, c_gui_screen_widget, get_focused_widget);

	if (m_suppress_focus)
	{
		return nullptr;
	}

	return m_current_focused_widget;
}

int32 c_gui_screen_widget::get_multilingual_unicode_string_list_index()
{
	//return INVOKE_CLASS_MEMBER(0x00AB0F60, c_gui_screen_widget, get_multilingual_unicode_string_list_index);

	return m_definition.string_list_tag_reference_index;
}

const char* c_gui_screen_widget::get_multilingual_unicode_string_list_tag_name()
{
	//return INVOKE_CLASS_MEMBER(0x00AB0F70, c_gui_screen_widget, get_multilingual_unicode_string_list_tag_name);

	if (m_definition.string_list_tag_reference_index == NONE)
	{
		return nullptr;
	}

	return tag_get_name_safe(m_definition.string_list_tag_reference_index);
}

//.text:00AB0F80 ; public: c_gui_group_widget* c_gui_screen_widget::get_primary_group_widget()

e_window_index c_gui_screen_widget::get_render_window()
{
	//return INVOKE_CLASS_MEMBER(0x00AB0FF0, c_gui_screen_widget, get_render_window);

	return window_manager_get()->get_render_window_for_screen(this);
}

bool c_gui_screen_widget::get_string_by_string_id(int32 string_identifier, c_static_wchar_string<1024>* buffer)
{
	return INVOKE_CLASS_MEMBER(0x00AB1000, c_gui_screen_widget, get_string_by_string_id, string_identifier, buffer);

	//ASSERT(buffer!=nullptr);
	//
	//if (string_identifier == k_string_id_empty_string)
	//{
	//	return false;
	//}
	//
	//if (string_identifier == _string_id_invalid)
	//{
	//	return false;
	//}
	//
	//int32 multilingual_unicode_string_list_index = get_multilingual_unicode_string_list_index();
	//if (multilingual_unicode_string_list_index == NONE)
	//{
	//	return false;
	//}
	//
	//string_list_get_interface_string(multilingual_unicode_string_list_index, string_identifier, buffer);
	//return true;
}

//.text:00AB1040 ; 
//.text:00AB1110 ; 

bool c_gui_screen_widget::handle_controller_alt_stick_attempt(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB11D0, c_gui_screen_widget, handle_controller_alt_stick_attempt, message);

	if (m_suppress_focus)
	{
		return false;
	}

	for (c_gui_widget* focused_widget = m_current_focused_widget; focused_widget; focused_widget = focused_widget->get_parent())
	{
		if (focused_widget->handle_alt_stick(message))
		{
			return true;
		}
	}

	return false;
}

bool c_gui_screen_widget::handle_controller_alt_tab_attempt(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB1240, c_gui_screen_widget, handle_controller_alt_tab_attempt, message);

	if (m_suppress_focus)
	{
		return false;
	}

	return m_current_focused_widget && m_current_focused_widget->handle_alt_tab(message);
}

bool c_gui_screen_widget::handle_controller_input_message(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB1270, c_gui_screen_widget, handle_controller_input_message, message);

	ASSERT(message != nullptr);

	if (c_gui_screen_widget* focused_widget = (c_gui_screen_widget*)m_current_focused_widget)
	{
		switch (message->get_event_type())
		{
		case _event_type_tab_up:
		case _event_type_tab_left:
		case _event_type_tab_down:
		case _event_type_tab_right:
		{
			if (c_gui_screen_widget::handle_controller_tab_attempt(message))
			{
				return true;
			}
		}
		break;
		case _event_type_alt_stick_up:
		case _event_type_alt_stick_left:
		case _event_type_alt_stick_down:
		case _event_type_alt_stick_right:
		{
			if (c_gui_screen_widget::handle_controller_alt_stick_attempt(message))
			{
				return true;
			}
		}
		break;
		case _event_type_alt_tab_up:
		case _event_type_alt_tab_left:
		case _event_type_alt_tab_down:
		case _event_type_alt_tab_right:
		{
			if (c_gui_screen_widget::handle_controller_alt_tab_attempt(message))
			{
				return true;
			}
		}
		break;
		case _event_type_button_press:
		{
			switch (message->get_component())
			{
			case _controller_component_button_a:
			{
				if (c_gui_screen_widget::handle_controller_selection_attempt(message))
				{
					return true;
				}
			}
			break;
			case _controller_component_button_b:
			{
				if (m_suppress_focus)
				{
					break;
				}

				if (m_current_focused_widget == this)
				{
					c_gui_widget::handle_widget_back_out();
					break;
				}

				if (m_current_focused_widget->handle_widget_back_out())
				{
					c_gui_widget::handle_widget_back_out();
					return true;
				}
			}
			break;
			case _controller_component_button_left_trigger:
			case _controller_component_button_right_trigger:
			{
				if (c_gui_screen_widget::handle_controller_tab_attempt(message))
				{
					return true;
				}
			}
			break;
			}
		}
		break;
		//case _event_type_mouse_click:
		//{
		//	if (c_gui_widget::transitioning_in() || c_gui_widget::transitioning_out())
		//	{
		//		break;
		//	}
		//
		//	point2d cursor_location{};
		//	user_interface_get_cursor_coords(&cursor_location);
		//
		//	c_gui_widget* new_hovered_widget = nullptr;
		//	c_gui_screen_widget::find_hovered_widget(&cursor_location, true, &new_hovered_widget);
		//	if (!new_hovered_widget)
		//	{
		//		c_gui_screen_widget::find_hovered_widget(&cursor_location, true, &new_hovered_widget);
		//		if (!new_hovered_widget)
		//		{
		//			break;
		//		}
		//	}
		//
		//	if (c_gui_screen_widget::handle_mouse_click(message, new_hovered_widget))
		//	{
		//		return true;
		//	}
		//}
		//break;
		}
	}

	if (message->get_event_type() != _event_type_button_press)
	{
		return false;
	}

	if (message->get_component() != _controller_component_button_b)
	{
		return false;
	}

	if (TEST_BIT(get_core_definition()->flags, 3))
	{
		return false;
	}

	transition_out_with_transition_type(_transition_out_back_out, _screen_transition_type_normal);
	return true;
}

bool c_gui_screen_widget::handle_controller_selection_attempt(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB1370, c_gui_screen_widget, handle_controller_selection_attempt, message);

	if (m_suppress_focus)
	{
		return false;
	}

	if (!m_current_focused_widget)
	{
		return false;
	}

	return m_current_focused_widget->handle_widget_selected() || handle_focused_widget_selected(message, m_current_focused_widget);
}

bool c_gui_screen_widget::handle_controller_tab_attempt(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB13C0, c_gui_screen_widget, handle_controller_tab_attempt, message);

	if (m_suppress_focus)
	{
		return false;
	}

	return m_current_focused_widget && m_current_focused_widget->handle_tab(message);
}

bool c_gui_screen_widget::handle_dialog_result(const c_dialog_result_message* dialog_result_message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB13F0, c_gui_screen_widget, handle_dialog_result, dialog_result_message);

	bool handled = false;

	if (dialog_result_message->get_screen_name() == STRING_ID(gui_dialog, need_to_select_storage_device_to_save))
	{
		if (dialog_result_message->get_dialog_result() == k_gui_dialog_choice_ok)
		{
			content_catalogue_display_device_selection_guide_interface(dialog_result_message->get_controller());
		}

		handled = true;
	}

	return handled;
}

bool c_gui_screen_widget::handle_focused_widget_selected(const c_controller_input_message* message, c_gui_widget* widget)
{
	return INVOKE_CLASS_MEMBER(0x00AB1420, c_gui_screen_widget, handle_focused_widget_selected, message, widget);
}

bool c_gui_screen_widget::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00AB14C0, c_gui_screen_widget, handle_list_item_chosen, message, list_name, list_item_widget, datasource);
	//return INVOKE_CLASS_MEMBER((uns32)__vftable->__funcs[49], c_gui_screen_widget, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	return false;
}

bool c_gui_screen_widget::handle_mouse_click(const c_controller_input_message* message, c_gui_widget* widget)
{
	switch (widget->m_type)
	{
	case _gui_text:
	{
		e_controller_component controller_component{};
		if (c_gui_screen_widget::button_key_to_controller_component((c_gui_text_widget*)widget, &controller_component))
		{
			c_controller_input_message* input_message = new (_ui_allocation_marker_dummy) c_controller_input_message(
				message->get_screen_name(),
				message->get_controller(),
				message->get_window(),
				_event_type_button_press,
				controller_component,
				0xFF);
			if (input_message)
			{
				if (c_gui_screen_widget::handle_controller_input_message(input_message))
				{
					ui_track_delete<c_controller_input_message>(input_message);
					return true;
				}
				else
				{
					user_interface_messaging_post(input_message);
					return true;
				}
			}
		}
	}
	break;
	case _gui_bitmap:
	{
		c_gui_list_widget* parent_list = widget->get_parent_list();
		if (parent_list)
		{
			e_event_type event_type = _event_type_none;
			if (widget == parent_list->m_more_items_preceeding_bitmap)
			{
				event_type = e_event_type(TEST_BIT(parent_list->m_definition.flags, 4) + _event_type_tab_up);
			}
			else if (widget == parent_list->m_more_items_following_bitmap)
			{
				event_type = e_event_type(TEST_BIT(parent_list->m_definition.flags, 4) + _event_type_tab_down);
			}

			if (event_type != _event_type_none)
			{
				c_controller_input_message* input_message = new (_ui_allocation_marker_dummy) c_controller_input_message(
					message->get_screen_name(),
					message->get_controller(),
					message->get_window(),
					event_type,
					_controller_component_button_left_thumb,
					0xFF);

				if (input_message)
				{
					if (c_gui_screen_widget::handle_controller_input_message(input_message))
					{
						return true;
					}
				}
			}
		}
	}
	break;
	}

	if (widget->can_receive_focus())
	{
		 c_gui_widget* current_focused_widget = c_gui_screen_widget::get_focused_widget();

		if (current_focused_widget != widget)
		{
			c_gui_list_widget* widget_parent = widget->get_parent_list();
			c_gui_list_widget* current_parent = m_current_focused_widget->get_parent_list();

			for (; current_parent && current_parent->m_is_submenu && current_parent != widget_parent;)
			{
				c_gui_list_widget* parent = current_parent->get_parent_list();
				parent->close_active_submenu(current_parent);
				current_parent = parent;
			}

			if (widget_parent)
			{
				int32 focused_element_handle = widget->get_element_handle();
				if (focused_element_handle < 0)
				{
					focused_element_handle = widget_parent->get_focused_element_handle();
				}

				c_gui_screen_widget::transfer_focus_to_list(widget_parent, focused_element_handle, false, false);
			}
			else
			{
				c_gui_screen_widget::transfer_focus(widget);
			}
		}
	}

	bool result = widget->handle_widget_selected();
	if (widget->can_receive_focus())
	{
		result |= c_gui_screen_widget::handle_focused_widget_selected(message, widget);
	}
	return result;
}

void c_gui_screen_widget::initialize()
{
	INVOKE_CLASS_MEMBER(0x00AB14D0, c_gui_screen_widget, initialize);

	//m_render_data.initialize(c_gui_screen_widget::get_render_data_size());
	//c_gui_screen_widget::reload_assets();
	//c_gui_widget::initialize();
	//c_gui_screen_widget::initialize_datasource();
}

bool s_window_manager_screen_render_data::initialize(int32 buffer_size)
{
	return INVOKE_CLASS_MEMBER(0x00AB1520, s_window_manager_screen_render_data, initialize, buffer_size);
}

//.text:00AB1560 ; public: virtual void c_gui_screen_codeless_widget::initialize_datasource()

void c_gui_screen_widget::initialize_datasource()
{
	//INVOKE_CLASS_MEMBER(0x00AB1650, c_gui_screen_widget, initialize_datasource);

	c_gui_screen_widget::dispose_datasource();

	for (int32 datasource_block_index = 0; datasource_block_index < m_definition.datasource_blocks.count; datasource_block_index++)
	{
		s_datasource_definition_block* datasource_definition_block = TAG_BLOCK_GET_ELEMENT(&m_definition.datasource_blocks, datasource_block_index, s_datasource_definition_block);
		if (!datasource_definition_block)
		{
			continue;
		}

		s_datasource_definition* datasource_definition = TAG_GET(GUI_DATASOURCE_DEFINITION_TAG, s_datasource_definition, datasource_definition_block->datasource_tag_reference.index);
		if (!datasource_definition)
		{
			continue;
		}

		int32 name = datasource_definition->name;
		if (name == _string_id_invalid || name == k_string_id_empty_string)
		{
			name = m_name;
		}

		c_gui_tag_datasource* datasource = new (_ui_allocation_marker_dummy) c_gui_tag_datasource(
			datasource_definition_block->datasource_tag_reference.index);
		if (datasource && datasource->initialize(name))
		{
			c_gui_screen_widget::add_datasource(datasource);
		}
		else
		{
			ui_track_delete<c_gui_tag_datasource>(datasource);
		}
	}
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

//.text:00AB17D0 ; private: c_gui_group_widget* c_gui_screen_widget::load_display_group(const s_runtime_screen_widget_definition*, c_gui_screen_widget::e_display_group_type, int32)

void c_gui_screen_widget::load_display_group_by_name(e_display_group_type group_type, int32 group_name)
{
	INVOKE_CLASS_MEMBER(0x00AB1830, c_gui_screen_widget, load_display_group_by_name, group_type, group_name);
}

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

	//if (const s_screen_widget_definition* screen_definition = get_screen_definition())
	//{
	//	if (const s_mouse_cursor_definition* mouse_cursor_definition = mouse_cursor_definition_get(screen_definition->mouse_cursor_tag.index))
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

bool c_gui_screen_widget::process_message(const c_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AB1BA0, c_gui_screen_widget, process_message, message);

	switch (message->get_type())
	{
	case _message_type_controller_input:
	{
		const c_controller_input_message* controller_input_message = (const c_controller_input_message*)message;
		if (handle_controller_input_message(controller_input_message))
		{
			c_window_manager::play_controller_sound(
				m_definition.sound_override_reference_index,
				controller_input_message->get_event_type(),
				controller_input_message->get_component());

			return true;
		}
	}
	break;
	case _message_type_dialog_result:
	{
		const c_dialog_result_message* dialog_result_message = (const c_dialog_result_message*)message;
		return handle_dialog_result(dialog_result_message);
	}
	break;
	}

	return false;
}

void c_gui_screen_widget::reconstruct()
{
	//INVOKE_CLASS_MEMBER(0x00AB1C10, c_gui_screen_widget, reconstruct);

	csmemset(&m_animated_state, 0, sizeof(m_animated_state));
	m_current_focused_widget = nullptr;
	c_gui_screen_widget::clear_display_groups();
	c_gui_screen_widget::dispose_child_screens();
	c_gui_widget::delete_all_children();
}

//.text:00AB1C80 ; private: void c_gui_screen_widget::recover_from_tag_changes()

void c_gui_screen_widget::reload_assets()
{
	//INVOKE_CLASS_MEMBER(0x00AB1CE0, c_gui_screen_widget, reload_assets);

	int32 widget_identifier = m_definition.widget_identifier;
	int32 screen_definition_index = window_manager_get()->locate_screen_definition_tag_by_name(widget_identifier);
	c_gui_widget::delete_all_children();
	s_runtime_screen_widget_definition baseline_definition{};
	m_definition = baseline_definition;
	m_definition.widget_identifier = widget_identifier;
	c_gui_screen_widget::clear_display_groups();
	if (screen_definition_index == NONE)
	{
		if (m_definition.widget_identifier == STRING_ID(gui, message))
		{
			c_motd_popup_screen_widget* motd_popup_screen_widget = (c_motd_popup_screen_widget*)this;
			motd_popup_screen_widget->setup_screen_in_code();
		}
	}
	else
	{
		bool was_templated = false;
		s_screen_widget_definition* screen_widget_definition = TAG_GET(GUI_SCREEN_WIDGET_DEFINITION_TAG, s_screen_widget_definition, screen_definition_index);
		if (screen_widget_definition->template_reference.index != NONE)
		{
			s_screen_widget_definition* template_screen_widget_definition = TAG_GET(GUI_SCREEN_WIDGET_DEFINITION_TAG, s_screen_widget_definition, screen_widget_definition->template_reference.index);
			c_gui_screen_widget::add_definition_fields(template_screen_widget_definition, false);
			was_templated = true;
		}
		c_gui_screen_widget::add_definition_fields(screen_widget_definition, was_templated);
	}
	m_name = m_definition.widget_identifier;
	c_gui_screen_widget::reposition_for_splitscreen();
	c_gui_screen_widget::create_and_add_child_widgets_from_definition(&m_definition);
}

void c_gui_screen_widget::reload_next_frame()
{
	//INVOKE_CLASS_MEMBER(0x00AB1DF0, c_gui_screen_widget, reload_next_frame);

	m_reload_next_frame = true;
}

//.text:00AB1E00 ; protected: void c_gui_screen_widget::remove_datasource(c_gui_data*)

void c_gui_screen_widget::reposition_for_splitscreen()
{
	INVOKE_CLASS_MEMBER(0x00AB1E80, c_gui_screen_widget, reposition_for_splitscreen);
}

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

void c_gui_screen_widget::set_responds_to_controller_events(bool value)
{
	m_responds_to_controller_events = value;
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

void c_gui_screen_widget::set_text_widget_string_id_from_focused_list_item(int32 text_widget_name, int32 list_name, int32 column_name)
{
	INVOKE_CLASS_MEMBER(0x00AB23A0, c_gui_screen_widget, set_text_widget_string_id_from_focused_list_item, text_widget_name, list_name, column_name);
}

void c_gui_screen_widget::set_text_widget_text(int32 widget_name, const wchar_t* widget_text)
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

	// $IMPLEMENT

	//c_gui_data* data = list_widget->get_data();
	//if (!data)
	//{
	//	return;
	//}
	//
	//if (data->contains(element_handle))
	//{
	//	element_handle = data->get_first_element_handle();
	//}
	//
	//if (element_handle == NONE)
	//{
	//	return;
	//}
}

//.text:00AB2740 ; 

void c_gui_screen_widget::transfer_focus_without_animations(c_gui_widget* new_focused_widget, bool play_received_animation, bool play_lost_animation)
{
	//INVOKE_CLASS_MEMBER(0x00AB27D0, c_gui_screen_widget, transfer_focus_without_animations, new_focused_widget, play_received_animation, play_lost_animation);

	ASSERT(new_focused_widget != nullptr);

	if (!new_focused_widget->can_receive_focus())
	{
		event(_event_verbose, "ui: tried to give focus to an invalid child widget (type= %s, name= %s)",
			widget_type_to_string(new_focused_widget->m_type),
			c_gui_widget::widget_name_to_string(new_focused_widget->m_name));
		return;
	}
	if (play_received_animation)
	{
		new_focused_widget->start_animation(_control_received_focus, true);
	}
	if (play_lost_animation)
	{
		m_current_focused_widget->start_animation(_control_lost_focus, true);
	}
	m_current_focused_widget = new_focused_widget;
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

void __cdecl c_gui_screen_scoreboard::translate_widget_recursive(c_gui_widget* widget, int32 x, int32 y)
{
	//INVOKE(0x00AB2870, c_gui_screen_scoreboard::translate_widget_recursive, widget, x, y);

	s_animation_transform transform = widget->m_animated_state;
	transform.position.x += (real32)x;
	transform.position.y += (real32)y;
	widget->set_full_animation_state(&transform, false);

	for (c_gui_widget* child_widget = widget->get_children(); child_widget; child_widget = child_widget->get_next())
	{
		c_gui_screen_scoreboard::translate_widget_recursive(child_widget, x, y);
	}
}

bool c_gui_screen_widget::try_and_get_render_data_emblem_info(c_gui_bitmap_widget* bitmap_widget, s_emblem_info* emblem_info)
{
	//return INVOKE_CLASS_MEMBER(0x00AB2930, c_gui_screen_widget, try_and_get_render_data_emblem_info, bitmap_widget, emblem_info);

	return false;
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

bool s_window_manager_screen_render_data::valid() const
{
	//INVOKE_CLASS_MEMBER(0x00AB29B0, s_window_manager_screen_render_data, valid);

	return render_data_buffer != nullptr;
}

