#include "cseries/async_helpers.hpp"

int32 __cdecl async_close_file(
	s_file_handle file_handle,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ACA70, async_close_file,
	//	file_handle,
	//	category,
	//	priority,
	//	done);

	s_async_task task{};
	task.close_file_task.file_handle = file_handle;
	return async_task_add(priority, &task, category, async_close_file_callback, done);
}

int32 __cdecl async_close_file_no_stfs_flush(
	s_file_handle file_handle,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ACAC0, async_close_file_no_stfs_flush,
	//	file_handle,
	//	category,
	//	priority,
	//	done);

	s_async_task task{};

	task.close_file_task.file_handle = file_handle;
	return async_task_add(priority, &task, category, async_close_file_no_stfs_flush_callback, done);
}

//.text:005ACB10 ; bool __cdecl async_compare_tasks(s_compare_tasks_data*, e_async_completion(__cdecl*)(s_async_task*), e_async_completion(__cdecl*)(s_async_task*), e_async_priority, e_async_priority, const s_async_task*, const s_async_task*)
//.text:005ACBA0 ; void __cdecl async_compare_tasks_start(s_compare_tasks_data*)

e_async_completion __cdecl async_copy_file_callback(s_async_task* task)
{
	return INVOKE(0x005ACBC0, async_copy_file_callback, task);
}

int32 __cdecl async_copy_file_from_existing_file(
	const wchar_t* source_file,
	const wchar_t* destination_file,
	void* copy_buffer,
	uns32 copy_buffer_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* success,
	c_synchronized_long* done)
{
	return INVOKE(0x005ACCE0, async_copy_file_from_existing_file,
		source_file,
		destination_file,
		copy_buffer,
		copy_buffer_size,
		category,
		priority,
		success,
		done);
}

int32 __cdecl async_copy_position(
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
	c_synchronized_long* done)
{
	return INVOKE(0x005ACD70, async_copy_position,
		source_handle,
		destination_handle,
		buffer,
		buffer_size,
		source_offset,
		destination_offset,
		category,
		priority,
		a9,
		size,
		done);
}

e_async_completion __cdecl async_copy_position_callback(s_async_task* task)
{
	return INVOKE(0x005ACE10, async_copy_position_callback, task);
}

//int32 __cdecl async_create_file(
//	const wchar_t* file_name,
//	uns32 flags1,
//	e_file_open_disposition file_open_disposition,
//	uns32 flags2,
//	e_async_category category,
//	e_async_priority priority,
//	s_file_handle volatile* file_handle,
//	c_synchronized_long* done)
int32 __cdecl async_create_file(
	const wchar_t* file_name,
	uns32 flags1,
	int32 file_open_disposition,
	uns32 flags2,
	e_async_category category,
	e_async_priority priority,
	s_file_handle volatile* file_handle,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ACF50, async_create_file,
	//	file_name,
	//	flags1,
	//	file_open_disposition,
	//	flags2,
	//	category,
	//	priority,
	//	file_handle,
	//	done);

	s_async_task task{};

	async_create_file_task_initialize(
		file_name,
		flags1,
		file_open_disposition,
		flags2,
		category,
		priority,
		file_handle,
		&task.create_file_task);

	return async_task_add(priority, &task, category, async_create_file_callback, done);
}

e_async_completion __cdecl async_create_file_callback(s_async_task* task)
{
	return INVOKE(0x005ACFA0, async_create_file_callback, task);
}

//.text:005AD130 ; void __cdecl async_create_file_from_reference_blocking(s_file_reference*, uns32, e_file_open_disposition, uns32, e_async_category, s_file_handle volatile*)
//.text:005AD1C0 ; void __cdecl async_create_file_synchronous(const wchar_t*, uns32, e_file_open_disposition, uns32, int32, int32, s_file_handle*)

//void __cdecl async_create_file_task_initialize(
//	const wchar_t* file_name,
//	uns32 flags1,
//	e_file_open_disposition file_open_disposition,
//	uns32 flags2,
//	int32 category,
//	int32 priority,
//	s_file_handle volatile* file_handle,
//	s_create_file_task* task)
void __cdecl async_create_file_task_initialize(
	const wchar_t* file_name,
	uns32 flags1,
	int32 file_open_disposition,
	uns32 flags2,
	int32 category,
	int32 priority,
	s_file_handle volatile* file_handle,
	s_create_file_task* create_file_task)
{
	INVOKE(0x005AD230, async_create_file_task_initialize,
		file_name,
		flags1,
		file_open_disposition,
		flags2,
		category,
		priority,
		file_handle,
		create_file_task);
}

//.text:005AD370 ; int32 __cdecl async_delete_file(const wchar_t*, bool, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)
//.text:005AD3F0 ; int32 __cdecl async_enumerate_files(uns32, const char*, int32, s_file_reference*, int32*, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)

int32 __cdecl async_flush_file(
	s_file_handle file_handle,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done)
{
	//return INVOKE(0x005AD4A0, async_flush_file, file_handle, category, priority, done);

	s_async_task task{};
	task.file_raw_handle_based_task.file_handle = file_handle;

	return async_task_add(priority, &task, category, async_flush_file_callback, done);
}

void __cdecl async_flush_file_blocking(s_file_handle file_handle, e_async_category category)
{
	INVOKE(0x005AD4F0, async_flush_file_blocking, file_handle, category);
}

e_async_completion __cdecl async_flush_file_callback(s_async_task* task)
{
	return INVOKE(0x005AD560, async_flush_file_callback, task);
}

int32 __cdecl async_get_file_size(
	s_file_handle file_handle,
	uns32 volatile* file_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done)
{
	//return INVOKE(0x005AD590, async_get_file_size,
	//	file_handle,
	//	file_size,
	//	category,
	//	priority,
	//	done);

	s_async_task task{};
	task.get_file_size_task.file_handle = file_handle;
	task.get_file_size_task.file_size = file_size;

	return async_task_add(priority, &task, category, async_get_file_size_callback, done);
}

//.text:005AD5F0 ; void __cdecl async_helpers_dispose()
//.text:005AD600 ; void __cdecl async_helpers_initialize()
//.text:005AD610 ; int32 __cdecl async_nuke_file(s_file_handle, uns32, const void*, e_async_category, e_async_priority, bool*, c_synchronized_long*)

e_async_completion __cdecl async_nuke_file_callback(s_async_task* task)
{
	return INVOKE(0x005AD660, async_nuke_file_callback, task);
}

//.text:005AD6E0 ; int32 __cdecl async_perform_operation_between_files(const wchar_t*, const wchar_t*, bool, bool, bool, bool, uns32, uns32, e_async_category, e_async_priority, s_async_task*, e_async_completion(__cdecl*)(s_async_task*), c_synchronized_long*, s_file_handle*, s_file_handle*)

int32 __cdecl async_queue_simple_callback(e_async_completion(__cdecl* callback)(s_async_task* task, void* data, int32 data_size), const void* data, int32 data_size, e_async_priority priority, c_synchronized_long* done)
{
	//return INVOKE(0x005AD8B0, async_queue_simple_callback, callback, data, data_size, priority, done);

	s_async_task task{};
	ASSERT(sizeof(task.simple_callback_task.callback_data) >= data_size);

	if (data_size <= sizeof(task.simple_callback_task.callback_data))
	{
		task.simple_callback_task.callback = callback;
		csmemcpy(task.simple_callback_task.callback_data, data, data_size);
		return async_task_add(priority, &task, _async_category_none, async_simple_callback_task_callback, done);
	}

	return NONE;
}

//.text:005AD9E0 ; int32 __cdecl async_read_entire_file(const wchar_t*, void*, uns32, uns32 volatile*, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)

int32 __cdecl async_read_position(
	s_file_handle file_handle,
	void* buffer,
	int32 buffer_size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* size,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ADA80, async_read_position,
	//	file_handle,
	//	buffer,
	//	buffer_size,
	//	offset,
	//	category,
	//	priority,
	//	size,
	//	done);

	s_async_task task{};
	return async_read_position_ex(
		file_handle,
		buffer,
		buffer_size,
		offset,
		category,
		priority,
		size,
		done,
		true);
}

e_async_completion __cdecl async_read_position_callback(s_async_task* task)
{
	return INVOKE(0x005ADAF0, async_read_position_callback, task);
}

int32 __cdecl async_read_position_ex(
	s_file_handle file_handle,
	void* buffer,
	int32 buffer_size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* size,
	c_synchronized_long* done,
	bool a9)
{
	//return INVOKE(0x005ADBA0, async_read_position_ex,
	//	file_handle,
	//	buffer,
	//	buffer_size,
	//	offset,
	//	category,
	//	priority,
	//	size,
	//	done,
	//	a9);

	s_async_task task{};

	task.read_position_task.file_handle = file_handle;
	task.read_position_task.buffer = buffer;
	task.read_position_task.buffer_size = buffer_size;
	task.read_position_task.file_offset = offset;
	task.read_position_task.size = size;

	return async_task_add_ex(priority, &task, category, async_read_position_callback, done, a9);
}

int32 __cdecl async_read_position_overlapped(
	s_file_handle file_handle,
	void* buffer,
	int32 buffer_size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* size,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ADC10, async_read_position_overlapped,
	//	file_handle,
	//	buffer,
	//	buffer_size,
	//	offset,
	//	category,
	//	priority,
	//	size,
	//	done);

	return async_read_position_overlapped_ex(
		file_handle,
		buffer,
		buffer_size,
		offset,
		category,
		priority,
		size,
		done,
		true);
}

e_async_completion __cdecl async_read_position_overlapped_callback(s_async_task* task)
{
	return INVOKE(0x005ADC40, async_read_position_overlapped_callback, task);
}

int32 __cdecl async_read_position_overlapped_ex(
	s_file_handle file_handle,
	void* buffer,
	int32 buffer_size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* size,
	c_synchronized_long* done,
	bool a9)
{
	return INVOKE(0x005ADD10, async_read_position_overlapped_ex,
		file_handle,
		buffer,
		buffer_size,
		offset,
		category,
		priority,
		size,
		done,
		a9);
}

int32 __cdecl async_set_file_size(
	s_file_handle file_handle,
	int32 file_size,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* success,
	c_synchronized_long* done)
{
	//return INVOKE(0x005ADDE0, async_set_file_size,
	//	file_handle,
	//	file_size,
	//	category,
	//	priority,
	//	success,
	//	done);

	if (success)
		*success = false;

	s_async_task task{};
	task.set_file_size_task.file_handle = file_handle;
	task.set_file_size_task.file_size = file_size;
	task.set_file_size_task.success = success;
	return async_task_add(priority, &task, category, async_set_file_size_callback, done);
}

e_async_completion __cdecl async_simple_callback_task_callback(s_async_task* task)
{
	//return INVOKE(0x005ADE50, async_simple_callback_task_callback, task);

	return task->simple_callback_task.callback(task, task->simple_callback_task.callback_data, task->simple_callback_task.callback_data_size);
}

int32 __cdecl async_validify_file(
	s_file_handle file_handle,
	void* buffer,
	int32 buffer_size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	bool a7,
	bool* success,
	c_synchronized_long* done)
{
	return INVOKE(0x005ADE70, async_validify_file,
		file_handle,
		buffer,
		buffer_size,
		offset,
		category,
		priority,
		a7,
		success,
		done);
}

e_async_completion __cdecl async_validify_file_callback(s_async_task* task)
{
	return INVOKE(0x005ADED0, async_validify_file_callback, task);
}

//.text:005ADFC0 ; int32 __cdecl async_write_buffer_to_file(const wchar_t*, const void*, uns32, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)

int32 __cdecl async_write_position(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	e_async_category category,
	e_async_priority priority,
	bool a7,
	c_synchronized_long* success) // success or size?
{
	//return INVOKE(0x005AE050, async_write_position,
	//	file,
	//	buffer,
	//	size,
	//	offset,
	//	category,
	//	priority,
	//	a7,
	//	success);

	return async_write_position_ex(file, buffer, size, offset, 0, category, priority, a7, success);
}

e_async_completion __cdecl async_write_position_callback(s_async_task* task)
{
	return INVOKE(0x005AE080, async_write_position_callback, task);
}

int32 __cdecl async_write_position_ex(
	s_file_handle file,
	void* buffer,
	int32 size,
	uns32 offset,
	c_flags<e_write_position_flags, uns32, k_write_position_flags> flags,
	e_async_category category,
	e_async_priority priority,
	bool a8,
	c_synchronized_long* success) // success or size?
{
	return INVOKE(0x005AE140, async_write_position_ex,
		file,
		buffer,
		size,
		offset,
		flags,
		category,
		priority,
		a8,
		success);
}

e_async_completion __cdecl async_close_file_callback(s_async_task* task)
{
	return INVOKE(0x005AE2F0, async_close_file_callback, task);
}

e_async_completion __cdecl async_close_file_no_stfs_flush_callback(s_async_task* task)
{
	return INVOKE(0x005AE320, async_close_file_no_stfs_flush_callback, task);
}

e_async_completion __cdecl async_delete_file_callback(s_async_task* task)
{
	return INVOKE(0x005AE490, async_delete_file_callback, task);
}

e_async_completion __cdecl async_enumerate_files_callback(s_async_task* task)
{
	return INVOKE(0x005AE4F0, async_enumerate_files_callback, task);
}

e_async_completion __cdecl async_get_file_size_callback(s_async_task* task)
{
	return INVOKE(0x005AE6F0, async_get_file_size_callback, task);
}

e_async_completion __cdecl async_read_entire_file_callback(s_async_task* task)
{
	return INVOKE(0x005AED10, async_read_entire_file_callback, task);
}

//.text:005AEEC0 ; 

e_async_completion __cdecl async_set_file_size_callback(s_async_task* task)
{
	return INVOKE(0x005AEEF0, async_set_file_size_callback, task);
}

//.text:005AF220 ; 

e_async_completion __cdecl async_write_buffer_to_file_callback(s_async_task* task)
{
	return INVOKE(0x005AF410, async_write_buffer_to_file_callback, task);
}

