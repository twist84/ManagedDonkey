#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_tag_resource_control_datum :
	s_datum_header
{
	uns16 flags;
	byte __data4[0x4];
	//s_tag_resource_location_handle_struct page_handle;
	uns32 page_handle;
};
COMPILE_ASSERT(sizeof(s_tag_resource_control_datum) == 0xC);

class c_tag_resource_cache_controller;
class c_tag_resource_cache_control_datum_info
{
public:
	c_tag_resource_cache_controller* m_cache_controller;
};

class c_tag_resource_page_table_control_interface;
class c_tag_resource_runtime_listener;
class c_tag_resource_cache_file_datum_handler;
class c_tag_resource_cache_controller
{
public:
	c_allocation_base* m_tracking_data_allocator;
	void* m_tracking_allocation_base;
	c_smart_data_array<s_tag_resource_control_datum> m_resource_control_data;
	c_wrapped_flags m_optional_resources;
	c_wrapped_flags m_usable_resources;
	c_wrapped_flags m_potentially_active_resources;
	c_wrapped_flags m_pages_ready_resources;
	c_wrapped_flags m_pending_resources;
	c_wrapped_flags m_last_published_deferred_resources;
	c_wrapped_flags m_demanded_deferred_resources;
	c_tag_resource_page_table_control_interface* m_resource_page_table;
	c_tag_resource_runtime_listener* m_resource_listener;
	c_tag_resource_cache_file_datum_handler* m_resource_datum_handler;
	c_tag_resource_cache_control_datum_info m_control_datum_info;
	uns32 m_usage_frame_index;
	uns32 m_update_since_demand_frame_counter;

	//bool m_any_deferred_demands_pending;
	//bool m_optional_caching_enabled;
	bool __unknown5C;

	bool m_prediction_can_occur;
	bool m_can_load_all_desired_resources;
	bool m_throttle_demand_to_io;
};
COMPILE_ASSERT(sizeof(c_tag_resource_cache_controller) == 0x60);

