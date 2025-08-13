#include "cseries/async_helpers.hpp"

int32 __cdecl async_close_file(
	s_file_handle file,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ACA70, async_close_file,
	//	file,
	//	category,
	//	priority,
	//	done);

	s_close_file_task task{};
	task.hFile = file.handle;
	return async_task_add(priority, &task.dummy_for_size, category, close_file_callback, done);
}

int32 __cdecl async_close_file_no_stfs_flush(
	s_file_handle file,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ACAC0, async_close_file_no_stfs_flush,
	//	file,
	//	category,
	//	priority,
	//	done);

	s_close_file_task task{};

	task.hFile = file.handle;
	return async_task_add(priority, &task.dummy_for_size, category, close_file_no_stfs_flush_callback, done);
}

//.text:005ACB10 ; bool __cdecl async_compare_tasks(s_compare_tasks_data*, e_async_completion(__cdecl*)(s_async_task*), e_async_completion(__cdecl*)(s_async_task*), e_async_priority, e_async_priority, const s_async_task*, const s_async_task*)
//.text:005ACBA0 ; void __cdecl async_compare_tasks_start(s_compare_tasks_data*)

e_async_completion __cdecl async_copy_file_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005ACBC0, async_copy_file_callback, in_task);
}

int32 __cdecl async_copy_file_from_existing_file(
	const wchar_t* source_file_name,
	const wchar_t* dest_file_name,
	void* buffer,
	uns32 buffer_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* success,
	c_synchronized_long* done)
{
	return INVOKE(0x005ACCE0, async_copy_file_from_existing_file,
		source_file_name,
		dest_file_name,
		buffer,
		buffer_size,
		category,
		priority,
		success,
		done);
}

int32 __cdecl async_copy_position(
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
	c_synchronized_long* done)
{
	return INVOKE(0x005ACD70, async_copy_position,
		src,
		dst,
		buffer,
		size,
		src_offset,
		dst_offset,
		category,
		priority,
		dst_on_utility_drive,
		bytes_copied,
		done);
}

e_async_completion __cdecl async_copy_position_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005ACE10, async_copy_position_callback,
		in_task);
}

int32 __cdecl async_create_file(
	const wchar_t* file_name,
	uns32 desired_access,
	int32 creation_disposition,
	uns32 flags_and_attributes,
	e_async_category category,
	e_async_priority priority,
	volatile s_file_handle* file,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ACF50, async_create_file,
	//	file_name,
	//	desired_access,
	//	creation_disposition,
	//	flags_and_attributes,
	//	category,
	//	priority,
	//	file,
	//	done);

	s_create_file_task task{};

	async_create_file_task_initialize(
		file_name,
		desired_access,
		creation_disposition,
		flags_and_attributes,
		category,
		priority,
		file,
		&task);

	return async_task_add(priority, &task.dummy_for_size, category, async_create_file_callback, done);
}

e_async_completion __cdecl async_create_file_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005ACFA0, async_create_file_callback,
		in_task);
}

//.text:005AD130 ; void __cdecl async_create_file_from_reference_blocking(s_file_reference*, uns32, e_file_open_disposition, uns32, e_async_category, s_file_handle volatile*)
//.text:005AD1C0 ; void __cdecl async_create_file_synchronous(const wchar_t*, uns32, e_file_open_disposition, uns32, int32, int32, s_file_handle*)

void __cdecl async_create_file_task_initialize(
	const wchar_t* file_name,
	uns32 desired_access,
	int32 creation_disposition,
	uns32 flags_and_attributes,
	int32 category,
	int32 priority,
	volatile s_file_handle* file,
	s_create_file_task* out_create_file_task)
{
	INVOKE(0x005AD230, async_create_file_task_initialize,
		file_name,
		desired_access,
		creation_disposition,
		flags_and_attributes,
		category,
		priority,
		file,
		out_create_file_task);
}

int32 __cdecl async_delete_file(
	const char* file_name,
	bool is_directory,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* success,
	c_synchronized_long* done)
{
	//return INVOKE(0x005AD370, async_delete_file,
	//	file_name,
	//	is_directory,
	//	category,
	//	priority,
	//	success,
	//	done);

	if (success)
	{
		success->set(0);
	}

	s_delete_file_task task{};
	csstrnzcpy(task.file_name, file_name, sizeof(task.file_name));
	task.is_directory = is_directory;
	task.success = success;
	return async_task_add(priority, &task.dummy_for_size, category, delete_file_callback, done);
}

int32 __cdecl async_enumerate_files(
	uns32 find_files_flags,
	const char* directory,
	int32 maximum_count,
	s_find_file_data* in_find_file_data,
	s_file_reference* out_references,
	int32* out_reference_count,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* success,
	c_synchronized_long* done)
{
	//return INVOKE(0x005AD3F0, async_enumerate_files, 
	//	find_files_flags,
	//	directory,
	//	maximum_count,
	//	in_find_file_data,
	//	out_references,
	//	out_reference_count,
	//	category,
	//	priority,
	//	success,
	//	done);

	ASSERT(directory != NULL);
	ASSERT(maximum_count > 0);
	ASSERT(in_find_file_data != NULL);
	ASSERT(out_references != NULL);
	ASSERT(out_reference_count != NULL);
	ASSERT(success != NULL);
	ASSERT(done != NULL);

	//*out_reference_count = 0;
	//success->set(0);
	//done->set(0);

	s_enumerate_files_task task{};
	csmemset(&task, 0, sizeof(s_enumerate_files_task));
	csstrnzcpy(task.directory, directory, sizeof(task.directory));
	task.find_files_flags = find_files_flags;
	task.maximum_count = maximum_count;
	task.find_file_data = in_find_file_data;
	task.out_references = out_references;
	task.out_reference_count = out_reference_count;
	task.success = success;

	return async_task_add(priority, &task.dummy_for_size, category, enumerate_files_callback, done);
}

int32 __cdecl async_flush_file(
	s_file_handle file,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done)
{
	//return INVOKE(0x005AD4A0, async_flush_file,
	//	file,
	//	category,
	//	priority,
	//	done);

	s_async_flush_file_task task{};
	task.hFile = file.handle;

	return async_task_add(priority, &task.dummy_for_size, category, async_flush_file_callback, done);
}

void __cdecl async_flush_file_blocking(
	s_file_handle file,
	e_async_category category)
{
	INVOKE(0x005AD4F0, async_flush_file_blocking, file, category);
}

e_async_completion __cdecl async_flush_file_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005AD560, async_flush_file_callback, in_task);
}

int32 __cdecl async_get_file_size(
	s_file_handle file,
	volatile uns32* file_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done)
{
	//return INVOKE(0x005AD590, async_get_file_size,
	//	file,
	//	file_size,
	//	category,
	//	priority,
	//	done);

	s_get_file_size_task task{};
	task.hFile = file.handle;
	task.file_size = file_size;

	return async_task_add(priority, &task.dummy_for_size, category, get_file_size_callback, done);
}

//.text:005AD5F0 ; void __cdecl async_helpers_dispose()
//.text:005AD600 ; void __cdecl async_helpers_initialize()
//.text:005AD610 ; int32 __cdecl async_nuke_file(s_file_handle, uns32, const void*, e_async_category, e_async_priority, bool*, c_synchronized_long*)

e_async_completion __cdecl async_nuke_file_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005AD660, async_nuke_file_callback, in_task);
}

//.text:005AD6E0 ; int32 __cdecl async_perform_operation_between_files(const wchar_t*, const wchar_t*, bool, bool, bool, bool, uns32, uns32, e_async_category, e_async_priority, s_async_task*, e_async_completion(__cdecl*)(s_async_task*), c_synchronized_long*, s_file_handle*, s_file_handle*)

int32 __cdecl async_queue_simple_callback(
	e_async_completion(__cdecl* callback)(s_async_task*, void*, int32),
	const void* data,
	int32 data_size,
	e_async_priority priority,
	c_synchronized_long* done)
{
	//return INVOKE(0x005AD8B0, async_queue_simple_callback,
	//	callback,
	//	data,
	//	data_size,
	//	priority,
	//	done);

	s_async_simple_callback_task task{};
	ASSERT(sizeof(task.callback_data) >= data_size);

	if (data_size <= sizeof(task.callback_data))
	{
		task.callback = callback;
		csmemcpy(task.callback_data, data, data_size);
		return async_task_add(priority, &task.dummy_for_size, _async_category_none, async_simple_callback_task_callback, done);
	}

	return NONE;
}

//.text:005AD9E0 ; int32 __cdecl async_read_entire_file(const wchar_t*, void*, uns32, uns32 volatile*, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)

int32 __cdecl async_read_position(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* bytes_read,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ADA80, async_read_position,
	//	file,
	//	buffer,
	//	size,
	//	offset,
	//	category,
	//	priority,
	//	bytes_read,
	//	done);

	s_async_task task{};
	return async_read_position_ex(
		file,
		buffer,
		size,
		offset,
		category,
		priority,
		bytes_read,
		done,
		true);
}

e_async_completion __cdecl async_read_position_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005ADAF0, async_read_position_callback, in_task);
}

int32 __cdecl async_read_position_ex(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* bytes_read,
	c_synchronized_long* done,
	bool block_if_task_list_is_full)
{
	//return INVOKE(0x005ADBA0, async_read_position_ex,
	//	file,
	//	buffer,
	//	size,
	//	offset,
	//	category,
	//	priority,
	//	bytes_read,
	//	done,
	//	block_if_task_list_is_full);

	s_read_position_task task{};

	task.file = file.handle;
	task.buffer = buffer;
	task.size = size;
	task.offset = offset;
	task.bytes_read = bytes_read;

	return async_task_add_ex(priority, &task.dummy_for_size, category, async_read_position_callback, done, block_if_task_list_is_full);
}

int32 __cdecl async_read_position_overlapped(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* bytes_read,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ADC10, async_read_position_overlapped,
	//	file,
	//	buffer,
	//	size,
	//	offset,
	//	category,
	//	priority,
	//	bytes_read,
	//	done);

	return async_read_position_overlapped_ex(
		file,
		buffer,
		size,
		offset,
		category,
		priority,
		bytes_read,
		done,
		true);
}

e_async_completion __cdecl async_read_position_overlapped_callback(s_async_task* in_task)
{
	return INVOKE(0x005ADC40, async_read_position_overlapped_callback, in_task);
}

int32 __cdecl async_read_position_overlapped_ex(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* bytes_read,
	c_synchronized_long* done,
	bool block_if_task_list_is_full)
{
	return INVOKE(0x005ADD10, async_read_position_overlapped_ex,
		file,
		buffer,
		size,
		offset,
		category,
		priority,
		bytes_read,
		done,
		block_if_task_list_is_full);
}

int32 __cdecl async_set_file_size(
	s_file_handle file,
	int32 size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* success,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ADDE0, async_set_file_size,
	//	file,
	//	size,
	//	category,
	//	priority,
	//	success,
	//	done);

	if (success)
		*success = false;

	s_set_file_size_task task{};
	task.file = file.handle;
	task.size = size;
	task.success = success;
	return async_task_add(priority, &task.dummy_for_size, category, set_file_size_callback, done);
}

e_async_completion __cdecl async_simple_callback_task_callback(
	s_async_task* in_task)
{
	//return INVOKE(0x005ADE50, async_simple_callback_task_callback,
	//	in_task);

	s_async_simple_callback_task* task = (s_async_simple_callback_task*)in_task;
	return task->callback(in_task, task->callback_data, task->callback_data_size);
}

int32 __cdecl async_validify_file(
	s_file_handle file,
	void* header,
	int32 header_size,
	uns32 file_size,
	e_async_category category,
	e_async_priority priority,
	bool dst_on_utility_drive,
	bool* success,
	c_synchronized_long* out_done)
{
	return INVOKE(0x005ADE70, async_validify_file,
		file,
		header,
		header_size,
		file_size,
		category,
		priority,
		dst_on_utility_drive,
		success,
		out_done);
}

e_async_completion __cdecl async_validify_file_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005ADED0, async_validify_file_callback,
		in_task);
}

//.text:005ADFC0 ; int32 __cdecl async_write_buffer_to_file(const wchar_t*, const void*, uns32, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)

int32 __cdecl async_write_position(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	bool dst_on_utility_drive,
	c_synchronized_long* bytes_written,
	c_synchronized_long* done)
{
	//return INVOKE(0x005AE050, async_write_position,
	//	file,
	//	buffer,
	//	size,
	//	offset,
	//	category,
	//	priority,
	//	dst_on_utility_drive,
	//	bytes_written,
	//	done);

	return async_write_position_ex(file, buffer, size, offset, 0, category, priority, dst_on_utility_drive, bytes_written, done);
}

e_async_completion __cdecl async_write_position_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005AE080, async_write_position_callback,
		in_task);
}

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
	c_synchronized_long* done)
{
	return INVOKE(0x005AE140, async_write_position_ex,
		file,
		buffer,
		size,
		offset,
		flags,
		category,
		priority,
		dst_on_utility_drive,
		bytes_written,
		done);
}

e_async_completion __cdecl close_file_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005AE2F0, close_file_callback,
		in_task);
}

e_async_completion __cdecl close_file_no_stfs_flush_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005AE320, close_file_no_stfs_flush_callback,
		in_task);
}

e_async_completion __cdecl delete_file_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005AE490, delete_file_callback,
		in_task);
}

e_async_completion __cdecl enumerate_files_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005AE4F0, enumerate_files_callback,
		in_task);
}

e_async_completion __cdecl get_file_size_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005AE6F0, get_file_size_callback,
		in_task);
}

e_async_completion __cdecl read_entire_file_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005AED10, read_entire_file_callback,
		in_task);
}

//.text:005AEEC0 ; 

e_async_completion __cdecl set_file_size_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005AEEF0, set_file_size_callback,
		in_task);
}

//.text:005AF220 ; 

e_async_completion __cdecl write_buffer_to_file_callback(
	s_async_task* in_task)
{
	return INVOKE(0x005AF410, write_buffer_to_file_callback,
		in_task);
}

