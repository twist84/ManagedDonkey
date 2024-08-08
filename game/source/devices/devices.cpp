#include "devices/devices.hpp"

//.text:00B92160 ; 
//.text:00B92190 ; 
//.text:00B921B0 ; 
//.text:00B921E0 ; 
//.text:00B921F0 ; void __cdecl __tls_set_g_device_groups_data_allocator(void*)
//.text:00B92220 ; 
//.text:00B92260 ; 
//.text:00B92280 ; 
//.text:00B922C0 ; void __cdecl create_initial_device_groups()
//.text:00B92380 ; 
//.text:00B923B0 ; void __cdecl device_add_control_children_to_groups(long)
//.text:00B92540 ; 
//.text:00B92550 ; bool __cdecl device_allow_interaction(long, real_point3d const*, vector3d const*)
//.text:00B92630 ; 
//.text:00B92670 ; void __cdecl device_animate_overlay(long, real, real, real, real)
//.text:00B92700 ; void __cdecl device_animate_position(long, real, real, real, real, bool)
//.text:00B927A0 ; 
//.text:00B92840 ; 
//.text:00B928E0 ; void __cdecl device_closes_automatically_set(long, bool)
//.text:00B92930 ; bool __cdecl device_compute_activation(long, s_game_cluster_bit_vectors const*, bool*)
//.text:00B92980 ; bool __cdecl device_compute_function_value(long, long, long, real*, bool*, bool*)
//.text:00B92CC0 ; void __cdecl device_delete(long)
//.text:00B92CD0 ; void __cdecl device_effect_new(long, long)
//.text:00B92DA0 ; 
//.text:00B92E30 ; long __cdecl device_get_action_string(long)
//.text:00B92E90 ; double __cdecl device_get_position(long)
//.text:00B92EE0 ; double __cdecl device_get_power(long)
//.text:00B92F30 ; void __cdecl device_group_change_only_once_more_set(long, bool)
//.text:00B92FE0 ; 

long __cdecl device_group_get_from_scenario_index(short device_index)
{
	return INVOKE(0x00B93040, device_group_get_from_scenario_index, device_index);
}

//.text:00B930A0 ; real __cdecl device_group_get_value(long)
//.text:00B930D0 ; long __cdecl device_group_new(long, real, dword)
//.text:00B93130 ; void __cdecl device_group_set_actual_value(long, real)
//.text:00B93270 ; bool __cdecl device_group_set_desired_value(long, long, real)
//.text:00B933E0 ; void __cdecl device_groups_reset()
//.text:00B93460 ; void __cdecl device_ignore_player_set(long, bool)
//.text:00B934B0 ; 
//.text:00B93550 ; bool __cdecl device_new(long, object_placement_data*, bool*)
//.text:00B93650 ; 
//.text:00B936A0 ; void __cdecl device_one_sided_set(long, bool)
//.text:00B936D0 ; void __cdecl device_operates_automatically_set(long, bool)
//.text:00B93720 ; void __cdecl device_place(long, s_scenario_device*)
//.text:00B938E0 ; void __cdecl device_preprocess_node_orientations(long, c_static_flags<255> const*, long, real_orientation*)
//.text:00B939C0 ; void __cdecl device_render_debug(long)
//.text:00B939D0 ; 
//.text:00B93A00 ; void __cdecl device_set_actual_position(long, real)
//.text:00B93A70 ; void __cdecl device_set_animation_changed(long)
//.text:00B93AF0 ; bool __cdecl device_set_desired_position(long, real)
//.text:00B93B60 ; void __cdecl device_set_never_appears_locked(long, bool)
//.text:00B93BA0 ; bool __cdecl device_set_overlay_track(long, long)
//.text:00B93C40 ; bool __cdecl device_set_position_track(long, long, real)
//.text:00B94330 ; void __cdecl device_set_power(long, real)
//.text:00B943C0 ; bool __cdecl device_setup_position_animation_channel(long, long)
//.text:00B94460 ; bool __cdecl device_setup_power_and_overlay_track_animation_channel(long, long)
//.text:00B94500 ; void __cdecl device_touched(long, long, bool)
//.text:00B94560 ; void __cdecl device_unplace(long)
//.text:00B946C0 ; bool __cdecl device_update(long)
//.text:00B94DD0 ; void __cdecl devices_dispose()
//.text:00B94E10 ; void __cdecl devices_dispose_from_old_map()
//.text:00B94E30 ; void __cdecl devices_initialize()
//.text:00B94E70 ; void __cdecl devices_initialize_for_new_map()
//.text:00B94EA0 ; 
//.text:00B94ED0 ; 
//.text:00B94EE0 ; 
//.text:00B94EF0 ; 
//.text:00B94F00 ; 
//.text:00B94F20 ; 
//.text:00B94F60 ; void __cdecl setup_device_animation_control(s_device_animation_control*, real, real, real, real, real, real)
//.text:00B95140 ; 

