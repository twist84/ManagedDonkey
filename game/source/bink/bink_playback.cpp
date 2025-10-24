#include "bink/bink_playback.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"

//.text:00615D30 ; 
//.text:00615D50 ; 
//.text:00615DB0 ; 
//.text:00615DD0 ; 
//.text:00615DE0 ; 
//.text:00615DF0 ; 
//.text:00615E30 ; 
//.text:00615E50 ; 
//.text:00615E60 ; 
//.text:00615E80 ; 
//.text:00615E90 ; 
//.text:00615EA0 ; 
//.text:00615EC0 ; 
//.text:00615ED0 ; 
//.text:00615EE0 ; 
//.text:00615FD0 ; 
//.text:00615FE0 ; void __cdecl __tls_set_g_bink_globals_allocator(void*)
//.text:00616000 ; 
//.text:00616020 ; 
//.text:00616060 ; public: virtual void* __cdecl c_bink_allocation::allocate(unsigned int, const char*)
//.text:00616070 ; void* __cdecl bink_alloc(unsigned int)
//.text:006161B0 ; void __cdecl bink_close(s_bink_runtime*)
//.text:00616220 ; 
//.text:00616240 ; void __cdecl bink_destroy_rasterizer()
//.text:00616250 ; void __cdecl bink_display_frame(const rectangle2d*)
//.text:00616420 ; void __cdecl bink_draw_frame()
//.text:006165A0 ; 
//.text:006166A0 ; 
//.text:006166E0 ; void __cdecl bink_free(void*)
//.text:00616780 ; real32 __cdecl bink_get_fade_amount()
//.text:00616800 ; int32 __cdecl bink_get_last_updated_frame()
//.text:00616830 ; void __cdecl bink_get_memory_available(const char*, bool)
//.text:00616870 ; int32 __cdecl bink_get_next_decompression_frame(const s_bink_video_frame*)
//.text:00616890 ; int32 __cdecl bink_get_texture_index_to_render()

void __cdecl bink_notify_load_masking_complete()
{
	INVOKE(0x006168C0, bink_notify_load_masking_complete);
}

//.text:006168E0 ; void __cdecl bink_playback_acquire_runtime_memory(uns32)

bool __cdecl bink_playback_active()
{
	return INVOKE(0x00616910, bink_playback_active);
}

bool __cdecl bink_playback_active_fullscreen()
{
	return INVOKE(0x00616930, bink_playback_active_fullscreen);
}

void __cdecl bink_playback_check_for_terminate_no_lock()
{
	INVOKE(0x00616960, bink_playback_check_for_terminate_no_lock);
}

void __cdecl bink_playback_dispose()
{
	INVOKE(0x006169A0, bink_playback_dispose);
}

void __cdecl bink_playback_dispose_from_old_map()
{
	INVOKE(0x00616A00, bink_playback_dispose_from_old_map);
}

void __cdecl bink_playback_dispose_from_old_non_bsp_zone_set(const s_game_non_bsp_zone_set* old_non_bsp_zone_set)
{
	INVOKE(0x00616A10, bink_playback_dispose_from_old_non_bsp_zone_set, old_non_bsp_zone_set);
}

void __cdecl bink_playback_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x00616A20, bink_playback_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl bink_playback_grab_memory()
{
	INVOKE(0x00616A30, bink_playback_grab_memory);
}

bool __cdecl bink_playback_in_progress()
{
	return INVOKE(0x00616AA0, bink_playback_in_progress);
}

void __cdecl bink_playback_initialize()
{
	INVOKE(0x00616AC0, bink_playback_initialize);
}

void __cdecl bink_playback_initialize_for_new_map()
{
	INVOKE(0x00616B70, bink_playback_initialize_for_new_map);
}

void __cdecl bink_playback_initialize_for_new_non_bsp_zone_set(const s_game_non_bsp_zone_set* new_non_bsp_zone_set)
{
	INVOKE(0x00616B80, bink_playback_initialize_for_new_non_bsp_zone_set, new_non_bsp_zone_set);
}

void __cdecl bink_playback_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x00616B90, bink_playback_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

void __cdecl bink_playback_memory_dispose()
{
	INVOKE(0x00616BA0, bink_playback_memory_dispose);
}

void __cdecl bink_playback_memory_initialize(e_map_memory_configuration configuration)
{
	INVOKE(0x00616C00, bink_playback_memory_initialize, configuration);
}

//.text:00616C40 ; void __cdecl bink_playback_prepare_for_game_state_load(int32)

void __cdecl bink_playback_prepare_for_new_zone_set(uns32 old_active_structure_bsp_mask, uns32 new_active_structure_bsp_mask)
{
	INVOKE(0x00616C50, bink_playback_prepare_for_new_zone_set, old_active_structure_bsp_mask, new_active_structure_bsp_mask);
}

void __cdecl bink_playback_prepare_for_non_bsp_zone_set_switch(const s_game_non_bsp_zone_set* old_non_bsp_zone_set, const s_game_non_bsp_zone_set* new_non_bsp_zone_set, c_scenario_resource_registry* active_tags_registry)
{
	INVOKE(0x00616C80, bink_playback_prepare_for_non_bsp_zone_set_switch, old_non_bsp_zone_set, new_non_bsp_zone_set, active_tags_registry);
}

//.text:00616C90 ; void __cdecl bink_playback_release_runtime_memory()

void __cdecl bink_playback_render()
{
	INVOKE(0x00616CC0, bink_playback_render);
}

//.text:00616D00 ; void __cdecl bink_playback_return_memory()

bool __cdecl bink_playback_should_be_playing_deterministic_bink()
{
	return INVOKE(0x00616D50, bink_playback_should_be_playing_deterministic_bink);
}

void __cdecl bink_playback_start(const char* full_pathname, int32 tag_index, uns32 flags)
{
	INVOKE(0x00616D60, bink_playback_start, full_pathname, tag_index, flags);

	//if (!TEST_BIT(flags, _bink_playback_requires_beaver_sized_appetite_bit)
	//	|| bink_globals.use_large_bink_size)
	//{
	//	if (predict_bink_movie_fullpath(full_pathname, tag_index, flags))
	//	{
	//		bink_prediction_update();
	//		bink_globals.bink_runtime.bink_playback_start = 1;
	//		bink_playback_start_internal(flags);
	//	}
	//	else
	//	{
	//		event(_event_error, "bink: failed to predict bink movie '%s' tag_index 0x%08X ('%s')",
	//			full_pathname,
	//			tag_index,
	//			tag_index == NONE ? "" : tag_get_name(tag_index));
	//	}
	//}
	//else
	//{
	//	event(_event_error, "bink: can't play bink movie '%s' tag_index 0x%08X ('%s') - not enough memory",
	//		full_pathname,
	//		tag_index,
	//		tag_index == NONE ? "" : tag_get_name(tag_index));
	//}
}

//.text:00616DB0 ; void __cdecl bink_playback_start_explicit(const char*, int32, uns32, BINK*)
//.text:00616E00 ; void __cdecl bink_playback_start_internal(uns32)
//.text:00616F20 ; void __cdecl bink_playback_start_or_queue(const char*, int32, uns32)

void __cdecl bink_playback_stop()
{
	INVOKE(0x00617040, bink_playback_stop);
}

void __cdecl bink_playback_stop_due_to_input_device_change()
{
	INVOKE(0x00617080, bink_playback_stop_due_to_input_device_change);
}

//.text:006170A0 ; void __cdecl bink_playback_terminate()
//.text:00617280 ; real64 __cdecl bink_playback_time_elapsed_seconds()
//.text:00617310 ; int32 __cdecl bink_playback_time_left_hs_ticks()

bool __cdecl bink_playback_ui_rendering_inhibited()
{
	return INVOKE(0x00617380, bink_playback_ui_rendering_inhibited);

	//return bink_globals.initialized && TEST_BIT(bink_globals.playback_flags, _bink_playback_dont_render_ui_bit);
}

void __cdecl bink_playback_update()
{
	INVOKE(0x006173A0, bink_playback_update);
}

//.text:00617450 ; void __cdecl bink_playback_update_internal(bool)
//.text:00617670 ; bool __cdecl bink_playback_use_large_bink_size()

bool __cdecl bink_playback_using_io_during_map_load_masking()
{
	return INVOKE(0x00617680, bink_playback_using_io_during_map_load_masking);
}

//.text:006176B0 ; void __cdecl bink_playback_wait_for_stop()
//.text:006176D0 ; 
//.text:00617720 ; bink_prediction_async_callback
//.text:00617760 ; bool __cdecl bink_prediction_in_progress_for_movie(const char*, int32)
//.text:006177B0 ; bool __cdecl bink_prediction_in_progress_or_queued()
//.text:006177C0 ; bool __cdecl bink_prediction_queued_and_not_active()
//.text:006177D0 ; void __cdecl bink_prediction_update()
//.text:00617880 ; bool __cdecl bink_query_analog_keyboard_keys() // bink_query_analog_controller_buttons
//.text:006178D0 ; void __cdecl bink_release_textures(int32)
//.text:00617900 ; void __cdecl bink_set_fade(real32)
//.text:00617940 ; void __cdecl bink_set_volume(real32)
//.text:006179B0 ; void __cdecl bink_setup_rasterizer(int32)
//.text:00617B60 ; 
//.text:00617BC0 ; bool __cdecl bink_thread_get_prediction_message(s_bink_runtime*, s_bink_prediction_message*)
//.text:00617C10 ; bool __cdecl bink_thread_should_predict(const s_bink_runtime*)
//.text:00617C20 ; bool __cdecl bink_thread_should_start(const s_bink_runtime*)
//.text:00617C30 ; bool __cdecl bink_thread_should_stop_playback(const s_bink_runtime*)
//.text:00617C40 ; void __cdecl bink_try_to_decompress_frame(s_bink_runtime*, bool)
//.text:00617D40 ; 
//.text:00617DE0 ; 
//.text:00617E70 ; 
//.text:00617EA0 ; public: virtual void __cdecl c_bink_allocation::deallocate(void*)
//.text:00617EB0 ; 
//.text:00617EE0 ; public: virtual void __cdecl c_bink_optional_cache_callback::get_memory_configuration(e_map_memory_configuration, s_optional_cache_user_memory_configuration*)
//.text:00617F60 ; 
//.text:00617FC0 ; 
//.text:00618010 ; 
//.text:00618180 ; bool __cdecl is_all_bink_memory_free()

void __cdecl play_bink_movie(const char* name)
{
	INVOKE(0x006181B0, play_bink_movie, name);
}

//.text:00618260 ; void __cdecl play_bink_movie_from_tag(int32)
//.text:006182F0 ; void __cdecl play_credits_movie_skippable_to_main_menu()
//.text:00618360 ; void __cdecl play_credits_movie_unskippable()
//.text:006183D0 ; void __cdecl predict_bink_movie(const char*)
//.text:00618410 ; void __cdecl predict_bink_movie_from_tag(int32)
//.text:00618430 ; bool __cdecl predict_bink_movie_fullpath(const char*, int32, uns32, BINK*)
//.text:00618500 ; bool __cdecl prepare_bink_movie_for_playback_async(s_bink_runtime*, const s_bink_prediction_message*)
//.text:006187B0 ; void __cdecl process_bink_messages()
//.text:00618810 ; void __cdecl rebase_bink_on_base_address_change(void*, void*)
//.text:00618860 ; 
//.text:00618870 ; void __cdecl release_unused_textures(int32)
//.text:006188B0 ; 
//.text:006188F0 ; 
//.text:00618940 ; 
//.text:00618960 ; public: virtual void __cdecl c_bink_optional_cache_callback::terminate(const void*)

