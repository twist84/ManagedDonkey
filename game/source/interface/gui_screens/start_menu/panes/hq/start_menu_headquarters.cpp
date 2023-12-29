#include "interface/gui_screens/start_menu/panes/hq/start_menu_headquarters.hpp"

#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "memory/module.hpp"

using t_motd_data_downloader = c_http_blf_simple_downloader<s_motd_data>;
e_download_status __thiscall t_motd_data_downloader::get_data(s_motd_data const** data, long* data_size)
{
	static s_motd_data static_data{};

	if (!static_data.message_length)
	{
		static_data.message.set("Need more Render Minutes?  Check your B.net Profile to get more.  And if you want to turn your in game clips into streaming video, but don’t have Bungie Pro, visit www.bungie.net/pro.");
		static_data.message_length = static_data.message.length();
	}

	if (data)
		*data = &static_data;

	if (data_size)
		*data_size = sizeof(static_data);

	return _http_download_status_succeeded;
}
HOOK_DECLARE_CLASS_MEMBER(0x00AEC340, t_motd_data_downloader, get_data);

