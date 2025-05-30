#include "objects/object_broadphase.hpp"

//.text:00B966B0 ; 
//.text:00B966D0 ; 
//.text:00B966F0 ; public: __cdecl c_object_broadphase_ray_cast_callback::c_object_broadphase_ray_cast_callback(s_collision_test_flags, uns32, const real_point3d*, const real_vector3d*, int32, int32, int32, collision_result*, real32, real32)
//.text:00B96780 ; 
//.text:00B96790 ; 
//.text:00B967B0 ; 
//.text:00B967C0 ; 
//.text:00B967D0 ; 
//.text:00B967E0 ; 
//.text:00B967F0 ; 
//.text:00B96800 ; 
//.text:00B96820 ; 
//.text:00B96830 ; public: virtual __cdecl c_object_broadphase_ray_cast_callback::~c_object_broadphase_ray_cast_callback()
//.text:00B96860 ; public: virtual __cdecl hkBroadPhaseCastCollector::~hkBroadPhaseCastCollector()
//.text:00B96890 ; protected: virtual real32 __cdecl c_object_broadphase_ray_cast_callback::addBroadPhaseHandle(const class hkBroadPhaseHandle*, int)
//.text:00B96970 ; 
//.text:00B969B0 ; bool __cdecl can_use_object_broadphase()
//.text:00B969C0 ; 
//.text:00B969F0 ; 
//.text:00B96A40 ; 
//.text:00B96A90 ; 
//.text:00B96AE0 ; 
//.text:00B96B10 ; 
//.text:00B96B40 ; 
//.text:00B96B50 ; 
//.text:00B96B60 ; 
//.text:00B96B70 ; 
//.text:00B96B80 ; 
//.text:00B96B90 ; 
//.text:00B96BA0 ; 
//.text:00B96BB0 ; 
//.text:00B96C40 ; void __cdecl object_broadphase_add_object(int32, const s_object_cluster_payload*)
//.text:00B96C40 ; void __cdecl object_broadphase_add_object(int32)
//.text:00B96E00 ; void __cdecl object_broadphase_aquire_havok_thread_memory()

void __cdecl object_broadphase_dispose()
{
	INVOKE(0x00B96E40, object_broadphase_dispose);
}

void __cdecl object_broadphase_dispose_from_old_map()
{
	INVOKE(0x00B96E50, object_broadphase_dispose_from_old_map);
}

void __cdecl object_broadphase_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x00B96E60, object_broadphase_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl object_broadphase_initialize()
{
	INVOKE(0x00B96EA0, object_broadphase_initialize);
}

void __cdecl object_broadphase_initialize_for_new_map()
{
	INVOKE(0x00B96F20, object_broadphase_initialize_for_new_map);
}

void __cdecl object_broadphase_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x00B96F30, object_broadphase_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:00B97420 ; void __cdecl object_broadphase_post_copy_fixup(s_object_broadphase*)
//.text:00B97510 ; bool __cdecl object_broadphase_ray_cast(s_collision_test_flags, uns32, const real_point3d*, const real_vector3d*, int32, int32, int32, collision_result*)
//.text:00B97720 ; void __cdecl object_broadphase_remove_object(int32)
//.text:00B97840 ; void __cdecl __tls_set_g_object_broadphase_allocator(void*)
//.text:00B97870 ; s_object_broadphase* object_broadphase_sweep_vtable_pointer_get()

void __cdecl object_broadphase_update_object(int32 object_index)
{
	INVOKE(0x00B97890, object_broadphase_update_object, object_index);
}

//.text:00B97A60 ; void __cdecl object_broadphase_update_object_payload(int32, const s_object_cluster_payload*)
//.text:00B97AC0 ; void __cdecl object_broaphase_load_from_game_state(int32) // broaphase Bungie really?
//.text:00B97AE0 ; void __cdecl object_broaphase_save_to_game_state(int32)   // broaphase Bungie really?
//.text:00B97AF0 ; void __cdecl object_calculate_broadphase_aabb(int32, hkAabb*)
//.text:00B97B80 ; 
//.text:00B97BB0 ; 
//.text:00B97BF0 ; 
//.text:00B97C20 ; 
//.text:00B97C60 ; 
//.text:00B97C80 ; 
//.text:00B97CA0 ; 
//.text:00B97CB0 ; 

