#include "interface/gui_screens/start_menu/panes/hq/start_menu_headquarters.hpp"

#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "memory/module.hpp"

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

