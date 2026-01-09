#include "interface/gui_screens/main_menu/gui_screen_main_menu.hpp"

#include "interface/c_controller.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"
#include "saved_games/saved_game_files.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AE80F0, c_main_menu_screen_widget, set_list_elements_);

void __thiscall c_main_menu_screen_widget::set_list_elements_()
{
	c_main_menu_screen_widget::set_list_elements();
}

//.text:00AE71E0 ; public: c_main_menu_screen_widget::c_main_menu_screen_widget(int32)
//.text:00AE72A0 ; 
//.text:00AE72B0 ; 
//.text:00AE72D0 ; public: virtual c_main_menu_screen_widget::~c_main_menu_screen_widget()
//.text:00AE7320 ; public: virtual void* c_http_stored_buffer_downloader<4665>::`scalar deleting destructor'(unsigned int)
//.text:00AE7380 ; public: virtual void* c_main_menu_screen_widget::`scalar deleting destructor'(unsigned int)
//.text:00AE7410 ; private: void c_main_menu_screen_widget::compute_active_players(s_player_identifier* const)
//.text:00AE7470 ; public: virtual void c_main_menu_screen_widget::dispose()
//.text:00AE7480 ; // extras portal related
//.text:00AE74E0 ; public: e_download_status c_http_blf_simple_downloader<s_message_of_the_day_popup>::get_data_no_update(const s_message_of_the_day_popup**, int32*)
//.text:00AE75A0 ; public: e_download_status c_http_blf_simple_downloader<s_message_of_the_day_popup>::get_download_status() const
//.text:00AE75B0 ; 
//.text:00AE75C0 ; public: virtual e_gui_location c_main_menu_screen_widget::get_gui_location() const

int32 c_main_menu_screen_widget::get_in_use_controller_count(e_controller_index* out_first_in_use_controller)
{
	//return INVOKE_CLASS_MEMBER(0x00AE75D0, c_main_menu_screen_widget, get_in_use_controller_count, out_first_in_use_controller);

	int32 controller_count = 0;
	if (out_first_in_use_controller)
	{
		*out_first_in_use_controller = k_no_controller;
	}
	for (e_controller_index controller_index = _controller0; controller_index < k_number_of_controllers; controller_index++)
	{
		if (controller_get(controller_index)->in_use())
		{
			if (out_first_in_use_controller && *out_first_in_use_controller == k_no_controller)
			{
				*out_first_in_use_controller = controller_index;
			}

			controller_count++;
		}
	}
	return controller_count;
}

//.text:00AE7630 ; 
//.text:00AE7640 ; 
//.text:00AE7650 ; 

#if 0
//.text:00AE7660 ; public: virtual 
bool c_main_menu_screen_widget::handle_controller_input_message(const c_controller_input_message* message)
{
	bool const window_manager_faded = !window_manager_get()->get_fading() && window_manager_get()->get_fading_amount() <= k_real_epsilon;

	bool message_handled = window_manager_faded;
	if (!message_handled && message->get_event_type() == _event_type_button_press)
	{
		e_controller_index controller_index = message->get_controller();
		e_controller_component component = message->get_component();
		switch (component)
		{
		case _controller_component_button_b:
		{
			const c_controller_interface* controller = controller_get(controller_index);
			if (!controller->is_unsigned_in_user())
			{
				c_load_dialog_screen_message* load_dialog_message = UI_MALLOC(c_load_dialog_screen_message,
					controller_index,
					c_gui_screen_widget::get_render_window(),
					m_name,
					STRING_ID(gui_dialog, back_out_main_menu_no_profile),
					m_name);
				if (load_dialog_message)
				{
					user_interface_messaging_post(load_dialog_message);
				}
				message_handled = true;
			}
		}
		break;
		case _controller_component_button_y:
		{
			online_guide_show_friends_ui(controller_index);
			message_handled = true;
		}
		break;
		}
	}
	if (!message_handled)
	{
		message_handled = c_gui_screen_widget::handle_controller_input_message(message);
	}
	return message_handled;
}
#endif

//.text:00AE7790 ; public: virtual bool c_main_menu_screen_widget::handle_dialog_result(const c_dialog_result_message*)
//.text:00AE77D0 ; public: virtual bool c_main_menu_screen_widget::handle_list_item_chosen(const c_controller_input_message*, int32, c_gui_list_item_widget*, c_gui_data*)
//.text:00AE7E80 ; public: virtual void c_main_menu_screen_widget::initialize()
//.text:00AE7EF0 ; public: bool c_http_blf_simple_downloader<s_message_of_the_day_popup>::is_submitting_to_cache() const
//.text:00AE7F00 ; private: void c_main_menu_screen_widget::load_progress_ui()
//.text:00AE7F10 ; // extras portal related
//.text:00AE8000 ; public: virtual void c_main_menu_screen_widget::post_initialize()

void c_main_menu_screen_widget::set_list_elements()
{
	//INVOKE_CLASS_MEMBER(0x00AE80F0, c_main_menu_screen_widget, set_list_elements);

	static bool x_disable_leave_game_element = false; // $TODO make the menu taller?
	static bool x_disable_theater_element = true;   // $TODO enable when saved films actually work?

	if (c_gui_data* datasource = get_data(STRING_ID(gui, main_menu), NULL))
	{
		bool one_and_only_one_saved_game_exists = false;
		e_controller_index saved_game_controller = k_no_controller;
		if (get_in_use_controller_count(&saved_game_controller) == 1 && !get_is_blue_disk())
		{
			ASSERT(saved_game_controller != k_no_controller);
			one_and_only_one_saved_game_exists = saved_game_files_controller_has_saved_game_state_blocking(saved_game_controller);
		}

		datasource->clear_disabled_elements();

		if (one_and_only_one_saved_game_exists)
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, start_new_campaign));
		}
		else
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, resume_campaign));
		}

		if (get_alpha_is_internal_beta())
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, campaign));
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, mapeditor));
		}
		else if (x_disable_leave_game_element)
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(global, leave_game));
		}

		if (get_alpha_custom_games_disabled())
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(global, multiplayer));
		}

		if (get_alpha_is_locked_down())
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, matchmaking));
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, theater));
		}
		else
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(global, locked));
		}

		if (x_disable_theater_element)
		{
			datasource->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, theater));
		}
	}

	if (!get_is_blue_disk())
	{
		if (c_gui_list_widget* list_widget = get_child_list_widget(STRING_ID(gui, main_menu)))
		{
			if (c_gui_list_item_widget* list_item = (c_gui_list_item_widget*)get_first_child_widget_by_type(_gui_list_item))
			{
				list_item->set_use_alternate_ambient_state(get_in_use_controller_count(NULL) > 1);
			}
		}
	}
}

//.text:00AE8240 ; public: void c_http_blf_simple_downloader<s_message_of_the_day_popup>::set_url(const c_url_string*)
//.text:00AE8250 ; private: void c_main_menu_screen_widget::show_motd_popup_if_needed()
//.text:00AE8370 ; 
//.text:00AE8380 ; public: void c_http_blf_simple_downloader<s_message_of_the_day_popup>::update()
//.text:00AE8390 ; public: virtual void c_main_menu_screen_widget::update(uns32)
//.text:00AE8650 ; private: void c_main_menu_screen_widget::update_motd_popup()

