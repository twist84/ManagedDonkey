#pragma once

#include "cseries/async.hpp"
#include "cseries/cseries.hpp"
#include "tag_files/files_windows.hpp"

struct c_synchronized_long;
long __cdecl async_create_file(
	wchar_t const* file_name,
	dword flags1,
	long file_open_disposition,
	dword flags2,
	e_async_category category,
	e_async_priority priority,
	s_file_handle volatile* file_handle,
	c_synchronized_long* done);
extern e_async_completion __cdecl async_create_file_callback(s_async_task* task);
void __cdecl async_create_file_task_initialize(
	wchar_t const* file_name,
	dword flags1,
	long file_open_disposition,
	dword flags2,
	long category,
	long priority,
	s_file_handle volatile* file_handle,
	s_create_file_task* create_file_task);
extern long __cdecl async_flush_file(
	s_file_handle file_handle,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done);
extern void __cdecl async_flush_file_blocking(s_file_handle file_handle, e_async_category category);
extern e_async_completion __cdecl async_flush_file_callback(s_async_task* task);
extern long __cdecl async_get_file_size(
	s_file_handle file_handle,
	dword volatile* file_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done);
extern long __cdecl async_read_position(
	s_file_handle file_handle,
	void* buffer,
	long buffer_size,
	dword offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* size,
	c_synchronized_long* done);
extern e_async_completion __cdecl async_read_position_callback(s_async_task* task);
extern long __cdecl async_read_position_ex(
	s_file_handle file_handle,
	void* buffer,
	long buffer_size,
	dword offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* size,
	c_synchronized_long* done,
	bool a9);
extern e_async_completion __cdecl async_get_file_size_callback(s_async_task* task);

