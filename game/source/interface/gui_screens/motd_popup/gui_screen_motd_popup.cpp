#include "interface/gui_screens/motd_popup/gui_screen_motd_popup.hpp"

#include "interface/c_controller.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "memory/module.hpp"

using t_motd_popup_data_downloader = c_http_blf_simple_downloader<s_message_of_the_day_popup, 4665>;
e_download_status __thiscall t_motd_popup_data_downloader::get_data(s_message_of_the_day_popup const** data, int32* data_size)
{
	static s_message_of_the_day_popup static_data{};

	if (static_data.message_index != 85839908)
	{
		static_data.message_index = 85839908;
		static_data.prevent_dismissal_duration_milliseconds = 5000;

#define MAKE_MOTD_TEXT(TEXT, TEXT_LEN, STRING) \
ustrnzcpy(TEXT, STRING, NUMBEROF(TEXT)); \
TEXT_LEN = sizeof(wchar_t) * ustrnlen(TEXT, NUMBEROF(TEXT));

		MAKE_MOTD_TEXT(static_data.title_text, static_data.title_text_length, L"Halo: Reach E3 Trailer!");
		MAKE_MOTD_TEXT(static_data.header_text, static_data.header_text_length, L"Prepare for Launch.");
		MAKE_MOTD_TEXT(static_data.accept_text, static_data.accept_text_length, L"Lift-off!");
		MAKE_MOTD_TEXT(static_data.wait_text, static_data.wait_text_length, L"T-minus 5...");
		MAKE_MOTD_TEXT(static_data.message_text, static_data.message_text_length, L"Going where no man has gone before is standard operating procedure for Noble Team, but even they weren't prepared for Operation Uppercut. Backs against the wall and facing the full might of the Covenant invasion, Six must rely on his unique skill set and previous involvement in ONI's secret Sabre program to launch a surprise attack against the Covenant fleet in the last place they expect to be met by resistance. And since you weren't on hand to witness this trailer's maiden voyage at E3, Bungie brought back some footage so you can relive the launch sequence on Bungie.net and Marketplace.");

#undef MAKE_MOTD_TEXT
	}

	if (data)
		*data = &static_data;

	if (data_size)
		*data_size = sizeof(static_data);

	return _http_download_status_succeeded;
}
//HOOK_DECLARE_CLASS_MEMBER(0x00AE74E0, t_motd_popup_data_downloader, get_data);

