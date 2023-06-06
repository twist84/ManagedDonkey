#pragma once

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cseries/cseries.hpp"

struct c_indirect_cache_file_bulk_read_iterator;
struct c_tag_resource_page_table;
struct c_tag_resource_cache_file_reader;
struct c_tag_resource_cache_file_datum_handler;
struct c_synchronous_resource_io
{
public:
	virtual void __cdecl request_data(long requested_page_handle, s_indirect_cache_file_read_request const* read_request);

protected:
	c_indirect_cache_file_bulk_read_iterator* m_bulk_read_iterator;
	c_static_sized_dynamic_array<qword, 1500> m_resource_sort_keys;
	c_static_sized_dynamic_array<s_indirect_cache_file_read_request, 1500> m_resource_read_requests;
	c_static_sized_dynamic_array<long, 1500> m_requested_page_handles;
	c_static_sized_dynamic_array<e_indirect_cache_file_read_result, 1500> m_read_results;
	long m_total_request_size;
	long m_finished_io_request_size;
	long __unknownEA84;
	long __unknownEA88;
	long m_outstanding_requests;
	c_tag_resource_page_table* m_page_table;
	c_tag_resource_cache_file_reader* m_cache_file_reader;
	c_tag_resource_cache_file_datum_handler* m_cache_file_datum_handler;
};
static_assert(sizeof(c_synchronous_resource_io) == 0x14860);

