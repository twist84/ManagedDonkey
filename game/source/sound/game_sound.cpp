#include "sound/game_sound.hpp"

#include "cache/cache_files.hpp"
#include "sound/sound_definitions.hpp"
#include "tag_files/tag_groups.hpp"

//.text:005D7920 ; int32 __cdecl game_looping_sound_attachment_new(int32, int32, int32, int32)
//.text:005D79D0 ; void __cdecl game_looping_sound_delete(int32, int32)
//.text:005D7A00 ; void __cdecl game_looping_sound_get_parameters(int32, s_game_looping_sound_parameters*)
//.text:005D7A40 ; bool __cdecl game_looping_sound_get_source(int32, s_sound_source*)
//.text:005D7F00 ; 
//.text:005D7F60 ; int32 __cdecl game_portal_get_door_occluder_index(int32, int32)
//.text:005D7FA0 ; void __cdecl game_sound_calculate_azimuth_image(const s_sound_listener*, const real_point3d*, structure_bsp*, int32, int32, real32, bool, s_sound_azimuth_image*)
//.text:005D8370 ; void __cdecl game_sound_clear(int32)
//.text:005D8410 ; void __cdecl game_sound_compute_cluster_pas(int32, c_static_flags<256>*)
//.text:005D85D0 ; void __cdecl game_sound_disable_at_game_finish()

void __cdecl game_sound_dispose()
{
	INVOKE(0x005D86D0, game_sound_dispose);
}

void __cdecl game_sound_dispose_from_old_map()
{
	INVOKE(0x005D8700, game_sound_dispose_from_old_map);
}

void __cdecl game_sound_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x005D8750, game_sound_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

//.text:00677EE0 ; real32 __cdecl game_sound_distance_to_portal(const structure_bsp*, const cluster_portal*, const real_point3d*)
//.text:005D8770 ; void __cdecl game_sound_fade_in_at_game_begin()
//.text:005D87E0 ; s_portal_reference __cdecl game_sound_find_closest_portal(const real_point3d*, s_cluster_reference, real32*)
//.text:005D8950 ; s_portal_reference __cdecl game_sound_find_closest_portal_directed(const real_point3d*, const real_vector3d*, s_cluster_reference, real32*)
//.text:005D8B90 ; real32 __cdecl game_sound_get_cluster_distance(s_cluster_reference, s_cluster_reference)
//.text:005D8BD0 ; real32 __cdecl game_sound_get_door_occlusion(int32, int32)
//.text:005D8C50 ; real32 __cdecl game_sound_get_fixed_fadeout_time()
//.text:005D8CA0 ; int32 __cdecl game_sound_get_loop_identifier(int32)
//.text:005D8D00 ; void __cdecl game_sound_get_object_velocity(int32, real_vector3d*)
//.text:005D8DA0 ; int32 __cdecl game_sound_get_playing_time(int32)
//.text:005D8DE0 ; int32 __cdecl game_sound_get_scripting_time(int32)
//.text:005D8E20 ; void __cdecl game_sound_handle_tag_reloads()
//.text:005D8EC0 ; int32 __cdecl game_sound_impulse_new_internal(int32, const s_game_sound_impulse_internal_parameters*)
//.text:005D9110 ; game_sound_impulse_stop?
//.text:005D9120 ; void __cdecl game_sound_scripted_impulses_update()

void __cdecl game_sound_initialize()
{
	INVOKE(0x005D92B0, game_sound_initialize);
}

void __cdecl game_sound_initialize_for_new_map()
{
	INVOKE(0x005D93D0, game_sound_initialize_for_new_map);
}

void __cdecl game_sound_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x005D9570, game_sound_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:005D95F0 ; void __cdecl game_sound_play_fog_event(int32, int32)

void __cdecl game_sound_process_update_messages()
{
	INVOKE(0x005D9630, game_sound_process_update_messages);
}

//.text:005D9690 ; uns32 __cdecl game_sound_quantize_normal3d(const real_vector3d*)
//.text:005D9770 ; void __cdecl game_sound_reset_at_round_finish()
//.text:005D9780 ; void __cdecl game_sound_reset_scripted_dialog_timer()
//.text:005D97A0 ; void __cdecl game_sound_restore(int32)
//.text:005D9A60 ; void __cdecl game_sound_set_door_occlusion(int32, int32, real32)
//.text:005D9AE0 ; void __cdecl game_sound_set_lipsync_data(int32, int32, const s_cache_file_sound_permutation*, real32)
//.text:005D9B50 ; void __cdecl game_sound_set_mouth_aperture(int32, real32)

void __cdecl game_sound_update(real32 game_seconds_elapsed)
{
	INVOKE(0x005D9BA0, game_sound_update, game_seconds_elapsed);
}

//.text:005D9F20 ; void __cdecl game_sound_update_background_sound(const s_cluster_reference&, int32, const real_matrix4x3*, int32, real32, real32)
//.text:005DAB20 ; void __cdecl game_sound_update_listeners(real32)
//.text:005DB160 ; int32 __cdecl generate_game_looping_sound_controller_identifier(int32, int32)

int32 __cdecl object_impulse_sound_new(int32 object_index, int32 definition_index, int16 node_index, const real_point3d* position, const real_vector3d* forward, real32 scale)
{
	return INVOKE(0x005DBBA0, object_impulse_sound_new, object_index, definition_index, node_index, position, forward, scale);
}

//.text:005DC410 ; void __cdecl scripted_looping_sound_set_alternate(int32, bool)
//.text:005DC470 ; void __cdecl scripted_looping_sound_set_scale(int32, real32)
//.text:005DC4D0 ; void __cdecl scripted_looping_sound_start(int32, int32, real32)

void __cdecl scripted_looping_sound_start_with_effect(int32 definition_index, int32 source_object_index, real32 scale, int32 playback_label)
{
	INVOKE(0x005DC500, scripted_looping_sound_start_with_effect, definition_index, source_object_index, scale, playback_label);
}

//.text:005DC530 ; void __cdecl scripted_looping_sound_start_with_effect_internal(int32, int32, real32, int32, bool)
//.text:005DC670 ; void __cdecl scripted_looping_sound_stop(int32)
//.text:005DC690 ; void __cdecl scripted_looping_sound_stop_immediately(int32)
//.text:005DC6B0 ; void __cdecl scripted_looping_sound_stop_internal(int32, e_looping_sound_fadeout_type)
//.text:005DC770 ; void __cdecl scripted_looping_sound_stop_music_internal(game_looping_sound_datum*, e_looping_sound_fadeout_type)
//.text:005DC830 ; void __cdecl scripted_music_stop_all()

void __cdecl scripting_looping_sound_spam()
{
	tag_iterator loop_iterator{};
	tag_iterator_new(&loop_iterator, SOUND_LOOPING_TAG);

	for (int32 loop_tag_index = tag_iterator_next(&loop_iterator); loop_tag_index != NONE; loop_tag_index = tag_iterator_next(&loop_iterator))
	{
		struct looping_sound_definition* looping_sound_definition = TAG_GET(SOUND_LOOPING_TAG, struct looping_sound_definition, loop_tag_index);
		if (TEST_BIT(looping_sound_definition->flags, _looping_sound_behave_like_impulse_sound_bit))
		{
			continue;
		}

		scripted_looping_sound_start_with_effect(loop_tag_index, NONE, 1.0f, 0);
	}
}

//.text:005DDFB0 ; int32 __cdecl unattached_impulse_sound_new(int32, const s_sound_location*, real32)
//.text:005DE0D0 ; real32 __cdecl unattached_looping_sound_get_amplitude(int32)
//.text:005DE1C0 ; void __cdecl unattached_looping_sound_set_scale(int32, real32)
//.text:005DE1F0 ; int32 __cdecl unattached_looping_sound_start(int32, int32, real32)
//.text:005DE250 ; int32 __cdecl unattached_looping_sound_start_with_effect(int32, int32, real32, int32)
//.text:005DE2D0 ; void __cdecl unattached_looping_sound_stop(int32)

int32 __cdecl unspatialized_impulse_sound_new(int32 definition_index, real32 scale)
{
	return INVOKE(0x005DE300, unspatialized_impulse_sound_new, definition_index, scale);
}

//.text:005DE360 ; void __cdecl update_potentially_audible_looping_sound(int32)
//.text:005DE810 ; void __cdecl update_spatialization_if_first_person(int32, int8*, int32*)
//.text:005DE8A0 ; void __cdecl update_unit_impulse_sound(int32, int32, int32, int32, real32)

