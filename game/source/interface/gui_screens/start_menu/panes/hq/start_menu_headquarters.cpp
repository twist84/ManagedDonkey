#include "interface/gui_screens/start_menu/panes/hq/start_menu_headquarters.hpp"

#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "memory/module.hpp"

using t_motd_data_downloader = c_http_blf_simple_downloader<s_motd_data, 2721>;
e_download_status __thiscall t_motd_data_downloader::get_data_no_update(const s_motd_data** data, int32* data_size)
{
	static s_motd_data static_data{};

	if (!static_data.message_length)
	{
		static_data.message.set("Need more Render Minutes?  Check your B.net Profile to get more.  And if you want to turn your in game clips into streaming video, but don't have Bungie Pro, visit www.bungie.net/pro.");
		static_data.message_length = static_data.message.length();
	}

	if (data)
	{
		*data = &static_data;
	}

	if (data_size)
	{
		*data_size = sizeof(static_data);
	}

	return _http_download_status_succeeded;
}
//HOOK_DECLARE_CLASS_MEMBER(0x00AEC340, t_motd_data_downloader, get_data);

//.text:00AEBFE0 ; 
//.text:00AEC020 ; 
//.text:00AEC040 ; 
//.text:00AEC060 ; public: c_start_menu_headquarters::c_start_menu_headquarters(int32)
//.text:00AEC130 ; 
//.text:00AEC140 ; 
//.text:00AEC150 ; 
//.text:00AEC160 ; 
//.text:00AEC1C0 ; public: virtual void* c_http_stored_buffer_downloader<2721>::`vector deleting destructor'(unsigned int)
//.text:00AEC1F0 ; public: virtual void* c_http_stored_buffer_downloader<61440>::`scalar deleting destructor'(unsigned int)
//.text:00AEC220 ; public: virtual void* c_start_menu_headquarters::`vector deleting destructor'(unsigned int)
//.text:00AEC290 ; public: virtual bool c_start_menu_headquarters::allow_pane_tab_change()
//.text:00AEC2A0 ; public: virtual c_gui_bitmap_widget* c_start_menu_headquarters::create_bitmap_widget(const s_runtime_bitmap_widget_definition*)
//.text:00AEC320 ; public: e_download_status c_http_blf_simple_downloader<s_message_of_the_day>::get_data(s_message_of_the_day const**, int32*)
//.text:00AEC340 ; public: e_download_status c_http_blf_simple_downloader<s_message_of_the_day>::get_data_no_update(s_message_of_the_day const**, int32*)
//.text:00AEC400 ; public: virtual bool c_start_menu_headquarters::handle_list_item_chosen(const c_controller_input_message*, int32, c_gui_list_item_widget*, c_gui_data*)
//.text:00AEC510 ; public: virtual void c_start_menu_headquarters::initialize()
//.text:00AEC5C0 ; bool __cdecl parse_xml_bnet_pro_button(void*, wchar_t*, int32)
//.text:00AEC660 ; bool __cdecl parse_xml_bnet_pro_message(void*, wchar_t*, int32)
//.text:00AEC700 ; public: void c_http_blf_simple_downloader<s_message_of_the_day>::set_url(const c_url_string*)
//.text:00AEC710 ; public: const wchar_t* c_player_marketplace::subscription_get_message(c_player_marketplace::e_message_type) const
//.text:00AEC730 ; public: uns64 c_player_marketplace::subscription_get_next_offer_id() const
//.text:00AEC770 ; public: void c_http_blf_simple_downloader<s_message_of_the_day>::update()
//.text:00AEC780 ; public: virtual void c_start_menu_headquarters::update(uns32)

