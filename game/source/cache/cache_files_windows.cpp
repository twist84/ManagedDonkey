#include "cache/cache_files_windows.hpp"

#include "config/version.hpp"
#include "cseries/async.hpp"
#include "cseries/cseries_events.hpp"
#include "main/global_preferences.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x018A1A50, c_cache_file_copy_optional_cache_callback, g_cache_file_copy_optional_cache_callback);
REFERENCE_DECLARE(0x018A1A54, c_cache_file_copy_fake_decompressor, g_copy_decompressor);
REFERENCE_DECLARE(0x0240B1E0, bool, g_cache_files_are_absolute);
REFERENCE_DECLARE(0x0240B1E8, s_cache_file_table_of_contents, cache_file_table_of_contents);
REFERENCE_DECLARE(0x0243C098, s_cache_file_copy_globals, cache_file_copy_globals);
REFERENCE_DECLARE(0x0243F780, c_asynchronous_io_arena, g_cache_file_io_arena);

HOOK_DECLARE(0x005AAE70, cache_files_copy_map_start_only);
HOOK_DECLARE(0x005ABFF0, canonicalize_map_path);

bool __cdecl cached_map_file_is_shared(e_map_file_index map_file_index)
{
	return INVOKE(0x00501E80, cached_map_file_is_shared, map_file_index);
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
	//for (long category_index = 0; category_index < NUMBEROF(categories); category_index++)
	//{
	//	e_async_category category = categories[category_index];
	//	while (async_category_in_queue(category))
	//		switch_to_thread();
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

//.text:005A9750 ; void __cdecl cache_file_copy_allocate_buffer(long)
//.text:005A97C0 ; bool __cdecl cache_file_copy_do_action()
//.text:005AA020 ; void __cdecl cache_file_copy_release_buffer()

long __cdecl cache_file_get_absolute_maximum_size()
{
	//return INVOKE(0x005AA050, cache_file_get_absolute_maximum_size);

	return 0x7FFFFFFF;
}

bool __cdecl cache_file_get_async_file_handle_from_index(e_map_file_index map_file_index, s_file_handle* out_handle)
{
	return INVOKE(0x005AA060, cache_file_get_async_file_handle_from_index, map_file_index, out_handle);
}

//.text:005AA0C0 ; bool __cdecl cache_file_get_async_file_handle_from_path(char const*, s_file_handle*)

c_static_string<256> __cdecl cache_file_get_canonical_path(char const* scenario_path)
{
	return INVOKE(0x005AA130, cache_file_get_canonical_path, scenario_path);
}

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
//.text:005AA690 ; void __cdecl cache_file_lock_for_io(char const*, long*)

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
//.text:005AA970 ; void __cdecl cache_file_unlock_for_io(long)
//.text:005AA980 ; void __cdecl cache_files_clear_map_of_type(long)
//.text:005AAA40 ; void __cdecl cache_files_clear_map_single(long)
//.text:005AAAC0 ; bool __cdecl cache_files_copy_describe(c_static_string<256>*, real*)

void __cdecl cache_files_copy_do_work()
{
	INVOKE(0x005AAB20, cache_files_copy_do_work);
}

void __cdecl cache_files_copy_halt()
{
	INVOKE(0x005AAC20, cache_files_copy_halt);
}

bool __cdecl cache_files_copy_halting()
{
	return INVOKE(0x005AAC50, cache_files_copy_halting);
}

bool __cdecl cache_files_copy_in_progress()
{
	return INVOKE(0x005AAC60, cache_files_copy_in_progress);
}

bool __cdecl cache_files_copy_in_progress_internal(real* out_progress)
{
	return INVOKE(0x005AAC70, cache_files_copy_in_progress_internal, out_progress);
}

//bool __cdecl cache_files_copy_map(char const* scenario_path, e_cache_file_load_action load_action)
bool __cdecl cache_files_copy_map(char const* scenario_path, long load_action)
{
	return INVOKE(0x005AAD30, cache_files_copy_map, scenario_path, load_action);
}

//bool __cdecl cache_files_copy_map_start_only(char const* scenario_path, e_cache_file_load_action load_action)
bool __cdecl cache_files_copy_map_start_only(char const* scenario_path, long load_action)
{
	//return INVOKE(0x005AAE70, cache_files_copy_map_start_only, scenario_path, load_action);

	return cache_files_get_file_status(scenario_path) == 0
		&& cache_files_copy_map(scenario_path, load_action);
}

void __cdecl cache_files_copy_pause()
{
	INVOKE(0x005AAEA0, cache_files_copy_pause);
}

void __cdecl cache_files_copy_resume()
{
	INVOKE(0x005AAEC0, cache_files_copy_resume);
}

void __cdecl cache_files_copy_stop(char const* scenario_path)
{
	INVOKE(0x005AAED0, cache_files_copy_stop, scenario_path);
}

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

//e_cache_file_status __cdecl cache_files_get_file_status(char const* scenario_path)
long __cdecl cache_files_get_file_status(char const* scenario_path)
{
	return INVOKE(0x005AB0E0, cache_files_get_file_status, scenario_path);
}

//.text:005AB180 ; c_asynchronous_io_arena* __cdecl cache_files_get_io_arena()
//.text:005AB190 ; e_cache_file_load_action __cdecl cache_files_get_load_action(char const*)
//.text:005AB210 ; e_cache_file_load_action __cdecl cache_files_get_pending_load_action(char const*)
//.text:005AB280 ; bool __cdecl cache_files_has_map_terminal_failure(char const*)
//.text:005AB2B0 ; real __cdecl cache_files_individual_map_progress(char const*)
//.text:005AB320 ; dword __cdecl cache_files_individual_map_size(char const*)

void __cdecl cache_files_initialize()
{
	//INVOKE(0x005AB370, cache_files_initialize);

	// seems like it was added in ODST?
	// controls a few things
	// this is the best name that we thought of
	g_cache_files_are_absolute = true;
	
	csmemset(&cache_file_table_of_contents, 0, sizeof(cache_file_table_of_contents));
	csmemset(&cache_file_copy_globals, 0, sizeof(cache_file_copy_globals));
	
	cache_file_table_of_contents.open_map_file_index = k_no_cached_map_file_index;
	cache_file_table_of_contents.locked_map_file_index = k_no_cached_map_file_index;
	cache_file_table_of_contents.__unknown30668 = NONE;
	
	cache_file_copy_globals.copy_task_is_done = 1;
	cache_file_copy_globals.copy_task_id = NONE;
	cache_file_copy_globals.map_file_index = k_no_cached_map_file_index;
	
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

	if (g_cache_files_are_absolute)
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

//.text:005AC500 ; bool __cdecl pending_cache_file_copy_request()

//.text:005AC510 ; 
//.text:005AC520 ; 
//.text:005AC550 ; 
//.text:005AC580 ; c_cache_file_copy_fake_decompressor::setup

char const* __cdecl shared_file_type_get_string(e_cache_file_shared_file_type shared_file_type)
{
	//return INVOKE(0x005AC5C0, shared_file_type_get_string, shared_file_type);

	switch (shared_file_type)
	{
	case _cache_file_shared_file_type_ui:
		return "ui";
	case _cache_file_shared_file_type_resources:
		return "resources.dat";
	case _cache_file_shared_file_type_textures:
		return "textures.dat";
	case _cache_file_shared_file_type_textures_b:
		return "textures_b.dat";
	case _cache_file_shared_file_type_audio:
		return "audio.dat";
	case _cache_file_shared_file_type_video:
		return "video.dat";
	}

	return "<unknown>";
}

//.text:005AC620 ; 
//.text:005AC630 ; 
//.text:005AC640 ; c_cache_file_copy_fake_decompressor::teardown
//.text:005AC660 ; c_cache_file_copy_optional_cache_callback::terminate
//.text:005AC6B0 ; 
//.text:005AC6C0 ; 
//.text:005AC6D0 ; c_cache_file_copy_fake_decompressor::wait_for_doneness

