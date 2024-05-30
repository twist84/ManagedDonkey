#include "interface/gui_screens/motd_popup/gui_screen_motd_popup.hpp"

#include "interface/c_controller.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "memory/module.hpp"

using t_motd_popup_data_downloader = c_http_blf_simple_downloader<s_motd_popup_data>;
e_download_status __thiscall t_motd_popup_data_downloader::get_data(s_motd_popup_data const** data, long* data_size)
{
	static s_motd_popup_data static_data{};

	if (static_data.title_index_identifier != 85839908)
	{
		static_data.title_index_identifier = 85839908;
		static_data.button_key_wait_time = 5000;

		static_data.title.set(L"Halo: Reach E3 Trailer!");
		static_data.title_size = sizeof(wchar_t) * static_data.title.length();

		static_data.header.set(L"Prepare for Launch.");
		static_data.header_size = sizeof(wchar_t) * static_data.header.length();

		static_data.button_key.set(L"Lift-off!");
		static_data.button_key_size = sizeof(wchar_t) * static_data.button_key.length();

		static_data.button_key_wait.set(L"T-minus 5...");
		static_data.button_key_wait_size = sizeof(wchar_t) * static_data.button_key_wait.length();

		static_data.message.set(L"Going where no man has gone before is standard operating procedure for Noble Team, but even they weren't prepared for Operation Uppercut. Backs against the wall and facing the full might of the Covenant invasion, Six must rely on his unique skill set and previous involvement in ONI's secret Sabre program to launch a surprise attack against the Covenant fleet in the last place they expect to be met by resistance. And since you weren't on hand to witness this trailer's maiden voyage at E3, Bungie brought back some footage so you can relive the launch sequence on Bungie.net and Marketplace.");
		static_data.message_size = sizeof(wchar_t) * static_data.message.length();
	}

	if (data)
		*data = &static_data;

	if (data_size)
		*data_size = sizeof(static_data);

	return _http_download_status_succeeded;
}
//HOOK_DECLARE_CLASS_MEMBER(0x00AE74E0, t_motd_popup_data_downloader, get_data);

