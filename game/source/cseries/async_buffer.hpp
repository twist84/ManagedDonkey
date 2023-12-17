#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

enum e_async_buffer_file_access
{
};

enum e_async_buffer_disposition
{
};

enum e_async_buffer_state
{
	k_async_buffer_state_none = NONE
};

struct s_async_buffer
{
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
	long m_buffer_count;
	c_static_array<s_async_buffer, 3> m_buffer;
};
static_assert(sizeof(c_async_buffer_set_base) == 0x64);

struct c_async_buffer_set : c_async_buffer_set_base
{
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

