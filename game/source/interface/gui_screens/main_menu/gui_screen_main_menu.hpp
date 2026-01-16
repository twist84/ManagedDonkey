#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/gui_screens/motd_popup/gui_screen_motd_popup.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"

class c_main_menu_screen_widget :
	public c_gui_screen_widget
{
public:
	void __thiscall set_list_elements_();

public:
	c_main_menu_screen_widget(int32 name) :
		c_gui_screen_widget(name),
		m_motd_popup_downloader()
	{
		DECLFUNC(0x00AE71E0, void, __thiscall, c_main_menu_screen_widget*, int32)(this, name);
	}

private:
	int32 get_in_use_controller_count(e_controller_index* out_first_in_use_controller);
	void set_list_elements();

public:
//protected:
	int32 m_last_ui_lockdown_level;
	bool m_checked_for_film_autoplayback;
	bool m_bink_was_active;
	bool m_content_enumeration_active;
	s_player_identifier m_last_active_players[4];
	uns32 m_progress_dialog_start_time_milliseconds;
	c_http_blf_simple_downloader<s_message_of_the_day_popup, 4665> m_motd_popup_downloader;
	bool m_motd_popup_shown;
	int32 m_url_key;
};
COMPILE_ASSERT(sizeof(c_main_menu_screen_widget) == 0x33B4);
COMPILE_ASSERT(0x1AA0 == OFFSETOF(c_main_menu_screen_widget, m_last_ui_lockdown_level));
COMPILE_ASSERT(0x1AA4 == OFFSETOF(c_main_menu_screen_widget, m_checked_for_film_autoplayback));
COMPILE_ASSERT(0x1AA5 == OFFSETOF(c_main_menu_screen_widget, m_bink_was_active));
COMPILE_ASSERT(0x1AA6 == OFFSETOF(c_main_menu_screen_widget, m_content_enumeration_active));
COMPILE_ASSERT(0x1AA7 == OFFSETOF(c_main_menu_screen_widget, m_last_active_players));
COMPILE_ASSERT(0x1AC8 == OFFSETOF(c_main_menu_screen_widget, m_progress_dialog_start_time_milliseconds));
COMPILE_ASSERT(0x1ACC == OFFSETOF(c_main_menu_screen_widget, m_motd_popup_downloader));
COMPILE_ASSERT(0x33AC == OFFSETOF(c_main_menu_screen_widget, m_motd_popup_shown));
COMPILE_ASSERT(0x33B0 == OFFSETOF(c_main_menu_screen_widget, m_url_key));

