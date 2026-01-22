#include "interface/gui_screens/start_menu/panes/game_campaign/start_menu_game_campaign.hpp"

#include "cseries/cseries_events.hpp"
#include "game/campaign_metagame.hpp"
#include "game/game.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/gui_custom_bitmap_widget.hpp"
#include "interface/gui_screens/start_menu/panes/game_multiplayer/start_menu_game_multiplayer.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_text.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "simulation/simulation_queue_global_events.hpp"

c_start_menu_game_campaign::c_start_menu_game_campaign(int32 name) :
	c_start_menu_pane_screen_widget(name)
{
	//DECLFUNC(0x00AE86E0, void, __thiscall, c_start_menu_game_campaign*, int32)(this, name);
}

//.text:00AE8700 ; public: virtual c_start_menu_game_campaign::~c_start_menu_game_campaign()
//.text:00AE8710 ; 
//.text:00AE8720 ; public: virtual void* c_start_menu_game_campaign::`scalar deleting destructor'(unsigned int)

bool c_start_menu_game_campaign::allow_pane_tab_change()
{
	//return INVOKE_CLASS_MEMBER(0x00AE8750, c_start_menu_game_campaign, allow_pane_tab_change);

	return true;
}

c_gui_bitmap_widget* c_start_menu_game_campaign::create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00AE8760, c_start_menu_game_campaign, create_bitmap_widget, definition);

	c_gui_bitmap_widget* bitmap_widget = NULL;
	if (definition->widget_identifier == STRING_ID(gui, map_image))
	{
		bitmap_widget = new (_ui_allocation_marker_dummy) c_gui_custom_bitmap_widget();
	}
	else
	{
		bitmap_widget = c_gui_widget::create_bitmap_widget(definition);
	}
	return bitmap_widget;
}

bool c_start_menu_game_campaign::handle_dialog_result(const c_dialog_result_message* dialog_result_message)
{
	//return INVOKE_CLASS_MEMBER(0x00AE87E0, c_start_menu_game_campaign, handle_dialog_result, message);

	bool handled = c_start_menu_game_multiplayer::handle_leave_game_response(this, dialog_result_message);

	if (!handled)
	{
		string_id dialog_name = dialog_result_message->get_dialog_name();
		switch (dialog_name)
		{
		case STRING_ID(gui_dialog, in_game_end_game):
		case STRING_ID(gui_dialog, in_campaign_save_and_quit):
		case STRING_ID(gui_dialog, in_coop_end_game):
		{
			if (dialog_result_message->get_dialog_result() == k_gui_dialog_choice_ok)
			{
				window_manager_get()->set_fade_out_and_quit_campaign(true, dialog_name == STRING_ID(gui_dialog, in_campaign_save_and_quit));
				c_start_menu_pane_screen_widget::close_start_menu0();
			}

			handled = true;
		}
		break;
		case STRING_ID(gui_dialog, in_campaign_revert_to_last_save):
		{
			if (dialog_result_message->get_dialog_result() == k_gui_dialog_choice_ok)
			{
				simulation_queue_game_global_event_insert(_simulation_queue_game_global_event_type_revert_map);
				c_start_menu_pane_screen_widget::close_start_menu0();
			}

			handled = true;
		}
		break;
		case STRING_ID(gui_dialog, in_campaign_restart_level):
		{
			if (dialog_result_message->get_dialog_result() == k_gui_dialog_choice_ok)
			{
				simulation_queue_game_global_event_insert(_simulation_queue_game_global_event_type_reset_map);
				c_start_menu_pane_screen_widget::close_start_menu0();
			}

			handled = true;
		}
		break;
		case STRING_ID(gui_dialog, in_game_change_network_privacy):
		{
			e_gui_network_session_advertisement_mode advertisement_mode = user_interface_networking_get_session_advertisement();

			switch (dialog_result_message->get_dialog_result())
			{
			case _gui_dialog_choice_first:
			{
				advertisement_mode = _network_advertise_xbox_live_public;
			}
			break;
			case _gui_dialog_choice_second:
			{
				advertisement_mode = _network_advertise_xbox_live_friends_only;
			}
			break;
			case _gui_dialog_choice_third:
			{
				advertisement_mode = _network_advertise_xbox_live_invite_only;
			}
			break;
			}

			if (!user_interface_networking_set_session_advertisement(advertisement_mode))
			{
				event(_event_warning, "ui: failed to set session advertisement mode to mode #%d",
					advertisement_mode);
			}
			handled = true;
		}
		break;
		}
	}

	if (!handled)
	{
		handled = c_gui_screen_widget::handle_dialog_result(dialog_result_message);
	}
	return handled;
}

bool c_start_menu_game_campaign::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return INVOKE_CLASS_MEMBER(0x00AE8930, c_start_menu_game_campaign, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	// $IMPLEMENT
}

void c_start_menu_game_campaign::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AE8B90, c_start_menu_game_campaign, initialize);

	c_gui_screen_widget::initialize();

	c_gui_screen_widget::add_game_tag_parser(new (_ui_allocation_marker_dummy) c_magic_string_game_tag_parser(L"<primary-objectives", this, parse_xml_primary_mission_objectives));
	c_gui_screen_widget::add_game_tag_parser(new (_ui_allocation_marker_dummy) c_magic_string_game_tag_parser(L"<secondary-objectives", this, parse_xml_secondary_mission_objectives));
	c_gui_screen_widget::add_game_tag_parser(new (_ui_allocation_marker_dummy) c_magic_string_game_tag_parser(L"<lobby-privacy", this, parse_xml_ui_screen_party_privacy));
}

void c_start_menu_game_campaign::initialize_datasource()
{
	//INVOKE_CLASS_MEMBER(0x00AE8C80, c_start_menu_game_campaign, initialize_datasource);

	c_gui_screen_widget::initialize_datasource();

	c_gui_data* datasource = c_gui_screen_widget::get_data(STRING_ID(gui, sidebar_items), NULL);
	if (datasource != NULL)
	{
		datasource->clear_disabled_elements();

		if (user_interface_squad_local_peer_is_host())
		{
			if (game_is_campaign() && !game_is_cooperative() && campaign_skull_is_active(_campaign_skull_primary_iron))
			{
				datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(global, revert_to_last_save));
			}
		}
		else
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(global, revert_to_last_save));
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(global, restart_level));
		}

		if (user_interface_squad_exists() && user_interface_squad_get_player_count() != 1)
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(global, save_and_quit));
		}
		else
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(global, leave_game));
		}

		if (!user_interface_squad_local_peer_is_leader())
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(gui_dialog, in_game_change_network_privacy));
		}

		if (user_interface_squad_get_session_class() != _network_session_class_xbox_live || game_is_cooperative())
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(gui_dialog, in_game_change_network_privacy));
		}
	}
}

bool __cdecl parse_xml_primary_mission_objectives(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00AE8D80, parse_xml_primary_mission_objectives, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_secondary_mission_objectives(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00AE8DD0, parse_xml_secondary_mission_objectives, this_ptr, buffer, buffer_length);
}

void c_start_menu_game_campaign::post_initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AE8E20, c_start_menu_game_campaign, post_initialize);
	
	c_gui_screen_widget::post_initialize();

	// H3 360 has profile and storage device popups
}

void c_start_menu_game_campaign::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00AE8E90, c_start_menu_game_campaign, update, current_milliseconds);

	c_start_menu_pane_screen_widget::update(current_milliseconds);

	c_gui_widget::set_child_visible(_gui_group, STRING_ID(global, pause), game_time_get_paused_for_reason(_game_time_pause_ui));

	if (c_gui_custom_bitmap_widget* map_image_bitmap = (c_gui_custom_bitmap_widget*)c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, map_image)))
	{
		if (game_options_valid())
		{
			map_image_bitmap->set_map_image(c_gui_custom_bitmap_widget::_custom_map_image_type_normal, game_options_get()->map_id, true);
		}
	}
}

