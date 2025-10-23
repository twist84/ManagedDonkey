#include "physics/breakable_surfaces.hpp"

//.text:005686B0 ; void __cdecl __tls_set_g_breakable_surface_globals_allocator(void*)
//.text:005686D0 ; void __cdecl __tls_set_g_breakable_surface_set_broken_event_allocator(void*)
//.text:00568700 ; 
//.text:00568770 ; 
//.text:005687B0 ; 
//.text:005687F0 ; 
//.text:00568810 ; void __cdecl breakable_surface_break(int32, int32, int32, int32, int32, const real_point3d*, const s_location*, const real_vector3d*)
//.text:00568A00 ; 
//.text:00568DF0 ; void __cdecl breakable_surface_break_unsupported_surfaces(int32, int32, int32, const real_point3d*, const s_location*)
//.text:00568FB0 ; void __cdecl breakable_surface_create_impact_decal(const real_point3d*, int32, int32, int32)
//.text:00569120 ; void __cdecl breakable_surface_damage(int32, int32, int32, int32, const s_damage_data*, int32)
//.text:00569580 ; void __cdecl breakable_surface_damage_area_of_effect(const s_damage_data*)
//.text:005695F0 ; void __cdecl breakable_surface_damage_area_of_effect(const s_location*, const real_point3d*, real32)
//.text:00569820 ; 
//.text:00569AB0 ; bool __cdecl breakable_surface_extant(int32, int32, int32, int32)
//.text:00569B10 ; void __cdecl breakable_surface_flags_internal_set(int32, int32, int32, int32, bool)
//.text:00569BA0 ; void __cdecl breakable_surface_flags_set(int32, int32, int32, int32, bool)
//.text:00569C00 ; c_collision_bsp_reference __cdecl breakable_surface_get_collision_bsp(int32, int32, const real_matrix4x3**)
//.text:00569C60 ; s_breakable_surface_set* __cdecl breakable_surface_get_first_set(int32, int32)
//.text:00569D30 ; int32 __cdecl breakable_surface_get_group_count(int32)
//.text:00569D70 ; 
//.text:00569DA0 ; 
//.text:00569DC0 ; 
//.text:00569DD0 ; 
//.text:00569DE0 ; int32 __cdecl breakable_surface_get_surface_set_count(int32, int32)
//.text:00569E30 ; 
//.text:00569E40 ; void __cdecl breakable_surface_group_set_gamestate_index(int32, int32, int32)
//.text:00569E50 ; void __cdecl breakable_surface_particle_initialize(int32, real_point3d*, real_vector3d*)
//.text:0056A310 ; bool __cdecl breakable_surface_particle_system_bounding_sphere_origin_initialize(int32, real_point3d*)
//.text:0056A360 ; void __cdecl breakable_surface_render_cracks(int32, int32, int32)
//.text:0056A8E0 ; void __cdecl breakable_surface_set_crack_stencil_decal_state()
//.text:0056A900 ; void __cdecl breakable_surface_set_crack_stencil_glass_state(int32)
//.text:0056A950 ; breakable_surface_surface_list_calculate
//.text:0056ACE0 ; bool __cdecl breakable_surface_valid(int32, int32, int32, int32, int32)
//.text:0056ADB0 ; void __cdecl breakable_surfaces_detach_from_simulation()

void __cdecl breakable_surfaces_dispose()
{
	INVOKE(0x0056ADC0, breakable_surfaces_dispose);
}

void __cdecl breakable_surfaces_dispose_from_old_map()
{
	INVOKE(0x0056ADF0, breakable_surfaces_dispose_from_old_map);
}

void __cdecl breakable_surfaces_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x0056AE10, breakable_surfaces_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl breakable_surfaces_enable(bool enable)
{
	INVOKE(0x0056AF20, breakable_surfaces_enable, enable);
}

//.text:0056AF40 ; breakable_surfaces_find_support_surfaces
//.text:0056B370 ; breakable_surfaces_find_supported_surfaces
//.text:0056B6C0 ; void __cdecl breakable_surfaces_handle_tag_reload()

void __cdecl breakable_surfaces_initialize()
{
	INVOKE(0x0056B6E0, breakable_surfaces_initialize);
}

void __cdecl breakable_surfaces_initialize_for_new_map()
{
	INVOKE(0x0056B770, breakable_surfaces_initialize_for_new_map);
}

//.text:0056B7C0 ; void __cdecl breakable_surfaces_initialize_for_new_map_internal()

void __cdecl breakable_surfaces_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x0056B800, breakable_surfaces_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

void __cdecl breakable_surfaces_reset()
{
	INVOKE(0x0056B960, breakable_surfaces_reset);
}

void __cdecl breakable_surfaces_update()
{
	INVOKE(0x0056BA20, breakable_surfaces_update);
}

//.text:0056BDC0 ; void __cdecl breakable_surfaces_update_support()
//.text:0056BE50 ; int32 __cdecl breakle_surface_impact_decal_allocate()

