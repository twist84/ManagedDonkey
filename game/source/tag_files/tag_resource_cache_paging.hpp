#pragma once

#include "cseries/cseries.hpp"

struct c_tag_resource_cache_new;
struct c_tag_resource_page_table_io_listener
{
	c_tag_resource_cache_new* m_resource_cache_new;
};
static_assert(sizeof(c_tag_resource_page_table_io_listener) == 0x4);

struct s_tag_resource_page_datum :
	s_datum_header
{
	// 1: c_io_result::set_disk_failure
	// 2: c_io_result::set_checksum_failure
	byte_flags flags;

	byte state;
	byte __data4[0x2];
	c_reference_count<short> reference_count;
	long reservation;
	c_basic_buffer<void> buffer;
};
static_assert(sizeof(s_tag_resource_page_datum) == 0x14);

struct s_lruv_cache;
struct c_tag_resource_page_range_allocator;
struct c_tag_resource_cache_file_reader;
struct c_cache_file_tag_resource_location_handler;
struct c_tag_resource_cache_published_location_interface;
struct c_tag_resource_page_table_control_interface;
struct c_tag_resource_page_table
{
	struct c_tag_resource_lruv_cache
	{
		long m_allocation_alignment;
		c_tag_resource_page_range_allocator* m_storage_allocator;
		s_lruv_cache* m_lruv_cache;
		c_basic_buffer<void> m_lruv_data;
	};
	static_assert(sizeof(c_tag_resource_lruv_cache) == 0x14);

	c_smart_data_array<s_tag_resource_page_datum> m_resource_page_data;
	c_tag_resource_page_table::c_tag_resource_lruv_cache __lruv_cache4;
	s_data_array* m_pending_resource_requests;
	c_wrapped_flags m_ready_pages_flags;
	c_wrapped_flags m_pinned_pages_flags;
	c_tag_resource_cache_file_reader* m_cache_file_reader;
	c_cache_file_tag_resource_location_handler* m_resource_location_handler;
	c_tag_resource_cache_published_location_interface* m_published_location_interface;
	c_allocation_base* m_allocator;
	void* m_allocation;
	c_tag_resource_page_table_control_interface* m_control_interface;

	byte __data44[0x4];
	s_data_iterator __unknown0_iterator;
	byte __data[0xC];
	bool m_stop_prefetching;
	bool __unknown61;
};
static_assert(sizeof(c_tag_resource_page_table) == 0x64);

struct s_indirect_cache_file_read_request
{
	c_basic_buffer<void> buffer;
	qword __unknown8;
	dword __unknown10;
	dword processed_compressed_size;
	dword __unknown18;
	long codec_index;
	bool __unknown20;
	dword checksum;
};
static_assert(sizeof(s_indirect_cache_file_read_request) == 0x28);

enum e_indirect_cache_file_read_result
{
	_indirect_cache_file_read_result_success = 0,
	_indirect_cache_file_read_result_location_invalid,
	_indirect_cache_file_read_result_io_failure,
	_indirect_cache_file_read_result_decompression_failure,
	_indirect_cache_file_read_result_invalid_checksum,
	_indirect_cache_file_read_result_failure_unknown,

	k_indirect_cache_file_read_result_count
};

struct c_indirect_cache_file_bulk_read_iterator;
struct c_tag_resource_cache_file_reader;
struct c_tag_resource_cache_file_datum_handler;
struct c_synchronous_resource_io
{
public:
	virtual void __cdecl request_data(long requested_page_handle, s_indirect_cache_file_read_request const* read_request);

//protected:
	c_indirect_cache_file_bulk_read_iterator* m_bulk_read_iterator;
	c_static_sized_dynamic_array<qword, 1500> m_resource_sort_keys;
	c_static_sized_dynamic_array<s_indirect_cache_file_read_request, 1500> m_resource_read_requests;
	c_static_sized_dynamic_array<long, 1500> m_requested_page_handles;
	c_static_sized_dynamic_array<e_indirect_cache_file_read_result, 1500> m_read_results;
	long m_total_request_size;
	long m_finished_io_request_size;
	long __unknown14848;
	long __unknown1484C;
	long m_outstanding_requests;
	c_tag_resource_page_table* m_page_table;
	c_tag_resource_cache_file_reader* m_cache_file_reader;
	c_tag_resource_cache_file_datum_handler* m_cache_file_datum_handler;
};
//static_assert(sizeof(c_synchronous_resource_io) == 0x14860);

