#include "objects/object_broadphase.hpp"

//.text:00B966B0 ; 
//.text:00B966D0 ; 
//.text:00B966F0 ; public: __cdecl c_object_broadphase_ray_cast_callback::c_object_broadphase_ray_cast_callback(s_collision_test_flags, dword, real_point3d const*, vector3d const*, long, long, long, collision_result*, real, real)
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
//.text:00B96890 ; protected: virtual real __cdecl c_object_broadphase_ray_cast_callback::addBroadPhaseHandle(class hkBroadPhaseHandle const *, int)
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
//.text:00B96C40 ; void __cdecl object_broadphase_add_object(long, s_object_cluster_payload const*)
//.text:00B96C40 ; void __cdecl object_broadphase_add_object(long)
//.text:00B96E00 ; 
//.text:00B96E40 ; void __cdecl object_broadphase_dispose()
//.text:00B96E50 ; void __cdecl object_broadphase_dispose_from_old_map()
//.text:00B96E60 ; void __cdecl object_broadphase_dispose_from_old_structure_bsp(dword)
//.text:00B96EA0 ; void __cdecl object_broadphase_initialize()
//.text:00B96F20 ; void __cdecl object_broadphase_initialize_for_new_map()
//.text:00B96F30 ; void __cdecl object_broadphase_initialize_for_new_structure_bsp(dword)
//.text:00B97420 ; 
//.text:00B97510 ; bool __cdecl object_broadphase_ray_cast(s_collision_test_flags, dword, real_point3d const*, vector3d const*, long, long, long, collision_result*)
//.text:00B97720 ; void __cdecl object_broadphase_remove_object(long)
//.text:00B97840 ; void __cdecl __tls_set_g_object_broadphase_allocator(void*)
//.text:00B97870 ; 
//.text:00B97890 ; void __cdecl object_broadphase_update_object(long)
//.text:00B97A60 ; 
//.text:00B97AC0 ; void __cdecl object_broaphase_load_from_game_state(long) // broaphase Bungie really?
//.text:00B97AE0 ; void __cdecl object_broaphase_save_to_game_state(long)   // broaphase Bungie really?
//.text:00B97AF0 ; 
//.text:00B97B80 ; 
//.text:00B97BB0 ; 
//.text:00B97BF0 ; 
//.text:00B97C20 ; 
//.text:00B97C60 ; 
//.text:00B97C80 ; 
//.text:00B97CA0 ; 
//.text:00B97CB0 ; 

