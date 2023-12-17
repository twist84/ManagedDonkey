#include "cseries/async_buffer.hpp"

//.text:005AC870 ; 
c_async_buffer_set::c_async_buffer_set(long buffer_count) :
	c_async_buffer_set_base(buffer_count)
{
	initialize();
	m_storage_initialized = false;
}

//.text:005AC8E0 ; 
c_async_buffer_set_base::c_async_buffer_set_base(long buffer_count) :
	m_buffer_count(buffer_count),
	m_buffer()
{
}

//.text:005AC910 ; s_async_buffer::s_async_buffer
s_async_buffer::s_async_buffer()
{
	__unknown14 = 0;
	async_ready = 0;
}

//.text:005AC9C0 ; c_static_array<s_async_buffer,3>::operator[]

bool c_async_buffer_set::allocate_storage(c_allocation_base* allocation, long allocation_size)
{
	return DECLFUNC(0x005AC9E0, bool, __thiscall, c_async_buffer_set*, c_allocation_base*, long)(this, allocation, allocation_size);
}

void c_async_buffer_set::async_read_buffer(long buffer_index)
{
	DECLFUNC(0x005AD930, void, __thiscall, c_async_buffer_set*, long)(this, buffer_index);
}

void c_async_buffer_set::async_write_buffer(long buffer_index)
{
	DECLFUNC(0x005ADF50, void, __thiscall, c_async_buffer_set*, long)(this, buffer_index);
}

bool c_async_buffer_set::at_end_of_file() const
{
	return DECLFUNC(0x005AE1E0, bool, __thiscall, c_async_buffer_set const*)(this);
}

void c_async_buffer_set::block_until_not_busy()
{
	DECLFUNC(0x005AE1F0, void, __thiscall, c_async_buffer_set*)(this);
}

bool c_async_buffer_set::close_file()
{
	return DECLFUNC(0x005AE250, bool, __thiscall, c_async_buffer_set*)(this);
}

void c_async_buffer_set::complete_pending_async_buffer_activity()
{
	DECLFUNC(0x005AE350, void, __thiscall, c_async_buffer_set*)(this);
}

long c_async_buffer_set::consume_bytes(void const* source, long bytes_to_consume)
{
	return DECLFUNC(0x005AE440, long, __thiscall, c_async_buffer_set_base*, void const*, long)(this, source, bytes_to_consume);
}

long c_async_buffer_set::fill_bytes(void* destination, long bytes_to_fill)
{
	return DECLFUNC(0x005AE5C0, long, __thiscall, c_async_buffer_set_base*, void*, long)(this, destination, bytes_to_fill);
}

bool c_async_buffer_set::flush()
{
	return DECLFUNC(0x005AE640, bool, __thiscall, c_async_buffer_set*)(this);
}

s_async_buffer* c_async_buffer_set_base::get_buffer(long buffer_index)
{
	return DECLFUNC(0x005AE6C0, s_async_buffer*, __thiscall, c_async_buffer_set_base*, long)(this, buffer_index);
}

long c_async_buffer_set_base::get_buffer_count() const
{
	return DECLFUNC(0x005AE6E0, long, __thiscall, c_async_buffer_set_base const*)(this);
}

long c_async_buffer_set::get_position()
{
	return DECLFUNC(0x005AE710, long, __thiscall, c_async_buffer_set*)(this);
}

bool c_async_buffer_set::handle_state(e_async_buffer_state new_state)
{
	return DECLFUNC(0x005AE760, bool, __thiscall, c_async_buffer_set*, e_async_buffer_state)(this, new_state);
}

void c_async_buffer_set::initialize()
{
	DECLFUNC(0x005AE7E0, void, __thiscall, c_async_buffer_set*)(this);
}

void c_async_buffer_set::initialize_buffer(long buffer_index)
{
	DECLFUNC(0x005AE7F0, void, __thiscall, c_async_buffer_set*, long)(this, buffer_index);
}

void c_async_buffer_set::initialize_internal()
{
	DECLFUNC(0x005AE830, void, __thiscall, c_async_buffer_set*)(this);
}

bool c_async_buffer_set::is_async_io_in_progress()
{
	return DECLFUNC(0x005AE8E0, bool, __thiscall, c_async_buffer_set*)(this);
}

bool c_async_buffer_set::is_data_waiting()
{
	return DECLFUNC(0x005AE920, bool, __thiscall, c_async_buffer_set*)(this);
}

bool c_async_buffer_set::open_file(wchar_t const* file_path, e_async_buffer_file_access file_access, e_async_buffer_disposition disposition)
{
	return DECLFUNC(0x005AE980, bool, __thiscall, c_async_buffer_set*, wchar_t const*, e_async_buffer_file_access, e_async_buffer_disposition)(this, file_path, file_access, disposition);
}

void c_async_buffer_set::prime_buffers_for_reading()
{
	DECLFUNC(0x005AEBD0, void, __thiscall, c_async_buffer_set*)(this);
}

void c_async_buffer_set::read(void* destination, long bytes_to_read, long* bytes_read)
{
	DECLFUNC(0x005AEC30, void, __thiscall, c_async_buffer_set*, void const*, long, long*)(this, destination, bytes_to_read, bytes_read);
}

bool c_async_buffer_set::ready_to_read()
{
	return DECLFUNC(0x005AEE10, bool, __thiscall, c_async_buffer_set*)(this);
}

bool c_async_buffer_set::ready_to_write()
{
	return DECLFUNC(0x005AEE40, bool, __thiscall, c_async_buffer_set*)(this);
}

void c_async_buffer_set::release_storage(c_allocation_base* allocation)
{
	DECLFUNC(0x005AEE70, void, __thiscall, c_async_buffer_set*, c_allocation_base*)(this, allocation);
}

bool c_async_buffer_set::set_position(long position)
{
	return DECLFUNC(0x005AEF60, bool, __thiscall, c_async_buffer_set*, long)(this, position);
}

bool c_async_buffer_set::set_state(e_async_buffer_state new_state)
{
	return DECLFUNC(0x005AF080, bool, __thiscall, c_async_buffer_set*, e_async_buffer_state)(this, new_state);
}

void c_async_buffer_set::swap_buffers()
{
	DECLFUNC(0x005AF110, void, __thiscall, c_async_buffer_set*)(this);
}

void c_async_buffer_set::use_external_storage(char** buffers, long a2, long allocation_size)
{
	DECLFUNC(0x005AF240, void, __thiscall, c_async_buffer_set*, char**, long, long)(this, buffers, a2, allocation_size);
}

void c_async_buffer_set::write(void const* source, long bytes_to_write, long* byte_written)
{
	DECLFUNC(0x005AF2A0, void, __thiscall, c_async_buffer_set*, void const*, long, long*)(this, source, bytes_to_write, byte_written);
}


