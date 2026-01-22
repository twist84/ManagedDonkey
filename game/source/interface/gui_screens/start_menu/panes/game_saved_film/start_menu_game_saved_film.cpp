#include "interface/gui_screens/start_menu/panes/game_saved_film/start_menu_game_saved_film.hpp"

#include "cseries/cseries_events.hpp"
#include "game/game_time.hpp"
#include "interface/gui_screens/start_menu/panes/game_multiplayer/start_menu_game_multiplayer.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_text.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "screenshots/screenshots_uploader.hpp"

c_start_menu_game_saved_film::c_start_menu_game_saved_film(int32 name) :
	c_start_menu_pane_screen_widget(name),
	m_controller_index()
{
	//DECLFUNC(0x00AEBB40, void, __thiscall, c_start_menu_game_saved_film*, int32)(this, name);
}

//.text:00AEBB70 ; public: virtual c_start_menu_game_saved_film::~c_start_menu_game_saved_film()
//.text:00AEBB80 ; public: virtual void* c_start_menu_game_saved_film::`vector deleting destructor'(unsigned int)

bool c_start_menu_game_saved_film::allow_pane_tab_change()
{
	//return INVOKE_CLASS_MEMBER(0x00AEBBB0, c_start_menu_game_saved_film, allow_pane_tab_change);

	return true;
}

bool c_start_menu_game_saved_film::handle_dialog_result(const c_dialog_result_message* dialog_result_message)
{
	//return INVOKE_CLASS_MEMBER(0x00AEBBC0, c_start_menu_game_saved_film, handle_dialog_result, dialog_result_message);

	bool handled = c_start_menu_game_multiplayer::handle_leave_game_response(this, dialog_result_message);
	if (!handled)
	{
		switch (dialog_result_message->get_dialog_name())
		{
		case STRING_ID(gui_dialog, in_game_exit_film_playback):
		{
			if (dialog_result_message->get_dialog_result() == k_gui_dialog_choice_ok)
			{
				c_start_menu_pane_screen_widget::close_start_menu1();
			}
			handled = true;
		}
		break;
		case STRING_ID(gui_dialog, screenshot_uploading_end_session):
		{
			if (dialog_result_message->get_dialog_result() == k_gui_dialog_choice_ok)
			{
				screenshots_uploader_get()->cancel_upload();
				c_start_menu_pane_screen_widget::close_start_menu1();
			}
			handled = true;
		}
		break;
		case STRING_ID(gui_dialog, in_game_change_network_privacy):
		{
			e_gui_network_session_advertisement_mode session_advertisement = user_interface_networking_get_session_advertisement();

			switch (dialog_result_message->get_dialog_result())
			{
			case _gui_dialog_choice_first:
			{
				session_advertisement = _network_advertise_xbox_live_public;
			}
			break;
			case _gui_dialog_choice_second:
			{
				session_advertisement = _network_advertise_xbox_live_friends_only;
			}
			break;
			case _gui_dialog_choice_third:
			{
				session_advertisement = _network_advertise_xbox_live_invite_only;
			}
			break;
			}

			if (!user_interface_networking_set_session_advertisement(session_advertisement))
			{
				event(_event_warning, "ui: failed to set session advertisement mode to mode #%d",
					session_advertisement);
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

bool c_start_menu_game_saved_film::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return INVOKE_CLASS_MEMBER(0x00AEBC90, c_start_menu_game_saved_film, handle_list_item_chosen, message, list_name, list_item_widget, datasource);
}

void c_start_menu_game_saved_film::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AEBEC0, c_start_menu_game_saved_film, initialize);

	c_gui_screen_widget::initialize();

	c_gui_screen_widget::add_game_tag_parser(new (_ui_allocation_marker_dummy) c_magic_string_game_tag_parser(L"<lobby-privacy", this, parse_xml_ui_screen_party_privacy));
}

void c_start_menu_game_saved_film::initialize_datasource()
{
	//INVOKE_CLASS_MEMBER(0x00AEBF30, c_start_menu_game_saved_film, initialize_datasource);

	c_gui_screen_widget::initialize_datasource();

	c_gui_data* sidebar_items_data = c_gui_screen_widget::get_data(STRING_ID(gui, sidebar_items), NULL);
	if (!sidebar_items_data)
	{
		return;
	}

	sidebar_items_data->clear_disabled_elements();

	if (user_interface_squad_local_peer_is_leader())
	{
		sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(global, leave_game));
	}
	else
	{
		sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, leave_film));
	}

	if (!user_interface_squad_local_peer_is_leader())
	{
		sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui_dialog, in_game_change_network_privacy));
	}

	sidebar_items_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui_dialog, in_game_change_network_privacy));
}

void c_start_menu_game_saved_film::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00AEBFB0, c_start_menu_game_saved_film, update, current_milliseconds);

	c_start_menu_pane_screen_widget::update(current_milliseconds);

	bool visible = game_time_get_paused_for_reason(_game_time_pause_ui) || saved_film_manager_get_playback_game_speed() == 0.0f;
	c_gui_widget::set_child_visible(_gui_group, STRING_ID(global, pause), visible);
}

