#include "networking/online/online_url.hpp"

#include "memory/module.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "text/unicode.hpp"

HOOK_DECLARE(0x004515F0, create_title_url_base);

bool g_title_url_base_override = true;
long g_title_url_base_length = k_title_url_base_override_length;

void create_title_url_base(c_url_string* url)
{
	HOOK_INVOKE(, create_title_url_base, url);

	if (g_title_url_base_override)
		url->m_string.set(k_title_url_base_override);

	g_title_url_base_length = url->m_string.length();
}

struct c_motd_data_downloader :
	public c_http_blf_simple_downloader<s_motd_data>
{
public:
	e_download_status __thiscall get_data(s_motd_data** data, long* data_size)
	{
		static s_motd_data message_of_the_day{};

		if (!message_of_the_day.message_length)
		{
			csstrnzcpy(message_of_the_day.message, "Need more Render Minutes?  Check your B.net Profile to get more.  And if you want to turn your in game clips into streaming video, but don’t have Bungie Pro, visit www.bungie.net/pro.", NUMBEROF(message_of_the_day.message));
			message_of_the_day.message_length = csstrnlen(message_of_the_day.message, NUMBEROF(message_of_the_day.message));
		}

		if (data)
			*data = &message_of_the_day;

		return _http_download_status_succeeded;
	}

protected:
	long __unknown_index_or_count;
	char const* m_chunk_buffer;
	long m_chunk_buffer_size;
};
HOOK_DECLARE_CLASS_MEMBER(0x00AEC340, c_motd_data_downloader, get_data);

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

			ustrnzcpy(message_of_the_day_popup.title, L"Halo: Reach E3 Trailer!", NUMBEROF(message_of_the_day_popup.title));
			message_of_the_day_popup.title_size = ustrnlen(message_of_the_day_popup.title, NUMBEROF(message_of_the_day_popup.title)) * 2;

			ustrnzcpy(message_of_the_day_popup.header, L"Prepare for Launch.", NUMBEROF(message_of_the_day_popup.header));
			message_of_the_day_popup.header_size = ustrnlen(message_of_the_day_popup.header, NUMBEROF(message_of_the_day_popup.header)) * 2;

			ustrnzcpy(message_of_the_day_popup.button_key, L"Lift-off!", NUMBEROF(message_of_the_day_popup.button_key));
			message_of_the_day_popup.button_key_size = ustrnlen(message_of_the_day_popup.button_key, NUMBEROF(message_of_the_day_popup.button_key)) * 2;

			ustrnzcpy(message_of_the_day_popup.button_key_wait, L"T-minus 5...", NUMBEROF(message_of_the_day_popup.button_key_wait));
			message_of_the_day_popup.button_key_wait_size = ustrnlen(message_of_the_day_popup.button_key_wait, NUMBEROF(message_of_the_day_popup.button_key_wait)) * 2;

			ustrnzcpy(message_of_the_day_popup.message, L"Going where no man has gone before is standard operating procedure for Noble Team, but even they weren't prepared for Operation Uppercut. Backs against the wall and facing the full might of the Covenant invasion, Six must rely on his unique skill set and previous involvement in ONI's secret Sabre program to launch a surprise attack against the Covenant fleet in the last place they expect to be met by resistance. And since you weren't on hand to witness this trailer's maiden voyage at E3, Bungie brought back some footage so you can relive the launch sequence on Bungie.net and Marketplace.", NUMBEROF(message_of_the_day_popup.message));
			message_of_the_day_popup.message_size = ustrnlen(message_of_the_day_popup.message, NUMBEROF(message_of_the_day_popup.message)) * 2;
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
HOOK_DECLARE_CLASS_MEMBER(0x00AE74E0, c_motd_popup_data_downloader, get_data);
DATA_PATCH_DECLARE2(0x00AE82C4, is_signed_in_to_live_test_jz, 2, { 0x74, 0x00 });

