#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_selection.hpp"

#include "interface/gui_selected_items.hpp"
#include "interface/gui_selected_items_saved_film.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B03950, c_gui_screen_pregame_selection, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00B04190, c_gui_screen_pregame_selection, handle_x_button_press_on_autosave_queue_file_);

bool c_gui_screen_pregame_selection::handle_controller_input_message_(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00B03950, c_gui_screen_pregame_selection, handle_controller_input_message_, message);

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_screen_pregame_selection, handle_controller_input_message_, message);
	return result;
}

void c_gui_screen_pregame_selection::handle_x_button_press_on_autosave_queue_file_(e_controller_index controller_index, const c_gui_selected_item* selected_item)
{
	HOOK_INVOKE_CLASS_MEMBER(, c_gui_screen_pregame_selection, handle_x_button_press_on_autosave_queue_file_, controller_index, selected_item);

	c_gui_screen_pregame_selection::handle_x_button_press_on_autosave_queue_file(controller_index, selected_item);
}

void c_gui_screen_pregame_selection::handle_x_button_press_on_autosave_queue_file(e_controller_index controller_index, const c_gui_selected_item* selected_item)
{
	//INVOKE_CLASS_MEMBER(0x00B04190, c_gui_screen_pregame_selection, handle_x_button_press_on_autosave_queue_file, controller_index, selected_item);

	switch (m_selection_type)
	{
	case _gui_selection_type_film:
	{
		const c_gui_saved_film_selected_item* saved_film_selected_item = (const c_gui_saved_film_selected_item*)selected_item;

		printf("");
	}
	break;
	}
}

