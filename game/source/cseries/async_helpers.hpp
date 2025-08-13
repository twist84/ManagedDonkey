#pragma once

#include "cseries/async.hpp"
#include "cseries/cseries.hpp"
#include "tag_files/files_windows.hpp"

class c_synchronized_long;
extern int32 __cdecl async_close_file(
	s_file_handle file,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done);

extern int32 __cdecl async_close_file_no_stfs_flush(
	s_file_handle file,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done);

extern e_async_completion __cdecl async_copy_file_callback(
	s_async_task* in_task);

extern int32 __cdecl async_copy_file_from_existing_file(
	const wchar_t* source_file_name,
	const wchar_t* dest_file_name,
	void* buffer,
	uns32 buffer_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* success,
	c_synchronized_long* done);

extern int32 __cdecl async_copy_position(
	s_file_handle src,
	s_file_handle dst,
	void* buffer,
	int32 size,
	int32 src_offset,
	int32 dst_offset,
	e_async_category category,
	e_async_priority priority,
	bool dst_on_utility_drive,
	c_synchronized_long* bytes_copied,
	c_synchronized_long* done);

extern e_async_completion __cdecl async_copy_position_callback(
	s_async_task* in_task);

int32 __cdecl async_create_file(
	const wchar_t* file_name,
	uns32 desired_access,
	int32 creation_disposition,
	uns32 flags_and_attributes,
	e_async_category category,
	e_async_priority priority,
	volatile s_file_handle* file,
	c_synchronized_long* done);

extern e_async_completion __cdecl async_create_file_callback(
	s_async_task* in_task);

void __cdecl async_create_file_task_initialize(
	const wchar_t* file_name,
	uns32 desired_access,
	int32 creation_disposition,
	uns32 flags_and_attributes,
	int32 category,
	int32 priority,
	volatile s_file_handle* file,
	s_create_file_task* out_create_file_task);

extern int32 __cdecl async_delete_file(
	const char* file_name,
	bool is_directory,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* success,
	c_synchronized_long* done);

extern int32 __cdecl async_flush_file(
	s_file_handle file_handle,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done);

extern void __cdecl async_flush_file_blocking(
	s_file_handle file,
	e_async_category category);

extern e_async_completion __cdecl async_flush_file_callback(
	s_async_task* in_task);

extern int32 __cdecl async_get_file_size(
	s_file_handle file,
	volatile uns32* file_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done);

extern e_async_completion __cdecl async_nuke_file_callback(
	s_async_task* in_task);

extern int32 __cdecl async_queue_simple_callback(
	e_async_completion(__cdecl* callback)(s_async_task*, void*, int32),
	const void* data,
	int32 data_size,
	e_async_priority priority,
	c_synchronized_long* done);

extern int32 __cdecl async_read_position(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* bytes_read,
	c_synchronized_long* done);

extern e_async_completion __cdecl async_read_position_callback(
	s_async_task* in_task);

extern int32 __cdecl async_read_position_ex(
	s_file_handle file_handle,
	void* buffer,
	int32 size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* bytes_read,
	c_synchronized_long* done,
	bool block_if_task_list_is_full);

extern int32 __cdecl async_read_position_overlapped(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* bytes_read,
	c_synchronized_long* done);

extern int32 __cdecl async_read_position_overlapped_ex(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* bytes_read,
	c_synchronized_long* done,
	bool block_if_task_list_is_full);
extern int32 __cdecl async_set_file_size(
	s_file_handle file_handle,
	int32 file_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* success,
	c_synchronized_long* done);

extern e_async_completion __cdecl async_read_position_overlapped_callback(
	s_async_task* in_task);

extern e_async_completion __cdecl async_simple_callback_task_callback(
	s_async_task* in_task);

extern int32 __cdecl async_validify_file(
	s_file_handle file,
	void* header,
	int32 header_size,
	uns32 file_size,
	e_async_category category,
	e_async_priority priority,
	bool dst_on_utility_drive,
	bool* success,
	c_synchronized_long* out_done);

extern e_async_completion __cdecl async_validify_file_callback(
	s_async_task* in_task);

int32 __cdecl async_write_position(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	bool dst_on_utility_drive,
	c_synchronized_long* bytes_written,
	c_synchronized_long* done);

extern e_async_completion __cdecl async_write_position_callback(
	s_async_task* in_task);

int32 __cdecl async_write_position_ex(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	c_flags<e_write_position_flags, uns32, k_write_position_flags> flags,
	e_async_category category,
	e_async_priority priority,
	bool dst_on_utility_drive,
	c_synchronized_long* bytes_written,
	c_synchronized_long* done);

extern e_async_completion __cdecl close_file_callback(
	s_async_task* in_task);

extern e_async_completion __cdecl close_file_no_stfs_flush_callback(
	s_async_task* in_task);

extern e_async_completion __cdecl delete_file_callback(
	s_async_task* in_task);

extern e_async_completion __cdecl enumerate_files_callback(
	s_async_task* in_task);

extern e_async_completion __cdecl get_file_size_callback(
	s_async_task* in_task);

extern e_async_completion __cdecl read_entire_file_callback(
	s_async_task* in_task);

extern e_async_completion __cdecl set_file_size_callback(
	s_async_task* in_task);

extern e_async_completion __cdecl write_buffer_to_file_callback(
	s_async_task* in_task);


