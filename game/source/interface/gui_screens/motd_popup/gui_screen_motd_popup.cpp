#include "interface/gui_screens/motd_popup/gui_screen_motd_popup.hpp"

#include "interface/c_controller.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "memory/module.hpp"

#ifdef _DEBUG
#ifdef ISPROXY

e_download_status __fastcall sub_AE74E0(c_http_blf_simple_downloader<s_motd_popup_data>* _this, void* unused, char const** out_buffer, long* out_buffer_size)
{
	e_download_status status = _this->get_data(_this, nullptr, out_buffer, out_buffer_size);

	if (out_buffer_size)
		*out_buffer_size = sizeof(s_motd_popup_data);

	if (out_buffer)
	{
		c_controller_interface* controller = controller_get(_controller_index0);
		controller->m_player_profile.popup_message_indices;
		controller->m_player_profile.popup_message_indices;

		s_motd_popup_data* popup_data = reinterpret_cast<s_motd_popup_data*>(const_cast<char*>(*out_buffer));

		popup_data->title_index_identifier = 1;
		popup_data->button_key_wait_time = 1000;

		popup_data->title.set(L"TEST TITLE");
		popup_data->title_size = popup_data->title.length() * 2;

		popup_data->header.set(L"TEST HEADER");
		popup_data->header_size = popup_data->header.length() * 2;

		popup_data->button_key.set(L"TEST BUTTON KEY");
		popup_data->button_key_size = popup_data->button_key.length() * 2;

		popup_data->button_key_wait.set(L"TEST BUTTON KEY WAIT");
		popup_data->button_key_wait_size = popup_data->button_key_wait.length() * 2;

		popup_data->message.set(L"TEST MESSAGE");
		popup_data->message_size = popup_data->message.length() * 2;

		status = _http_download_status_succeeded;
	}

	return status;
}
HOOK_DECLARE_CALL(0x00AE8290, sub_AE74E0); // c_main_menu_screen_widget
HOOK_DECLARE_CALL(0x00B22CCE, sub_AE74E0); // c_gui_screen_pregame_lobby

// allows `c_motd_popup_screen_message` constructor to be called
DATA_PATCH_DECLARE2(0x00AE82BD, motd_popup, 0x6, { 0xEB, 0x4E, 0x90, 0x90, 0x90, 0x90 }); // c_main_menu_screen_widget
DATA_PATCH_DECLARE2(0x00B22CFD, motd_popup, 0x6, { 0xEB, 0x4A, 0x90, 0x90, 0x90, 0x90 }); // c_gui_screen_pregame_lobby

// prevents attempting to download the flavor_image bitmap
DATA_PATCH_DECLARE2(0x00B13DA4, motd_popup, 0x4, { 0xE9, 0xFE, 0x00, 0x90 }); // c_motd_popup_screen_widget::update

#endif // ISPROXY
#endif // _DEBUG

