#pragma once

#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"

class c_gui_screen_pregame_lobby_theater :
	public c_gui_screen_pregame_lobby
{
public:
	void __thiscall update_(uns32 current_milliseconds);

public:
	virtual ~c_gui_screen_pregame_lobby_theater() = default;
	virtual void initialize() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual void initialize_datasource() override;

protected:
	virtual bool team_switching_allowed() override;

public:
	virtual int32 get_lobby_header() override;
	virtual int32 get_lobby_title() override;
	virtual int32 get_start_button_name() override;
	virtual e_gui_game_mode get_gui_game_mode() override;

public:
	c_gui_screen_pregame_lobby_theater(int32 name);

protected:
	bool m_recent_film_set;
};
static_assert(sizeof(c_gui_screen_pregame_lobby_theater) == sizeof(c_gui_screen_pregame_lobby) + 0x4);

extern bool __cdecl parse_xml_lobby_theater_film(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_theater_film_length(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_theater_game(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_theater_map(void* this_ptr, wchar_t* buffer, int32 buffer_length);

