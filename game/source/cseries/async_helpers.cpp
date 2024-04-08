#include "cseries/async_helpers.hpp"

long __cdecl async_read_position(
	s_file_handle file_handle,
	void* buffer,
	long buffer_size,
	dword offset,
	e_async_category async_category,
	e_async_priority async_priority,
	c_synchronized_long* size,
	c_synchronized_long* done)
{
	return INVOKE(0x005ADA80, async_read_position,
		file_handle,
		buffer,
		buffer_size,
		offset,
		async_category,
		async_priority,
		size,
		done);
}

