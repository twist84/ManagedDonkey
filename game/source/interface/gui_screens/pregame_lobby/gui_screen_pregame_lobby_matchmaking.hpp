#pragma once

#include "cseries/cseries.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "networking/session/network_session_parameters_matchmaking.hpp"

struct s_dynamic_matchmaking_hopper_statistics;
struct c_gui_screen_pregame_lobby_matchmaking :
	public c_gui_screen_pregame_lobby
{
public:
	//bool __thiscall handle_list_item_chosen_(c_controller_input_message const* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);

	c_gui_screen_pregame_lobby_matchmaking(int32 name) :
		c_gui_screen_pregame_lobby(name)
	{
		DECLFUNC(0x00B01580, c_gui_screen_pregame_lobby_matchmaking*, __thiscall, c_gui_screen_pregame_lobby_matchmaking*, int32)(this, name);
	}

//protected:
	c_http_blf_simple_downloader<s_dynamic_matchmaking_hopper_statistics, 2465> m_hopper_statstics_downloader;
	int32 m_url_key;
	c_http_stored_buffer_downloader<102400> m_nightmap_downloader;
	int32 pad;
	bool m_nightmap_download_completed;
	int32 m_last_hopper_catalog_load_status;
	int16 m_statistics_current_hopper_id;
	s_matchmaking_single_hopper_statistics m_statistics_current_statistics;
};
static_assert(sizeof(c_gui_screen_pregame_lobby_matchmaking) == sizeof(c_gui_screen_pregame_lobby) + 0x1A6F8);
static_assert(0x03390 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_hopper_statstics_downloader));
static_assert(0x043D8 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_url_key));
static_assert(0x043DC == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_nightmap_downloader));
static_assert(0x1DA70 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, pad));
static_assert(0x1DA74 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_nightmap_download_completed));
static_assert(0x1DA78 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_last_hopper_catalog_load_status));
static_assert(0x1DA7C == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_statistics_current_hopper_id));
static_assert(0x1DA80 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_statistics_current_statistics));

