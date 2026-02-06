#include "interface/gui_screens/director/gui_screen_snippet.hpp"

#include "interface/c_gui_list_item_widget.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "saved_games/saved_film_snippet.hpp"

c_saved_film_snippet_screen::c_saved_film_snippet_screen(string_id name) :
	c_gui_screen_widget(name)
{
	//DECLFUNC(0x00AE6B80, void, __thiscall, c_saved_film_snippet_screen*, string_id)(this, name);
}

//.text:00AE6BA0 ; c_saved_film_snippet_screen::~c_saved_film_snippet_screen()
//.text:00AE6BB0 ; public: virtual void* c_saved_film_snippet_screen::`vector deleting destructor'(unsigned int)

void c_saved_film_snippet_screen::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00AE6BE0, c_saved_film_snippet_screen, dispose);

	c_gui_screen_widget::dispose();

	c_load_screen_message* load_screen_message = new (_ui_allocation_marker_dummy) c_load_screen_message(
		STRING_ID(gui, saved_film_control_pad),
		k_any_controller,
		k_number_of_player_windows,
		get_name());

	if (load_screen_message != nullptr)
	{
		string_id initial_focused_item;

		e_saved_film_snippet_state snippet_state = saved_film_manager_get_snippet_state();
		if (snippet_state == _saved_film_snippet_state_previewing_waiting_for_seek || snippet_state == _saved_film_snippet_state_previewing)
		{
			initial_focused_item = STRING_ID(gui, end_preview);
		}
		else
		{
			initial_focused_item = saved_film_manager_get_playback_game_speed() > 0.0f ? STRING_ID(gui, play_film) : STRING_ID(gui, pause_film);
		}

		load_screen_message->set_focus_on_load_by_name(STRING_ID(gui, button_list), STRING_ID(gui, gui_item), initial_focused_item);
		user_interface_messaging_post(load_screen_message);
	}
}

bool c_saved_film_snippet_screen::handle_controller_input_message(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AE6C70, c_saved_film_snippet_screen, handle_controller_input_message, message);

	bool handled = false;

	e_controller_component component = message->get_component();
	bool suppress = component == _controller_component_button_b || component == _controller_component_button_y;

	if (!handled && !suppress)
	{
		handled = c_gui_screen_widget::handle_controller_input_message(message);
	}
	return handled;
}

bool c_saved_film_snippet_screen::handle_list_item_chosen(const c_controller_input_message* message, string_id list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00AE6CA0, c_saved_film_snippet_screen, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	bool handled = false;

	if (list_name == STRING_ID(gui, snippet_option_list))
	{
		string_id item_name = _string_id_invalid;
		long element_handle = list_item_widget->get_element_handle();
		if (datasource->get_string_id_value(element_handle, STRING_ID(gui, gui_item), &item_name))
		{
			e_controller_index controller_index = message->get_controller();
			switch (item_name)
			{
			case STRING_ID(gui, snippet_discard):
				saved_film_manager_delete_current_snippet();
				transition_out(_transition_out_normal);
				handled = true;
				break;
			case STRING_ID(gui, snippet_preview):
				saved_film_manager_preview_snippet_start();
				transition_out(_transition_out_normal);
				handled = true;
				break;
			case STRING_ID(gui, save_as):
				saved_film_manager_commit_snippet_keyboard(controller_index);
				transition_out(_transition_out_normal);
				handled = true;
				break;
			case STRING_ID(gui, save_auto):
				saved_film_manager_commit_snippet_autoname(controller_index);
				transition_out(_transition_out_normal);
				handled = true;
				break;
			default:
				break;
			}
		}
	}

	if (!handled)
	{
		handled = c_gui_screen_widget::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
	}
	return handled;
}

void c_saved_film_snippet_screen::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AE6D40, c_saved_film_snippet_screen, initialize);

	c_gui_screen_widget::initialize();
}

void c_saved_film_snippet_screen::initialize_datasource()
{
	//INVOKE_CLASS_MEMBER(0x00AE6D50, c_saved_film_snippet_screen, initialize_datasource);

	c_gui_screen_widget::initialize_datasource();
}

void c_saved_film_snippet_screen::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00AE6DA0, c_saved_film_snippet_screen, update, current_milliseconds);

	c_gui_screen_widget::update(current_milliseconds);
}

