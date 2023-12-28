#include "interface/gui_screens/motd_popup/gui_screen_motd_popup.hpp"

#include "interface/c_controller.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "memory/module.hpp"

struct c_motd_popup_data_downloader :
	public c_http_blf_simple_downloader<s_motd_popup_data>
{
public:
	e_download_status __thiscall get_data(s_motd_popup_data** data, long* data_size)
	{
		static s_motd_popup_data message_of_the_day_popup{};

		if (message_of_the_day_popup.title_index_identifier != 85839908)
		{
			message_of_the_day_popup.title_index_identifier = 85839908;
			message_of_the_day_popup.button_key_wait_time = 5000;

			message_of_the_day_popup.title.set(L"Halo: Reach E3 Trailer!");
			message_of_the_day_popup.title_size = message_of_the_day_popup.title.length();

			message_of_the_day_popup.header.set(L"Prepare for Launch.");
			message_of_the_day_popup.header_size = message_of_the_day_popup.header.length();

			message_of_the_day_popup.button_key.set(L"Lift-off!");
			message_of_the_day_popup.button_key_size = message_of_the_day_popup.button_key.length();

			message_of_the_day_popup.button_key_wait.set(L"T-minus 5...");
			message_of_the_day_popup.button_key_wait_size = message_of_the_day_popup.button_key_wait.length();

			message_of_the_day_popup.message.set(L"Going where no man has gone before is standard operating procedure for Noble Team, but even they weren't prepared for Operation Uppercut. Backs against the wall and facing the full might of the Covenant invasion, Six must rely on his unique skill set and previous involvement in ONI's secret Sabre program to launch a surprise attack against the Covenant fleet in the last place they expect to be met by resistance. And since you weren't on hand to witness this trailer's maiden voyage at E3, Bungie brought back some footage so you can relive the launch sequence on Bungie.net and Marketplace.");
			message_of_the_day_popup.message_size = message_of_the_day_popup.message.length();
		}

		if (data)
			*data = &message_of_the_day_popup;

		return _http_download_status_succeeded;
	}

protected:
	long __unknown_index_or_count;
	char const* m_chunk_buffer;
	long m_chunk_buffer_size;
};
//HOOK_DECLARE_CLASS_MEMBER(0x00AE74E0, c_motd_popup_data_downloader, get_data);
//DATA_PATCH_DECLARE2(0x00AE82C4, is_signed_in_to_live_test_jz, 2, { 0x74, 0x00 });


