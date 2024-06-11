#include "cseries/async_helpers.hpp"

//.text:005ACA70 ; long __cdecl async_close_file(s_file_handle, e_async_category, e_async_priority, c_synchronized_long*)
//.text:005ACAC0 ; long __cdecl async_close_file_no_stfs_flush(s_file_handle, e_async_category, e_async_priority, c_synchronized_long*)
//.text:005ACB10 ; bool __cdecl async_compare_tasks(s_compare_tasks_data*, e_async_completion(__cdecl*)(s_async_task*), e_async_completion(__cdecl*)(s_async_task*), e_async_priority, e_async_priority, s_async_task const*, s_async_task const*)
//.text:005ACBA0 ; void __cdecl async_compare_tasks_start(s_compare_tasks_data*)
//.text:005ACBC0 ; e_async_completion __cdecl async_copy_file_callback(s_async_task*)
//.text:005ACCE0 ; long __cdecl async_copy_file_from_existing_file(wchar_t const*, wchar_t const*, void*, dword, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)
//.text:005ACD70 ; long __cdecl async_copy_position(s_file_handle, s_file_handle, void*, long, long, long, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)
//.text:005ACE10 ; e_async_completion __cdecl async_copy_position_callback(s_async_task*)

//long __cdecl async_create_file(wchar_t const* file_name, dword flags1, e_file_open_disposition file_open_disposition, dword flags2, e_async_category category, e_async_priority priority, s_file_handle volatile* file_handle, c_synchronized_long* done)
long __cdecl async_create_file(
	wchar_t const* file_name,
	dword flags1,
	long file_open_disposition,
	dword flags2,
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

//.text:005AD130 ; void __cdecl async_create_file_from_reference_blocking(s_file_reference*, dword, e_file_open_disposition, dword, e_async_category, s_file_handle volatile*)
//.text:005AD1C0 ; void __cdecl async_create_file_synchronous(wchar_t const*, dword, e_file_open_disposition, dword, long, long, s_file_handle*)

//void __cdecl async_create_file_task_initialize(wchar_t const* file_name, dword flags1, e_file_open_disposition file_open_disposition, dword flags2, long category, long priority, s_file_handle volatile* file_handle, s_create_file_task* task)
void __cdecl async_create_file_task_initialize(
	wchar_t const* file_name,
	dword flags1,
	long file_open_disposition,
	dword flags2,
	long category,
	long priority,
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

//.text:005AD370 ; long __cdecl async_delete_file(wchar_t const*, bool, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)
//.text:005AD3F0 ; long __cdecl async_enumerate_files(dword, char const*, long, s_file_reference*, long*, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)

long __cdecl async_flush_file(
	s_file_handle file_handle,
	e_async_category category,
	e_async_priority priority,
	c_synchronized_long* done)
{
	//return INVOKE(0x005AD4A0, async_flush_file, file_handle, category, priority, done);

	s_async_task task{};
	task.file_raw_handle_based_task.file = file_handle;

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

long __cdecl async_get_file_size(
	s_file_handle file_handle,
	dword volatile* file_size,
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
	task.get_file_size_task.file = file_handle;
	task.get_file_size_task.file_size = file_size;

	return async_task_add(priority, &task, category, async_get_file_size_callback, done);
}

//.text:005AD5F0 ; 
//.text:005AD600 ; 
//.text:005AD610 ; 
//.text:005AD610 ; long __cdecl async_nuke_file(s_file_handle, dword, void const*, e_async_category, e_async_priority, bool*, c_synchronized_long*)
//.text:005AD660 ; e_async_completion __cdecl async_nuke_file_callback(s_async_task*)
//.text:005AD6E0 ; long __cdecl async_perform_operation_between_files(wchar_t const*, wchar_t const*, bool, bool, bool, bool, dword, dword, e_async_category, e_async_priority, s_async_task*, e_async_completion(__cdecl*)(s_async_task*), c_synchronized_long*, s_file_handle*, s_file_handle*)
//.text:005AD8B0 ; long __cdecl async_queue_simple_callback(e_async_completion(__cdecl*)(s_async_task*, void*, long), void const*, long, e_async_priority, c_synchronized_long*)
//.text:005AD9E0 ; long __cdecl async_read_entire_file(wchar_t const*, void*, dword, dword volatile*, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)

long __cdecl async_read_position(
	s_file_handle file_handle,
	void* buffer,
	long buffer_size,
	dword offset,
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

long __cdecl async_read_position_ex(
	s_file_handle file_handle,
	void* buffer,
	long buffer_size,
	dword offset,
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

	task.read_position_task.file = file_handle;
	task.read_position_task.buffer = buffer;
	task.read_position_task.buffer_size = buffer_size;
	task.read_position_task.file_offset = offset;
	task.read_position_task.size = size;

	return async_task_add_ex(priority, &task, category, async_read_position_callback, done, a9);
}

//.text:005ADC10 ; long __cdecl async_read_position_overlapped(s_file_handle, void*, long, dword, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)
//.text:005ADC40 ; e_async_completion __cdecl async_read_position_overlapped_callback(s_async_task*)
//.text:005ADD10 ; long __cdecl async_read_position_overlapped_ex(s_file_handle, void*, long, dword, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*, bool)
//.text:005ADDE0 ; long __cdecl async_set_file_size(s_file_handle, long, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)
//.text:005ADE50 ; e_async_completion __cdecl async_simple_callback_task_callback(s_async_task*)
//.text:005ADE70 ; long __cdecl async_validify_file(s_file_handle, void*, long, dword, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*, bool, bool*, c_synchronized_long*)
//.text:005ADED0 ; e_async_completion __cdecl async_validify_file_callback(s_async_task*)
//.text:005ADFC0 ; long __cdecl async_write_buffer_to_file(wchar_t const*, void const*, dword, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)
//.text:005AE050 ; long __cdecl async_write_position(s_file_handle, void*, long, dword, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)
//.text:005AE080 ; e_async_completion __cdecl async_write_position_callback(s_async_task*)
//.text:005AE140 ; long __cdecl async_write_position_ex(s_file_handle, void*, long, dword, c_flags<e_write_position_flags, dword, 1>, e_async_category, e_async_priority, c_synchronized_long*, c_synchronized_long*)
//.text:005AE2F0 ; e_async_completion __cdecl async_close_file_callback(s_async_task*)
//.text:005AE320 ; e_async_completion __cdecl async_close_file_no_stfs_flush_callback(s_async_task*)
//.text:005AE490 ; e_async_completion __cdecl async_delete_file_callback(s_async_task*)
//.text:005AE4F0 ; e_async_completion __cdecl async_enumerate_files_callback(s_async_task*)

e_async_completion __cdecl async_get_file_size_callback(s_async_task* task)
{
	return INVOKE(0x005AE6F0, async_get_file_size_callback, task);
}

//.text:005AED10 ; e_async_completion __cdecl async_read_entire_file_callback(s_async_task*)
//.text:005AEEC0 ; 
//.text:005AEEF0 ; e_async_completion __cdecl async_set_file_size_callback(s_async_task*)
//.text:005AF220 ; 
//.text:005AF410 ; e_async_completion __cdecl async_write_buffer_to_file_callback(s_async_task*)

