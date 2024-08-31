#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "tag_files/files.hpp"
#include "shell/shell.hpp"

struct c_screenshots_uploader
{
	c_http_post_source m_post_source;
	c_http_stored_buffer_downloader<1024> m_buffer_downloader;
	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> m_controller_index;
	long m_screenshot_uploading; // bool? upload_state?
	dword m_upload_timestamp;
	c_url_string m_url_string;
	long m_content_item_index;
	s_file_reference m_file_reference;

	// if (m_upload_in_progress)
	// {
	//     window_manager_get()->get_screen_by_name(STRING_ID(gui, in_progress_mini))->transition_out_with_transition_type(0);
	//     m_upload_in_progress = false;
	// }
	bool m_upload_in_progress;
};
static_assert(sizeof(c_screenshots_uploader) == 0xE08);

extern c_screenshots_uploader*& g_screenshots_uploader;

extern void __cdecl screenshots_uploader_dispose();
extern void __cdecl screenshots_uploader_dispose_from_old_map();
extern void __cdecl screenshots_uploader_initialize();
extern void __cdecl screenshots_uploader_initialize_for_new_map();
extern void __cdecl screenshots_uploader_update();

