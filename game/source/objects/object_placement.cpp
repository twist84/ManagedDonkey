#include "objects/object_placement.hpp"

//.text:00B59150 ; bool __cdecl object_placement_attached_to_valid_parent(s_scenario_object const*)

void __cdecl object_placement_create_active_zone_set_objects(e_object_placement_zone_set_create_mode create_mode)
{
	INVOKE(0x00B591C0, object_placement_create_active_zone_set_objects, create_mode);
}

//.text:00B594C0 ; void __cdecl object_placement_create_active_zone_set_skies(long scenario_definition_index, long structure_bsp_index)

void __cdecl object_placement_create_global_objects(e_game_mode game_mode, bool a2)
{
	INVOKE(0x00B59510, object_placement_create_global_objects, game_mode, a2);
}

//.text:00B596C0 ; bool __cdecl object_placement_data_new_from_scenario_object(e_object_type, long, s_scenario_object const*, s_tag_block*, bool, bool, struct object_placement_data*)
//.text:00B59CA0 ; void __cdecl object_placement_deactivate_on_zone_set_switch(long)
//.text:00B59D10 ; void __cdecl object_placement_dispose()
//.text:00B59D20 ; void __cdecl object_placement_dispose_from_old_map()
//.text:00B59D30 ; void __cdecl object_placement_dispose_from_old_structure_bsp(dword)
//.text:00B59D40 ; bool __cdecl object_placement_find_initial_location(dword, real_point3d const*, real, s_location*)
//.text:00B59E10 ; void __cdecl object_placement_initialize()
//.text:00B59EB0 ; void __cdecl object_placement_initialize_for_new_map()
//.text:00B59FC0 ; void __cdecl object_placement_initialize_for_new_structure_bsp(dword)

