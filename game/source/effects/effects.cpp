#include "effects/effects.hpp"

#include "cache/cache_files.hpp"
#include "effects/effect_definitions.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/render_debug.hpp"

HOOK_DECLARE(0x005BCF60, effects_render);

bool debug_damage_effects = false;
bool g_debug_effects_full = false;

bool enable_opaque_effect_render_pass = true;
bool enable_transparents_effect_render_pass = true;
bool enable_distortion_effect_render_pass = true;
bool enable_first_person_effect_render_pass = true;

bool effects_render_pass_check(e_effect_pass pass)
{
	bool result = false;
	result |= !enable_opaque_effect_render_pass && pass == _effect_pass_opaque;
	result |= !enable_transparents_effect_render_pass && pass == _effect_pass_transparents;
	result |= !enable_distortion_effect_render_pass && pass == _effect_pass_distortion;
	result |= !enable_first_person_effect_render_pass && pass == _effect_pass_first_person;
	return result;
}

void __cdecl effects_prepare_for_new_zone_set(dword old_structure_bsp_mask, dword new_structure_bsp_mask)
{
	INVOKE(0x00530200, effects_prepare_for_new_zone_set, old_structure_bsp_mask, new_structure_bsp_mask);
}

void __cdecl effects_initialize_for_new_non_bsp_zone_set(s_game_non_bsp_zone_set const* new_non_bsp_zone_set)
{
	INVOKE(0x00530210, effects_initialize_for_new_non_bsp_zone_set, new_non_bsp_zone_set);
}

void __cdecl effects_dispose_from_old_non_bsp_zone_set(s_game_non_bsp_zone_set const* old_non_bsp_zone_set)
{
	INVOKE(0x00530220, effects_dispose_from_old_non_bsp_zone_set, old_non_bsp_zone_set);
}

//.text:005B4620 ; void __cdecl __tls_set_g_effect_counts_allocator(void*)
//.text:005B4640 ; void __cdecl __tls_set_g_effect_data_allocator(void*)
//.text:005B4670 ; void __cdecl __tls_set_g_effect_lightprobe_data_allocator(void*)
//.text:005B4690 ; void __cdecl __tls_set_g_effect_lightprobe_slot_data_allocator(void*)
//.text:005B46C0 ; void __cdecl __tls_set_g_effect_location_data_allocator(void*)
//.text:005B46F0 ; void __cdecl __tls_set_g_effect_message_queue_allocator(void*)
//.text:005B4710 ; void __cdecl __tls_set_g_event_data_allocator(void*)
//.text:005B4740 ; 
//.text:005B4780 ; public: bool __cdecl t_message_queue<s_effect_message, 512>::try_add_message(s_effect_message const*)
//.text:005B47F0 ; 
//.text:005B4800 ; void __cdecl adopt_old_effect_render_message_queue(void*)
//.text:005B4830 ; 
//.text:005B4870 ; 
//.text:005B48B0 ; 
//.text:005B48F0 ; 
//.text:005B4930 ; 
//.text:005B4970 ; 
//.text:005B49B0 ; 
//.text:005B4A00 ; 
//.text:005B4A20 ; void __cdecl build_common_effect_vectors(real_point3d const*, vector3d const*, vector3d const*, s_effect_vector*)
//.text:005B4CF0 ; void __cdecl build_gravity_effect_vectors(real_point3d const*, s_effect_vector* const)
//.text:005B4D50 ; 
//.text:005B4D60 ; 
//.text:005B4D70 ; 

bool __cdecl dangerous_effects_near_player()
{
	return INVOKE(0x005B4DA0, dangerous_effects_near_player);
}

//.text:005B4FB0 ; 
//.text:005B4FE0 ; 
//.text:005B5010 ; 
//.text:005B5040 ; 
//.text:005B5070 ; 
//.text:005B50A0 ; 
//.text:005B50D0 ; 
//.text:005B5100 ; 
//.text:005B5110 ; 
//.text:005B51A0 ; long __cdecl effect_allocate(s_effect_creation_data const*)
//.text:005B53D0 ; long __cdecl effect_allocate_location(effect_datum*, short)
//.text:005B5470 ; 
//.text:005B54B0 ; 
//.text:005B54F0 ; 
//.text:005B5550 ; void __cdecl effect_build_locations(long, long, bool, s_effect_vector const*, long, long, bool)
//.text:005B5A50 ; 
//.text:005B5AA0 ; 
//.text:005B5AC0 ; 
//.text:005B5B10 ; long __cdecl effect_lightprobe_create_from_effect_definition(long)
//.text:005B5BB0 ; void __cdecl effect_creation_data_initialize(s_effect_creation_data*)
//.text:005B5C30 ; void __cdecl effect_creation_data_initialize_from_markers(s_effect_creation_data*, c_tag_index, long, s_effect_vector const*, plane3d const*, s_location const*, e_effect_deterministic)
//.text:005B5C80 ; void __cdecl effect_delete(long)
//.text:005B5D10 ; void __cdecl effect_delete_first_person_locations(effect_datum*)
//.text:005B5DF0 ; void __cdecl effect_delete_internal(long, bool)
//.text:005B5E90 ; void __cdecl effect_delete_locations(long)
//.text:005B5F70 ; void __cdecl effect_delete_without_detaching(long)
//.text:005B6000 ; void __cdecl effect_destroy(long, bool)
//.text:005B6120 ; void __cdecl effect_detach(long)
//.text:005B61A0 ; bool __cdecl effect_filter_message_for_destroy(s_effect_message*, dword)
//.text:005B61C0 ; 
//.text:005B61E0 ; void __cdecl effect_filter_messages_for_destroy(long)
//.text:005B6210 ; 
//.text:005B6240 ; e_effect_reuse_result __cdecl effect_find_reusable_instance(s_effect_creation_data const*, long*)
//.text:005B63A0 ; 

void __cdecl effect_render(long effect_index, long user_index)
{
	INVOKE(0x005B63E0, effect_render, effect_index, user_index);
}

//.text:005B6740 ; event_datum const* __cdecl effect_get_event(effect_datum const*, long)
//.text:005B67A0 ; enum e_event_status __cdecl effect_get_event_status(long, long, word, real*)
//.text:005B68A0 ; bool __cdecl effect_get_first_person(long, long)
//.text:005B68C0 ; real_matrix4x3 const* __cdecl effect_get_first_valid_location(effect_datum const*, real_matrix4x3*)
//.text:005B6980 ; real_point2d const* __cdecl effect_get_impact_size(long)
//.text:005B69B0 ; void __cdecl effect_get_location_from_object(effect_datum*)
//.text:005B6A80 ; long __cdecl effect_get_next_event_index(effect_datum*)
//.text:005B6B40 ; void __cdecl effect_get_node_matrix(effect_datum const*, short, real_matrix4x3*)
//.text:005B6C40 ; void __cdecl effect_get_node_matrix_world_or_local(effect_datum const*, short, real_matrix4x3*, bool)
//.text:005B6CA0 ; 
//.text:005B6CE0 ; void __cdecl effect_get_part_to_node(effect_part_definition const*, real_matrix4x3 const*, real_matrix4x3*)
//.text:005B6D40 ; void __cdecl effect_get_part_to_world(effect_datum const*, effect_part_definition const*, short, real_matrix4x3 const*, real_matrix4x3*)
//.text:005B6E00 ; 
//.text:005B6E30 ; real_matrix4x3 const* __cdecl effect_get_transformed_location(effect_datum const*, effect_location_datum const*, real_matrix4x3*)
//.text:005B6E80 ; real_matrix4x3 const* __cdecl effect_get_transformed_location_world_or_local(effect_datum const*, effect_location_datum const*, real_matrix4x3*, bool)
//.text:005B6F60 ; 
//.text:005B6F90 ; vector3d const* __cdecl effect_get_velocity(long)
//.text:005B6FC0 ; void __cdecl effect_handle_deleted_atmosphere(long)
//.text:005B6FE0 ; void __cdecl effect_handle_deleted_object(long)
//.text:005B7160 ; bool __cdecl effect_in_player_range(s_effect_creation_data const*)
//.text:005B72C0 ; void __cdecl effect_lightprobe_initialize(long, s_effect_creation_data const*)
//.text:005B7370 ; bool __cdecl effect_is_attached_sound_active(long)
//.text:005B73E0 ; bool __cdecl effect_is_looping(effect_datum const*)
//.text:005B7400 ; e_effect_reuse_result __cdecl effect_is_reusable(s_effect_creation_data const*, long)
//.text:005B74E0 ; real_rgb_color const* __cdecl effect_get_diffuse_tint(long)
//.text:005B7530 ; 
//.text:005B7570 ; real_argb_color const __cdecl effect_lightprobe_get_lightmap_tint_rgbe(long)
//.text:005B77E0 ; void __cdecl effect_setup_lighting_constants(long)
//.text:005B7840 ; effect_location_datum* __cdecl effect_location_get_next_instance(effect_datum const*, long*, short)
//.text:005B7890 ; long __cdecl effect_location_get_next_valid_index(effect_datum const*, long, short)
//.text:005B7910 ; 
//.text:005B7930 ; void __cdecl effect_marker_list_get_marker(object_marker*, effect_marker_list const*, short, plane3d const*)
//.text:005B7C30 ; long __cdecl effect_new_attached_from_marker_name(c_tag_index, s_damage_owner const*, s_damage_reporting_info const&, long, short, long, real, real, real_rgb_color const*, effect_vector_field const*, long, e_effect_deterministic)
//.text:005B7D70 ; void __cdecl handle_effect_reverse_message(s_effect_reverse_message const*)
//.text:005B7E20 ; void __cdecl effect_new_attached_from_particle(c_tag_index, long, byte, byte, real_point3d const*, vector3d const*, vector3d const*, real, real, s_location const*, bool)
//.text:005B7EF0 ; long __cdecl effect_new_breakable_surface(c_tag_index, long, real, real)
//.text:005B7FC0 ; long __cdecl effect_new_from_creation_data(s_effect_creation_data const*)
//.text:005B8550 ; long __cdecl effect_new_from_object(c_tag_index, s_damage_owner const*, s_damage_reporting_info const&, long, real, real, real_rgb_color const*, effect_vector_field const*, e_effect_deterministic)
//.text:005B86A0 ; 

//long __cdecl effect_new_from_point_vector(long, real_point3d const*, vector3d const*, vector3d const*, e_match_all_markers, e_effect_deterministic, plane3d const*, s_cluster_reference*)
long __cdecl effect_new_from_point_vector(long effect_index, real_point3d const* position, vector3d const* forward, vector3d const* normal, long match_all_markers, long effect_deterministic, plane3d const* plane, s_cluster_reference* cluster_reference)
{
	return INVOKE(0x005B87F0, effect_new_from_point_vector, effect_index, position, forward, normal, match_all_markers, effect_deterministic, plane, cluster_reference);
}

//.text:005B88E0 ; long __cdecl effect_new_looping(c_tag_index, s_damage_owner const*, s_damage_reporting_info const&, long, long, long, long, short)
//.text:005B89E0 ; long __cdecl effect_new_unattached_from_markers(c_tag_index, s_damage_owner const*, long, vector3d const*, long, s_effect_vector const*, plane3d const*, s_location const*, real, e_effect_deterministic)
//.text:005B8A90 ; long __cdecl effect_new_unattached_from_markers_ex(c_tag_index, s_damage_owner const*, long, vector3d const*, long, s_effect_vector const*, plane3d const*, s_location const*, real, real, real_rgb_color const*, effect_vector_field const*, long, e_effect_deterministic)
//.text:005B8B50 ; long __cdecl effect_new_unattached_from_markers_lite(c_tag_index, s_damage_owner const*, s_damage_reporting_info const&, long, s_effect_vector const*, plane3d const*, s_location const*, e_effect_deterministic)
//.text:005B8BD0 ; long __cdecl effect_new_weather(c_tag_index)
//.text:005B8C80 ; bool __cdecl effect_parent_particle_valid(effect_datum const*)
//.text:005B8CC0 ; 
//.text:005B8CE0 ; 
//.text:005B8D40 ; 
//.text:005B8DA0 ; bool __cdecl effect_ping(long, long, real, real, bool)
//.text:005B8E20 ; 
//.text:005B8ED0 ; bool __cdecl effect_ping_from_impact(long, real, real, real_matrix4x3 const*, vector3d const*, real_point2d const*, plane3d const*)
//.text:005B9100 ; 
//.text:005B9130 ; void __cdecl effect_ping_from_ricochet(long)
//.text:005B9170 ; void __cdecl effect_queue_message(s_effect_message const*)
//.text:005B91F0 ; 
//.text:005B9210 ; void __cdecl effect_random_angular_velocity(char const*, bool, effect_datum const*, vector3d*, real, real, dword, dword)
//.text:005B92F0 ; void __cdecl effect_random_direction3d(char const*, bool, effect_datum const*, vector3d*)
//.text:005B9340 ; void __cdecl effect_random_translational_velocity(char const*, bool, effect_datum const*, vector3d const*, vector3d*, vector3d*, real, real, real, dword, dword)
//.text:005B9500 ; real __cdecl effect_real_random(char const*, bool, effect_datum const*)
//.text:005B9580 ; real __cdecl effect_real_random_range(char const*, bool, effect_datum const*, real, real)
//.text:005B9610 ; real __cdecl effect_real_random_range(char const*, bool, effect_datum const*, real, real, dword, dword, short)
//.text:005B9720 ; void __cdecl effect_refresh_location(long, s_cluster_reference const*)
//.text:005B9760 ; void __cdecl effect_refresh_locations(long)

void __cdecl sub_5B9820(long effect_index, long user_index)
{
	//INVOKE(0x005B9820, sub_5B9820, effect_index, user_index);

	TLS_DATA_GET_VALUE_REFERENCE(effect_data);
	
	effect_datum* effect = (effect_datum*)datum_get(*effect_data, effect_index);
	if (!TEST_BIT(effect->flags, 1) && !TEST_BIT(effect->flags, 2) && !TEST_BIT(effect->flags, 5))
		effect_render(effect_index, user_index);
}

//.text:005B9870 ; void __cdecl effect_restart_all_events(long)
//.text:005B98D0 ; 
//.text:005B9920 ; dword* __cdecl effect_seed(bool, effect_datum const*)
//.text:005B9960 ; void __cdecl effect_send_event_restart_message(long, long)
//.text:005B9A20 ; void __cdecl effect_send_relocated_messages(long, bool)
//.text:005B9CE0 ; void __cdecl effect_send_restart_message(long)
//.text:005B9DA0 ; void __cdecl effect_send_sample_lightmap_message(long)
//.text:005B9E50 ; void __cdecl effect_set_event(long, short)
//.text:005B9EF0 ; void __cdecl effect_setup_tiling_constants(long, real, real)
//.text:005B9F20 ; bool __cdecl effect_should_frame_advance(long)
//.text:005B9F60 ; bool __cdecl effect_should_render(long)
//.text:005B9FA0 ; bool __cdecl effect_should_skip_event(effect_datum const*, long)
//.text:005BA1F0 ; void __cdecl effect_start_event(long)
//.text:005BA400 ; void __cdecl effect_stop(long, bool)
//.text:005BA480 ; 
//.text:005BA490 ; void __cdecl effect_stop_looping_sounds(effect_datum*)
//.text:005BA4C0 ; 
//.text:005BA500 ; void __cdecl effect_update(long, real)
//.text:005BA540 ; bool __cdecl effect_update_attached_sound(long, real_point3d*, vector3d*, s_location*, real*)
//.text:005BA640 ; void __cdecl effect_update_beam_system(c_beam_system*, real)
//.text:005BAAD0 ; void __cdecl effect_update_contrail_locations(effect_datum const*, c_contrail_system*, real)
//.text:005BAD90 ; void __cdecl effect_update_contrail_system(c_contrail_system*, real)
//.text:005BAE40 ; void __cdecl effect_update_light_volume_system(c_light_volume_system*, real)
//.text:005BB070 ; void __cdecl effect_update_lighting_samples(effect_datum const*)
//.text:005BB340 ; bool __cdecl effect_update_object(long)
//.text:005BB530 ; void __cdecl effect_update_parent_particle(effect_datum*)
//.text:005BB6C0 ; void __cdecl effect_update_particle_locations(effect_datum const*, c_particle_system*, real_matrix4x3 const*)
//.text:005BB940 ; void __cdecl effect_update_particle_system(c_particle_system*)
//.text:005BBA90 ; bool __cdecl effect_update_time(long, real)
//.text:005BBC10 ; bool __cdecl effect_update_visibility(long)
//.text:005BBD20 ; 
//.text:005BBDE0 ; void __cdecl effects_assert_no_pending_messages()
//.text:005BBE10 ; 

void __cdecl effects_dispose()
{
	INVOKE(0x005BBE70, effects_dispose);
}

void __cdecl effects_dispose_from_old_map()
{
	INVOKE(0x005BBF10, effects_dispose_from_old_map);
}

void __cdecl effects_dispose_from_old_structure_bsp(dword old_structure_bsp_mask)
{
	INVOKE(0x005BBFD0, effects_dispose_from_old_structure_bsp, old_structure_bsp_mask);
}

void __cdecl effects_frame_advance(real seconds_elapsed)
{
	INVOKE(0x005BBFE0, effects_frame_advance, seconds_elapsed);
}

void __cdecl effects_frame_advance_gpu(real seconds_elapsed)
{
	c_rasterizer_profile_scope _effects_frame_advance(_rasterizer_profile_element_effects, L"effects_frame_advance");

	INVOKE(0x005BC0B0, effects_frame_advance_gpu, seconds_elapsed);
}

//.text:005BC1A0 ; void __cdecl effects_gather_damage_obstacles(c_damage_effect_obstacles*)
//.text:005BC380 ; bool __cdecl effects_get_weather_occlusion_obb(render_camera const*, s_oriented_bounding_box*)
//.text:005BC3E0 ; void __cdecl effects_handle_deleted_object(long)
//.text:005BC5B0 ; 
//.text:005BC5C0 ; void __cdecl effects_information_get(effects_information*, effects_information*)

void __cdecl effects_initialize()
{
	INVOKE(0x005BC740, effects_initialize);
}

void __cdecl effects_initialize_for_new_map()
{
	INVOKE(0x005BC990, effects_initialize_for_new_map);
}

void __cdecl effects_initialize_for_new_structure_bsp(dword new_structure_bsp_mask)
{
	INVOKE(0x005BCA80, effects_initialize_for_new_structure_bsp, new_structure_bsp_mask);
}

//.text:005BCB70 ; void __cdecl effects_notify_first_person_weapon_user_change(long, long, bool)
//.text:005BCD40 ; void __cdecl effects_notify_unit_user_change(long, long, bool)
//.text:005BCDF0 ; 

void __cdecl effects_prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const* old_non_bsp_zone_set, s_game_non_bsp_zone_set const* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry)
{
	INVOKE(0x005BCE50, effects_prepare_for_non_bsp_zone_set_switch, old_non_bsp_zone_set, new_non_bsp_zone_set, pending_zone_registry);
}

//.text:005BCF20 ; void __cdecl effects_process_render_thread_messages()

void __cdecl effects_render(long user_index, e_effect_pass pass)
{
	//INVOKE(0x005BCF60, effects_render, output_user_index, pass);

	if (effects_render_pass_check(pass))
		return;

	if (g_debug_effects_full)
		debug_effects_full();

	c_rasterizer::set_z_buffer_mode(c_rasterizer::e_z_buffer_mode(pass != _effect_pass_opaque));

	if (pass == _effect_pass_transparents)
	{
		TLS_DATA_GET_VALUE_REFERENCE(effect_data);

		for (long effect_index = data_next_index(*effect_data, NONE);
			effect_index != NONE;
			effect_index = data_next_index(*effect_data, effect_index))
		{
			sub_5B9820(effect_index, user_index);
		}
	}

	c_contrail_system::submit_all(user_index, pass);
	c_light_volume_system::submit_all(user_index, pass);
	c_beam_system::submit_all(user_index, pass);
	c_particle_system::submit_all(user_index, pass);

	if (pass == _effect_pass_first_person)
		effects_submit_cheap_first_person_attachments(user_index);
}

//.text:005BD040 ; void __cdecl effects_reset()
//.text:005BD0D0 ; void __cdecl effects_submit_cheap_attachments(long, byte)

void __cdecl effects_submit_cheap_first_person_attachments(long user_index)
{
	INVOKE(0x005BD490, effects_submit_cheap_first_person_attachments, user_index);
}

void __cdecl effects_update()
{
	INVOKE(0x005BD680, effects_update);
}

//.text:005BD8B0 ; 
//.text:005BD8C0 ; void __cdecl event_add_beam_system_location(s_effect_message const*)
//.text:005BD930 ; void __cdecl event_add_contrail_system_location(s_effect_message const*)
//.text:005BD980 ; void __cdecl event_add_light_volume_system_location(s_effect_message const*)
//.text:005BD9C0 ; void __cdecl event_add_particle_system_location(s_effect_message const*)
//.text:005BDA30 ; long __cdecl event_allocate(effect_definition const*)
//.text:005BDB80 ; void __cdecl event_create_beam_system(s_effect_message const*)
//.text:005BDC50 ; void __cdecl event_create_contrail_system(s_effect_message const*)
//.text:005BDD30 ; void __cdecl event_create_decal_system(s_effect_message const*)
//.text:005BDD60 ; void __cdecl event_create_light_volume_system(s_effect_message const*)
//.text:005BDE30 ; void __cdecl event_create_particle_system(s_effect_message const*)
//.text:005BDF40 ; void __cdecl event_create_particle_systems(long, long)
//.text:005BE530 ; void __cdecl event_destroy(long, bool)
//.text:005BE5A0 ; void __cdecl event_generate_accelerations(effect_datum*, long, real)
//.text:005BE930 ; void __cdecl event_generate_part(long, long, byte*, effect_part_definition const*, long, bool, real_matrix4x3 const*, real)
//.text:005BF2A0 ; void __cdecl event_generate_parts(long, long, bool)
//.text:005BF670 ; void __cdecl event_initialize_beam_system(s_effect_message const*)
//.text:005BF6C0 ; void __cdecl event_initialize_contrail_system(s_effect_message const*)
//.text:005BF730 ; void __cdecl event_initialize_light_volume_system(s_effect_message const*)
//.text:005BF780 ; void __cdecl event_initialize_particle_system(s_effect_message const*)
//.text:005BF7C0 ; void __cdecl event_request_beam_system(long, long, effect_part_definition const*, long)
//.text:005BF8B0 ; void __cdecl event_request_beam_system_initialization(long, long, effect_part_definition const*)
//.text:005BF940 ; void __cdecl event_request_beam_system_location(long, long, effect_part_definition const*, real_matrix4x3 const*, bool)
//.text:005BFC70 ; void __cdecl event_request_contrail_system(long, long, effect_part_definition const*, long)
//.text:005BFD70 ; void __cdecl event_request_contrail_system_initialization(long, long, effect_part_definition const*)
//.text:005BFE00 ; void __cdecl event_request_contrail_system_location(long, long, effect_part_definition const*, real_matrix4x3 const*, bool)
//.text:005C0130 ; void __cdecl event_request_decal_system(long, long, effect_part_definition const*, real_matrix4x3 const*)
//.text:005C03E0 ; void __cdecl event_request_light_volume_system(long, long, effect_part_definition const*, long)
//.text:005C04D0 ; void __cdecl event_request_light_volume_system_initialization(long, long, effect_part_definition const*)
//.text:005C0560 ; void __cdecl event_request_light_volume_system_location(long, long, effect_part_definition const*, real_matrix4x3 const*, bool)
//.text:005C0890 ; bool __cdecl event_update_time(long, long, real)
//.text:005C0A00 ; bool __cdecl events_create(long)
//.text:005C0A50 ; 
//.text:005C0A90 ; public: void __cdecl t_message_queue<s_effect_message, 512>::filter_messages(bool(__cdecl*)(s_effect_message*, long), long)
//.text:005C0B20 ; 
//.text:005C0B60 ; 
//.text:005C0B70 ; 
//.text:005C0B80 ; 
//.text:005C0B90 ; 
//.text:005C0BA0 ; 
//.text:005C0BB0 ; 
//.text:005C0BC0 ; 
//.text:005C0BD0 ; 
//.text:005C0BE0 ; 
//.text:005C0BF0 ; 
//.text:005C0C20 ; 
//.text:005C0C50 ; 
//.text:005C0C80 ; 
//.text:005C0CB0 ; 
//.text:005C0CE0 ; 
//.text:005C0D10 ; 
//.text:005C0D40 ; 
//.text:005C0D50 ; 
//.text:005C0D90 ; 
//.text:005C0DA0 ; public: static c_beam_location* __cdecl c_beam_location::get(long)
//.text:005C0DD0 ; public: static c_beam_system* __cdecl c_beam_system::get(long)
//.text:005C0E00 ; public: static c_contrail_location* __cdecl c_contrail_location::get(long)
//.text:005C0E30 ; public: static c_contrail_system* __cdecl c_contrail_system::get(long)
//.text:005C0E60 ; public: static c_light_volume_location* __cdecl c_light_volume_location::get(long)
//.text:005C0E90 ; public: static c_light_volume_system* __cdecl c_light_volume_system::get(long)
//.text:005C0EC0 ; public: static c_particle_location* __cdecl c_particle_location::get(long)
//.text:005C0EF0 ; public: static c_particle_system* __cdecl c_particle_system::get(long)
//.text:005C0F20 ; public: static s_camo_effect_definition const* __cdecl s_camo_effect_definition::get(long)
//.text:005C0F40 ; 
//.text:005C0F50 ; 
//.text:005C0F60 ; 
//.text:005C0F70 ; 
//.text:005C0F80 ; 
//.text:005C0F90 ; 
//.text:005C0FA0 ; 
//.text:005C0FB0 ; 
//.text:005C0FC0 ; 
//.text:005C0FD0 ; 
//.text:005C0FE0 ; 
//.text:005C0FF0 ; 
//.text:005C1000 ; 
//.text:005C1010 ; 
//.text:005C1020 ; 
//.text:005C1030 ; 
//.text:005C1040 ; 
//.text:005C1050 ; 
//.text:005C1060 ; 
//.text:005C1070 ; 
//.text:005C1080 ; 
//.text:005C1090 ; 
//.text:005C10A0 ; 
//.text:005C10B0 ; 
//.text:005C10C0 ; 
//.text:005C10D0 ; 
//.text:005C10E0 ; 
//.text:005C10F0 ; 
//.text:005C1100 ; 
//.text:005C1110 ; 
//.text:005C1120 ; 
//.text:005C1130 ; 
//.text:005C1140 ; 
//.text:005C1150 ; 
//.text:005C1160 ; void __cdecl handle_effect_render_message(s_effect_message const*, dword)
//.text:005C1290 ; bool __cdecl handle_effect_render_message_by_asserting(s_effect_message*, dword)
//.text:005C12A0 ; 
//.text:005C12B0 ; void __cdecl handle_effect_reverse_messages()
//.text:005C1300 ; 
//.text:005C1320 ; 
//.text:005C13A0 ; 
//.text:005C13C0 ; bool __cdecl object_should_create_effects(long, long)
//.text:005C1410 ; 
//.text:005C1420 ; public: void __cdecl t_message_queue<s_effect_message, 512>::process_messages(void (__cdecl*)(s_effect_message*, long), long)
//.text:005C1480 ; void __cdecl process_relocated_location(s_effect_message const*)
//.text:005C14D0 ; void __cdecl process_restart_effect(s_effect_message const*)
//.text:005C1510 ; void __cdecl process_restart_event(s_effect_message const*)
//.text:005C1560 ; void __cdecl process_sample_lightmap(s_effect_message const*)
//.text:005C15B0 ; 
//.text:005C15C0 ; 
//.text:005C15E0 ; 
//.text:005C1600 ; 
//.text:005C1620 ; 
//.text:005C1630 ; 
//.text:005C1670 ; 
//.text:005C16B0 ; 
//.text:005C16F0 ; 
//.text:005C1730 ; 
//.text:005C1770 ; 
//.text:005C17B0 ; 
//.text:005C1800 ; public: void __cdecl t_event_queue<s_effect_reverse_message, 128>::reset()
//.text:005C1840 ; void __cdecl reset_effect_render_message_queue()
//.text:005C18B0 ; 
//.text:005C18E0 ; 
//.text:005C1910 ; 
//.text:005C1930 ; 
//.text:005C1950 ; 
//.text:005C1990 ; 
//.text:005C19C0 ; 
//.text:005C19F0 ; 
//.text:005C1A40 ; 
//.text:005C1A50 ; 
//.text:005C1A70 ; 
//.text:005C1A90 ; 
//.text:005C1AB0 ; 
//.text:005C1AC0 ; 
//.text:005C1AD0 ; 
//.text:005C1AE0 ; 
//.text:005C1AF0 ; 
//.text:005C1B10 ; 
//.text:005C1B30 ; 
//.text:005C1B50 ; 
//.text:005C1B70 ; 
//.text:005C1B90 ; 
//.text:005C1BB0 ; 
//.text:005C1BD0 ; 
//.text:005C1BF0 ; 
//.text:005C1C10 ; 
//.text:005C1C30 ; 
//.text:005C1C50 ; 
//.text:005C1C60 ; 
//.text:005C1C70 ; 
//.text:005C1C80 ; 
//.text:005C1CA0 ; 
//.text:005C1CD0 ; 
//.text:005C1CF0 ; 
//.text:005C1D00 ; 
//.text:005C1D10 ; 
//.text:005C1D30 ; 
//.text:005C1D40 ; 
//.text:005C1D60 ; 
//.text:005C1D80 ; 
//.text:005C1D90 ; 
//.text:005C1DA0 ; 
//.text:005C1DB0 ; 
//.text:005C1DD0 ; 
//.text:005C1DF0 ; 
//.text:005C1E10 ; 
//.text:005C1E30 ; 
//.text:005C1E50 ; 
//.text:005C1E70 ; 
//.text:005C1E90 ; 
//.text:005C1EB0 ; 
//.text:005C1ED0 ; 
//.text:005C1F00 ; 
//.text:005C1F20 ; 
//.text:005C1F30 ; 
//.text:005C1F60 ; public: static c_particle* __cdecl c_particle::try_get(long)
//.text:005C1F90 ; 
//.text:005C1FA0 ; 
//.text:005C20A0 ; 
//.text:005C2110 ; bool __cdecl user_is_first_person(long)
//.text:005C2140 ; 
//.text:005C2150 ; 
//.text:005C2160 ; 

void render_debug_damage_effects()
{
	if (debug_damage_effects)
	{

	}
}

void debug_effects_full()
{
	TLS_DATA_GET_VALUE_REFERENCE(effect_data);
	TLS_DATA_GET_VALUE_REFERENCE(event_data);

	c_static_string<8192> string;
	string.print("|n|n|n|n|nlayer  index sort  alpha  name|n");

	for (long effect_index = data_next_index(*effect_data, NONE); effect_index != NONE; effect_index = data_next_index(*effect_data, effect_index))
	{
		effect_datum* effect = (effect_datum*)datum_get(*effect_data, effect_index);
		struct effect_definition* effect_definition = (struct effect_definition*)tag_get(EFFECT_TAG, effect->definition_index);

		c_static_string<256> effect_string;
		effect_string.print("effect: %s (%s %s %s)|n",
			tag_get_name(effect->definition_index),
			TEST_BIT(effect->flags, 1) ? "stopped" : "running",
			TEST_BIT(effect->flags, 2) ? "invisible" : "visible",
			TEST_BIT(effect->flags, 5) ? "deleted" : "");
		string.append(effect_string.get_string());

		if (!TEST_BIT(effect->flags, 1) && !TEST_BIT(effect->flags, 2) && !TEST_BIT(effect->flags, 5))
		{
			event_datum* _event = NULL;
			for (long event_index = effect->event_datum_head; event_index != NONE; event_index = _event->next_event_index)
			{
				_event = (event_datum*)datum_get(*event_data, event_index);
				effect_event_definition& effect_event = effect_definition->events[_event->event_block_index];

				real duration = _event->duration;
				real age = _event->time;
				if (duration != 0.0f)
					age /= duration;

				c_static_string<256> effect_event_string;
				effect_event_string.print("  event: %s (%s, %s, %d parts, age %f)|n",
					effect_event.event_name.get_string(),
					TEST_BIT(_event->flags, 0) ? "actual" : "delay",
					TEST_BIT(_event->flags, 1) ? "stopped" : "running",
					effect_event.parts.count,
					age);
				string.append(effect_event_string.get_string());

				for (long part_index = 0; part_index < effect_event.parts.count; part_index++)
				{
					effect_part_definition& effect_part = effect_event.parts[part_index];

					c_static_string<256> effect_part_string;
					effect_part_string.print("    part %d: (%s)|n",
						part_index,
						effect_part.type.index == NONE ? "<none>" : effect_part.type.get_name());
					string.append(effect_part_string.get_string());
				}
			}
		}
	}

	render_debug_string(string.get_string());
}

