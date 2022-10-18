#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "saved_games/c_storage_device.hpp"
#include "saved_games/content_catalogue_tasks.hpp"
#include "saved_games/content_item.hpp"
#include "shell/shell.hpp"

struct s_data_array;
struct c_content_catalogue
{
	c_enum<e_controller_index, long, k_number_of_controllers> m_controller_index;
	s_data_array* m_content_item_data_array;
	s_content_item* m_content_item_array;
	s_content_item m_content_item;
	c_content_item_overlapped_task m_content_item_task;
	c_content_enumeration_overlapped_task m_content_enumeration_task;
	dword __unknown29C;
	dword __unknown2A0;
	c_synchronized_long m_content_item_count;
	dword m_flags;
	dword __unknown2AC;
};
static_assert(sizeof(c_content_catalogue) == 0x2B0);

struct s_content_catalogue_globals
{
	bool initialized;
	dword : 32;
	c_content_catalogue user_catalogues[4];
	c_storage_device storage_devices[4];
};
const size_t k_content_catalogue_globals_size = sizeof(s_content_catalogue_globals);
static_assert(sizeof(s_content_catalogue_globals) == 0xB88);

extern s_content_catalogue_globals& g_content_catalogue_globals;