#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

enum e_async_buffer_state
{
	_async_buffer_state_reading = 0,
	_async_buffer_state_writing,

	k_async_buffer_state_count,
	k_async_buffer_state_none = NONE,
};

enum e_async_buffer_file_access
{
	_async_buffer_file_access_write = 0,
	_async_buffer_file_access_read,
	_async_buffer_file_access_read_write,

	k_async_buffer_file_access_count,
	k_async_buffer_file_access_none = NONE,
};

enum e_async_buffer_disposition
{
	_async_buffer_disposition_create_always = 0,
	_async_buffer_disposition_open_existing,

	k_async_buffer_disposition_count,
	k_async_buffer_disposition_none = NONE,
};

struct s_async_buffer
{
	s_async_buffer();

	void* data;
	int32 allocated_size;
	bool external_buffer;
	int32 data_offset;
	int32 data_size;
	c_synchronized_long async_bytes;
	int32 async_expected_bytes;
	c_synchronized_long async_ready;
};
COMPILE_ASSERT(sizeof(s_async_buffer) == 0x20);

class c_async_buffer_set_base
{
public:
	enum
	{
		k_maximum_buffer_count = 3,
	};

public:
	c_async_buffer_set_base(int32 buffer_count);

	s_async_buffer* get_buffer(int32 buffer_index);
	int32 get_buffer_count() const;

	int32 m_buffer_count;
	c_static_array<s_async_buffer, k_maximum_buffer_count> m_buffers;
	bool m_destination_on_utility_drive;
};
COMPILE_ASSERT(sizeof(c_async_buffer_set_base) == 0x68);

class c_async_buffer_set :
	public c_async_buffer_set_base
{
public:
	c_async_buffer_set(int32 buffer_count);

	bool allocate_storage(c_allocation_base* allocator, int32 buffer_size);
	void async_read_buffer(int32 buffer_index);
	void async_write_buffer(int32 buffer_index);
	bool at_end_of_file() const;
	void block_until_not_busy();
	bool close_file();
	void complete_pending_async_buffer_activity();
	int32 consume_bytes(const void* source, int32 bytes_to_consume);
	int32 fill_bytes(void* destination, int32 bytes_to_fill);
	bool flush();
	int32 get_position();
	e_async_buffer_state get_state();
	bool handle_state(e_async_buffer_state new_state);
	void initialize();
	void initialize_buffer(int32 buffer_index);
	void initialize_internal();
	bool is_async_io_in_progress();
	bool is_data_waiting();
	bool open_file(const wchar_t* file_path, e_async_buffer_file_access file_access, e_async_buffer_disposition disposition);
	void prime_buffers_for_reading();
	void read(void* destination, int32 bytes_to_read, int32* bytes_read);
	bool ready_to_read();
	bool ready_to_write();
	void release_storage(c_allocation_base* allocator);
	bool set_position(int32 file_position);
	bool set_state(e_async_buffer_state new_state);
	void swap_buffers();
	void use_external_storage(char** buffers, int32 buffer_count, int32 buffer_size);
	void write(const void* source, int32 bytes_to_write, int32* byte_written);

protected:
	e_async_buffer_file_access m_file_access;
	e_async_buffer_state m_state;
	s_file_handle m_async_file_handle;
	int32 m_file_offset;
	int32 m_file_position;
	int32 m_file_size;
	int32 m_buffer_index;
	bool m_storage_initialized;
	bool m_end_of_file_reached;
	bool m_fatal_error_encountered;
	bool m_explicit_flush;
};
COMPILE_ASSERT(sizeof(c_async_buffer_set) == 0x88);
COMPILE_ASSERT(sizeof(c_async_buffer_set) == sizeof(c_async_buffer_set_base) + 0x20);

template<int32 k_buffer_count>
class c_async_stored_buffer_set :
	public c_async_buffer_set
{
public:
	c_async_stored_buffer_set() :
		c_async_buffer_set(k_buffer_count)
	{

	}
};


