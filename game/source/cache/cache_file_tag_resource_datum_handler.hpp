#pragma once

#include "cseries/cseries.hpp"

struct s_cache_file_resource_gestalt;
class c_cache_file_resource_uber_location_table;
class c_cache_file_combined_tag_resource_datum_handler
{
	void* __vftable;
	s_cache_file_resource_gestalt* m_resource_gestalt;
	uns32 __unknown8;
	c_basic_buffer<void>* m_interop_buffer;
	bool m_running_off_dvd;
	c_cache_file_resource_uber_location_table* m_uber_location_table;
};
static_assert(sizeof(c_cache_file_combined_tag_resource_datum_handler) == 0x18);

