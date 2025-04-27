#include "visibility/visibility_portal_activation.hpp"

//.text:005F9AF0 
//.text:005F9B10 ; 
//.text:005F9B20 ; 
//.text:005F9B30 ; void __cdecl __tls_set_g_active_portal_bitvectors_allocator(void*)
//.text:005F9B50 ; 
//.text:005F9B90 ; 
//.text:005F9BB0 ; 
//.text:005F9BE0 ; 
//.text:005F9C10 ; 
//.text:005F9C20 ; void __cdecl portal_activation_clear_portals()
void __cdecl portal_activation_dispose()
{
	INVOKE(0x005F9CA0, portal_activation_dispose);
}

void __cdecl portal_activation_dispose_from_old_map()
{
	INVOKE(0x005F9CB0, portal_activation_dispose_from_old_map);
}

void __cdecl portal_activation_dispose_from_old_structure_bsp(uint32 deactivating_structure_bsp_mask)
{
	INVOKE(0x005F9CC0, portal_activation_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

//.text:005F9CD0 ; void __cdecl portal_activation_door_portal_search_for_doors()
void __cdecl portal_activation_initialize()
{
	INVOKE(0x005F9E30, portal_activation_initialize);
}

void __cdecl portal_activation_initialize_for_new_map()
{
	INVOKE(0x005F9E90, portal_activation_initialize_for_new_map);
}

void __cdecl portal_activation_initialize_for_new_structure_bsp(uint32 activating_structure_bsp_mask)
{
	INVOKE(0x005F9EA0, portal_activation_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:005FA040 ; 
//.text:005FA090 ; void __cdecl portal_activation_set_portal_active(s_portal_reference, bool)
//.text:005FA0F0 ; void __cdecl portal_activation_update_machine_activity(long)
//.text:005FA130 ; 

