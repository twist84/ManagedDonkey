#include "interface/gui_screens/start_menu/panes/settings_appearance_model/start_menu_settings_appearance_model.hpp"

#include "interface/c_controller.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_model_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "interface/user_interface_utilities.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00AF94C0, parse_xml_part_count_current);
HOOK_DECLARE(0x00AF9570, parse_xml_part_count_total);
HOOK_DECLARE(0x00AF9600, parse_xml_part_description);

enum e_customization_area
{
	_customization_area_helmet = 0,
	_customization_area_leftshoulder = 1,
	_customization_area_rightshoulder = 2,
	_customization_area_body = 3,
};

c_start_menu_settings_appearance_model::c_start_menu_settings_appearance_model(int32 name) :
	c_start_menu_pane_screen_widget(name)
{
	m_automatic_button_key = _string_id_gui__a_confirm_b_cancel;
}

void c_start_menu_settings_appearance_model::add_model_customization_selections_datasource(int32 area_name, e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00AF8AA0, c_start_menu_settings_appearance_model, add_model_customization_selections_datasource, area_name, controller_index);
}

bool c_start_menu_settings_appearance_model::handle_controller_input_message(const c_controller_input_message* message)
{
	e_controller_index controller_index = message->get_controller();
	if (message->get_event_type() == _event_type_button_press && message->get_component() == _controller_component_button_a)
	{
		c_gui_widget* focused_widget = c_gui_screen_widget::get_focused_widget();
		if (focused_widget && focused_widget->m_type == _gui_list_item)
		{
			c_gui_list_widget* parent_list = focused_widget->get_parent_list();
			if (parent_list)
			{
				c_gui_data* data = parent_list->get_data();
				if (data)
				{
					int32 selected_item_name = NONE;
					if (!VALID_CONTROLLER(controller_index)
						&& !data->get_string_id_value(parent_list->get_focused_element_handle(), STRING_ID(global, name), &selected_item_name))
					{
						c_gui_screen_widget::play_sound(_ui_global_sound_effect_failure);
					}
					else
					{
						c_controller_interface* controller = controller_get(controller_index);
						c_player_profile_interface* player_profile = controller->get_player_profile_interface();

						int32 integer_value = NONE;
						if (data->get_integer_value(parent_list->get_focused_element_handle(), STRING_ID(global, value), &integer_value))
						{
							int8 selection_index = (int8)integer_value;

							int32 area_name = parent_list->get_name();
							switch (area_name)
							{
							case STRING_ID(global, body):
							{
								e_player_model_choice player_model_choice = player_profile->get_player_model_choice();
								player_profile->set_model_customization_selection(player_model_choice, _customization_area_body, selection_index, true);
							}
							break;
							case STRING_ID(global, model):
							{
								player_profile->set_player_model_choice((e_player_model_choice)integer_value, true);
							}
							break;
							default:
							{
								if (area_name == string_id_retrieve("helmet"))
								{
									e_player_model_choice player_model_choice = player_profile->get_player_model_choice();
									player_profile->set_model_customization_selection(player_model_choice, _customization_area_helmet, selection_index, true);
								}
								else if (area_name == string_id_retrieve("leftshoulder"))
								{
									e_player_model_choice player_model_choice = player_profile->get_player_model_choice();
									player_profile->set_model_customization_selection(player_model_choice, _customization_area_leftshoulder, selection_index, true);
								}
								else if (area_name == string_id_retrieve("rightshoulder"))
								{
									e_player_model_choice player_model_choice = player_profile->get_player_model_choice();
									player_profile->set_model_customization_selection(player_model_choice, _customization_area_rightshoulder, selection_index, true);
								}
							}
							break;
							}

							c_start_menu_settings_appearance_model::update_model_widget(true);
							focused_widget->handle_widget_back_out();

							return true;
						}
					}
				}
			}
		}
	}

	return c_start_menu_pane_screen_widget::handle_controller_input_message(message);
}

void c_start_menu_settings_appearance_model::initialize()
{
	c_gui_screen_widget::initialize();

	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<part-description", this, parse_xml_part_description));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<part-count-current", this, parse_xml_part_count_current));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<part-count-total", this, parse_xml_part_count_total));
}

void c_start_menu_settings_appearance_model::initialize_datasource()
{
	c_gui_screen_widget::initialize_datasource();

	e_controller_index controller_index = c_gui_widget::get_single_responding_controller();
	if (!VALID_CONTROLLER(controller_index))
	{
		return;
	}

	c_controller_interface* controller = controller_get(controller_index);
	if (!controller->in_use())
	{
		return;
	}

	c_player_profile_interface* player_profile = controller->get_player_profile_interface();

	int32 datasource_index = NONE;
	if (c_gui_data* data = c_gui_screen_widget::get_data(STRING_ID(gui, sidebar_items), &datasource_index))
	{
		// $IMPLEMENT `c_settings_appearance_model_sidebar_items_datasource`

		//c_settings_appearance_model_sidebar_items_datasource* sidebar_items_datasource = UI_MALLOC(c_settings_appearance_model_sidebar_items_datasource,
		//	data,
		//	player_profile,
		//	this);
	}

	c_start_menu_settings_appearance_model::add_model_customization_selections_datasource(string_id_retrieve("helmet"), controller_index);
	c_start_menu_settings_appearance_model::add_model_customization_selections_datasource(string_id_retrieve("leftshoulder"), controller_index);
	c_start_menu_settings_appearance_model::add_model_customization_selections_datasource(string_id_retrieve("rightshoulder"), controller_index);
	c_start_menu_settings_appearance_model::add_model_customization_selections_datasource(STRING_ID(global, body), controller_index);
}

void c_start_menu_settings_appearance_model::submenu_invoked(c_gui_list_widget* submenu_widget)
{
	e_controller_index controller_index = c_gui_widget::get_single_responding_controller();
	if (!VALID_CONTROLLER(controller_index))
	{
		return;
	}

	c_controller_interface* controller = controller_get(controller_index);
	if (!controller->in_use())
	{
		return;
	}

	c_player_profile_interface* player_profile = controller->get_player_profile_interface();

	c_gui_screen_widget::submenu_invoked(submenu_widget);

	int32 model_customization_selection = get_current_model_customization_value(player_profile, submenu_widget->get_name());
	if (model_customization_selection != NONE)
	{
		c_start_menu_pane_screen_widget::set_focused_datasource_element_from_value(
			submenu_widget,
			STRING_ID(global, value),
			model_customization_selection,
			false);
	}
}

void c_start_menu_settings_appearance_model::update(uns32 current_milliseconds)
{
	c_start_menu_pane_screen_widget::update(current_milliseconds);

	c_gui_text_widget* part_count_text = c_gui_widget::get_child_text_widget(STRING_ID(gui, part_count));
	c_start_menu_settings_appearance_model::update_model_widget(false);
	if (part_count_text)
	{
		bool visible = true;

		int32 area_name = NONE;
		int32 current_selection_index = NONE;
		if (get_current_model_selection_area_and_item_index(this, &area_name, &current_selection_index))
		{
			visible = area_name != STRING_ID(global, model);
		}
		part_count_text->set_visible(visible);
	}
}

void c_start_menu_settings_appearance_model::update_model_widget(bool force_update)
{
	int32 area_name = NONE;
	int32 current_selection_index = NONE;
	if (!get_current_model_selection_area_and_item_index(this, &area_name, &current_selection_index))
	{
		return;
	}

	c_gui_model_widget* model_widget = NULL;
	if (c_gui_widget* group_widget = c_gui_widget::get_first_child_widget_by_type(_gui_group))
	{
		model_widget = (c_gui_model_widget*)group_widget->get_first_child_widget_by_type(_gui_model);
	}

	c_gui_data* data = c_gui_screen_widget::get_data(area_name, NULL);
	if (!model_widget || !data)
	{
		return;
	}

	// $TODO
	//model_widget->reset_player_appearance();
	//s_player_appearance player_appearance = model_widget->get_player_appearance();

	int32 chosen_permutation = NONE;
	int32 chosen_permutation_name = NONE;
	int32 first_permutation = NONE;
	if (data->get_integer_value(current_selection_index, STRING_ID(global, value), &chosen_permutation)
		&& data->get_integer_value(current_selection_index, STRING_ID(global, name), &chosen_permutation_name)
		&& data->get_integer_value(current_selection_index, STRING_ID(global, value), &first_permutation))
	{
		if (chosen_permutation_name == STRING_ID(global, locked))
		{
			chosen_permutation = first_permutation;
		}

		switch (area_name)
		{
		case STRING_ID(global, body):
		{
			// $IMPLEMENT
			//player_appearance.model_permutations[(e_player_model_choice)player_appearance.player_model_choice][_customization_area_body] = 0;
		}
		break;
		case STRING_ID(global, model):
		{
			// $IMPLEMENT
			//player_appearance.player_model_choice = _player_model_choice_spartan;
		}
		break;
		default:
		{
			if (area_name == string_id_retrieve("helmet"))
			{
				// $IMPLEMENT
				//player_appearance.model_permutations[(e_player_model_choice)player_appearance.player_model_choice][_customization_area_helmet] = 0;
			}
			else if (area_name == string_id_retrieve("leftshoulder"))
			{
				// $IMPLEMENT
				//player_appearance.model_permutations[(e_player_model_choice)player_appearance.player_model_choice][_customization_area_leftshoulder] = 0;
			}
			else if (area_name == string_id_retrieve("rightshoulder"))
			{
				// $IMPLEMENT
				//player_appearance.model_permutations[(e_player_model_choice)player_appearance.player_model_choice][_customization_area_rightshoulder] = 0;
			}
		}
		break;
		}
	}

	// $TODO
	//model_widget->set_player_appearance(&player_appearance);
}

void c_start_menu_settings_appearance_model::update_render_state(uns32 current_milliseconds)
{
	e_controller_index controller_index = c_gui_widget::get_single_responding_controller();
	c_controller_interface* controller = controller_get(controller_index);
	c_player_profile_interface* player_profile = controller->get_player_profile_interface();

	e_player_color_index primary_change_color = player_profile->get_primary_change_color();

	c_gui_widget::update_render_state(current_milliseconds);
	for (c_gui_bitmap_widget* base_color_bitmap = c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, base_color));
		base_color_bitmap != NULL;
		base_color_bitmap = base_color_bitmap->get_next_bitmap_widget())
	{
		if (base_color_bitmap->get_name() == STRING_ID(gui, base_color))
		{
			tint_widget_to_change_color(base_color_bitmap, primary_change_color, false);
		}
	}
}

static int32 get_current_model_customization_value(const c_player_profile_interface* player_profile, int32 area_name)
{
	int32 result = NONE;
	switch (area_name)
	{
	case STRING_ID(global, body):
	{
		e_player_model_choice player_model_choice = player_profile->get_player_model_choice();
		result = player_profile->get_model_customization_selection(player_model_choice, _customization_area_body);
	}
	break;
	case STRING_ID(global, model):
	{
		e_player_model_choice player_model_choice = player_profile->get_player_model_choice();
		result = player_model_choice;
	}
	break;
	default:
	{
		if (area_name == string_id_retrieve("helmet"))
		{
			e_player_model_choice player_model_choice = player_profile->get_player_model_choice();
			result = player_profile->get_model_customization_selection(player_model_choice, _customization_area_helmet);
		}
		else if (area_name == string_id_retrieve("leftshoulder"))
		{
			e_player_model_choice player_model_choice = player_profile->get_player_model_choice();
			result = player_profile->get_model_customization_selection(player_model_choice, _customization_area_leftshoulder);
		}
		else if (area_name == string_id_retrieve("rightshoulder"))
		{
			e_player_model_choice player_model_choice = player_profile->get_player_model_choice();
			result = player_profile->get_model_customization_selection(player_model_choice, _customization_area_rightshoulder);
		}
	}
	break;
	}
	return result;
}

static bool get_current_model_selection_area_and_item_index(c_gui_screen_widget* screen_widget, int32* area_name, int32* current_selection_index)
{
	c_gui_list_widget* child_list_widget = screen_widget->get_child_list_widget(STRING_ID(gui, sidebar));
	c_gui_widget* focused_widget = screen_widget->get_focused_widget();
	if (!child_list_widget || !focused_widget)
	{
		return false;
	}

	c_gui_list_widget* parent_list = focused_widget->get_parent_list();
	if (child_list_widget->m_submenu_item && parent_list && parent_list->m_is_submenu)
	{
		*area_name = parent_list->m_name;
		*current_selection_index = parent_list->get_focused_item_index();
		return true;
	}
	c_gui_data* data = child_list_widget->get_data();
	int32 focused_element_handle = child_list_widget->get_focused_element_handle();
	if (!data || !data->get_string_id_value(focused_element_handle, STRING_ID(global, name), area_name))
	{
		return false;
	}

	e_controller_index controller_index = screen_widget->get_single_responding_controller();
	if (!VALID_CONTROLLER(controller_index))
	{
		return false;
	}

	c_controller_interface* controller = controller_get(controller_index);
	if (!controller || !controller->in_use())
	{
		return false;
	}

	c_player_profile_interface* player_profile = controller->get_player_profile_interface();
	int32 current_model_customization_value = get_current_model_customization_value(player_profile, *area_name);
	if (*area_name == NONE || *area_name <= k_string_id_empty_string)
	{
		return false;
	}

	c_gui_data* area_name_data = screen_widget->get_data(*area_name, NULL);
	if (!area_name_data)
	{
		return false;
	}

	for (int32 element_handle = area_name_data->get_first_element_handle();
		element_handle != NONE;
		element_handle = area_name_data->get_next_element_handle(element_handle))
	{
		int32 possible_value = NONE;
		if (area_name_data->get_integer_value(element_handle, STRING_ID(global, value), &possible_value)
			&& possible_value == current_model_customization_value)
		{
			*current_selection_index = element_handle;
			return true;
		}
	}

	return false;
}

bool __cdecl parse_xml_part_count_current(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	ASSERT(this_ptr);
	ASSERT(buffer != NULL);
	ASSERT(buffer_length > 0);

	c_gui_screen_widget* screen_widget = (c_gui_screen_widget*)this_ptr;

	int32 area_name = NONE;
	int32 current_selection_index = NONE;
	if (!screen_widget || !get_current_model_selection_area_and_item_index(screen_widget, &area_name, &current_selection_index))
	{
		return false;
	}

	c_static_wchar_string<256> text_buffer;
	text_buffer.print(L"%d", (current_selection_index + 1));
	text_buffer.copy_to(buffer, buffer_length);

	return true;
}

bool __cdecl parse_xml_part_count_total(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	ASSERT(this_ptr);
	ASSERT(buffer != NULL);
	ASSERT(buffer_length > 0);

	c_gui_screen_widget* screen_widget = (c_gui_screen_widget*)this_ptr;

	int32 area_name = NONE;
	int32 current_selection_index = NONE;
	if (!screen_widget || !get_current_model_selection_area_and_item_index(screen_widget, &area_name, &current_selection_index))
	{
		return false;
	}

	c_gui_data* data = screen_widget->get_data(area_name, NULL);
	if (!data)
	{
		return false;
	}

	c_static_wchar_string<256> text_buffer;
	text_buffer.print(L"%d", data->get_current_item_count());
	text_buffer.copy_to(buffer, buffer_length);

	return true;
}

bool __cdecl parse_xml_part_description(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	ASSERT(this_ptr);
	ASSERT(buffer != NULL);
	ASSERT(buffer_length > 0);

	c_gui_screen_widget* screen_widget = (c_gui_screen_widget*)this_ptr;

	int32 area_name = NONE;
	int32 current_selection_index = NONE;
	if (!screen_widget || !get_current_model_selection_area_and_item_index(screen_widget, &area_name, &current_selection_index))
	{
		return false;
	}

	c_gui_data* data = screen_widget->get_data(area_name, NULL);
	if (!data)
	{
		return false;
	}

	int32 description = NONE;
	if (!data->get_string_id_value(current_selection_index, STRING_ID(global, description), &description))
	{
		return false;
	}

	if (description == NONE || description <= k_string_id_empty_string)
	{
		return false;
	}

	c_static_wchar_string<1024> text_buffer;
	if (!screen_widget->get_string_by_string_id(description, &text_buffer))
	{
		return false;
	}

	text_buffer.print(L"%d", data->get_current_item_count());
	text_buffer.copy_to(buffer, buffer_length);

	return true;
}

