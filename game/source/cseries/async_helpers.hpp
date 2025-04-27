#pragma once

#include "cseries/async.hpp"
#include "cseries/cseries.hpp"
#include "tag_files/files_windows.hpp"

class c_synchronized_long;
extern int32 __cdecl async_close_file(
	s_file_handle file_handle,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done);
extern int32 __cdecl async_close_file_no_stfs_flush(
	s_file_handle file_handle,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done);
extern e_async_completion __cdecl async_copy_file_callback(s_async_task* task);
extern int32 __cdecl async_copy_file_from_existing_file(
	wchar_t const* source_file,
	wchar_t const* destination_file,
	void* copy_buffer,
	uint32 copy_buffer_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* success,
	c_synchronized_long* done);
extern int32 __cdecl async_copy_position(
	s_file_handle source_handle,
	s_file_handle destination_handle,
	void* buffer,
	int32 buffer_size,
	int32 source_offset,
	int32 destination_offset,
	e_async_category category,
	e_async_priority priority,
	bool a9,
	c_synchronized_long* size,
	c_synchronized_long* done);
extern e_async_completion __cdecl async_copy_position_callback(s_async_task* task);
int32 __cdecl async_create_file(
	wchar_t const* file_name,
	uint32 flags1,
	int32 file_open_disposition,
	uint32 flags2,
	e_async_category category,
	e_async_priority priority,
	s_file_handle volatile* file_handle,
	c_synchronized_long* done);
extern e_async_completion __cdecl async_create_file_callback(s_async_task* task);
void __cdecl async_create_file_task_initialize(
	wchar_t const* file_name,
	uint32 flags1,
	int32 file_open_disposition,
	uint32 flags2,
	int32 category,
	int32 priority,
	s_file_handle volatile* file_handle,
	s_create_file_task* create_file_task);
extern int32 __cdecl async_flush_file(
	s_file_handle file_handle,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done);
extern void __cdecl async_flush_file_blocking(s_file_handle file_handle, e_async_category category);
extern e_async_completion __cdecl async_flush_file_callback(s_async_task* task);
extern int32 __cdecl async_get_file_size(
	s_file_handle file_handle,
	uint32 volatile* file_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done);
extern e_async_completion __cdecl async_nuke_file_callback(s_async_task* task);
extern int32 __cdecl async_queue_simple_callback(
	e_async_completion(__cdecl* callback)(s_async_task* task, void* data, int32 data_size),
	void const* data,
	int32 data_size,
	e_async_priority priority,
	c_synchronized_long* done);
extern int32 __cdecl async_read_position(
	s_file_handle file_handle,
	void* buffer,
	int32 buffer_size,
	uint32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* size,
	c_synchronized_long* done);
extern e_async_completion __cdecl async_read_position_callback(s_async_task* task);
extern int32 __cdecl async_read_position_ex(
	s_file_handle file_handle,
	void* buffer,
	int32 buffer_size,
	uint32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* size,
	c_synchronized_long* done,
	bool a9);
extern int32 __cdecl async_read_position_overlapped(
	s_file_handle file_handle,
	void* buffer,
	int32 buffer_size,
	uint32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* size,
	c_synchronized_long* done);
extern int32 __cdecl async_read_position_overlapped_ex(
	s_file_handle file_handle,
	void* buffer,
	int32 buffer_size,
	uint32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* size,
	c_synchronized_long* done,
	bool a9);
extern int32 __cdecl async_set_file_size(
	s_file_handle file_handle,
	int32 file_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* success,
	c_synchronized_long* done);
extern e_async_completion __cdecl async_read_position_overlapped_callback(s_async_task* task);
extern e_async_completion __cdecl async_simple_callback_task_callback(s_async_task* task);
extern int32 __cdecl async_validify_file(
	s_file_handle file_handle,
	void* buffer,
	int32 buffer_size,
	uint32 offset,
	e_async_category category,
	e_async_priority priority,
	bool a7,
	bool* success,
	c_synchronized_long* done);
extern e_async_completion __cdecl async_validify_file_callback(s_async_task* task);
int32 __cdecl async_write_position(
	s_file_handle file,
	void* buffer,
	int32 size,
	uint32 offset,
	e_async_category category,
	e_async_priority priority,
	bool a7,
	c_synchronized_long* success);
extern e_async_completion __cdecl async_write_position_callback(s_async_task* task);
int32 __cdecl async_write_position_ex(
	s_file_handle file,
	void* buffer,
	int32 size,
	uint32 offset,
	c_flags<e_write_position_flags, uint32, k_write_position_flags> flags,
	e_async_category category,
	e_async_priority priority,
	bool a8,
	c_synchronized_long* success);
extern e_async_completion __cdecl async_close_file_callback(s_async_task* task);
extern e_async_completion __cdecl async_close_file_no_stfs_flush_callback(s_async_task* task);
extern e_async_completion __cdecl async_delete_file_callback(s_async_task* task);
extern e_async_completion __cdecl async_enumerate_files_callback(s_async_task* task);
extern e_async_completion __cdecl async_get_file_size_callback(s_async_task* task);
extern e_async_completion __cdecl async_read_entire_file_callback(s_async_task* task);
extern e_async_completion __cdecl async_set_file_size_callback(s_async_task* task);
extern e_async_completion __cdecl async_write_buffer_to_file_callback(s_async_task* task);

