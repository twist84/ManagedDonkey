#include "objects/object_recycling.hpp"

//.text:00BB9260 ; int32 __cdecl recycle_worst_object_available(int32, uns8*, int32, bool*, char*, int32)
//.text:00BB9330 ; int32 __cdecl setup_recycling_workspace(int32, uns8*, int32, bool*, char*, int32)
//.text:00BB9360 ; 
//.text:00BB9370 ; 
//.text:00BB9390 ; 
//.text:00BB93B0 ; 
//.text:00BB93C0 ; 
//.text:00BB93D0 ; void __cdecl __tls_set_g_recycling_groups_allocator(void*)
//.text:00BB9400 ; void __cdecl __tls_set_g_recycling_volumes_allocator(void*)
//.text:00BB9420 ; 
//.text:00BB9460 ; 
//.text:00BB94A0 ; 
//.text:00BB94C0 ; 
//.text:00BB94D0 ; 
//.text:00BB9550 ; 
//.text:00BB9580 ; 
//.text:00BB95B0 ; int32 __cdecl find_worst_object_in_group(int32, const s_recycling_group*, int32)
//.text:00BB96B0 ; 
//.text:00BB96C0 ; 
//.text:00BB96F0 ; 
//.text:00BB9720 ; 
//.text:00BB9730 ; 
//.text:00BB9740 ; int32 __cdecl get_player_head_positions(real_point3d*, int32)
//.text:00BB97D0 ; 
//.text:00BB97E0 ; 
//.text:00BB97F0 ; 
//.text:00BB9800 ; 
//.text:00BB9820 ; 
//.text:00BB9830 ; void __cdecl move_recycling_object_between_groups(int32, int32, int32, int32)
//.text:00BB98F0 ; 
//.text:00BB9910 ; void __cdecl object_recycling_clear_history()
//.text:00BB9920 ; void __cdecl object_recycling_group_remove_object(int32)
//.text:00BB99B0 ; void __cdecl object_recycling_reset_volume_list()
//.text:00BB9A40 ; void __cdecl object_recycling_switch_lists(int32, int32*, int32*)
//.text:00BB9AD0 ; void __cdecl object_recycling_system_add_volume(int32, int32, int32)
//.text:00BB9BA0 ; void __cdecl objects_recycling_dispose()
//.text:00BB9BB0 ; void __cdecl objects_recycling_dispose_from_old_map()
//.text:00BB9BF0 ; void __cdecl objects_recycling_group_process(s_recycling_workspace*, int32)
//.text:00BBA020 ; void __cdecl objects_recycling_initialize()
//.text:00BBA0B0 ; void __cdecl objects_recycling_initialize_for_new_map()
//.text:00BBA0F0 ; void __cdecl objects_recycling_render(s_recycling_workspace*)
//.text:00BBA100 ; void __cdecl objects_recycling_update(e_recycling_methods)
//.text:00BBA330 ; void __cdecl objects_recycling_update_membership(s_recycling_workspace*)
//.text:00BBA860 ; void __cdecl prepare_recycling_group_list(s_recycling_workspace*)
//.text:00BBAA00 ; uns32 __cdecl recycling_get_target_flags(uns32)
//.text:00BBAA90 ; uns32 __cdecl recycling_get_tolerance_flags()
//.text:00BBAB00 ; void __cdecl recycling_release_object(int32)
//.text:00BBAB30 ; 
//.text:00BBAB70 ; 
//.text:00BBABB0 ; 
//.text:00BBABE0 ; int __cdecl sort_recycling_entry_descending(const void*, const void*)
//.text:00BBAC00 ; void __cdecl sort_recycling_entry_list(s_recycling_workspace*)
//.text:00BBAC30 ; 
//.text:00BBAC50 ; void __cdecl update_active_recycling_volumes(s_recycling_workspace*)
//.text:00BBAF80 ; void __cdecl update_recycling_group_list(s_recycling_workspace*)

