#include "objects/lights.hpp"

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
//.text:00B9A8A0 ; 
//.text:00B9A8D0 ; 
//.text:00B9A900 ; 
//.text:00B9A930 ; 
//.text:00B9A950 ; 
//.text:00B9A970 ; 
//.text:00B9A990 ; 
//.text:00B9A9D0 ; 
//.text:00B9AA10 ; 
//.text:00B9AA50 ; 
//.text:00B9AA90 ; 
//.text:00B9AAD0 ; 
//.text:00B9AB10 ; 
//.text:00B9AB30 ; 
//.text:00B9AB40 ; long __cdecl cluster_get_first_light(long*, s_cluster_reference)
//.text:00B9AB70 ; long __cdecl cluster_get_next_light(long*)
//.text:00B9ABA0 ; 
//.text:00B9ABD0 ; 
//.text:00B9AC00 ; 
//.text:00B9AC30 ; 
//.text:00B9AC60 ; 
//.text:00B9AC90 ; 
//.text:00B9ACC0 ; 
//.text:00B9AEA0 ; 
//.text:00B9AED0 ; 
//.text:00B9AF00 ; 
//.text:00B9AF30 ; 
//.text:00B9AF60 ; 
//.text:00B9AF90 ; 
//.text:00B9AFC0 ; bool __cdecl game_can_use_flashlights()
//.text:00B9AFE0 ; void __cdecl game_can_use_flashlights_set(bool)
//.text:00B9B030 ; double __cdecl game_flashlight_magnitude()
//.text:00B9B0D0 ; 
//.text:00B9B0E0 ; 
//.text:00B9B0F0 ; 
//.text:00B9B120 ; 
//.text:00B9B2B0 ; bool __cdecl light_adjust_position_from_attachment_last_minute(light_datum*, render_camera const*)
//.text:00B9B3D0 ; void __cdecl light_build_intermediate_geometry(real_point3d const*, real_point3d const*, vector3d const*, vector3d const*, real, long, light_geometry*)
//.text:00B9B6F0 ; bool __cdecl light_build_render_geometry(long, light_geometry*)
//.text:00B9B7C0 ; 
//.text:00B9BFD0 ; 
//.text:00B9C080 ; void __cdecl light_delete(long)
//.text:00B9C0A0 ; void __cdecl light_delete_internal(long, bool)
//.text:00B9C1E0 ; 
//.text:00B9C260 ; void __cdecl light_disconnect_from_map(long)
//.text:00B9C2B0 ; bool __cdecl light_find_valid_point(real_point3d const*, real_point3d const*, real_point3d*, real*)
//.text:00B9C490 ; void __cdecl light_get_bounding_sphere(long, real_point3d*, real*)
//.text:00B9C4E0 ; 
//.text:00B9C8F0 ; void __cdecl light_get_geometry(long, light_geometry*)
//.text:00B9C9C0 ; 
//.text:00B9CA30 ; 
//.text:00B9CAB0 ; 
//.text:00B9CB00 ; 
//.text:00B9CBA0 ; 
//.text:00B9CBE0 ; 
//.text:00B9CC50 ; 
//.text:00B9CC90 ; void __cdecl light_marker_begin()
//.text:00B9CCA0 ; void __cdecl light_marker_end()
//.text:00B9CCB0 ; long __cdecl light_new_attached(long, long, short, long, short)
//.text:00B9CD80 ; 
//.text:00B9D120 ; long __cdecl light_new_unattached(long, long, short, real_point3d const*, vector3d const*, vector3d const*, real)
//.text:00B9D300 ; 
//.text:00B9D4B0 ; void __cdecl light_reconnect_to_map(long)
//.text:00B9D6C0 ; 
//.text:00B9D740 ; void __cdecl light_submit_lens_flares(long, e_output_user_index)
//.text:00B9D920 ; bool __cdecl light_theoretically_produces_light(long, long)
//.text:00B9DAE0 ; bool __cdecl light_unmarked(long)
//.text:00B9DB10 ; 
//.text:00B9DEC0 ; 
//.text:00B9DF80 ; void __cdecl light_update_state(long)
//.text:00B9E370 ; 
//.text:00B9E450 ; void __cdecl lights_delete_all()
//.text:00B9E4C0 ; 
//.text:00B9E4E0 ; 
//.text:00B9E510 ; 
//.text:00B9E5D0 ; 
//.text:00B9E650 ; 
//.text:00B9E670 ; 
//.text:00B9E690 ; 
//.text:00B9E740 ; void __cdecl lights_handle_deleted_object(long)
//.text:00B9E7D0 ; void __cdecl lights_initialize()
//.text:00B9E8C0 ; void __cdecl lights_initialize_for_new_map()
//.text:00B9E920 ; void __cdecl lights_initialize_for_new_structure_bsp(dword)
//.text:00B9E9B0 ; 
//.text:00B9E9D0 ; 
//.text:00B9EA00 ; void __cdecl lights_place(long)
//.text:00B9EA80 ; void __cdecl lights_prepare_for_window(long)
//.text:00B9EB20 ; long __cdecl lights_unattached_connect_to_object(long, bool, bool)

void __cdecl lights_update()
{
	INVOKE(0x00B9EBE0, lights_update);
}

//.text:00B9ED30 ; 
//.text:00B9ED50 ; 
//.text:00B9ED90 ; 
//.text:00B9EDD0 ; 
//.text:00B9EE10 ; 
//.text:00B9EE50 ; 
//.text:00B9EE90 ; 
//.text:00B9EED0 ; 
//.text:00B9EF30 ; 
//.text:00B9EF60 ; 
//.text:00B9EFA0 ; 
//.text:00B9F070 ; 

