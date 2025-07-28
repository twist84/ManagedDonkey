#include "interface/gui_screens/director/gui_screen_saved_film_control_pad.hpp"

#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "memory/module.hpp"
#include "saved_games/saved_film_manager.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AE4B10, c_saved_film_control_pad, handle_list_item_chosen_);
//HOOK_DECLARE_CLASS_MEMBER(0x00AE4D80, c_saved_film_control_pad, update_);
HOOK_DECLARE_CLASS_MEMBER(0x00AE4F90, c_saved_film_control_pad, update_enabled_menu_items_);

// skip `STRING_ID(gui, play_film)` check in `c_saved_film_control_pad::update`
// jnz     short loc_AE4F38
// jmp     short loc_AE4F38
const t_value_type<byte> gui_play_film_jump = { .value = 0xEB };
DATA_PATCH_DECLARE(0x00AE4F2D, gui_play_film, gui_play_film_jump.bytes);

bool __thiscall c_saved_film_control_pad::handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	bool result = false;
	if (list_name == STRING_ID(gui, button_list))
	{
		int32 item_name = NONE;
		if (datasource->get_string_id_value(list_item_widget->get_element_handle(), STRING_ID(gui, gui_item), &item_name))
		{
			switch (item_name)
			{
			case STRING_ID(gui, skip_back):
			{
				saved_film_manager_request_revert(_saved_film_revert_backwards);
				result = true;
			}
			break;
			case STRING_ID(gui, skip_forward):
			{
				saved_film_manager_request_revert(_saved_film_revert_forwards);
				result = true;
			}
			break;
			//case STRING_ID(gui, record_clip):
			//{
			//	// $TODO: implement me
			//}
			//return true;
			case STRING_ID(gui, play_film):
			{
				saved_film_manager_set_playback_game_speed(1.0f);
				result = true;
			}
			break;
			case STRING_ID(gui, pause_film):
			{
				saved_film_manager_set_playback_game_speed(0.0f);
				result = true;
			}
			break;
			//case STRING_ID(gui, stop_recording):
			//{
			//	if (saved_film_manager_get_snippet_state() == _saved_film_snippet_state_recording)
			//	{
			//		saved_film_manager_stop_recording_snippet();
			//	}
			//}
			//return true;
			case STRING_ID(gui, replay_film):
			{
				saved_film_manager_replay_film();
				result = true;
			}
			break;
			//case STRING_ID(gui, end_preview):
			//{
			//	if (saved_film_manager_get_snippet_state() == _saved_film_snippet_state_previewing)
			//	{
			//		saved_film_manager_preview_snippet_stop();
			//	}
			//}
			//return true;
			}
		}
	}

	if (result)
	{
		return true;
	}

	//result c_gui_screen_widget::handle_list_item_chosen(message, list_name, list_item_widget, datasource);

	HOOK_INVOKE_CLASS_MEMBER(result =, c_saved_film_control_pad, handle_list_item_chosen_, message, list_name, list_item_widget, datasource);
	return result;
}

void __thiscall c_saved_film_control_pad::update_(uns32 current_milliseconds)
{
	e_saved_film_snippet_state snippet_state = saved_film_manager_get_snippet_state();

	c_gui_data* data = c_gui_screen_widget::get_data(STRING_ID(gui, saved_film_control_buttons), NULL);
	c_gui_list_widget* button_list = c_gui_widget::get_child_list_widget(STRING_ID(gui, button_list));

	c_gui_screen_widget::update(current_milliseconds);
	c_saved_film_control_pad::update_enabled_menu_items();

	if (data && button_list)
	{
		button_list->set_enabled(!!(snippet_state));

		for (c_gui_list_item_widget* child_button = (c_gui_list_item_widget*)button_list->get_first_child_widget_by_type(_gui_list_item);
			child_button != NULL;
			child_button = child_button->get_next_list_item_widget(true))
		{
			int32 element_handle = child_button->get_element_handle();

			static_assert(0x10010 == STRING_ID(gui, gui_item));

			if (element_handle != NONE)
			{
				int32 button_name = NONE;
				if (data->get_string_id_value(element_handle, STRING_ID(gui, gui_item), &button_name))
				{
					for (c_gui_bitmap_widget* child_button_bitmap = (c_gui_bitmap_widget*)child_button->get_first_child_widget_by_type(_gui_bitmap);
						child_button_bitmap != NULL;
						child_button_bitmap = child_button_bitmap->get_next_bitmap_widget())
					{
						if (child_button_bitmap->get_name() != STRING_ID(gui, hilite))
						{
							child_button_bitmap->set_visible(false);
						}
					}

					child_button->set_child_visible(_gui_bitmap, button_name, true);

					// $TODO: implement the rest
				}
			}
		}
	}

	c_gui_screen_widget::set_text_widget_string_id_from_focused_list_item(STRING_ID(gui, status), STRING_ID(gui, button_list), STRING_ID(gui, gui_item));

	if (m_previous_snippet_state != _saved_film_snippet_state_recorded_and_ready &&
		snippet_state == _saved_film_snippet_state_recorded_and_ready)
	{
		// $TODO: implement me
	}

	if (saved_film_manager_film_is_ended(NULL))
	{
		c_gui_screen_widget::transition_out(_transition_out_normal);
	}

	m_previous_snippet_state = snippet_state;
}

void __thiscall c_saved_film_control_pad::update_enabled_menu_items_()
{
	c_saved_film_control_pad::update_enabled_menu_items();
}

//.text:00AE4900 ; public: c_saved_film_control_pad::c_saved_film_control_pad(int32)
//.text:00AE4930 ; public: virtual c_saved_film_control_pad::~c_saved_film_control_pad()
//.text:00AE4940 ; public: virtual void* c_saved_film_control_pad::`scalar deleting destructor'(unsigned int)
//.text:00AE4970 ; 
//.text:00AE4980 ; public: virtual bool c_saved_film_control_pad::handle_controller_input_message(const c_controller_input_message*)
//.text:00AE4A20 ; public: virtual bool c_saved_film_control_pad::handle_dialog_result(const c_dialog_result_message*)
//.text:00AE4B10 ; public: virtual bool c_saved_film_control_pad::handle_list_item_chosen(const c_controller_input_message*, int32, c_gui_list_item_widget*, c_gui_data*)
//.text:00AE4D30 ; public: virtual void c_saved_film_control_pad::post_initialize()
//.text:00AE4D40 ; 
//.text:00AE4D50 ; 
//.text:00AE4D60 ; 
//.text:00AE4D70 ; 
//.text:00AE4D80 ; public: virtual void c_saved_film_control_pad::update(uns32)

void c_saved_film_control_pad::update_enabled_menu_items()
{
	//INVOKE_CLASS_MEMBER(0x00AE4F90, c_saved_film_control_pad, update_enabled_menu_items);

	c_gui_data* data = c_gui_screen_widget::get_data(STRING_ID(gui, saved_film_control_buttons), NULL);
	if (!data)
	{
		return;
	}

	data->clear_disabled_elements();

	data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, take_screenshot_upload));
	data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, take_screenshot_disabled));

	if (saved_film_manager_get_ticks_remaining())
	{
		if (saved_film_manager_get_playback_game_speed() > 0.0f)
		{
			data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, play_film));
		}
		else
		{
			data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, pause_film));
		}
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, replay_film));
	}
	else
	{
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, pause_film));
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, play_film));
	}

	e_saved_film_snippet_state snippet_state = saved_film_manager_get_snippet_state();
	switch (snippet_state)
	{
	case _saved_film_snippet_state_none:
	case _saved_film_snippet_state_recording_waiting_for_seek:
	case _saved_film_snippet_state_recording_waiting_for_start:
	case _saved_film_snippet_state_previewing_waiting_for_seek:
	case _saved_film_snippet_state_commiting_invoking_title_keyboard:
	case _saved_film_snippet_state_commiting_waiting_title_keyboard:
	case _saved_film_snippet_state_commiting_invoking_description_keyboard:
	case _saved_film_snippet_state_commiting_waiting_description_keyboard:
	case _saved_film_snippet_state_commiting_initiate_creation:
	case _saved_film_snippet_state_commiting_wait_for_creation:
	case _saved_film_snippet_state_commiting_initiate_metadata_update:
	case _saved_film_snippet_state_commiting_wait_for_metadata_update:
	case _saved_film_snippet_state_resetting:
	{
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, stop_recording));
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, end_preview));
	}
	break;
	case _saved_film_snippet_state_recording:
	case _saved_film_snippet_state_recorded_and_ready:
	{
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, record_clip));
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, end_preview));
	}
	break;
	case _saved_film_snippet_state_previewing:
	{
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, record_clip));
		data->set_disabled_element(STRING_ID(gui, gui_item), STRING_ID(gui, stop_recording));
	}
	break;
	}
}

