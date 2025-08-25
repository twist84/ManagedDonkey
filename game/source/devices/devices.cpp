#include "devices/devices.hpp"

//.text:00B92160 ; void __cdecl simulation_action_object_update<e_simulation_device_update_flag>(int32, e_simulation_device_update_flag)
//.text:00B92190 ; 
//.text:00B921B0 ; public: __cdecl c_simulation_object_update_flags::c_simulation_object_update_flags(int32, e_simulation_device_update_flag)
//.text:00B921E0 ; 
//.text:00B921F0 ; void __cdecl __tls_set_g_device_groups_data_allocator(void*)
//.text:00B92220 ; 
//.text:00B92260 ; 
//.text:00B92280 ; void __cdecl clear_device_animation_control(s_device_animation_control*)
//.text:00B922C0 ; void __cdecl create_initial_device_groups()
//.text:00B92380 ; 
//.text:00B923B0 ; void __cdecl device_add_control_children_to_groups(int32)
//.text:00B92540 ; 
//.text:00B92550 ; bool __cdecl device_allow_interaction(int32, const real_point3d*, const real_vector3d*)
//.text:00B92630 ; 
//.text:00B92670 ; void __cdecl device_animate_overlay(int32, real32, real32, real32, real32)
//.text:00B92700 ; void __cdecl device_animate_position(int32, real32, real32, real32, real32, bool)
//.text:00B927A0 ; bool __cdecl device_can_change_position(int32)
//.text:00B92840 ; 
//.text:00B928E0 ; void __cdecl device_closes_automatically_set(int32, bool)
//.text:00B92930 ; bool __cdecl device_compute_activation(int32, const s_game_cluster_bit_vectors*, bool*)
//.text:00B92980 ; bool __cdecl device_compute_function_value(int32, int32, int32, real32*, bool*, bool*)
//.text:00B92CC0 ; void __cdecl device_delete(int32)
//.text:00B92CD0 ; void __cdecl device_effect_new(int32, int32)
//.text:00B92DA0 ; bool __cdecl device_frontfacing(int32, const real_point3d*, const real_vector3d*)
//.text:00B92E30 ; int32 __cdecl device_get_action_string(int32)
//.text:00B92E90 ; real64 __cdecl device_get_position(int32)
//.text:00B92EE0 ; real64 __cdecl device_get_power(int32)
//.text:00B92F30 ; void __cdecl device_group_change_only_once_more_set(int32, bool)
//.text:00B92FE0 ; bool __cdecl device_group_delete(int32, int32) // $TODO confirm this

int32 __cdecl device_group_get_from_scenario_index(int16 device_index)
{
	return INVOKE(0x00B93040, device_group_get_from_scenario_index, device_index);
}

//.text:00B930A0 ; real32 __cdecl device_group_get_value(int32)
//.text:00B930D0 ; int32 __cdecl device_group_new(int32, real32, uns32)
//.text:00B93130 ; void __cdecl device_group_set_actual_value(int32, real32)
//.text:00B93270 ; bool __cdecl device_group_set_desired_value(int32, int32, real32)
//.text:00B933E0 ; void __cdecl device_groups_reset()
//.text:00B93460 ; void __cdecl device_ignore_player_set(int32, bool)
//.text:00B934B0 ; 
//.text:00B93550 ; bool __cdecl device_new(int32, object_placement_data*, bool*)
//.text:00B93650 ; void __cdecl device_one_sided_for_player_set(int32, bool)
//.text:00B936A0 ; void __cdecl device_one_sided_set(int32, bool)
//.text:00B936D0 ; void __cdecl device_operates_automatically_set(int32, bool)
//.text:00B93720 ; void __cdecl device_place(int32, s_scenario_device*)
//.text:00B938E0 ; void __cdecl device_preprocess_node_orientations(int32, const c_static_flags<255>*, int32, real_orientation*)
//.text:00B939C0 ; void __cdecl device_render_debug(int32)
//.text:00B939D0 ; void __cdecl device_reset_animation_channels(int32)
//.text:00B93A00 ; void __cdecl device_set_actual_position(int32, real32)
//.text:00B93A70 ; void __cdecl device_set_animation_changed(int32)
//.text:00B93AF0 ; bool __cdecl device_set_desired_position(int32, real32)
//.text:00B93B60 ; void __cdecl device_set_never_appears_locked(int32, bool)
//.text:00B93BA0 ; bool __cdecl device_set_overlay_track(int32, int32)
//.text:00B93C40 ; bool __cdecl device_set_position_track(int32, int32, real32)
//.text:00B94330 ; void __cdecl device_set_power(int32, real32)
//.text:00B943C0 ; bool __cdecl device_setup_position_animation_channel(int32, int32)
//.text:00B94460 ; bool __cdecl device_setup_power_and_overlay_track_animation_channel(int32, int32)
//.text:00B94500 ; void __cdecl device_touched(int32, int32, bool)
//.text:00B94560 ; void __cdecl device_unplace(int32)
//.text:00B946C0 ; bool __cdecl device_update(int32)
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
//.text:00B94F60 ; void __cdecl setup_device_animation_control(s_device_animation_control*, real32, real32, real32, real32, real32, real32)
//.text:00B95140 ; bool __cdecl update_device_animation_control(int32, s_device_animation_control*, const c_animation_manager*, c_animation_channel*, real32, real32*)

