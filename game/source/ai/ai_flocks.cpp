#include "ai/ai_flocks.hpp"

//.text:014BFA20 ; 
//.text:014BFA40 ; 
//.text:014BFA50 ; void __cdecl __tls_set_g_flocks_data_allocator(void*)
//.text:014BFA80 ; 
//.text:014BFBF0 ; void __cdecl accumulate_danger_vector(flock_datum*, struct flock_definition*, real_point3d*, real32*, real_vector3d*, bool, real32*)
//.text:014C0240 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_flocks_data_allocator(void*)>::allocate
//.text:014C0280 ; 
//.text:014C02A0 ; void __cdecl boid_state_initialize(int32, struct boid_state*, int16, real32)
//.text:014C0380 ; void __cdecl boid_update(int32, int32)
//.text:014C19F0 ; void __cdecl boid_update_target(int32, int32)
//.text:014C1DB0 ; void __cdecl boundary_force_bounding_volume(s_flock_instance const*, real_point3d*, real32*, real_vector3d*, bool, bool*)
//.text:014C2180 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_flocks_data_allocator(void*)>::deallocate
//.text:014C21B0 ; bool __cdecl flock_add_creature(int32, int32)
//.text:014C2220 ; void __cdecl flock_boid_iterator_new(int32, flock_boid_iterator*)
//.text:014C2260 ; creature_datum* __cdecl flock_boid_iterator_next(flock_boid_iterator*)
//.text:014C22C0 ; int16 __cdecl flock_choose_source(flock_datum*, s_flock_instance const*)
//.text:014C2550 ; int32 __cdecl flock_create_creature(int32, int16)
//.text:014C2A40 ; void __cdecl flock_delete(int32)
//.text:014C2AE0 ; int32 __cdecl flock_get_by_name(int32)
//.text:014C2B90 ; void __cdecl flock_handle_creature_death(int32, int32, int16)
//.text:014C2C40 ; void __cdecl flock_handle_creature_delete(int32)
//.text:014C2D50 ; int16 __cdecl flock_instance_get_by_name(int32)
//.text:014C2DA0 ; void __cdecl flock_iterator_new(flock_iterator*)
//.text:014C2DE0 ; flock_datum* __cdecl flock_iterator_next(flock_iterator*)
//.text:014C2E30 ; int32 __cdecl flock_new(int16, int32)
//.text:014C2EF0 ; bool __cdecl flock_place(int32)
//.text:014C2FA0 ; bool __cdecl flock_remove_creature(int32, int32)
//.text:014C3040 ; void __cdecl flock_set_spewing(flock_datum*, s_flock_instance*, bool)
//.text:014C3100 ; void __cdecl flock_unperch(int32)
//.text:014C31E0 ; void __cdecl flock_update(int32)
//.text:014C3480 ; bool __cdecl flock_update_activation_status(int32)
//.text:014C35A0 ; void __cdecl flock_update_bounding_box(int32)
//.text:014C36D0 ; void __cdecl flock_update_danger(int32)
//.text:014C37D0 ; void __cdecl flock_update_targets(int32)
//.text:014C38C0 ; bool __cdecl flock_volume_test_point(int32, real_point3d*, real32)
//.text:014C39E0 ; void __cdecl flocks_create_for_new_map()

void __cdecl flocks_dispose_from_old_map()
{
	INVOKE(0x014C39F0, flocks_dispose_from_old_map);
}

void __cdecl flocks_dispose_from_old_structure_bsp()
{
	INVOKE(0x014C3AE0, flocks_dispose_from_old_structure_bsp);
}

//.text:014C3C20 ; void __cdecl flocks_handle_weapon_detonation(real_point3d*)

void __cdecl flocks_initialize()
{
	INVOKE(0x014C3D20, flocks_initialize);
}

void __cdecl flocks_initialize_for_new_map()
{
	INVOKE(0x014C3D50, flocks_initialize_for_new_map);
}

void __cdecl flocks_initialize_for_new_structure_bsp(uint32 activating_structure_bsp_mask)
{
	INVOKE(0x014C3D70, flocks_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

void __cdecl flocks_update()
{
	INVOKE(0x014C3DE0, flocks_update);
}

//.text:014C3EF0 ; 
//.text:014C3F20 ; 
//.text:014C3F30 ; 
//.text:014C3F40 ; 
//.text:014C3F60 ; void __cdecl perlin_state_reset(s_perlin_state*)
//.text:014C3F90 ; void __cdecl perlin_state_update(s_perlin_state*, real32, real32, real32, bool, real_point3d*)
//.text:014C41E0 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_flocks_data_allocator(void*)>::reserve_memory

