#include "objects/object_placement.hpp"

#include "cseries/cseries.hpp"
#include "cseries/progress.hpp"
#include "cutscene/cinematics.hpp"
#include "physics/havok.hpp"
#include "scenario/scenario.hpp"

bool __cdecl object_definition_can_be_placed(int32 object_definition_index, int32 model_variant_index)
{
	return INVOKE(0x00B58FF0, object_definition_can_be_placed, object_definition_index, model_variant_index);
}

bool __cdecl object_placement_attached_to_valid_parent(const s_scenario_object* scenario_object)
{
	return INVOKE(0x00B59150, object_placement_attached_to_valid_parent, scenario_object);
}

int32 __cdecl object_placement_bsp_index_key_from_bsp_indices(int32 first_bsp_index, int32 second_bsp_index)
{
	return INVOKE(0x00B59190, object_placement_bsp_index_key_from_bsp_indices, first_bsp_index, second_bsp_index);
}

void __cdecl object_placement_create_active_zone_set_objects(e_object_placement_zone_set_create_mode create_mode)
{
	INVOKE(0x00B591C0, object_placement_create_active_zone_set_objects, create_mode);
}

void __cdecl object_placement_create_active_zone_set_skies(int32 scenario_index, int32 active_bsp_mask)
{
	INVOKE(0x00B594C0, object_placement_create_active_zone_set_skies, scenario_index, active_bsp_mask);
}

void __cdecl object_placement_create_global_objects(e_game_mode game_mode, bool test_object_existence_before_creation)
{
	INVOKE(0x00B59510, object_placement_create_global_objects, game_mode, test_object_existence_before_creation);
}

//.text:00B596C0 ; bool __cdecl object_placement_data_new_from_scenario_object(e_object_type, int32, const s_scenario_object*, s_tag_block*, bool, bool, struct object_placement_data*)

void __cdecl object_placement_deactivate_on_zone_set_switch(int32 object_index)
{
	INVOKE(0x00B59CA0, object_placement_deactivate_on_zone_set_switch, object_index);
}

void __cdecl object_placement_dispose()
{
	INVOKE(0x00B59D10, object_placement_dispose);
}

void __cdecl object_placement_dispose_from_old_map()
{
	INVOKE(0x00B59D20, object_placement_dispose_from_old_map);
}

void __cdecl object_placement_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x00B59D30, object_placement_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

bool __cdecl object_placement_find_initial_location(uns32 active_bsp_mask, const real_point3d* center, real32 radius, s_location* out_location)
{
	return INVOKE(0x00B59D40, object_placement_find_initial_location, active_bsp_mask, center, radius, out_location);
}

void __cdecl object_placement_initialize()
{
	INVOKE(0x00B59E10, object_placement_initialize);
}

void __cdecl object_placement_initialize_for_new_map()
{
	INVOKE(0x00B59EB0, object_placement_initialize_for_new_map);
}

void __cdecl object_placement_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	//INVOKE(0x00B59FC0, object_placement_initialize_for_new_structure_bsp, activating_structure_bsp_mask);

	progress_new("placing objects on bsp");
	havok_can_modify_state_allow();
	if (cinematic_in_progress() && cinematic_suppressing_bsp_object_creation())
	{
		object_placement_create_active_zone_set_skies(global_scenario_index_get(), global_structure_bsp_active_mask_get());
		havok_can_modify_state_disallow();
	}
	else
	{
		object_placement_create_active_zone_set_objects(_object_placement_zone_set_create_default);
		havok_can_modify_state_disallow();
	}
	progress_done();
}

//.text:00B5A020 ; bool __cdecl object_placement_inside_structure_bsp(e_object_type, const s_scenario_object*)
//.text:00B5A040 ; bool __cdecl object_placement_inside_structure_bsp_mask(uns32, e_object_type, const s_scenario_object*)
//.text:00B5A100 ; bool __cdecl object_placement_is_unloaded(int32)

void __cdecl object_placement_mark_name_on_create(int16 object_name_index)
{
	INVOKE(0x00B5A110, object_placement_mark_name_on_create, object_name_index);
}

//.text:00B5A190 ; void __cdecl object_placement_mark_name_on_delete(int16)

void __cdecl object_placement_mark_object_on_delete(int32 object_index)
{
	INVOKE(0x00B5A200, object_placement_mark_object_on_delete, object_index);
}

//.text:00B5A270 ; 
//.text:00B5A2C0 ; 

void __cdecl object_placement_prepare_for_new_zone_set(uns32 old_active_structure_bsp_mask, uns32 new_active_structure_bsp_mask)
{
	INVOKE(0x00B5A310, object_placement_prepare_for_new_zone_set, old_active_structure_bsp_mask, new_active_structure_bsp_mask);
}

void __cdecl object_placement_prepare_for_non_bsp_zone_set_switch(const s_game_non_bsp_zone_set* old_non_bsp_zone_set, const s_game_non_bsp_zone_set* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry)
{
	INVOKE(0x00B5A470, object_placement_prepare_for_non_bsp_zone_set_switch, old_non_bsp_zone_set, new_non_bsp_zone_set, pending_zone_registry);
}

//.text:00B5A4E0 ; void __cdecl object_placement_reload(int32)

void __cdecl object_placement_reset_from_teleport(int32 object_index)
{
	INVOKE(0x00B5A4F0, object_placement_reset_from_teleport, object_index);
}

//.text:00B5A550 ; void __cdecl object_placement_restore(int32)
//.text:00B5A560 ; void __cdecl object_placement_set_in_limbo(int32, bool)
//.text:00B5A5A0 ; void __cdecl object_placement_synchronize_for_designer_zone_debugger(bool)
//.text:00B5A620 ; void __cdecl object_placement_unload(int32)
//.text:00B5A640 ; int32 __cdecl object_type_get_scenario_placement_offset(e_object_type)
//.text:00B5A650 ; 
//.text:00B5A660 ; 
//.text:00B5A6A0 ; private: void __cdecl c_scenario_object_to_object_datum_mapping::remove_dirty_objects()
//.text:00B5A840 ; 
//.text:00B5A880 ; s_tag_block* __cdecl scenario_get_object_type_scenario_datums(s_scenario*, e_object_type, int32*)
//.text:00B5A8B0 ; s_tag_block* __cdecl scenario_get_object_type_scenario_palette(s_scenario*, e_object_type)
//.text:00B5A8D0 ; 
//.text:00B5A920 ; 
//.text:00B5A9A0 ; 
//.text:00B5A9E0 ; const s_scenario_object* __cdecl scenario_object_try_and_get(s_scenario*, e_object_type, int32)
//.text:00B5AA30 ; object_definition* __cdecl scenario_object_get_object_definition(s_scenario*, const s_scenario_object*)
//.text:00B5AA60 ; int32 __cdecl scenario_object_get_object_definition_tag_index(s_scenario*, const s_scenario_object*)
//.text:00B5AAD0 ; scenario_object_palette_entry* __cdecl scenario_object_get_palette_entry(s_scenario*, const s_scenario_object*)
//.text:00B5AB30 ; real32 __cdecl scenario_object_get_scale(const s_scenario_object*)
//.text:00B5AB60 ; bool __cdecl scenario_object_has_persistent_state(s_scenario*, const s_scenario_object*)
//.text:00B5ABB0 ; bool __cdecl scenario_object_is_placed_globally(s_scenario*, const s_scenario_object*)
//.text:00B5AC40 ; bool __cdecl scenario_object_managed_by_script(s_scenario*, const s_scenario_object*)
//.text:00B5AC60 ; const s_scenario_object* __cdecl scenario_object_try_and_get(scenario*, e_object_type, int32)
//.text:00B5ACB0 ; void __cdecl scenario_objects_synchronize_parent_placement(const uns32*, s_scenario*)

