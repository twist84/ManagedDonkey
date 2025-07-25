#pragma once

#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"

class c_gui_screen_pregame_lobby_mapeditor :
	public c_gui_screen_pregame_lobby
{
public:
	virtual ~c_gui_screen_pregame_lobby_mapeditor();
	virtual void initialize() override;
	virtual void post_initialize() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message);
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual int32 get_lobby_header() override;
	virtual int32 get_lobby_title() override;
	virtual int32 get_start_button_name() override;
	virtual e_gui_game_mode get_gui_game_mode() override;

public:
	c_gui_screen_pregame_lobby_mapeditor(int32 name);
};

extern bool __cdecl parse_xml_lobby_mapeditor_map(void* this_ptr, wchar_t* buffer, int32 buffer_length);

