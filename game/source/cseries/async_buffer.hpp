#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

enum e_async_buffer_state
{
	k_async_buffer_state_none = NONE
};

enum e_async_buffer_file_access
{
};

enum e_async_buffer_disposition
{
};

struct s_async_buffer
{
	s_async_buffer();

	void* data;
	long __unknown4;
	bool use_external_storage;
	long __unknownC;
	long data_size;
	c_synchronized_long __unknown14;
	long __unknown18;
	c_synchronized_long async_ready;
};
static_assert(sizeof(s_async_buffer) == 0x20);

struct c_async_buffer_set_base
{
public:
	c_async_buffer_set_base(long buffer_count);

	s_async_buffer* get_buffer(long buffer_index);
	long get_buffer_count() const;

	long m_buffer_count;
	c_static_array<s_async_buffer, 3> m_buffer;
};
static_assert(sizeof(c_async_buffer_set_base) == 0x64);

struct c_async_buffer_set :
	public c_async_buffer_set_base
{
public:
	c_async_buffer_set(long buffer_count);

	bool allocate_storage(c_allocation_base* allocation, long allocation_size);
	void async_read_buffer(long buffer_index);
	void async_write_buffer(long buffer_index);
	bool at_end_of_file() const;
	void block_until_not_busy();
	bool close_file();
	void complete_pending_async_buffer_activity();
	long consume_bytes(void const* source, long bytes_to_consume);
	long fill_bytes(void* destination, long bytes_to_fill);
	bool flush();
	long get_position();
	bool handle_state(e_async_buffer_state new_state);
	void initialize();
	void initialize_buffer(long buffer_index);
	void initialize_internal();
	bool is_async_io_in_progress();
	bool is_data_waiting();
	bool open_file(wchar_t const* file_path, e_async_buffer_file_access file_access, e_async_buffer_disposition disposition);
	void prime_buffers_for_reading();
	void read(void* destination, long bytes_to_read, long* bytes_read);
	bool ready_to_read();
	bool ready_to_write();
	void release_storage(c_allocation_base* allocation);
	bool set_position(long position);
	bool set_state(e_async_buffer_state new_state);
	void swap_buffers();
	void use_external_storage(char** buffers, long a2, long allocation_size);
	void write(void const* source, long bytes_to_write, long* byte_written);

protected:
	bool __unknown64;
	e_async_buffer_file_access m_file_access;
	e_async_buffer_state m_state;
	s_file_handle m_async_file_handle;
	long __unknown74;
	long m_position;
	long m_file_size;
	long m_buffer_index;
	bool m_storage_initialized;
	bool m_at_end_of_file;
	bool __unknown86;
	bool __unknown87;
};
static_assert(sizeof(c_async_buffer_set) == 0x88);

