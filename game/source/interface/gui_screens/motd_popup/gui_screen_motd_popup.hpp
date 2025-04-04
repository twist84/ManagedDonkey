#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

struct s_message_of_the_day_popup
{
	static long const k_chunk_type = 'mtdp';
	static long const k_version_major = 4;
	//static long const k_version_minor = 0;

	// c_player_profile_interface::popup_message_titles[title_index] = message_index;
	long message_index;
	long prevent_dismissal_duration_milliseconds;

	long title_text_length;
	wchar_t title_text[48];

	long header_text_length;
	wchar_t header_text[48];

	long accept_text_length;
	wchar_t accept_text[48];

	long wait_text_length;
	wchar_t wait_text[48];

	long message_text_length;
	wchar_t message_text[1024];
};
static_assert(sizeof(s_message_of_the_day_popup) == 0x99C);

