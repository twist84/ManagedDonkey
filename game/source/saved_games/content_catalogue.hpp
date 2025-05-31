#pragma once

#include "multithreading/synchronized_value.hpp"
#include "saved_games/c_storage_device.hpp"
#include "saved_games/content_catalogue_tasks.hpp"
#include "saved_games/content_item.hpp"

struct s_data_array;
struct c_content_catalogue
{
public:
	void initialize(e_controller_index controller_index);

//protected:
	e_controller_index m_controller_index;
	s_data_array* m_content_item_data_array;
	s_content_item* m_content_item_array;
	s_content_item m_content_item;
	c_content_item_overlapped_task m_content_item_task;
	c_content_enumeration_overlapped_task m_enumeration_task;
	uns32 __unknown29C;
	uns32 __unknown2A0;
	c_synchronized_long m_content_item_count;
	uns32 m_flags;
	uns32 __unknown2AC;
};
static_assert(sizeof(c_content_catalogue) == 0x2B0);

struct s_content_catalogue_globals
{
	bool initialized;
	uns32 : 32;
	c_content_catalogue user_catalogues[4];
	c_storage_device user_storage_devices[4];
};
const size_t k_content_catalogue_globals_size = sizeof(s_content_catalogue_globals);
static_assert(sizeof(s_content_catalogue_globals) == 0xB88);

extern s_content_catalogue_globals& g_content_catalogue_globals;

extern void __cdecl content_catalogue_close_all_dlc(bool allow_exceptions);
extern c_content_catalogue* __cdecl content_catalogue_get_interface(e_controller_index controller_index);
extern bool __cdecl content_catalogue_open_dlc(const wchar_t* path, bool block);

