#include "screenshots/screenshots_uploader.hpp"

REFERENCE_DECLARE(0x0244ECE0, c_screenshots_uploader*, g_screenshots_uploader);

//.text:006191F0 ; public: c_screenshots_uploader::c_screenshots_uploader()
//.text:00619280 ; public: c_screenshots_uploader::~c_screenshots_uploader()
//.text:00619290 ; public: void* c_screenshots_uploader::`scalar deleting destructor'(unsigned int)
//.text:006192C0 ; public: bool c_screenshots_uploader::begin_screenshot_upload(e_controller_index controller_index, int32 content_item_index, const s_file_reference* file_reference, int32 game_id)

void c_screenshots_uploader::cancel_upload()
{
	INVOKE_CLASS_MEMBER(0x006194B0, c_screenshots_uploader, cancel_upload);
}

//.text:006194C0 ; public: void c_screenshots_uploader::dispose(void)
//.text:00619500 ; private: void c_screenshots_uploader::end_progress_spinner()
//.text:00619540 ; private: void c_screenshots_uploader::end_screenshot_upload()
//.text:006195E0 ; public: bool c_screenshots_uploader::get_busy()
//.text:006195F0 ; public: c_screenshots_uploader::e_screenshots_uploader_task c_screenshots_uploader::get_current_task() const
//.text:00619600 ; public: static bool __cdecl c_screenshots_uploader::initialize()

void __cdecl screenshots_uploader_dispose()
{
	INVOKE(0x00619610, screenshots_uploader_dispose);
}

void __cdecl screenshots_uploader_dispose_from_old_map()
{
	INVOKE(0x00619620, screenshots_uploader_dispose_from_old_map);
}

c_screenshots_uploader* __cdecl screenshots_uploader_get()
{
	return INVOKE(0x00619670, screenshots_uploader_get);
}

void __cdecl screenshots_uploader_initialize()
{
	INVOKE(0x00619680, screenshots_uploader_initialize);
}

void __cdecl screenshots_uploader_initialize_for_new_map()
{
	INVOKE(0x00619690, screenshots_uploader_initialize_for_new_map);
}

void __cdecl screenshots_uploader_memory_dispose()
{
	INVOKE(0x006196A0, screenshots_uploader_memory_dispose);
}

void __cdecl screenshots_uploader_memory_initialize(e_map_memory_configuration memory_configuration)
{
	INVOKE(0x006196D0, screenshots_uploader_memory_initialize, memory_configuration);
}

c_screenshots_uploader* __cdecl screenshots_uploader_try_and_get()
{
	return INVOKE(0x00619760, screenshots_uploader_try_and_get);
}

void __cdecl screenshots_uploader_update()
{
	INVOKE(0x00619770, screenshots_uploader_update);
}

//.text:00619780 ; private: void c_screenshots_uploader::start_progress_spinner(bool)
//.text:00619870 ; public: void c_screenshots_uploader::update()
//.text:00619920 ; private: void c_screenshots_uploader::update_progress_spinner()
//.text:00619970 ; private: void c_screenshots_uploader::update_screenshot_upload()

