#include "cseries/async_buffer.hpp"

#include "cseries/async.hpp"
#include "cseries/cseries_events.hpp"

//.text:005AC870 ; 
c_async_buffer_set::c_async_buffer_set(int32 buffer_count) :
	c_async_buffer_set_base(buffer_count)
{
	initialize();
	m_storage_initialized = false;
}

//.text:005AC8E0 ; 
c_async_buffer_set_base::c_async_buffer_set_base(int32 buffer_count) :
	m_buffer_count(buffer_count),
	m_buffers()
{
	ASSERT(buffer_count < m_buffers.get_count());
}

//.text:005AC910 ; s_async_buffer::s_async_buffer
s_async_buffer::s_async_buffer() :
	data(nullptr),
	allocated_size(0),
	external_buffer(false),
	data_offset(0),
	data_size(0),
	async_bytes(0),
	async_expected_bytes(0),
	async_ready(0)
{
}

//.text:005AC9C0 ; c_static_array<s_async_buffer,3>::operator[]

bool c_async_buffer_set::allocate_storage(c_allocation_base* allocator, int32 buffer_size)
{
	return INVOKE_CLASS_MEMBER(0x005AC9E0, c_async_buffer_set, allocate_storage, allocator, buffer_size);

	//ASSERT(allocator);
	//ASSERT(buffer_size > 0);
	//ASSERT(!m_storage_initialized);
	//
	//bool failed_to_allocate_storage = false;
	//for (int32 buffer_index = 0; buffer_index < get_buffer_count(); buffer_index++)
	//{
	//	s_async_buffer* buffer = get_buffer(buffer_index);
	//	buffer->data = allocator->allocate(buffer_size, "dbl buffer");
	//	buffer->use_external_storage = false;
	//
	//	if (!buffer->data)
	//	{
	//		event(_event_critical, "### async_double_bufffer: failed to allocate %d bytes for buffer %d", buffer_size, buffer_index);
	//		failed_to_allocate_storage = true;
	//		break;
	//	}
	//
	//	buffer->data_allocation_size = buffer_size;
	//}
	//
	//if (failed_to_allocate_storage)
	//{
	//	for (int32 buffer_index = 0; buffer_index < get_buffer_count(); buffer_index++)
	//	{
	//		s_async_buffer* buffer = get_buffer(buffer_index);
	//		if (buffer->data)
	//			allocator->deallocate(buffer->data);
	//
	//		buffer->data_allocation_size = 0;
	//	}
	//
	//}
	//m_storage_initialized = !failed_to_allocate_storage;
	//
	//return m_storage_initialized;
}

void c_async_buffer_set::async_read_buffer(int32 buffer_index)
{
	INVOKE_CLASS_MEMBER(0x005AD930, c_async_buffer_set, async_read_buffer, buffer_index);
}

void c_async_buffer_set::async_write_buffer(int32 buffer_index)
{
	INVOKE_CLASS_MEMBER(0x005ADF50, c_async_buffer_set, async_write_buffer, buffer_index);

	//ASSERT(ready_to_write());
	//
	//s_async_buffer* buffer = get_buffer(buffer_index);
	//ASSERT(buffer->async_ready);
	//ASSERT(buffer->data_size > 0);
	//
	//// ...
}

bool c_async_buffer_set::at_end_of_file() const
{
	return INVOKE_CLASS_MEMBER(0x005AE1E0, c_async_buffer_set, at_end_of_file);

	//return m_at_end_of_file;
}

void c_async_buffer_set::block_until_not_busy()
{
	INVOKE_CLASS_MEMBER(0x005AE1F0, c_async_buffer_set, block_until_not_busy);

	//if (async_usable() && (ready_to_read() || ready_to_write()))
	//{
	//	complete_pending_async_buffer_activity();
	//	flush();
	//	complete_pending_async_buffer_activity();
	//}
}

bool c_async_buffer_set::close_file()
{
	return INVOKE_CLASS_MEMBER(0x005AE250, c_async_buffer_set, close_file);

	//if (async_usable() && file_handle_is_valid(m_async_file_handle))
	//{
	//	ASSERT(m_storage_initialized);
	//	complete_pending_async_buffer_activity();
	//
	//	c_synchronized_long done = 0;
	//	if (async_close_file(m_async_file_handle, _async_category_none, _async_priority_blocking_generic, &done) == NONE)
	//	{
	//		event(_event_warning, "async:double_buffer: failed to close file");
	//	}
	//	else
	//	{
	//		internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);
	//	}
	//
	//	return true;
	//}
	//initialize_internal();
	//return false;
}

void c_async_buffer_set::complete_pending_async_buffer_activity()
{
	INVOKE_CLASS_MEMBER(0x005AE350, c_async_buffer_set, complete_pending_async_buffer_activity);

	//switch (m_state)
	//{
	//case 0:
	//{
	//	ASSERT(ready_to_read());
	//	for (int32 buffer_index = 0; buffer_index < get_buffer_count(); buffer_index++)
	//	{
	//		s_async_buffer* buffer = get_buffer(buffer_index);
	//		internal_async_yield_until_done(&buffer->async_ready, false, false, __FILE__, __LINE__);
	//	}
	//}
	//break;
	//case 1:
	//{
	//	ASSERT(ready_to_write());
	//	for (int32 buffer_index = 0; buffer_index < get_buffer_count(); buffer_index++)
	//	{
	//		s_async_buffer* buffer = get_buffer(buffer_index);
	//		if (buffer->async_ready && buffer->data_size > 0 && !buffer->__unknown18)
	//		{
	//			async_write_buffer(buffer_index);
	//		}
	//		if (!buffer->async_ready)
	//		{
	//			internal_async_yield_until_done(&buffer->async_ready, false, false, __FILE__, __LINE__);
	//			if (buffer->__unknown18 == buffer->__unknown14)
	//			{
	//				initialize_buffer(buffer_index);
	//			}
	//			else
	//			{
	//				event(_event_warning, "async:double_buffer: fatal error encountered during write (expected byte count 0x%8X != 0x%8X)", buffer->__unknown18, buffer->__unknown14);
	//				m_fatal_error_occurred = true;
	//			}
	//		}
	//	}
	//}
	//break;
	//case k_async_buffer_state_none:
	//{
	//	if (m_file_access == 2)
	//		UNREACHABLE();
	//}
	//break;
	//default:
	//{
	//	UNREACHABLE();
	//}
	//break;
	//}
}

int32 c_async_buffer_set::consume_bytes(const void* source, int32 bytes_to_consume)
{
	return INVOKE_CLASS_MEMBER(0x005AE440, c_async_buffer_set, consume_bytes, source, bytes_to_consume);

	//s_async_buffer* buffer = get_buffer(m_buffer_index);
	//if (bytes_to_consume > buffer->data_allocation_size - buffer->data_size)
	//{
	//	bytes_to_consume = buffer->data_allocation_size - buffer->data_size;
	//}
	//ASSERT(buffer->async_ready);
	//if (bytes_to_consume <= 0)
	//{
	//	swap_buffers();
	//}
	//else
	//{
	//	csmemcpy(offset_pointer(buffer->data, buffer->data_size), source, bytes_to_consume);
	//	buffer->data_size += bytes_to_consume;
	//}
	//return bytes_to_consume;
}

int32 c_async_buffer_set::fill_bytes(void* destination, int32 bytes_to_fill)
{
	return INVOKE_CLASS_MEMBER(0x005AE5C0, c_async_buffer_set, fill_bytes, destination, bytes_to_fill);

	//s_async_buffer* buffer = get_buffer(m_buffer_index);
	//int32 bytes_available_to_read = buffer->__unknown14 - buffer->__unknownC;
	//if (bytes_to_fill > bytes_available_to_read)
	//{
	//	bytes_to_fill = bytes_available_to_read;
	//}
	//ASSERT(bytes_available_to_read >= 0);
	//ASSERT(buffer->async_ready);
	//if (bytes_to_fill <= 0)
	//{
	//	if (buffer->__unknown14 == buffer->data_allocation_size)
	//		swap_buffers();
	//	else
	//		m_at_end_of_file = true;
	//}
	//else
	//{
	//	csmemcpy(destination, offset_pointer(buffer->data, buffer->__unknownC), bytes_to_fill);
	//	buffer->__unknownC += bytes_to_fill;
	//}
	//return bytes_to_fill;
}

bool c_async_buffer_set::flush()
{
	return INVOKE_CLASS_MEMBER(0x005AE640, c_async_buffer_set, flush);

	//if (async_usable() && ready_to_write())
	//{
	//	if (get_buffer(m_buffer_index)->data_size > 0)
	//	{
	//		async_write_buffer(m_buffer_index);
	//		__unknown87 = true;
	//		m_buffer_index = (m_buffer_index + 1) % m_buffer_count;
	//	}
	//	return true;
	//}
	//return false;
}

s_async_buffer* c_async_buffer_set_base::get_buffer(int32 buffer_index)
{
	return INVOKE_CLASS_MEMBER(0x005AE6C0, c_async_buffer_set_base, get_buffer, buffer_index);

	//return &m_buffers[buffer_index];
}

int32 c_async_buffer_set_base::get_buffer_count() const
{
	return INVOKE_CLASS_MEMBER(0x005AE6E0, c_async_buffer_set_base, get_buffer_count);

	//return m_buffer_count;
}

int32 c_async_buffer_set::get_position()
{
	return INVOKE_CLASS_MEMBER(0x005AE710, c_async_buffer_set, get_position);

	//if (async_usable() && (ready_to_write() || ready_to_read()))
	//{
	//	return m_file_position;
	//}
	//return NONE;
}

e_async_buffer_state c_async_buffer_set::get_state()
{
	return m_state;
}

bool c_async_buffer_set::handle_state(e_async_buffer_state new_state)
{
	return INVOKE_CLASS_MEMBER(0x005AE760, c_async_buffer_set, handle_state, new_state);

	//ASSERT(new_state != k_async_buffer_state_none);
	//if (async_usable())
	//{
	//	//...
	//}
}

void c_async_buffer_set::initialize()
{
	INVOKE_CLASS_MEMBER(0x005AE7E0, c_async_buffer_set, initialize);

	//initialize_internal();
}

void c_async_buffer_set::initialize_buffer(int32 buffer_index)
{
	INVOKE_CLASS_MEMBER(0x005AE7F0, c_async_buffer_set, initialize_buffer, buffer_index);

	//s_async_buffer* buffer = get_buffer(buffer_index);
	//buffer->__unknown14 = 0;
	//buffer->__unknown18 = 0;
	//buffer->data_size = 0;
	//buffer->__unknownC = 0;
	//buffer->async_ready = true;
}

void c_async_buffer_set::initialize_internal()
{
	INVOKE_CLASS_MEMBER(0x005AE830, c_async_buffer_set, initialize_internal);

	//m_file_access = _async_buffer_file_access_none;
	//m_state = k_async_buffer_state_none;
	//__unknown87 = false;
	//m_at_end_of_file = false;
	//__unknown74 = 0;
	//m_file_position = 0;
	//m_file_size = 0;
	//m_buffer_index = 0;
	//m_fatal_error_occurred = false;
	//invalidate_file_handle(&m_async_file_handle);
	//for (int32 buffer_index = 0; buffer_index < get_buffer_count(); buffer_index++)
	//{
	//	initialize_buffer(buffer_index);
	//}
}

bool c_async_buffer_set::is_async_io_in_progress()
{
	return INVOKE_CLASS_MEMBER(0x005AE8E0, c_async_buffer_set, is_async_io_in_progress);

	//for (int32 buffer_index = 0; buffer_index < get_buffer_count(); buffer_index++)
	//{
	//	if (!get_buffer(buffer_index)->async_ready)
	//		return true;
	//}
	//return false;
}

bool c_async_buffer_set::is_data_waiting()
{
	return INVOKE_CLASS_MEMBER(0x005AE920, c_async_buffer_set, is_data_waiting);

	//return async_usable() && (ready_to_read() || ready_to_write()) && get_buffer(m_buffer_index)->data_size > 0;
}

bool c_async_buffer_set::open_file(const wchar_t* file_path, e_async_buffer_file_access file_access, e_async_buffer_disposition disposition)
{
	return INVOKE_CLASS_MEMBER(0x005AE980, c_async_buffer_set, open_file, file_path, file_access, disposition);

	//ASSERT(m_storage_initialized);
	//if (!async_usable())
	//{
	//	ASSERT(file_path);
	//	ASSERT(!file_handle_is_valid(m_async_file_handle));
	//	//...
	//}
	//return false;
}

void c_async_buffer_set::prime_buffers_for_reading()
{
	INVOKE_CLASS_MEMBER(0x005AEBD0, c_async_buffer_set, prime_buffers_for_reading);

	//if (async_usable() && ready_to_read())
	//{
	//	ASSERT(m_buffer_index == 0);
	//	complete_pending_async_buffer_activity();
	//	for (int32 buffer_index = 0; buffer_index < get_buffer_count(); buffer_index++)
	//	{
	//		async_read_buffer(buffer_index);
	//	}
	//	internal_async_yield_until_done(&get_buffer(0)->async_ready, false, false, __FILE__, __LINE__);
	//}
}

void c_async_buffer_set::read(void* destination, int32 bytes_to_read, int32* bytes_read)
{
	INVOKE_CLASS_MEMBER(0x005AEC30, c_async_buffer_set, read, destination, bytes_to_read, bytes_read);

	//ASSERT(destination);
	//ASSERT(bytes_to_read > 0);
	//ASSERT(bytes_read);
	////...
}

bool c_async_buffer_set::ready_to_read()
{
	return INVOKE_CLASS_MEMBER(0x005AEE10, c_async_buffer_set, ready_to_read);

	//return file_handle_is_valid(m_async_file_handle) && (m_file_access == 1 || m_file_access == 2);
}

bool c_async_buffer_set::ready_to_write()
{
	return INVOKE_CLASS_MEMBER(0x005AEE40, c_async_buffer_set, ready_to_write);

	//return file_handle_is_valid(m_async_file_handle) && (m_file_access == 0 || m_file_access == 2);
}

void c_async_buffer_set::release_storage(c_allocation_base* allocator)
{
	INVOKE_CLASS_MEMBER(0x005AEE70, c_async_buffer_set, release_storage, allocator);

	//if (m_storage_initialized)
	//{
	//	close_file();
	//	for (int32 buffer_index = 0; buffer_index < get_buffer_count(); buffer_index++)
	//	{
	//		s_async_buffer* buffer = get_buffer(buffer_index);
	//		ASSERT(buffer->data);
	//		if (!buffer->use_external_storage)
	//		{
	//			ASSERT(allocator);
	//			allocator->deallocate(buffer->data);
	//		}
	//		buffer->data_allocation_size = 0;
	//		buffer->use_external_storage = false;
	//	}
	//	m_storage_initialized = false;
	//}
}

bool c_async_buffer_set::set_position(int32 file_position)
{
	return INVOKE_CLASS_MEMBER(0x005AEF60, c_async_buffer_set, set_position, file_position);

	//ASSERT(file_position >= 0);
	////...
}

bool c_async_buffer_set::set_state(e_async_buffer_state new_state)
{
	return INVOKE_CLASS_MEMBER(0x005AF080, c_async_buffer_set, set_state, new_state);

	//if (async_usable() && (ready_to_read() || ready_to_write()))
	//{
	//	return handle_state(new_state);
	//}
	//return false;
}

void c_async_buffer_set::swap_buffers()
{
	INVOKE_CLASS_MEMBER(0x005AF110, c_async_buffer_set, swap_buffers);
}

void c_async_buffer_set::use_external_storage(char** buffers, int32 buffer_count, int32 buffer_size)
{
	INVOKE_CLASS_MEMBER(0x005AF240, c_async_buffer_set, use_external_storage, buffers, buffer_count, buffer_size);

	//ASSERT(buffer_count == get_buffer_count());
	//ASSERT(buffer_size > 0);
	//ASSERT(!m_storage_initialized);
	//for (int32 buffer_index = 0; buffer_index < get_buffer_count(); buffer_index++)
	//{
	//	s_async_buffer* buffer = get_buffer(buffer_index);
	//	buffer->data = buffers[buffer_index];
	//	buffer->data_allocation_size = buffer_size;
	//	buffer->use_external_storage = true;
	//}
}

void c_async_buffer_set::write(const void* source, int32 bytes_to_write, int32* byte_written)
{
	INVOKE_CLASS_MEMBER(0x005AF2A0, c_async_buffer_set, write, source, bytes_to_write, byte_written);

	//ASSERT(source);
	//ASSERT(bytes_to_write > 0);
	//ASSERT(m_storage_initialized);
	////...
}


