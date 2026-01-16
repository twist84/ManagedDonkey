#pragma once

#include "multithreading/synchronized_value.hpp"
#include "saved_games/c_storage_device.hpp"
#include "saved_games/content_catalogue_tasks.hpp"
#include "saved_games/content_item.hpp"

class c_dialog_result_message;
struct s_data_array;

class c_content_catalogue
{
public:
	enum e_content_catalogue_flags
	{
		_need_to_enumerate_content_item_metadata_bit = 0,
		_enumeration_completed_bit,
		_render_debug_info_open_content_items_bit,
		_render_debug_info_all_content_items_bit,
		_dispose_recursion_lock_bit,

		k_number_of_content_catalogue_flags,
	};

public:
	bool enumeration_has_completed();
	void initialize(e_controller_index controller_index);
	bool valid() const;

//protected:
	e_controller_index m_controller_index;
	s_data_array* m_content_item_data_array;
	s_content_item* m_content_item_array;
	s_content_item m_content_item_to_delete;
	c_content_item_overlapped_task m_content_item_task;
	c_content_enumeration_overlapped_task m_enumeration_task;
	int32 m_enumeration_metadata_task;
	c_synchronized_long m_enumeration_metadata_complete;
	c_flags<e_content_catalogue_flags, uns32, k_number_of_content_catalogue_flags> m_flags;
	uns32 __unknown2AC;
};
COMPILE_ASSERT(sizeof(c_content_catalogue) == 0x2B0);
COMPILE_ASSERT(0x000 == OFFSETOF(c_content_catalogue, m_controller_index));
COMPILE_ASSERT(0x004 == OFFSETOF(c_content_catalogue, m_content_item_data_array));
COMPILE_ASSERT(0x008 == OFFSETOF(c_content_catalogue, m_content_item_array));
COMPILE_ASSERT(0x010 == OFFSETOF(c_content_catalogue, m_content_item_to_delete));
COMPILE_ASSERT(0x250 == OFFSETOF(c_content_catalogue, m_content_item_task));
COMPILE_ASSERT(0x270 == OFFSETOF(c_content_catalogue, m_enumeration_task));
COMPILE_ASSERT(0x2A0 == OFFSETOF(c_content_catalogue, m_enumeration_metadata_task));
COMPILE_ASSERT(0x2A4 == OFFSETOF(c_content_catalogue, m_enumeration_metadata_complete));
COMPILE_ASSERT(0x2A8 == OFFSETOF(c_content_catalogue, m_flags));
COMPILE_ASSERT(0x2AC == OFFSETOF(c_content_catalogue, __unknown2AC));

struct s_content_catalogue_globals
{
	bool initialized;
	uns32 : 32;
	c_content_catalogue user_catalogues[4];
	c_storage_device user_storage_devices[4];
};
const size_t k_content_catalogue_globals_size = sizeof(s_content_catalogue_globals);
COMPILE_ASSERT(sizeof(s_content_catalogue_globals) == 0xB88);

extern s_content_catalogue_globals& g_content_catalogue_globals;

extern void __cdecl content_catalogue_close_all_dlc(bool allow_exceptions);
extern void __cdecl content_catalogue_display_device_selection_guide_interface(e_controller_index controller_index);
extern void __cdecl content_catalogue_dispose();
extern void __cdecl content_catalogue_dispose_from_old_map();
extern bool __cdecl content_catalogue_handle_dialog_result_message(const c_dialog_result_message* dialog_result_message);
extern bool __cdecl content_catalogue_enumeration_active_on_controllers(int32* out_controller_mask);
extern c_content_catalogue* __cdecl content_catalogue_get_interface(e_controller_index controller_index);
extern bool __cdecl content_catalogue_open_dlc(const wchar_t* path, bool block);

