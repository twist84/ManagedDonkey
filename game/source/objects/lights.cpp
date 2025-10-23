#include "objects/lights.hpp"

#include "memory/thread_local.hpp"

//.text:00B9A780 ; 
//.text:00B9A7A0 ; 
//.text:00B9A7C0 ; 
//.text:00B9A7E0 ; 
//.text:00B9A800 ; 
//.text:00B9A820 ; 
//.text:00B9A840 ; 
//.text:00B9A850 ; 
//.text:00B9A860 ; 
//.text:00B9A870 ; 
//.text:00B9A880 ; 
//.text:00B9A890 ; 
//.text:00B9A8A0 ; void __cdecl __tls_set_g_light_cluster_reference_allocator(void*)
//.text:00B9A8D0 ; void __cdecl __tls_set_g_light_data_allocator(void*)
//.text:00B9A900 ; void __cdecl __tls_set_g_light_data_reference_allocator(void*)
//.text:00B9A930 ; void __cdecl __tls_set_g_light_first_data_allocator(void*)
//.text:00B9A950 ; void __cdecl __tls_set_g_light_render_data_allocator(void*)
//.text:00B9A970 ; void __cdecl __tls_set_g_lights_game_globals_allocator(void*)
//.text:00B9A990 ; tls
//.text:00B9A9D0 ; tls
//.text:00B9AA10 ; tls
//.text:00B9AA50 ; tls
//.text:00B9AA90 ; tls
//.text:00B9AAD0 ; tls
//.text:00B9AB10 ; 
//.text:00B9AB30 ; 
//.text:00B9AB40 ; int32 __cdecl cluster_get_first_light(int32*, s_cluster_reference)
//.text:00B9AB70 ; int32 __cdecl cluster_get_next_light(int32*)
//.text:00B9ABA0 ; tls
//.text:00B9ABD0 ; tls
//.text:00B9AC00 ; tls
//.text:00B9AC30 ; tls
//.text:00B9AC60 ; tls
//.text:00B9AC90 ; tls
//.text:00B9ACC0 ; void __cdecl force_orthogonal(const real_vector3d*, real_vector3d*)
//.text:00B9AEA0 ; tls
//.text:00B9AED0 ; tls
//.text:00B9AF00 ; tls
//.text:00B9AF30 ; tls
//.text:00B9AF60 ; tls
//.text:00B9AF90 ; tls
//.text:00B9AFC0 ; bool __cdecl game_can_use_flashlights()
//.text:00B9AFE0 ; void __cdecl game_can_use_flashlights_set(bool)
//.text:00B9B030 ; real64 __cdecl game_flashlight_magnitude()
//.text:00B9B0D0 ; 
//.text:00B9B0E0 ; 
//.text:00B9B0F0 ; s_nondeterministic_light_data* __cdecl get_nondeterministic_light_data(int32)
//.text:00B9B120 ; bool __cdecl light_adjust_position_from_attachment(int32)
//.text:00B9B2B0 ; bool __cdecl light_adjust_position_from_attachment_last_minute(light_datum*, const render_camera*)
//.text:00B9B3D0 ; void __cdecl light_build_intermediate_geometry(const real_point3d*, const real_point3d*, const real_vector3d*, const real_vector3d*, real32, int32, light_geometry*)
//.text:00B9B6F0 ; bool __cdecl light_build_render_geometry(int32, light_geometry*)
//.text:00B9B7C0 ; void __cdecl light_build_visibility_projections(const real_vector3d*, const real_vector3d*, const light_geometry*, visibility_projection*, int16*)
//.text:00B9BFD0 ; void __cdecl light_calculate_lod_bounding_sphere(int32)
//.text:00B9C080 ; void __cdecl light_delete(int32)
//.text:00B9C0A0 ; void __cdecl light_delete_internal(int32, bool)
//.text:00B9C1E0 ; void __cdecl light_deleted_from_scenario(int32)
//.text:00B9C260 ; void __cdecl light_disconnect_from_map(int32)
//.text:00B9C2B0 ; bool __cdecl light_find_valid_point(const real_point3d*, const real_point3d*, real_point3d*, real32*)
//.text:00B9C490 ; void __cdecl light_get_bounding_sphere(int32, real_point3d*, real32*)
//.text:00B9C4E0 ; bool __cdecl light_get_frustum_points(int32, real_point3d*, int32)
//.text:00B9C8F0 ; void __cdecl light_get_geometry(int32, light_geometry*)
//.text:00B9C9C0 ; void __cdecl light_get_geometry_from_definition(const light_definition*, light_geometry*)
//.text:00B9CA30 ; void __cdecl light_get_geometry_from_scenario(const light_definition*, const s_scenario_light*, light_geometry*)
//.text:00B9CAB0 ; void __cdecl light_get_lod_sphere(int32, real_point3d*, real32*)
//.text:00B9CB00 ; int32 __cdecl light_index_from_name_index(int16)
//.text:00B9CBA0 ; bool __cdecl light_is_connected_to_map(int32)
//.text:00B9CBE0 ; bool __cdecl light_is_static_light(int32)
//.text:00B9CC50 ; bool __cdecl light_mark(int32)
//.text:00B9CC90 ; void __cdecl light_marker_begin()
//.text:00B9CCA0 ; void __cdecl light_marker_end()
//.text:00B9CCB0 ; int32 __cdecl light_new_attached(int32, int32, int16, int32, int16)
//.text:00B9CD80 ; int32 __cdecl light_new_from_scenario(int32, int32, bool, const s_scenario_light*)
//.text:00B9D120 ; int32 __cdecl light_new_unattached(int32, int32, int16, const real_point3d*, const real_vector3d*, const real_vector3d*, real32)
//.text:00B9D300 ; real32 __cdecl light_prepare_for_window(int32, int32, real_matrix4x3*, real32)
//.text:00B9D4B0 ; void __cdecl light_reconnect_to_map(int32)
//.text:00B9D6C0 ; void __cdecl light_set_active_bit(int32, bool)
//.text:00B9D740 ; void __cdecl light_submit_lens_flares(int32, int32)
//.text:00B9D920 ; bool __cdecl light_theoretically_produces_light(int32, int32)
//.text:00B9DAE0 ; bool __cdecl light_unmarked(int32)
//.text:00B9DB10 ; void __cdecl light_update_bounding_sphere(int32)
//.text:00B9DEC0 ; bool __cdecl light_update_from_scenario(int32)
//.text:00B9DF80 ; void __cdecl light_update_state(int32)
//.text:00B9E370 ; void __cdecl lights_add_structure_bsp_marker_lights(int32)
//.text:00B9E450 ; void __cdecl lights_delete_all()
//.text:00B9E4C0 ; void __cdecl lights_dispose()
//.text:00B9E4E0 ; void __cdecl lights_dispose_from_old_map()
//.text:00B9E510 ; void __cdecl lights_dispose_from_old_structure_bsp(uns32)
//.text:00B9E5D0 ; void __cdecl lights_dispose_structure_bsp_marker_lights(int32)

bool __cdecl lights_enable(bool enable)
{
	//return INVOKE(0x00B9E650, lights_enable, enable);

	lights_game_globals->render_lights = enable;
	return enable;
}

//.text:00B9E670 ; s_data_array* __cdecl lights_get_data_array()
//.text:00B9E690 ; int32 __cdecl lights_get_static_light_count()
//.text:00B9E740 ; void __cdecl lights_handle_deleted_object(int32)
//.text:00B9E7D0 ; void __cdecl lights_initialize()
//.text:00B9E8C0 ; void __cdecl lights_initialize_for_new_map()
//.text:00B9E920 ; void __cdecl lights_initialize_for_new_structure_bsp(uns32)
//.text:00B9E9B0 ; int32 __cdecl lights_iterate_first_index()
//.text:00B9E9D0 ; int32 __cdecl lights_iterate_next_index(int32)
//.text:00B9EA00 ; void __cdecl lights_place(int32)

void __cdecl lights_prepare_for_window(int32 player_window_index)
{
	INVOKE(0x00B9EA80, lights_prepare_for_window, player_window_index);
}

//.text:00B9EB20 ; int32 __cdecl lights_unattached_connect_to_object(int32, bool, bool)

void __cdecl lights_update()
{
	INVOKE(0x00B9EBE0, lights_update);
}

//.text:00B9ED30 ; 
//.text:00B9ED50 ; tls
//.text:00B9ED90 ; tls
//.text:00B9EDD0 ; tls
//.text:00B9EE10 ; tls
//.text:00B9EE50 ; tls
//.text:00B9EE90 ; tls
//.text:00B9EED0 ; int32 __cdecl scenario_light_place(int32, bool)
//.text:00B9EF30 ; 
//.text:00B9EF60 ; 
//.text:00B9EFA0 ; 
//.text:00B9F070 ; 

