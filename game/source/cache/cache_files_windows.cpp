#include "cache/cache_files_windows.hpp"

#include "cache/cache_file_codec_work.hpp"
#include "config/version.hpp"
#include "cseries/async.hpp"
#include "cseries/async_helpers.hpp"
#include "cseries/cseries_events.hpp"
#include "main/global_preferences.hpp"
#include "main/levels.hpp"
#include "memory/module.hpp"
#include "shell/shell.hpp"

REFERENCE_DECLARE_ARRAY(0x0165B348, e_async_priority, k_action_to_async_priority, 7);
REFERENCE_DECLARE_ARRAY(0x0165B364, int32, k_action_to_copy_buffer_size, 7);

REFERENCE_DECLARE(0x018A1A50, c_cache_file_copy_optional_cache_callback, g_cache_file_copy_optional_cache_callback);
REFERENCE_DECLARE(0x018A1A54, c_cache_file_copy_fake_decompressor, g_copy_decompressor);
REFERENCE_DECLARE(0x0240B1E0, bool, g_cache_files_read_from_map_directory);
REFERENCE_DECLARE(0x0240B1E8, s_cache_file_table_of_contents, cache_file_table_of_contents);
REFERENCE_DECLARE(0x0243C098, s_cache_file_copy_globals, cache_file_copy_globals);
REFERENCE_DECLARE(0x0243F780, c_asynchronous_io_arena, g_cache_file_io_arena);

HOOK_DECLARE(0x005AA7C0, cache_file_open);
HOOK_DECLARE(0x005AA870, cache_file_read_ex);
HOOK_DECLARE(0x005AAE70, cache_files_copy_map_start_only);
HOOK_DECLARE(0x005AB4F0, cache_files_read_from_map_directory);
HOOK_DECLARE(0x005AB540, cache_files_running_off_dvd);
HOOK_DECLARE(0x005AB630, cached_map_file_close);
HOOK_DECLARE(0x005ABFF0, canonicalize_map_path);

bool __cdecl cached_map_file_is_shared(e_map_file_index map_file_index)
{
	return INVOKE(0x00501E80, cached_map_file_is_shared, map_file_index);

	//bool render = !sub_42E5D0() || map_file_index != 1 && map_file_index != 2 && map_file_index != 4;
	//bool sound = !sub_42E5E0() || map_file_index != 3;
	//return render && sound;
}

void __cdecl cache_file_block_until_not_busy()
{
	INVOKE(0x005A96C0, cache_file_block_until_not_busy);

	//e_async_category categories[]
	//{
	//	_async_category_cache_file_misc,
	//	_async_category_cache_file_texture,
	//	_async_category_cache_file_geometry,
	//	_async_category_cache_file_sound,
	//	_async_category_cache_file_animation
	//};
	//
	//for (int32 category_index = 0; category_index < NUMBEROF(categories); category_index++)
	//{
	//	e_async_category category = categories[category_index];
	//	while (async_category_in_queue(category))
	//	{
	//		switch_to_thread();
	//	}
	//}
}

void __cdecl cache_file_close()
{
	//INVOKE(0x005A9730, cache_file_close);

	if (cache_file_table_of_contents.open_map_file_index != k_no_cached_map_file_index)
	{
		cache_requests_flush();
		cache_file_table_of_contents.open_map_file_index = k_no_cached_map_file_index;
	}
}

void __cdecl cache_file_copy_allocate_buffer(int32 size)
{
	INVOKE(0x005A9750, cache_file_copy_allocate_buffer, size);

	//if (cache_file_copy_globals.buffer.size() != size)
	//{
	//	cache_file_copy_release_buffer();
	//
	//	void* buffer = _optional_cache_try_to_allocate((e_optional_cache_user)0, (e_optional_cache_user_priority)0, size, &g_cache_file_copy_optional_cache_callback);
	//	cache_file_copy_globals.buffer.set_buffer(buffer, buffer ? cache_file_copy_globals.buffer.size() : size);
	//}
}

bool __cdecl cache_file_copy_do_action()
{
	return INVOKE(0x005A97C0, cache_file_copy_do_action);

	/*
	e_cache_copy_state copy_state = cache_file_copy_globals.copy_state;
	e_async_priority priority = k_action_to_async_priority[cache_file_copy_globals.current_request.action];
	
	bool valid = true;
	switch (copy_state)
	{
	case _cache_copy_state_idle:
	{
		if (!cache_file_copy_globals.pending_request.map_name.is_empty() &&
			!cache_files_has_map_terminal_failure(cache_file_copy_globals.pending_request.map_name.get_string()) &&
			cache_files_get_file_status(cache_file_copy_globals.pending_request.map_name.get_string()) != 3)
		{
			c_static_string<k_tag_long_string_length> copying_map_file_path{};
			canonicalize_map_path(cache_file_copy_globals.pending_request.map_name.get_string(), &copying_map_file_path);
			copying_map_file_path.copy_to(cache_file_copy_globals.copying_map_file_path, k_tag_long_string_length);

			cache_file_copy_globals.current_request = cache_file_copy_globals.pending_request;
			copy_state = e_cache_copy_state(_cache_copy_state_create_source_file - levels_path_is_dlc(cache_file_copy_globals.copying_map_file_path));
		}
	
		csmemset(&cache_file_copy_globals.pending_request, 0, sizeof(s_cache_copy_request));
		cache_file_copy_globals.finish_reason = _cache_copy_finish_reason_invalid;
	}
	break;
	case _cache_copy_state_mount_dlc:
	{
		levels_open_dlc(cache_file_copy_globals.copying_map_file_path, true);
		copy_state = _cache_copy_state_create_source_file;
	}
	break;
	case _cache_copy_state_create_source_file:
	{
		wchar_t source_file[256]{};
		ascii_string_to_wchar_string(cache_file_copy_globals.copying_map_file_path, source_file, NUMBEROF(source_file), nullptr);
	
		cache_file_copy_globals.copy_task_id = async_create_file(
			source_file,
			FLAG(0),
			0,
			FLAG(3),
			_async_category_background_copy,
			priority,
			&cache_file_copy_globals.source_file_handle,
			&cache_file_copy_globals.copy_task_is_done);
		copy_state = _cache_copy_state_verify_create_source_file;
	}
	break;
	case _cache_copy_state_verify_create_source_file:
	{
		valid = file_handle_is_valid(cache_file_copy_globals.source_file_handle);
		copy_state = _cache_copy_state_read_dvd_header;
	}
	break;
	case _cache_copy_state_read_dvd_header:
	{
		csmemset(&cache_file_copy_globals.copy_file_header, 0, sizeof(s_cache_file_header));
		cache_file_copy_globals.copy_task_id = async_read_position_overlapped_ex(
			cache_file_copy_globals.source_file_handle,
			&cache_file_copy_globals,
			sizeof(s_cache_file_header),
			0,
			_async_category_background_copy,
			priority,
			&cache_file_copy_globals.copy_bytes_transferred,
			&cache_file_copy_globals.copy_task_is_done,
			false);
		copy_state = e_cache_copy_state((cache_file_copy_globals.copy_task_id != INVALID_ASYNC_TASK_ID) + _cache_copy_state_read_dvd_header);
	}
	break;
	case _cache_copy_state_verify_read_dvd_header:
	{
		get_current_file_time(&cache_file_copy_globals.copy_file_header.slot_modification_date);
		cache_file_copy_globals.copy_file_header.path = cache_file_copy_globals.copying_map_file_path;
		if (cache_file_copy_globals.copy_bytes_transferred.peek() == sizeof(s_cache_file_header) &&
			cache_file_header_verify(&cache_file_copy_globals.copy_file_header, cache_file_copy_globals.copying_map_file_path, false))
		{
			uns32 shared_files_flags = 0;
			valid = cached_map_file_dependencies_loaded(&cache_file_copy_globals.copy_file_header, &shared_files_flags);
		}
		else
		{
			valid = false;
		}
	
		copy_state = _cache_copy_state_get_dvd_file_size;
	}
	break;
	case _cache_copy_state_get_dvd_file_size:
	{
		cache_file_copy_globals.copy_task_id = async_get_file_size(
			cache_file_copy_globals.source_file_handle,
			&cache_file_copy_globals.source_file_size,
			_async_category_background_copy,
			priority,
			&cache_file_copy_globals.copy_task_is_done);
	
		copy_state = _cache_copy_state_verify_get_dvd_file_size;
	}
	break;
	case _cache_copy_state_verify_get_dvd_file_size:
	{
		if (cache_file_copy_globals.source_file_size != NONE && cache_file_copy_globals.source_file_size >= sizeof(s_cache_file_header))
		{
			valid = true;
		}
		else
		{
			valid = false;
		}
	
		cache_file_copy_globals.running_realtime_checksum = 0;
		copy_state = _cache_copy_state_find_free_map_and_clear_header;
	}
	break;
	case _cache_copy_state_find_free_map_and_clear_header:
	{
		e_map_file_index map_file_index = cached_map_files_find_free_utility_drive_map(cache_file_copy_globals.copy_file_header.file_size, 5);
		if (map_file_index != k_no_cached_map_file_index)
		{
			cache_file_copy_globals.copying_to_map_file_index = map_file_index;
			cache_file_copy_globals.nuke_cache_file_success = false;
			cache_map_file_nuke(
				map_file_index,
				_async_category_background_copy,
				priority,
				&cache_file_copy_globals.nuke_cache_file_success,
				&cache_file_copy_globals.copy_task_is_done);
			copy_state = _cache_copy_state_verify_find_free_map_and_clear_header;
		}
	}
	break;
	case _cache_copy_state_verify_find_free_map_and_clear_header:
	{
		valid = cache_file_copy_globals.copy_bytes_transferred.peek() == sizeof(s_cache_file_header);
		copy_state = _cache_copy_state_flush_clear_header;
	}
	break;
	case _cache_copy_state_flush_clear_header:
	{
		cache_file_copy_globals.copy_task_id = async_flush_file(
			cached_map_file_get_handle(cache_file_copy_globals.copying_to_map_file_index),
			_async_category_background_copy,
			priority,
			&cache_file_copy_globals.copy_task_is_done);
	
		copy_state = _cache_copy_state_prepare_copy_thread;
	}
	break;
	case _cache_copy_state_start_copying_map_data:
	{
		cache_file_copy_globals.copy_bytes_transferred = 0L;
		cache_file_copy_globals.total_copy_bytes_transferred = sizeof(s_cache_file_header);
		cache_file_copy_globals.copy_start_time_ms = system_milliseconds();
		copy_state = _cache_copy_state_copy_map_data;
	}
	break;
	case _cache_copy_state_copy_map_data:
	{
		cache_file_copy_allocate_buffer(k_action_to_copy_buffer_size[cache_file_copy_globals.current_request.action]);
		if (cache_file_copy_globals.buffer)
		{
			uns32 buffer_size = cache_file_copy_globals.buffer_size;
			if (buffer_size > cache_file_copy_globals.source_file_size - cache_file_copy_globals.total_copy_bytes_transferred)
				buffer_size = cache_file_copy_globals.source_file_size - cache_file_copy_globals.total_copy_bytes_transferred;
	
			cache_file_copy_globals.intended_copy_bytes_transferred = buffer_size;
	
			cache_file_copy_globals.copy_task_id = async_copy_position(
				cache_file_copy_globals.source_file_handle,
				cached_map_file_get_handle(cache_file_copy_globals.copying_to_map_file_index),
				cache_file_copy_globals.buffer,
				buffer_size,
				cache_file_copy_globals.total_copy_bytes_transferred,
				cache_file_copy_globals.total_copy_bytes_transferred,
				_async_category_background_copy,
				priority,
				true,
				&cache_file_copy_globals.copy_bytes_transferred,
				&cache_file_copy_globals.copy_task_is_done);
	
			copy_state = _cache_copy_state_verify_copy_map_data;
		}
		else
		{
			copy_state = _cache_copy_state_copy_map_data;
		}
	}
	break;
	case _cache_copy_state_verify_copy_map_data:
	{
		valid = cache_file_copy_globals.intended_copy_bytes_transferred == cache_file_copy_globals.copy_bytes_transferred.peek();
		cache_file_copy_globals.running_realtime_checksum ^= compute_realtime_checksum((char*)cache_file_copy_globals.buffer, cache_file_copy_globals.copy_bytes_transferred.peek());
		if (valid)
		{
			cache_file_copy_globals.total_copy_bytes_transferred += cache_file_copy_globals.copy_bytes_transferred.peek();
			if (cache_file_copy_globals.total_copy_bytes_transferred < cache_file_copy_globals.source_file_size)
			{
				copy_state = _cache_copy_state_copy_map_data;
			}
			else
			{
				valid = true;
				copy_state = _cache_copy_state_flush_after_copy;
			}
		}
	}
	break;
	case _cache_copy_state_prepare_copy_thread:
	{
		cache_file_copy_globals.copy_bytes_transferred = 0L;
		cache_file_copy_globals.total_copy_bytes_transferred = sizeof(s_cache_file_header);
		cache_file_copy_globals.copy_start_time_ms = system_milliseconds();
	
		copy_state = _cache_copy_state_kick_off_copy_thread;
	}
	break;
	case _cache_copy_state_kick_off_copy_thread:
	{
		if (!cache_file_copy_globals.reference_count.m_reference_count)
			cache_file_copy_allocate_buffer(k_action_to_copy_buffer_size[cache_file_copy_globals.current_request.action]);
		
		if (cache_file_copy_globals.buffer)
		{
			cache_file_copy_globals.copy_abort_signal = false;
			{
				c_basic_buffer<void> buffer;
				buffer.set_buffer(cache_file_copy_globals.buffer, cache_file_copy_globals.buffer_size / 2);
				g_copy_decompressor.setup(
					cached_map_file_get_handle(cache_file_copy_globals.copying_to_map_file_index),
					cache_file_copy_globals.total_copy_bytes_transferred,
					cache_file_copy_globals.running_realtime_checksum,
					buffer);
			}
		
			{
				c_basic_buffer<void> buffer;
				buffer.set_buffer(cache_file_copy_globals.buffer, cache_file_copy_globals.buffer_size / 2);
				cache_file_copy_globals.copy_task_id = async_decompress_file_section(
					priority,
					cache_file_copy_globals.source_file_handle,
					cache_file_copy_globals.total_copy_bytes_transferred,
					cache_file_copy_globals.source_file_size - cache_file_copy_globals.total_copy_bytes_transferred,
					0,
					buffer,
					&g_copy_decompressor,
					{},
					&cache_file_copy_globals.copy_finished_success,
					&cache_file_copy_globals.copy_abort_signal,
					0,
					&cache_file_copy_globals.copy_task_is_done);
			}
		
			copy_state = _cache_copy_state_finish_copy_from_thread;
		}
		else
		{
			copy_state = _cache_copy_state_kick_off_copy_thread;
		}
	}
	break;
	case _cache_copy_state_finish_copy_from_thread:
	{
		g_copy_decompressor.teardown();
		cache_file_copy_globals.total_copy_bytes_transferred = g_copy_decompressor.m_file_offset;
		cache_file_copy_globals.running_realtime_checksum = g_copy_decompressor.m_checksum;
		if ((uns32)g_copy_decompressor.m_file_offset < cache_file_copy_globals.source_file_size)
		{
			valid = cache_file_copy_globals.copy_abort_signal.peek() != 0;
			copy_state = _cache_copy_state_kick_off_copy_thread;
		}
		else
		{
			valid = true;
			copy_state = _cache_copy_state_flush_after_copy;
		}
	}
	break;
	case _cache_copy_state_flush_after_copy:
	{
		cache_file_copy_globals.copy_task_id = async_flush_file(
			cached_map_file_get_handle(cache_file_copy_globals.copying_to_map_file_index),
			_async_category_background_copy,
			priority,
			&cache_file_copy_globals.copy_task_is_done);
	
		copy_state = _cache_copy_state_write_header;
	}
	break;
	case _cache_copy_state_write_header:
	{
		get_current_file_time(&cache_file_copy_globals.copy_file_header.slot_modification_date);
		cache_file_copy_globals.validify_cache_file_success = false;
		cache_file_copy_globals.copy_task_id = async_validify_file(
			cached_map_file_get_handle(cache_file_copy_globals.copying_to_map_file_index),
			&cache_file_copy_globals.copy_file_header,
			sizeof(s_cache_file_header),
			cached_map_file_get_size(cache_file_copy_globals.copying_to_map_file_index),
			_async_category_background_copy,
			priority,
			true,
			&cache_file_copy_globals.validify_cache_file_success,
			&cache_file_copy_globals.copy_task_is_done);
		
		copy_state = _cache_copy_state_verify_write_header;
	}
	break;
	case _cache_copy_state_verify_write_header:
	{
		valid = cache_file_copy_globals.validify_cache_file_success;
		copy_state = _cache_copy_state_mark_file_as_loaded;
	}
	break;
	case _cache_copy_state_mark_file_as_loaded:
	{
		cache_file_copy_globals.finish_reason = _cache_copy_finish_reason_copied;
		copy_state = _cache_copy_state_halt;
	}
	break;
	case _cache_copy_state_halt:
	{
		copy_state = _cache_copy_state_close_source_file;
	}
	break;
	case _cache_copy_state_close_source_file:
	{
		if (file_handle_is_valid(cache_file_copy_globals.source_file_handle))
		{
			cache_file_copy_globals.copy_task_id = async_close_file_no_stfs_flush(
				cache_file_copy_globals.source_file_handle,
				_async_category_none,
				_async_priority_blocking_generic,
				&cache_file_copy_globals.copy_task_is_done);
		}
	
		copy_state = _cache_copy_state_finish;
	}
	break;
	case _cache_copy_state_finish:
	{
		s_cache_copy_request action = cache_file_copy_globals.current_request;
	
		if (cache_file_copy_globals.finish_reason == _cache_copy_finish_reason_copied)
		{
			cached_map_file_get(cache_file_copy_globals.copying_to_map_file_index)->header = cache_file_copy_globals.copy_file_header;
			csmemset(&cache_file_copy_globals.copy_file_header, 0, sizeof(sizeof(s_cache_file_header)));
			if (s_failed_map* failed_map = find_failed_map(action.map_name.get_string()))
				csmemset(failed_map, 0, sizeof(s_failed_map));
		}
		else if (cache_file_copy_globals.finish_reason == _cache_copy_finish_reason_failed && !cache_files_has_map_terminal_failure(action.map_name.get_string()))
		{
			cache_files_copy_map(action.map_name.get_string(), action.action);
		}
	
		cache_file_copy_release_buffer();
		cache_file_copy_globals.copying_to_map_file_index = k_no_cached_map_file_index;
		csmemset(&cache_file_copy_globals.copy_file_header, 0, sizeof(sizeof(s_cache_file_header)));
		csmemset(&cache_file_copy_globals.current_request, 0, sizeof(s_cache_copy_request));
		cache_file_copy_globals.copying_map_file_path[0] = 0;
		invalidate_file_handle(&cache_file_copy_globals.source_file_handle);
	
		copy_state = _cache_copy_state_idle;
	}
	break;
	}
	
	if (cache_file_copy_globals.release_buffer)
	{
		cache_file_copy_release_buffer();
		cache_file_copy_globals.release_buffer = false;
	}
	
	if (valid)
	{
		cache_file_copy_globals.last_copy_state = cache_file_copy_globals.copy_state;
		cache_file_copy_globals.copy_state = copy_state;
	}
	else
	{
		cache_file_copy_release_buffer();
		cache_file_map_has_failed(cache_file_copy_globals.copying_map_file_path);
		cache_file_copy_globals.copy_state = _cache_copy_state_halt;
		cache_file_copy_globals.finish_reason = _cache_copy_finish_reason_failed;
	}
	
	return valid;
	*/
}

void __cdecl cache_file_copy_release_buffer()
{
	INVOKE(0x005AA020, cache_file_copy_release_buffer);

	//if (cache_file_copy_globals.buffer.begin())
	//{
	//	_optional_cache_free((e_optional_cache_user)0, cache_file_copy_globals.buffer.begin());
	//	cache_file_copy_globals.buffer.set_buffer(nullptr, 0ul);
	//}
}

int32 __cdecl cache_file_get_absolute_maximum_size()
{
	//return INVOKE(0x005AA050, cache_file_get_absolute_maximum_size);

	return 0x7FFFFFFF;
}

bool __cdecl cache_file_get_async_file_handle_from_index(e_map_file_index map_file_index, s_file_handle* out_handle)
{
	return INVOKE(0x005AA060, cache_file_get_async_file_handle_from_index, map_file_index, out_handle);
}

//.text:005AA0C0 ; bool __cdecl cache_file_get_async_file_handle_from_path(const char*, s_file_handle*)

c_static_string<256> __cdecl cache_file_get_canonical_path(const char* scenario_path)
{
	return INVOKE(0x005AA130, cache_file_get_canonical_path, scenario_path);
}

int32 __cdecl cache_file_get_failed_count(const char* scenario_path)
{
	return INVOKE(0x005AA150, cache_file_get_failed_count, scenario_path);

	//if (s_failed_map* failed_map = find_failed_map(scenario_path))
	//	return failed_map->index;
	//
	//return 0;
}

bool __cdecl cache_file_get_indirect_file_handle_from_index(e_map_file_index map_file_index, s_indirect_file* out_handle)
{
	return INVOKE(0x005AA170, cache_file_get_indirect_file_handle_from_index, map_file_index, out_handle);
}

//.text:005AA1D0 ; bool __cdecl cache_file_get_indirect_file_handle_from_path(const char*, s_indirect_file**)
//.text:005AA240 ; cache_file_get_?

bool __cdecl cache_file_get_master_async_file_handle(s_file_handle* out_handle)
{
	return INVOKE(0x005AA260, cache_file_get_master_async_file_handle, out_handle);
}

//.text:005AA290 ; bool __cdecl cache_file_get_master_canonical_path(c_static_string<256>*)

bool __cdecl cache_file_get_master_indirect_file_handle(s_indirect_file* out_handle)
{
	return INVOKE(0x005AA2D0, cache_file_get_master_indirect_file_handle, out_handle);
}

bool __cdecl cache_file_get_master_overlapped_file_handle(s_file_handle* out_handle)
{
	return INVOKE(0x005AA300, cache_file_get_master_overlapped_file_handle, out_handle);
}

bool __cdecl cache_file_get_master_resource_section_offset(uns32* out_offset)
{
	return INVOKE(0x005AA330, cache_file_get_master_resource_section_offset, out_offset);
}

//.text:005AA360 ; uns32 __cdecl cache_file_get_maximum_size(e_scenario_type)

bool __cdecl cache_file_get_overlapped_file_handle_from_index(e_map_file_index map_file_index, s_file_handle* out_handle)
{
	return INVOKE(0x005AA3C0, cache_file_get_overlapped_file_handle_from_index, map_file_index, out_handle);
}

//.text:005AA420 ; bool __cdecl cache_file_get_overlapped_file_handle_from_path(const char*, s_file_handle*)
//.text:005AA490 ; bool __cdecl cache_file_get_path_from_dvd_path(const char*, char*, int32)
//.text:005AA510 ; bool __cdecl cache_file_get_path_from_file_identifier(int32, char*, int32)
//.text:005AA560 ; bool __cdecl cache_file_get_resource_section_offset_from_path(const char*, uns32*)
//.text:005AA5D0 ; void __cdecl cache_file_header_get_file_progress_sizes(const s_cache_file_header*, int32, uns32*, uns32*, uns32*)
//.text:005AA630 ; cache_file_is_?
//.text:005AA660 ; bool __cdecl cache_file_is_ready(const char*)
//.text:005AA690 ; void __cdecl cache_file_lock_for_io(const char*, int32*)

void __cdecl cache_file_map_clear_all_failures()
{
	INVOKE(0x005AA6B0, cache_file_map_clear_all_failures);
}

//.text:005AA6D0 ; void __cdecl cache_file_map_clear_failure(const char*)

void __cdecl cache_file_map_has_failed(const char* scenario_path)
{
	INVOKE(0x005AA700, cache_file_map_has_failed, scenario_path);
}

bool __cdecl cache_file_open(const char* scenario_path, void* header)
{
	//return INVOKE(0x005AA7C0, cache_file_open, scenario_path, header);

	if (levels_path_is_dlc(scenario_path) && cache_files_read_from_map_directory())
	{
		e_map_file_index map_file_index = cached_map_files_find_map(scenario_path);
		if (map_file_index != k_no_cached_map_file_index)
		{
			cached_map_file_close(map_file_index);
		}
	}

	e_map_file_index map_file_index = cached_map_files_find_map(scenario_path);
	if (levels_path_is_dlc(scenario_path))
	{
		levels_open_dlc(scenario_path, true);
	}

	if (cache_files_read_from_map_directory())
	{
		if (map_file_index == k_no_cached_map_file_index)
		{
			if (!cached_map_file_load(map_file_index = (e_map_file_index)7, scenario_path))
			{
				return false;
			}
		}
	}
	else if (map_file_index == k_no_cached_map_file_index)
	{
		return false;
	}

	cache_file_table_of_contents.open_map_file_index = map_file_index;
	csmemcpy(header, &cache_file_table_of_contents.map_files[map_file_index].header, sizeof(s_cache_file_header));
	return true;
}

int32 __cdecl cache_file_read_ex(int32 section, int32 offset, int32 size, void* buffer, c_synchronized_long* bytes_read, c_synchronized_long* completion_flag_reference, e_async_category category, e_async_priority priority)
{
	//return INVOKE(0x005AA870, cache_file_read_ex, section, offset, size, buffer, bytes_read, completion_flag_reference, category, priority);

	int32 section_base_offset = cache_file_table_of_contents.map_files[cache_file_table_of_contents.open_map_file_index].header.section_offsets[section];
	if (section_base_offset == NONE)
	{
		return NONE;
	}

	s_file_handle section_handle = cache_file_table_of_contents.map_files[cache_file_table_of_contents.open_map_file_index].file_handle;
	return async_read_position(section_handle, buffer, size, section_base_offset + offset, category, priority, bytes_read, completion_flag_reference);
}

int32 __cdecl cache_file_round_up_read_size(int32 size)
{
	//return INVOKE(0x005AA8D0, cache_file_round_up_read_size, size);

	if ((size & MASK(4)) == 0)
	{
		return size;
	}

	return (size | MASK(4)) + 1;
}

const s_cache_file_shared_resource_usage* __cdecl cache_file_try_to_get_master_shared_resource_usage()
{
	return INVOKE(0x005AA8E0, cache_file_try_to_get_master_shared_resource_usage);
}

//.text:005AA910 ; const s_cache_file_shared_resource_usage* __cdecl cache_file_get_shared_resource_usage_from_path(const char*)
//.text:005AA970 ; void __cdecl cache_file_unlock_for_io(int32)
//.text:005AA980 ; void __cdecl cache_files_clear_map_of_type(int32)
//.text:005AAA40 ; void __cdecl cache_files_clear_map_single(int32)
//.text:005AAAC0 ; bool __cdecl cache_files_copy_describe(c_static_string<256>*, real32*)

void __cdecl cache_files_copy_do_work()
{
	INVOKE(0x005AAB20, cache_files_copy_do_work);
}

void __cdecl cache_files_copy_halt()
{
	INVOKE(0x005AAC20, cache_files_copy_halt);

	//e_cache_copy_state copy_state = cache_file_copy_globals.copy_state;
	//if (copy_state && copy_state < _cache_copy_state_halt)
	//{
	//	cache_file_copy_globals.copy_abort_signal = true;
	//	cache_file_copy_globals.finish_reason = _cache_copy_finish_reason_canceled;
	//	cache_file_copy_globals.copy_state = _cache_copy_state_halt;
	//}
}

bool __cdecl cache_files_copy_halting()
{
	return INVOKE(0x005AAC50, cache_files_copy_halting);

	//return cache_file_copy_globals.copy_state >= _cache_copy_state_halt;
}

bool __cdecl cache_files_copy_in_progress()
{
	return INVOKE(0x005AAC60, cache_files_copy_in_progress);

	//return cache_files_copy_in_progress_internal(nullptr);
}

bool __cdecl cache_files_copy_in_progress_internal(real32* out_progress)
{
	return INVOKE(0x005AAC70, cache_files_copy_in_progress_internal, out_progress);
}

//bool __cdecl cache_files_copy_map(const char* scenario_path, e_cache_file_load_action load_action)
bool __cdecl cache_files_copy_map(const char* scenario_path, int32 load_action)
{
	return INVOKE(0x005AAD30, cache_files_copy_map, scenario_path, load_action);
}

//bool __cdecl cache_files_copy_map_start_only(const char* scenario_path, e_cache_file_load_action load_action)
bool __cdecl cache_files_copy_map_start_only(const char* scenario_path, int32 load_action)
{
	//return INVOKE(0x005AAE70, cache_files_copy_map_start_only, scenario_path, load_action);

	return cache_files_get_file_status(scenario_path) == 0
		&& cache_files_copy_map(scenario_path, load_action);
}

void __cdecl cache_files_copy_pause()
{
	INVOKE(0x005AAEA0, cache_files_copy_pause);

	//cache_file_copy_globals.copy_abort_signal = true;
	//cache_file_copy_globals.reference_count++;
	//cache_file_copy_globals.release_buffer = true;
}

void __cdecl cache_files_copy_resume()
{
	INVOKE(0x005AAEC0, cache_files_copy_resume);

	//cache_file_copy_globals.reference_count--;
}

void __cdecl cache_files_copy_stop(const char* scenario_path)
{
	INVOKE(0x005AAED0, cache_files_copy_stop, scenario_path);

	//int32 file_status = cache_files_get_file_status(scenario_path);
	//if (file_status == 2 || file_status == 1)
	//{
	//	if (map_names_are_equal(scenario_path, cache_file_copy_globals.current_action.map_name.get_string()))
	//	{
	//		cache_files_copy_halt();
	//	}
	//	else if (map_names_are_equal(scenario_path, cache_file_copy_globals.current_action.pending_map_name.get_string()))
	//	{
	//		cache_file_copy_globals.current_action.pending_action = 0;
	//	}
	//}
}

void __cdecl cache_files_delete_all()
{
	INVOKE(0x005AAF50, cache_files_delete_all);

	//cached_map_files_delete(_map_file_index_shared_ui, k_total_tracked_cached_map_files_count);
}

void __cdecl cache_files_delete_if_build_number_has_changed()
{
	//INVOKE(0x005AAF60, cache_files_delete_if_build_number_has_changed);

	int32 build_number = version_get_build_number();
	if (build_number != global_preferences_get_build_number())
	{
		cache_files_delete_all();
		global_preferences_set_build_number(build_number);
	}
}

void __cdecl cache_files_delete_if_language_has_changed()
{
	//INVOKE(0x005AAF90, cache_files_delete_if_language_has_changed);

	e_language language = get_current_language();
	if (language != global_preferences_get_last_language())
	{
		cache_files_delete_all();
		global_preferences_set_last_language(language);
	}
}

//.text:005AAFC0 ; void __cdecl cache_files_dispose()
//.text:005AAFD0 ; e_cache_file_load_action __cdecl cache_files_get_current_load_action(const char*)
//.text:005AB040 ; uns32 __cdecl cache_files_get_estimated_dvd_speed()
//.text:005AB050 ; bool __cdecl cache_files_get_file_progress_sizes(const char*, int32, uns32*, uns32*, uns32*)

//e_cache_file_status __cdecl cache_files_get_file_status(const char* scenario_path)
int32 __cdecl cache_files_get_file_status(const char* scenario_path)
{
	return INVOKE(0x005AB0E0, cache_files_get_file_status, scenario_path);
}

//.text:005AB180 ; c_asynchronous_io_arena* __cdecl cache_files_get_io_arena()
//.text:005AB190 ; e_cache_file_load_action __cdecl cache_files_get_load_action(const char*)
//.text:005AB210 ; e_cache_file_load_action __cdecl cache_files_get_pending_load_action(const char*)

bool __cdecl cache_files_has_map_terminal_failure(const char* scenario_path)
{
	return INVOKE(0x005AB280, cache_files_has_map_terminal_failure, scenario_path);
}

//.text:005AB2B0 ; real32 __cdecl cache_files_individual_map_progress(const char*)
//.text:005AB320 ; uns32 __cdecl cache_files_individual_map_size(const char*)

void __cdecl cache_files_initialize()
{
	//INVOKE(0x005AB370, cache_files_initialize);

	g_cache_files_read_from_map_directory = true;

	csmemset(&cache_file_table_of_contents, 0, sizeof(cache_file_table_of_contents));
	csmemset(&cache_file_copy_globals, 0, sizeof(cache_file_copy_globals));

	cache_file_table_of_contents.open_map_file_index = k_no_cached_map_file_index;
	cache_file_table_of_contents.locked_map_file_index = k_no_cached_map_file_index;
	cache_file_table_of_contents.pending_map_file_index = k_no_cached_map_file_index;

	cache_file_copy_globals.copy_task_is_done = true;
	cache_file_copy_globals.copy_task_id = INVALID_ASYNC_TASK_ID;
	cache_file_copy_globals.copying_to_map_file_index = k_no_cached_map_file_index;

	cache_files_delete_if_language_has_changed();
	cache_files_delete_if_build_number_has_changed();

	bool success = false;
	cached_map_files_open_all(&success);

	optional_cache_register_user((e_optional_cache_user)0, &g_cache_file_copy_optional_cache_callback);
}

//.text:005AB450 ; bool __cdecl cache_files_locked_by_map(e_map_file_index, e_map_file_index)
//.text:005AB490 ; bool __cdecl cache_files_locked_by_map_and_header(e_map_file_index, e_map_file_index, const s_cache_file_header*)
//.text:005AB4E0 ; 

bool __cdecl cache_files_read_from_map_directory()
{
	//return INVOKE(0x005AB4F0, cache_files_read_from_map_directory);

	return g_cache_files_read_from_map_directory;
}

//.text:005AB500 ; void __cdecl cache_files_reset_failed_dlc_maps()

bool __cdecl cache_files_running_off_dvd()
{
	//return INVOKE(0x005AB540, cache_files_running_off_dvd);

	return false;
}

//.text:005AB550 ; bool __cdecl cache_map_file_can_use_for_io(e_map_file_index)
//.text:005AB5B0 ; bool __cdecl cache_map_file_is_loaded(e_map_file_index)

int32 __cdecl cache_map_file_nuke(e_map_file_index map_file_index, e_async_category category, e_async_priority priority, bool* success, c_synchronized_long* done)
{
	return INVOKE(0x005AB5D0, cache_map_file_nuke, map_file_index, category, priority, success, done);
}

void __cdecl cache_requests_flush()
{
	//INVOKE(0x005AB620, cache_requests_flush);

	cache_file_block_until_not_busy();
}

void __cdecl cached_map_file_close(e_map_file_index map_file_index)
{
	//INVOKE(0x005AB630, cached_map_file_close, map_file_index);

	s_cached_map_file* map_file = cached_map_file_get(map_file_index);
	if (file_handle_is_valid(map_file->file_handle))
	{
		c_synchronized_long done{};
		async_close_file(map_file->file_handle, _async_category_none, _async_priority_blocking_generic, &done);
		internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);
		invalidate_file_handle(&map_file->file_handle);

		g_cache_file_io_arena.close_file(&map_file->bulk_read_handle);

		async_close_file(map_file->overlapped_handle, _async_category_none, _async_priority_blocking_generic, &done);
		internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);
		invalidate_file_handle(&map_file->overlapped_handle);
	}
	csmemset(&map_file->header, 0, sizeof(map_file->header));
}

bool __cdecl cached_map_file_dependencies_loaded(const s_cache_file_header* header, uns32* shared_files_flags)
{
	return INVOKE(0x005AB6F0, cached_map_file_dependencies_loaded, header, shared_files_flags);
}

s_cached_map_file* __cdecl cached_map_file_get(e_map_file_index map_file_index)
{
	//return INVOKE(0x005AB7E0, cached_map_file_get, map_file_index);

	ASSERT(VALID_INDEX(map_file_index, k_total_tracked_cached_map_files_count));

	return &cache_file_table_of_contents.map_files[map_file_index];
}

s_file_handle __cdecl cached_map_file_get_handle(e_map_file_index map_file_index)
{
	//return INVOKE(0x005AB800, cached_map_file_get_handle, map_file_index);

	s_file_handle handle = cached_map_file_get(map_file_index)->file_handle;
	ASSERT(file_handle_is_valid(handle));

	return cached_map_file_get(map_file_index)->file_handle;
}

//.text:005AB820 ; void __cdecl cached_map_file_get_path(e_map_file_index, char*, uns32)
//.text:005AB8B0 ; void __cdecl cached_map_file_get_path_explicit_drive(e_map_file_index, const char*, char*, uns32)
//.text:005AB8E0 ; const char* __cdecl cached_map_file_get_root(e_map_file_index)
//.text:005AB910 ; e_map_file_index __cdecl cached_map_file_get_shared_index(e_cache_file_shared_file_type)

uns32 __cdecl cached_map_file_get_size(e_map_file_index map_file_index)
{
	return INVOKE(0x005AB970, cached_map_file_get_size, map_file_index);
}

//.text:005AB9B0 ; 
//.text:005AB9E0 ; bool __cdecl cached_map_file_open(s_cached_map_file*, const char*)

bool __cdecl cached_map_file_load(e_map_file_index map_file_index, const char* scenario_path)
{
	return INVOKE(0x005ABAD0, cached_map_file_load, map_file_index, scenario_path);
}

//.text:005ABBD0 ; bool __cdecl cached_map_file_read_header(e_map_file_index)
//.text:005ABD00 ; 

void __cdecl cached_map_files_delete(e_map_file_index start_map_file_index, e_map_file_index end_map_file_index)
{
	INVOKE(0x005ABD40, cached_map_files_delete, start_map_file_index, end_map_file_index);
}

e_map_file_index __cdecl cached_map_files_find_free_utility_drive_map(int32 size, int16 map_type)
{
	return INVOKE(0x005ABDF0, cached_map_files_find_free_utility_drive_map, size, map_type);
}

e_map_file_index __cdecl cached_map_files_find_map(const char* scenario_path)
{
	return INVOKE(0x005ABE90, cached_map_files_find_map, scenario_path);
}

void __cdecl cached_map_files_open_all(bool* success)
{
	//INVOKE(0x005ABF00, cached_map_files_open_all, success);

	event(_event_message, "cache: open all cache map files");

	if (cache_files_read_from_map_directory())
	{
		for (s_cached_map_file& map_file : cache_file_table_of_contents.map_files)
		{
			invalidate_file_handle(&map_file.file_handle);
			invalidate_file_handle(&map_file.overlapped_handle);
		}

		// k_main_menu_scenario_tag
		//  - levels\\ui\\mainmenu\\mainmenu
		// k_multiplayer_shared_scenario_tag
		//  - levels\\shared\\shared\\shared
		// k_single_player_shared_scenario_tag 
		//  - levels\\shared\\shared\\campaign

		c_static_sized_dynamic_array<s_cache_file_share_map, k_total_tracked_cached_map_files_count> shared_files;
		shared_files[shared_files.new_element_index()] = { .file_path = k_main_menu_scenario_tag,               .index = _map_file_index_shared_ui,         .previous_index = k_no_cached_map_file_index        };
		shared_files[shared_files.new_element_index()] = { .file_path = g_cache_file_globals.resource_files[0], .index = _map_file_index_shared_resources,  .previous_index = _map_file_index_shared_ui         };
		shared_files[shared_files.new_element_index()] = { .file_path = g_cache_file_globals.resource_files[1], .index = _map_file_index_shared_textures,   .previous_index = _map_file_index_shared_resources  };
		shared_files[shared_files.new_element_index()] = { .file_path = g_cache_file_globals.resource_files[2], .index = _map_file_index_shared_textures_b, .previous_index = _map_file_index_shared_textures   };
		shared_files[shared_files.new_element_index()] = { .file_path = g_cache_file_globals.resource_files[3], .index = _map_file_index_shared_audio,      .previous_index = _map_file_index_shared_textures_b };
		shared_files[shared_files.new_element_index()] = { .file_path = g_cache_file_globals.resource_files[4], .index = _map_file_index_shared_video,      .previous_index = _map_file_index_shared_audio      };

		// for some reason the above array is iterated over in reverse
		for (s_cache_file_share_map& shared_file : shared_files.reverse())
		{
			if (cached_map_file_is_shared(shared_file.previous_index))
				cached_map_file_load(shared_file.index, shared_file.file_path);
		}

		*success = true;
	}
	else
	{
		*success = false;
	}

	event(_event_message, "cache: open all cache map files complete");
}

void __cdecl canonicalize_map_path(const char* path, c_static_string<256>* out_path)
{
	//INVOKE(0x005ABFF0, canonicalize_map_path, path, out_path);

	c_static_string<256> stripped_path;
	copy_and_strip_suffix(tag_name_strip_path(path), &stripped_path);
	out_path->print(k_cache_path_format, stripped_path.get_string());
}

//.text:005AC070 ; c_cache_file_copy_fake_decompressor::check_for_doneness

void __cdecl copy_and_strip_suffix(const char* path, c_static_string<256>* stripped_path)
{
	INVOKE(0x005AC0F0, copy_and_strip_suffix, path, stripped_path);
}

//.text:005AC140 ; 

//.text:005AC150 ; c_cache_file_copy_fake_decompressor::decompress_buffer

s_failed_map* __cdecl find_failed_map(const char* scenario_path)
{
	return INVOKE(0x005AC210, find_failed_map, scenario_path);
}

//.text:005AC2A0 ; const s_map_file_table* __cdecl find_map_file_table_entry(e_map_file_index)

//.text:005AC2D0 ; c_cache_file_copy_fake_decompressor::finish
//.text:005AC300 ; c_cache_file_copy_fake_decompressor::get_copy_progress

//.text:005AC310 ; c_cache_file_copy_optional_cache_callback::get_memory_configuration

//.text:005AC370 ; c_cache_file_copy_fake_decompressor::get_running_checksum
//.text:005AC380 ; c_cache_file_copy_fake_decompressor::get_volatile_copy_progress

bool __cdecl map_names_are_equal(const char* map_name_a, const char* map_name_b)
{
	return INVOKE(0x005AC390, map_names_are_equal, map_name_a, map_name_b);
}

//.text:005AC420 ; bool __cdecl new_slot_is_better_fit(e_map_file_index, e_map_file_index)

//.text:005AC4F0 ; c_cache_file_copy_fake_decompressor::overall_copy_in_progress

//.text:005AC500 ; bool __cdecl pending_cache_file_copy_request()

//.text:005AC510 ; 
//.text:005AC520 ; 
//.text:005AC550 ; 

void c_cache_file_copy_fake_decompressor::setup(s_file_handle file_handle, uns32 offset, uns32 checksum, c_basic_buffer<void> buffer)
{
	INVOKE_CLASS_MEMBER(0x005AC580, c_cache_file_copy_fake_decompressor, setup, file_handle, offset, checksum, buffer);
}

const char* __cdecl shared_file_type_get_string(e_cache_file_shared_file_type shared_file_type)
{
	//return INVOKE(0x005AC5C0, shared_file_type_get_string, shared_file_type);

	switch (shared_file_type)
	{
	case _cache_file_shared_file_type_ui:         return "ui";
	case _cache_file_shared_file_type_resources:  return "resources.dat";
	case _cache_file_shared_file_type_textures:   return "textures.dat";
	case _cache_file_shared_file_type_textures_b: return "textures_b.dat";
	case _cache_file_shared_file_type_audio:      return "audio.dat";
	case _cache_file_shared_file_type_video:      return "video.dat";
	}

	return "<unknown>";
}

//.text:005AC620 ; 
//.text:005AC630 ; 

void c_cache_file_copy_fake_decompressor::teardown()
{
	INVOKE_CLASS_MEMBER(0x005AC640, c_cache_file_copy_fake_decompressor, teardown);

	//m_done.peek();
	//m_buffer.set_buffer(nullptr, 0ul);
	//m_overall_copy_in_progress = false;
}

//.text:005AC660 ; c_cache_file_copy_optional_cache_callback::terminate
//.text:005AC6B0 ; 
//.text:005AC6C0 ; 
//.text:005AC6D0 ; c_cache_file_copy_fake_decompressor::wait_for_doneness

