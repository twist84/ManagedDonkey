#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

struct s_message_of_the_day_popup
{
	static int32 const k_chunk_type = 'mtdp';
	static int32 const k_version_major = 4;
	//static int32 const k_version_minor = 0;

	// c_player_profile_interface::popup_message_titles[title_index] = message_index;
	int32 message_index;
	int32 prevent_dismissal_duration_milliseconds;

	int32 title_text_length;
	wchar_t title_text[48];

	int32 header_text_length;
	wchar_t header_text[48];

	int32 accept_text_length;
	wchar_t accept_text[48];

	int32 wait_text_length;
	wchar_t wait_text[48];

	int32 message_text_length;
	wchar_t message_text[1024];
};
static_assert(sizeof(s_message_of_the_day_popup) == 0x99C);

