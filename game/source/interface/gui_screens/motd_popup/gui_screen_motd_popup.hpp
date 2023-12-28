#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

struct s_motd_popup_data
{
	static long const k_chunk_type = 'mtdp';
	static long const k_version_major = 4;
	//static long const k_version_minor = 0;

	// c_player_profile_interface::popup_message_titles[title_index] = title_index_identifier;
	long title_index_identifier;
	dword button_key_wait_time;

	long title_size;
	c_static_wchar_string<48> title;

	long header_size;
	c_static_wchar_string<48> header;

	long button_key_size;
	c_static_wchar_string<48> button_key;

	long button_key_wait_size;
	c_static_wchar_string<48> button_key_wait;

	long message_size;
	c_static_wchar_string<1024> message;
};
static_assert(sizeof(s_motd_popup_data) == 0x99C);

