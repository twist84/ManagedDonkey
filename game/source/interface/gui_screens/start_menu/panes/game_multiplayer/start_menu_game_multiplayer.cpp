#include "interface/gui_screens/start_menu/panes/game_multiplayer/start_menu_game_multiplayer.hpp"

#include "game/game.hpp"
#include "game/game_engine_team.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/gui_custom_bitmap_widget.hpp"
#include "interface/user_interface_controller.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AE9C10, c_start_menu_game_multiplayer, submenu_invoked_);

void __thiscall c_start_menu_game_multiplayer::submenu_invoked_(c_gui_list_widget* submenu_widget)
{
	c_start_menu_game_multiplayer::submenu_invoked(submenu_widget);
}

c_start_menu_game_multiplayer::c_start_menu_game_multiplayer(int32 name) :
	c_start_menu_pane_screen_widget(name),
	m_gametype(),
	m_gametype_description()
{
	//DECLFUNC(0x00AE9080, void, __thiscall, c_start_menu_game_multiplayer*, int32)(this, name);

	set_automatic_button_key(STRING_ID(gui, a_confirm_b_cancel));
}

//.text:00AE90B0 ; public: virtual c_start_menu_game_multiplayer::~c_start_menu_game_multiplayer()
//.text:00AE90C0 ; public: virtual void* c_start_menu_game_multiplayer::`scalar deleting destructor'(unsigned int)

bool c_start_menu_game_multiplayer::allow_pane_tab_change()
{
	//return INVOKE_CLASS_MEMBER(0x00AE90F0, c_start_menu_game_multiplayer, allow_pane_tab_change);

	return true;
}

//.text:00AE9100 ; public: static void __cdecl c_start_menu_game_multiplayer::change_team(e_controller_index, e_game_team)

c_gui_bitmap_widget* c_start_menu_game_multiplayer::create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00AE9150, c_start_menu_game_multiplayer, create_bitmap_widget, definition);

	if (definition->widget_identifier == STRING_ID(gui, map_image))
	{
		return UI_MALLOC(c_gui_custom_bitmap_widget);
	}

	return c_gui_widget::create_bitmap_widget(definition);
}

//.text:00AE91D0 ; public: const wchar_t* c_start_menu_game_multiplayer::get_hopper_description() const
//.text:00AE91E0 ; public: const wchar_t* c_start_menu_game_multiplayer::get_hopper_name() const

bool c_start_menu_game_multiplayer::handle_dialog_result(const c_dialog_result_message* message)
{
	return INVOKE_CLASS_MEMBER(0x00AE91F0, c_start_menu_game_multiplayer, handle_dialog_result, message);
}

void c_start_menu_game_multiplayer::handle_leave_game_choice(c_start_menu_pane_screen_widget* screen, const c_controller_input_message* message)
{
	//INVOKE(0x00AE9310, c_start_menu_game_multiplayer::handle_leave_game_choice, screen, message);
	//DECLFUNC(0x00AE9310, void, __cdecl, c_start_menu_pane_screen_widget*, const c_controller_input_message*)(screen, message);

	c_start_menu_game_multiplayer::handle_leave_game_choice(screen, message->get_controller());
}

void c_start_menu_game_multiplayer::handle_leave_game_choice(c_start_menu_pane_screen_widget* screen, e_controller_index controller_index)
{
	//INVOKE(0x00AE9330, c_start_menu_game_multiplayer::handle_leave_game_choice, screen, controller_index);
	DECLFUNC(0x00AE9330, void, __cdecl, c_start_menu_pane_screen_widget*, e_controller_index)(screen, controller_index);
}

bool c_start_menu_game_multiplayer::handle_leave_game_response(c_start_menu_pane_screen_widget* screen, const c_dialog_result_message* dialog_result_message)
{
	return INVOKE(0x00AE94E0, c_start_menu_game_multiplayer::handle_leave_game_response, screen, dialog_result_message);
}

bool c_start_menu_game_multiplayer::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return INVOKE_CLASS_MEMBER(0x00AE95F0, c_start_menu_game_multiplayer, handle_list_item_chosen, message, list_name, list_item_widget, datasource);
}

void c_start_menu_game_multiplayer::initialize()
{
	INVOKE_CLASS_MEMBER(0x00AE9850, c_start_menu_game_multiplayer, initialize);
}

void c_start_menu_game_multiplayer::initialize_datasource()
{
	//INVOKE_CLASS_MEMBER(0x00AE99E0, c_start_menu_game_multiplayer, initialize_datasource);

	c_gui_screen_widget::initialize_datasource();

	c_gui_data* sidebar_items_data = c_gui_screen_widget::get_data(STRING_ID(gui, sidebar_items), NULL);
	if (sidebar_items_data)
	{
		sidebar_items_data->clear_disabled_elements();

		if (!is_change_teams_allowed(c_gui_widget::get_single_responding_controller()))
		{
			sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(global, change_teams));
		}

		if (!user_interface_squad_local_peer_is_leader() || user_interface_squad_in_matchmaking())
		{
			sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, select_network_mode));
			sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(global, end_game));
		}

		if (user_interface_squad_get_player_count() <= user_interface_squad_get_local_player_count())
		{
			sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(global, leave_game));
		}

		sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, select_network_mode));

		if (!user_interface_squad_local_peer_is_leader())
		{
			sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui_dialog, in_game_change_network_privacy));
		}

		if (user_interface_squad_get_session_class() != _network_session_class_xbox_live || user_interface_squad_get_ui_game_mode() == _ui_game_mode_matchmaking)
		{
			sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui_dialog, in_game_change_network_privacy));
		}

		if (!should_display_editor_elements())
		{
			sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, save_map));
			sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, save_map_as));
		}
	}
}

bool __cdecl c_start_menu_pane_screen_widget::is_change_teams_allowed(e_controller_index controller_index)
{
	return INVOKE(0x00AE9B00, c_start_menu_pane_screen_widget::is_change_teams_allowed, controller_index);
}

bool __cdecl parse_game_description(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00AE9BA0, parse_game_description, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_game_type(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00AE9BC0, parse_game_type, this_ptr, buffer, buffer_length);
}

void c_start_menu_pane_screen_widget::set_automatic_button_key(int32 value)
{
	//INVOKE_CLASS_MEMBER(0x00AE9BE0, c_start_menu_pane_screen_widget, set_automatic_button_key, value);

	m_automatic_button_key = value;
}

bool __cdecl should_display_editor_elements()
{
	//return INVOKE(0x00AE9BF0, should_display_editor_elements);

	return current_game_engine()->get_type() == _game_engine_type_sandbox;
}

void c_start_menu_game_multiplayer::submenu_invoked(c_gui_list_widget* submenu_widget)
{
	//INVOKE_CLASS_MEMBER(0x00AE9C10, c_start_menu_game_multiplayer, submenu_invoked, submenu_widget);

	ASSERT(VALID_INDEX(get_single_responding_controller(), k_number_of_controllers));

	c_gui_screen_widget::submenu_invoked(submenu_widget);

	int32 value_to_set = NONE;

	switch (submenu_widget->m_name)
	{
	case STRING_ID(global, change_teams):
		value_to_set = user_interface_controller_get_team_index(c_gui_widget::get_single_responding_controller());
		break;
	}

	if (value_to_set != NONE)
	{
		submenu_widget->set_focused_datasource_element_from_value(STRING_ID(global, value), value_to_set, false);
	}
}

void c_start_menu_game_multiplayer::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00AE9C60, c_start_menu_game_multiplayer, update, current_milliseconds);

	c_start_menu_pane_screen_widget::update(current_milliseconds);

	if (c_gui_ordered_data* datasource = (c_gui_ordered_data*)c_gui_screen_widget::get_data(STRING_ID(gui, teams), NULL))
	{
		const c_game_variant* variant = user_interface_game_settings_get_game_variant();

		e_map_id map_id = _map_id_none;
		e_campaign_id campaign_id = _campaign_id_none;
		if (user_interface_session_get_map(&campaign_id, &map_id) && variant)
		{
			ASSERT(campaign_id == _campaign_id_none);
			ASSERT(map_id != _map_id_none);

			datasource->clear_disabled_elements();

			uns16 available_teams = game_engine_get_available_teams();
			for (int32 element_handle = datasource->get_first_element_handle();
				element_handle != NONE;
				element_handle = datasource->get_next_element_handle(element_handle))
			{
				e_game_team team_index;
				if (datasource->get_integer_value(element_handle, STRING_ID(global, value), (int32*)&team_index) && (!TEST_BIT(available_teams, team_index) ||
					team_index == k_observer_team && !game_engine_variant_is_observer_allowed(variant)))
				{
					int32 value = NONE;
					if (datasource->get_string_id_value(element_handle, STRING_ID(global, name), &value))
					{
						datasource->set_disabled_element(STRING_ID(global, name), value);
					}
				}
			}
		}
	}

	if (c_gui_custom_bitmap_widget* map_image_bitmap = (c_gui_custom_bitmap_widget*)c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, map_image)))
	{
		if (game_options_valid())
		{
			map_image_bitmap->set_map_image(c_gui_custom_bitmap_widget::_custom_map_image_type_normal, game_options_get()->map_id, true);
		}
	}
}

