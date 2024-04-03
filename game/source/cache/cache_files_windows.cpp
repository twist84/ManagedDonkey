#include "cache/cache_files_windows.hpp"

#include "config/version.hpp"
#include "cseries/cseries_events.hpp"
#include "main/global_preferences.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x018A1A50, c_cache_file_copy_optional_cache_callback, g_cache_file_copy_optional_cache_callback);
REFERENCE_DECLARE(0x018A1A54, c_cache_file_copy_fake_decompressor, g_copy_decompressor);
REFERENCE_DECLARE(0x0240B1E0, bool, g_cache_files_are_absolute);
REFERENCE_DECLARE(0x0240B1E8, s_cache_file_table_of_contents, cache_file_table_of_contents);
REFERENCE_DECLARE(0x0243C098, s_cache_file_copy_globals, cache_file_copy_globals);
REFERENCE_DECLARE(0x0243F780, c_asynchronous_io_arena, g_cache_file_io_arena);

HOOK_DECLARE(0x005ABFF0, canonicalize_map_path);

bool __cdecl cached_map_file_is_shared(e_map_file_index map_file_index)
{
	return INVOKE(0x00501E80, cached_map_file_is_shared, map_file_index);
}

void __cdecl cache_file_block_until_not_busy()
{
	INVOKE(0x005A96C0, cache_file_block_until_not_busy);
}

void __cdecl cache_file_close()
{
	//INVOKE(0x005A9730, cache_file_close);

	if (cache_file_table_of_contents.open_map_file_index != _map_file_index_none)
	{
		cache_requests_flush();
		cache_file_table_of_contents.open_map_file_index = _map_file_index_none;
	}
}

//.text:005A9750 ; void __cdecl cache_file_copy_allocate_buffer(long)
//.text:005A97C0 ; bool __cdecl cache_file_copy_do_action()
//.text:005AA020 ; void __cdecl cache_file_copy_release_buffer()

long __cdecl cache_file_get_absolute_maximum_size()
{
	//return INVOKE(0x005AA050, cache_file_get_absolute_maximum_size);

	return 0x7FFFFFFF;
}

//.text:005AA060 ; bool __cdecl cache_file_get_async_file_handle_from_index(char const*, s_file_handle*)
//.text:005AA0C0 ; bool __cdecl cache_file_get_async_file_handle_from_path(char const*, s_file_handle*)
//.text:005AA130 ; c_static_string<256> __cdecl cache_file_get_canonical_path(char const*)

long __cdecl cache_file_get_failed_count(char const* map_path)
{
	return INVOKE(0x005AA150, cache_file_get_failed_count, map_path);
}

//.text:005AA170 ; 
//.text:005AA1D0 ; bool __cdecl cache_file_get_indirect_file_handle_from_path(char const*, s_indirect_file**)
//.text:005AA240 ; 
//.text:005AA260 ; bool __cdecl cache_file_get_master_async_file_handle(s_file_handle*)
//.text:005AA290 ; bool __cdecl cache_file_get_io_identifier_canonical_path(c_static_string<256>*)
//.text:005AA2D0 ; bool __cdecl cache_file_get_master_indirect_file_handle(s_indirect_file**)
//.text:005AA300 ; bool __cdecl cache_file_get_master_overlapped_file_handle(s_file_handle*)
//.text:005AA330 ; bool __cdecl cache_file_get_master_resource_section_offset(dword*)
//.text:005AA360 ; 
//.text:005AA3C0 ; bool __cdecl cache_file_get_overlapped_file_handle_from_index(long, s_file_handle*)
//.text:005AA420 ; bool __cdecl cache_file_get_overlapped_file_handle_from_path(char const*, s_file_handle*)
//.text:005AA490 ; 
//.text:005AA510 ; 
//.text:005AA560 ; bool __cdecl cache_file_get_resource_section_offset_from_path(char const*, dword*)
//.text:005AA5D0 ; void __cdecl cache_file_header_get_file_progress_sizes(s_cache_file_header const*, long, dword*, dword*, dword*)
//.text:005AA630 ; 
//.text:005AA660 ; bool __cdecl cache_file_is_ready(char const*)
//.text:005AA690 ; 

void __cdecl cache_file_map_clear_all_failures()
{
	INVOKE(0x005AA6B0, cache_file_map_clear_all_failures);
}

//.text:005AA6D0 ; void __cdecl cache_file_map_clear_failure(char const*)
//.text:005AA700 ; void __cdecl cache_file_map_has_failed(char const*)

bool __cdecl cache_file_open(char const* scenario_path, void* header)
{
	return INVOKE(0x005AA7C0, cache_file_open, scenario_path, header);
}

//.text:005AA870 ; long __cdecl cache_file_read_ex(long, long, long, void*, c_synchronized_long*, c_synchronized_long*, e_async_category, e_async_priority)

long __cdecl cache_file_round_up_read_size(long size)
{
	//return INVOKE(0x005AA8D0, cache_file_round_up_read_size, size);

	if ((size & MASK(4)) == 0)
		return size;

	return (size | MASK(4)) + 1;
}

//.text:005AA8E0 ; s_cache_file_shared_resource_usage const* __cdecl cache_file_try_to_get_master_shared_resource_usage()
//.text:005AA910 ; 
//.text:005AA970 ; 
//.text:005AA980 ; void __cdecl cache_files_clear_map_of_type(long)
//.text:005AAA40 ; void __cdecl cache_files_clear_map_single(long)
//.text:005AAAC0 ; bool __cdecl cache_files_copy_describe(c_static_string<256>*, real*)

void __cdecl cache_files_copy_do_work()
{
	INVOKE(0x005AAB20, cache_files_copy_do_work);
}

//.text:005AAC20 ; void __cdecl cache_files_copy_halt()
//.text:005AAC50 ; 
//.text:005AAC60 ; bool __cdecl cache_files_copy_in_progress()
//.text:005AAC70 ; bool __cdecl cache_files_copy_in_progress_internal(real*)
//.text:005AAD30 ; bool __cdecl cache_files_copy_map(char const*, e_cache_file_load_action)
//.text:005AAE70 ; bool __cdecl cache_files_copy_map_start_only(char const*, e_cache_file_load_action)

void __cdecl cache_files_copy_pause()
{
	INVOKE(0x005AAEA0, cache_files_copy_pause);
}

void __cdecl cache_files_copy_resume()
{
	INVOKE(0x005AAEC0, cache_files_copy_resume);
}

//.text:005AAED0 ; void __cdecl cache_files_copy_stop(char const*)

void __cdecl cache_files_delete_all()
{
	INVOKE(0x005AAF50, cache_files_delete_all);
}

void __cdecl cache_files_delete_if_build_number_has_changed()
{
	//INVOKE(0x005AAF60, cache_files_delete_if_build_number_has_changed);

	long build_number = version_get_build_number();
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
//.text:005AAFD0 ; e_cache_file_load_action __cdecl cache_files_get_current_load_action(char const*)
//.text:005AB040 ; dword __cdecl cache_files_get_estimated_dvd_speed()
//.text:005AB050 ; bool __cdecl cache_files_get_file_progress_sizes(char const*, long, dword*, dword*, dword*)
//.text:005AB0E0 ; e_cache_file_status __cdecl cache_files_get_file_status(char const*)
//.text:005AB180 ; c_asynchronous_io_arena* __cdecl cache_files_get_io_arena()
//.text:005AB190 ; e_cache_file_load_action __cdecl cache_files_get_load_action(char const*)
//.text:005AB210 ; e_cache_file_load_action __cdecl cache_files_get_pending_load_action(char const*)
//.text:005AB280 ; 
//.text:005AB2B0 ; real __cdecl cache_files_individual_map_progress(char const*)
//.text:005AB320 ; dword __cdecl cache_files_individual_map_size(char const*)

void __cdecl cache_files_initialize()
{
	//INVOKE(0x005AB370, cache_files_initialize);

	// controls a few things
	// this is the best name that we thought of
	g_cache_files_are_absolute = true;
	
	csmemset(&cache_file_table_of_contents, 0, sizeof(cache_file_table_of_contents));
	csmemset(&cache_file_copy_globals, 0, sizeof(cache_file_copy_globals));
	
	cache_file_table_of_contents.open_map_file_index = _map_file_index_none;
	cache_file_table_of_contents.locked_map_file_index = _map_file_index_none;
	cache_file_table_of_contents.__unknown30668 = NONE;
	
	cache_file_copy_globals.copy_task_is_done = 1;
	cache_file_copy_globals.copy_task_id = NONE;
	cache_file_copy_globals.map_file_index = _map_file_index_none;
	
	cache_files_delete_if_language_has_changed();
	cache_files_delete_if_build_number_has_changed();
	
	bool success = false;
	cached_map_files_open_all(&success);
	
	optional_cache_register_user((e_optional_cache_user)0, &g_cache_file_copy_optional_cache_callback);
}

//.text:005AB450 ; bool __cdecl cache_files_locked_by_map(e_map_file_index, e_map_file_index)
//.text:005AB490 ; bool __cdecl cache_files_locked_by_map_and_header(e_map_file_index, e_map_file_index, s_cache_file_header const*)
//.text:005AB4F0 ; cache_files_are_absolute?
//.text:005AB500 ; void __cdecl cache_files_reset_failed_dlc_maps()
//.text:005AB540 ; bool __cdecl cache_files_running_off_dvd()
//.text:005AB550 ; bool __cdecl cache_map_file_can_use_for_io(e_map_file_index)
//.text:005AB5B0 ; bool __cdecl cache_map_file_is_loaded(e_map_file_index)
//.text:005AB5D0 ; long __cdecl cache_map_file_nuke(e_map_file_index, e_async_category, e_async_priority, bool*, c_synchronized_long*)

void __cdecl cache_requests_flush()
{
	//INVOKE(0x005AB620, cache_requests_flush);

	cache_file_block_until_not_busy();
}

void __cdecl cached_map_file_close(e_map_file_index map_file_index)
{
	INVOKE(0x005AB630, cached_map_file_close, map_file_index);
}

//.text:005AB6F0 ; cached_map_file_dependencies_loaded

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

//.text:005AB820 ; void __cdecl cached_map_file_get_path(e_map_file_index, char*, dword)
//.text:005AB8B0 ; void __cdecl cached_map_file_get_path_explicit_drive(e_map_file_index, char const*, char*, dword)
//.text:005AB8E0 ; char const* __cdecl cached_map_file_get_root(e_map_file_index)
//.text:005AB910 ; e_map_file_index __cdecl cached_map_file_get_shared_index(e_cache_file_shared_file_type)
//.text:005AB970 ; dword __cdecl cached_map_file_get_size(e_map_file_index)
//.text:005AB9B0 ; 
//.text:005AB9E0 ; bool __cdecl cached_map_file_open(s_cached_map_file*, char const*)

bool __cdecl cached_map_file_load(e_map_file_index map_file_index, char const* scenario_path)
{
	return INVOKE(0x005ABAD0, cached_map_file_load, map_file_index, scenario_path);
}

//.text:005ABBD0 ; bool __cdecl cached_map_file_read_header(e_map_file_index)
//.text:005ABD00 ; 
//.text:005ABD40 ; void __cdecl cached_map_files_delete(e_map_file_index, e_map_file_index)
//.text:005ABDF0 ; e_map_file_index __cdecl cached_map_files_find_free_utility_drive_map(long, short)
//.text:005ABE90 ; e_map_file_index __cdecl cached_map_files_find_map(char const*)

void __cdecl cached_map_files_open_all(bool* success)
{
	//INVOKE(0x005ABF00, cached_map_files_open_all, success);

	generate_event(_event_level_message, "cache: open all cache map files");

	for (s_cached_map_file& map_file : cache_file_table_of_contents.map_files)
	{
		invalidate_file_handle(&map_file.file_handle);
		invalidate_file_handle(&map_file.overlapped_handle);
	}

	// k_main_menu_scenario_tag
	// k_multiplayer_shared_scenario_tag
	// k_single_player_shared_scenario_tag

	s_cache_file_share_map shared_files[]
	{
		{ .file_path = "levels\\ui\\mainmenu\\mainmenu",       .index = _map_file_index_shared_ui,         .previous_index = _map_file_index_none              }, // k_main_menu_scenario_tag
		{ .file_path = g_cache_file_globals.resource_files[0], .index = _map_file_index_shared_resources,  .previous_index = _map_file_index_shared_ui         },
		{ .file_path = g_cache_file_globals.resource_files[1], .index = _map_file_index_shared_textures,   .previous_index = _map_file_index_shared_resources  },
		{ .file_path = g_cache_file_globals.resource_files[2], .index = _map_file_index_shared_textures_b, .previous_index = _map_file_index_shared_textures   },
		{ .file_path = g_cache_file_globals.resource_files[3], .index = _map_file_index_shared_audio,      .previous_index = _map_file_index_shared_textures_b },
		{ .file_path = g_cache_file_globals.resource_files[4], .index = _map_file_index_shared_video,      .previous_index = _map_file_index_shared_audio      },
	};
	ASSERT(NUMBEROF(shared_files) == k_cached_map_file_shared_count);

	for (long shared_file_index = NUMBEROF(shared_files) - 1; shared_file_index >= 0; shared_file_index--)
	{
		s_cache_file_share_map* shared_file = &shared_files[shared_file_index];
		if (cached_map_file_is_shared(shared_file->previous_index))
			cached_map_file_load(shared_file->index, shared_file->file_path);
	}

	*success = true;

	generate_event(_event_level_message, "cache: open all cache map files complete");
}

void __cdecl canonicalize_map_path(char const* path, c_static_string<256>* out_path)
{
	//INVOKE(0x005ABFF0, canonicalize_map_path, path, out_path);

	c_static_string<256> stripped_path;
	copy_and_strip_suffix(tag_name_strip_path(path), &stripped_path);
	out_path->print(k_cache_path_format, stripped_path.get_string());
}

//.text:005AC070 ; c_cache_file_copy_fake_decompressor::check_for_doneness

void __cdecl copy_and_strip_suffix(char const* path, c_static_string<256>* stripped_path)
{
	INVOKE(0x005AC0F0, copy_and_strip_suffix, path, stripped_path);
}

//.text:005AC140 ; 

//.text:005AC150 ; c_cache_file_copy_fake_decompressor::decompress_buffer

//.text:005AC210 ; s_failed_map* __cdecl find_failed_map(char const*)
//.text:005AC2A0 ; s_map_file_table const* __cdecl find_map_file_table_entry(e_map_file_index)

//.text:005AC2D0 ; c_cache_file_copy_fake_decompressor::finish
//.text:005AC300 ; c_cache_file_copy_fake_decompressor::get_copy_progress

//.text:005AC310 ; c_cache_file_copy_optional_cache_callback::get_memory_configuration

//.text:005AC370 ; c_cache_file_copy_fake_decompressor::get_running_checksum
//.text:005AC380 ; c_cache_file_copy_fake_decompressor::get_volatile_copy_progress

//.text:005AC390 ; bool __cdecl map_names_are_equal(char const*, char const*)
//.text:005AC420 ; bool __cdecl new_slot_is_better_fit(e_map_file_index, e_map_file_index)

//.text:005AC4F0 ; c_cache_file_copy_fake_decompressor::overall_copy_in_progress

//.text:005AC500 ; 

