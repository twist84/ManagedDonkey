#pragma once

#include "interface/c_gui_screen_widget.hpp"

class __declspec(align(8)) c_gui_screen_in_progress :
	public c_gui_screen_widget
{
public:
    virtual void update(uns32 current_milliseconds) override;

public:
    c_gui_screen_in_progress(string_id name);

    bool get_loaded_manually() const;
    void set_loaded_manually(bool loaded_manually);
    void setup(string_id title, string_id message);

private:
	string_id m_title;
	string_id m_message;
	bool m_loaded_manually;
};
COMPILE_ASSERT(sizeof(c_gui_screen_in_progress) == 0x1AB0);
COMPILE_ASSERT(sizeof(c_gui_screen_in_progress) == sizeof(c_gui_screen_widget) + 0x10);

