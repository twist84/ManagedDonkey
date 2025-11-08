#include "objects/object_recycling.hpp"

//.text:00BB9260 ; int32 __cdecl recycle_worst_object_available(int32 filter_flags, uns8* working_memory, int32 working_memory_size, bool* more_to_release, char* description, int32 description_length)
//.text:00BB9330 ; int32 __cdecl setup_recycling_workspace(int32 filter_flags, uns8* working_memory, int32 working_memory_size, bool* more_to_release, char* description, int32 description_length)
//.text:00BB9360 ; 
//.text:00BB9370 ; tls
//.text:00BB9390 ; tls
//.text:00BB93B0 ; tls
//.text:00BB93C0 ; tls
//.text:00BB93D0 ; void __cdecl __tls_set_g_recycling_groups_allocator(void* new_address)
//.text:00BB9400 ; void __cdecl __tls_set_g_recycling_volumes_allocator(void* new_address)
//.text:00BB9420 ; tls
//.text:00BB9460 ; tls
//.text:00BB94A0 ; public: void c_data_iterator<s_recycling_group>::begin(s_data_array* data)
//.text:00BB94C0 ; 
//.text:00BB94D0 ; real32 __cdecl closest_player_distance_squared(const real_point3d* test_point, const real_point3d* player_heads, int32 player_count)
//.text:00BB9550 ; tls
//.text:00BB9580 ; tls
//.text:00BB95B0 ; int32 __cdecl find_worst_object_in_group(int32 filter_flags, const s_recycling_group* group, int32 current_time)
//.text:00BB96B0 ; 
//.text:00BB96C0 ; tls
//.text:00BB96F0 ; tls
//.text:00BB9720 ; 
//.text:00BB9730 ; 
//.text:00BB9740 ; int32 __cdecl get_player_head_positions(real_point3d* player_heads, int32 max_player_heads)
//.text:00BB97D0 ; 
//.text:00BB97E0 ; 
//.text:00BB97F0 ; 
//.text:00BB9800 ; 
//.text:00BB9820 ; 
//.text:00BB9830 ; void __cdecl move_recycling_object_between_groups(int32 object_index, int32 previous_object_index, int32 source_group_index, int32 dest_group_index)
//.text:00BB98F0 ; public: bool c_data_iterator<struct s_recycling_group>::next()
//.text:00BB9910 ; void __cdecl object_recycling_clear_history()
//.text:00BB9920 ; void __cdecl object_recycling_group_remove_object(int32 object_index)
//.text:00BB99B0 ; void __cdecl object_recycling_reset_volume_list()
//.text:00BB9A40 ; void __cdecl object_recycling_switch_lists(int32 volume_index, int32* top_of_source_list, int32* top_of_dest_list)
//.text:00BB9AD0 ; void __cdecl object_recycling_system_add_volume(int32 trigger_volume_index, int32 max_object_count, int32 duration)
//.text:00BB9BA0 ; void __cdecl objects_recycling_dispose()
//.text:00BB9BB0 ; void __cdecl objects_recycling_dispose_from_old_map()
//.text:00BB9BF0 ; void __cdecl objects_recycling_group_process(s_recycling_workspace* recycling_workspace, int32 list_index)
//.text:00BBA020 ; void __cdecl objects_recycling_initialize()
//.text:00BBA0B0 ; void __cdecl objects_recycling_initialize_for_new_map()
//.text:00BBA0F0 ; void __cdecl objects_recycling_render(s_recycling_workspace* recycling_workspace)
//.text:00BBA100 ; void __cdecl objects_recycling_update(e_recycling_methods method)
//.text:00BBA330 ; void __cdecl objects_recycling_update_membership(s_recycling_workspace* recycling_workspace)
//.text:00BBA860 ; void __cdecl prepare_recycling_group_list(s_recycling_workspace* recycling_workspace)
//.text:00BBAA00 ; uns32 __cdecl recycling_get_target_flags(uns32)
//.text:00BBAA90 ; uns32 __cdecl recycling_get_tolerance_flags()
//.text:00BBAB00 ; void __cdecl recycling_release_object(int32)
//.text:00BBAB30 ; tls
//.text:00BBAB70 ; tls
//.text:00BBABB0 ; 
//.text:00BBABE0 ; int __cdecl sort_recycling_entry_descending(const void* p1, const void* p2)
//.text:00BBAC00 ; void __cdecl sort_recycling_entry_list(s_recycling_workspace* recycling_workspace)
//.text:00BBAC30 ; 
//.text:00BBAC50 ; void __cdecl update_active_recycling_volumes(s_recycling_workspace* recycling_workspace)
//.text:00BBAF80 ; void __cdecl update_recycling_group_list(s_recycling_workspace* recycling_workspace)

