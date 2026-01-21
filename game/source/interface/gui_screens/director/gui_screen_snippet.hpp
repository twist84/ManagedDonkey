#pragma once

#include "interface/c_gui_screen_widget.hpp"

class c_saved_film_snippet_screen :
    public c_gui_screen_widget
{
public:
    c_saved_film_snippet_screen(string_id name);
    virtual void initialize() override;
    virtual void dispose() override;
    virtual void update(uns32 current_milliseconds) override;
    virtual bool handle_controller_input_message(const c_controller_input_message* message) override;
    virtual void initialize_datasource() override;
    virtual bool handle_list_item_chosen(const c_controller_input_message* message, string_id list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
};
COMPILE_ASSERT(sizeof(c_saved_film_snippet_screen) == 0x1AA0);
COMPILE_ASSERT(sizeof(c_saved_film_snippet_screen) == sizeof(c_gui_screen_widget));

