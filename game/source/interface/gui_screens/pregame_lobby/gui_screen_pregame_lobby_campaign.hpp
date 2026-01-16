#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"

class c_gui_screen_pregame_lobby_campaign :
	public c_gui_screen_pregame_lobby
{
public:
	bool __thiscall handle_controller_input_message_(const c_controller_input_message* message);
	bool __thiscall handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);

public:
	virtual ~c_gui_screen_pregame_lobby_campaign() = default;
	virtual void initialize() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual bool postgame_stats_enabled() override;
	virtual int32 get_lobby_header() override;
	virtual int32 get_lobby_title() override;
	virtual int32 get_start_button_name() override;
	virtual e_gui_game_mode get_gui_game_mode() override;

public:
	c_gui_screen_pregame_lobby_campaign(int32 name);

private:
	void load_progress_ui(e_controller_index controller_index);
	void restore_game_setup_from_controller_saved_game(e_controller_index controller_index);

public:

//protected:
	int32 m_last_player_count;
	e_network_session_class m_session_class;
	c_static_array<s_player_identifier, 16> m_last_player_identifiers;
	e_controller_index m_use_saved_game_from_controller;
	uns32 m_progress_dialog_start_time_milliseconds;
	bool m_catalogue_enumeration_active;
};
COMPILE_ASSERT(sizeof(c_gui_screen_pregame_lobby_campaign) == sizeof(c_gui_screen_pregame_lobby) + 0x94);
COMPILE_ASSERT(0x03390 == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_last_player_count));
COMPILE_ASSERT(0x03394 == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_session_class));
COMPILE_ASSERT(0x03398 == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_last_player_identifiers));
COMPILE_ASSERT(0x03418 == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_use_saved_game_from_controller));
COMPILE_ASSERT(0x0341C == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_progress_dialog_start_time_milliseconds));
COMPILE_ASSERT(0x03420 == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_catalogue_enumeration_active));

