#include "interface/gui_screens/in_progress/gui_screen_in_progress.hpp"

#include "interface/user_interface_messages.hpp"

c_gui_screen_in_progress::c_gui_screen_in_progress(string_id name) :
	c_gui_screen_widget(name),
	m_title(_string_id_invalid),
	m_message(_string_id_invalid),
	m_loaded_manually(false)
{
}

bool c_gui_screen_in_progress::get_loaded_manually() const
{
	return m_loaded_manually;
}

void c_gui_screen_in_progress::set_loaded_manually(bool loaded_manually)
{
	m_loaded_manually = loaded_manually;
}

void c_gui_screen_in_progress::setup(string_id title, string_id message)
{
	m_title = title;
	m_message = message;
}

void c_gui_screen_in_progress::update(uns32 current_milliseconds)
{
	c_gui_screen_widget::update(current_milliseconds);

	c_gui_screen_widget::set_text_widget_string_id(STRING_ID(global, title), m_title);
	c_gui_screen_widget::set_text_widget_string_id(STRING_ID(gui, message), m_message);
}

