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
	bool __thiscall handle_controller_input_message_(c_controller_input_message* input_message);
	void __thiscall initialize_();

protected:
	long __unknown1AA0;
	bool __unknown1AA4;
	bool __unknown1AA5;
	bool __unknown1AA6;
	bool __unknown1AA7;

	struct //c_http_blf_simple_downloader<s_motd_struct> m_motd_popup_downloader;
	{
		c_http_stored_buffer_downloader<4665> downloader;
		long __unknown_index_or_count;
		char const* m_chunk_buffer;
		long m_chunk_buffer_size;

	} m_motd_popup_downloader;
	bool m_motd_popup_downloaded;

	long __unknown338C;
};
static_assert(sizeof(c_gui_screen_pregame_lobby) == sizeof(c_gui_screen_widget) + 0x18F0);

