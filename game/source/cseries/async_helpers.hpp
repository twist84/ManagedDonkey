#pragma once

#include "cseries/async.hpp"
#include "cseries/cseries.hpp"
#include "tag_files/files_windows.hpp"

struct c_synchronized_long;

extern long __cdecl async_read_position(
	s_file_handle file_handle,
	void* buffer,
	long buffer_size,
	dword offset,
	e_async_category async_category,
	e_async_priority async_priority,
	c_synchronized_long* size,
	c_synchronized_long* done);

