#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "tag_files/files.hpp"

struct c_screenshots_uploader
{
public:
	//c_screenshots_uploader();
	//~c_screenshots_uploader();
	//bool begin_screenshot_upload(e_controller_index controller_index, long content_item_index, s_file_reference const* file_reference, long game_id);
	//void cancel_upload();
	//void dispose();

private:
	//void end_progress_spinner();
	//void end_screenshot_upload();

public:
	//bool get_busy();
	//c_screenshots_uploader::e_screenshots_uploader_task get_current_task() const;
	//static bool initialize();

private:
	//void start_progress_spinner(bool);

public:
	//void update();

private:
	//void update_progress_spinner();
	//void update_screenshot_upload();

public:
	c_http_post_source m_http_post_source;
	c_http_stored_buffer_downloader<1024> m_http_uploader;
	long : 32; // pad for `c_http_stored_buffer_downloader` this shouldn't be needed
	c_enum<e_controller_index, long, _controller0, k_number_of_controllers> m_controller_index;
	long m_screenshots_uploader_task;
	long m_screenshot_upload_time;
	c_url_string m_screenshots_uploader_url;
	long m_content_item_index;
	s_file_reference m_screenshot_file;
	bool m_progress_spinner_active;
};
static_assert(sizeof(c_screenshots_uploader) == 0xE08);
static_assert(0x000 == OFFSETOF(c_screenshots_uploader, m_http_post_source));
static_assert(0x13C == OFFSETOF(c_screenshots_uploader, m_http_uploader));
static_assert(0xBD4 == OFFSETOF(c_screenshots_uploader, m_controller_index));
static_assert(0xBD8 == OFFSETOF(c_screenshots_uploader, m_screenshots_uploader_task));
static_assert(0xBDC == OFFSETOF(c_screenshots_uploader, m_screenshot_upload_time));
static_assert(0xBE0 == OFFSETOF(c_screenshots_uploader, m_screenshots_uploader_url));
static_assert(0xCF0 == OFFSETOF(c_screenshots_uploader, m_content_item_index));
static_assert(0xCF4 == OFFSETOF(c_screenshots_uploader, m_screenshot_file));
static_assert(0xE04 == OFFSETOF(c_screenshots_uploader, m_progress_spinner_active));

extern c_screenshots_uploader*& g_screenshots_uploader;

extern void __cdecl screenshots_uploader_dispose();
extern void __cdecl screenshots_uploader_dispose_from_old_map();
extern c_screenshots_uploader* __cdecl screenshots_uploader_get();
extern void __cdecl screenshots_uploader_initialize();
extern void __cdecl screenshots_uploader_initialize_for_new_map();
extern void __cdecl screenshots_uploader_memory_dispose();
extern void __cdecl screenshots_uploader_memory_initialize(e_map_memory_configuration memory_configuration);
extern c_screenshots_uploader* __cdecl screenshots_uploader_try_and_get();
extern void __cdecl screenshots_uploader_update();

