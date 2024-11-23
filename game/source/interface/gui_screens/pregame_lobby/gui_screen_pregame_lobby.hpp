#pragma once

#include "interface/c_gui_screen_widget.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"

struct s_motd_struct
{
	byte __data[0x18D1];
};
static_assert(sizeof(s_motd_struct) == 0x18D1);

struct c_gui_screen_pregame_lobby :
	public c_gui_screen_widget
{
public:
	bool __thiscall handle_controller_input_message_(c_controller_input_message* message);
	void __thiscall initialize_();

protected:
	long m_last_countdown_value;
	bool m_is_fading;
	bool m_advanced_options_visible;
	bool m_change_teams_visible;
	bool m_postgame_stats_visible;
	c_http_blf_simple_downloader<s_motd_struct, 4665> m_motd_popup_downloader;
	bool m_motd_popup_downloaded;

	long __unknown338C;
};
static_assert(sizeof(c_gui_screen_pregame_lobby) == sizeof(c_gui_screen_widget) + 0x18F0);

