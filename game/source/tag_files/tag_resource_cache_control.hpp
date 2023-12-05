#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_tag_resource_control_datum : s_datum_header
{
	word_flags flags;
	byte __data4[0x4];
	//s_tag_resource_location_handle_struct page_handle;
	dword page_handle;
};
static_assert(sizeof(s_tag_resource_control_datum) == 0xC);

struct c_tag_resource_page_table_control_interface;
struct c_cache_file_combined_tag_resource_datum_handler;
struct c_tag_resource_control_datum;
struct c_tag_resource_cache_controller
{
	c_allocation_base* m_allocator;
	void* m_allocation; // "tag resource cache controller"

	// "tag resource cache control state"
	c_smart_data_array<s_tag_resource_control_datum> m_resource_control_data;

	c_wrapped_flags __flagsC;
	c_wrapped_flags __flags14;

	// prepare_resource_for_page_read_success
	c_wrapped_flags m_demanded_deferred_resources;

	c_wrapped_flags __flags24;
	c_wrapped_flags __flags2C;
	c_wrapped_flags __flags34;
	c_wrapped_flags __flags3C;

	c_tag_resource_page_table_control_interface* m_controller_interface;

	void* __unknown48;

	c_cache_file_combined_tag_resource_datum_handler* m_resource_datum_handler;
	c_tag_resource_control_datum* m_resource_control_datum;

	byte __data54[0x4];

	// update_active_resources, __unknown58++
	long __unknown58;

	byte __unknown5C;

	// refresh_deferred_resources
	bool m_prediction_can_occur;

	bool m_can_load_all_desired_resources;

	byte __unknown5F;
};
static_assert(sizeof(c_tag_resource_cache_controller) == 0x60);

