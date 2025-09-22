#include "editor/editor_stubs.hpp"

#include "memory/module.hpp"
#include "scenario/scenario.hpp"

//HOOK_DECLARE(0x0042DF80, editor_change_pvs);
//HOOK_DECLARE(0x0042DF90, editor_dispose);
//HOOK_DECLARE(0x0042DFA0, editor_dispose_from_old_map);
//HOOK_DECLARE(0x0042DFB0, editor_dispose_from_old_structure_bsp);
HOOK_DECLARE(0x0042DFD0, editor_get_cluster_color);
HOOK_DECLARE(0x0042DFE0, editor_get_map_name);
HOOK_DECLARE(0x0042DFF0, editor_handle_tag_reload);
HOOK_DECLARE(0x0042E000, sub_42E000);
//HOOK_DECLARE(0x0042E010, editor_initialize);
//HOOK_DECLARE(0x0042E020, editor_initialize_for_new_map);
//HOOK_DECLARE(0x0042E030, editor_initialize_for_new_structure_bsp);
HOOK_DECLARE(0x0042E040, editor_input_inhibited);
HOOK_DECLARE(0x0042E060, editor_orphan_object);
HOOK_DECLARE(0x0042E070, editor_prepare_for_tag_reload);
HOOK_DECLARE(0x0042E080, editor_register_new_object);
HOOK_DECLARE(0x0042E0C0, editor_save_progress);
HOOK_DECLARE(0x0042E0D0, editor_should_render_object);
HOOK_DECLARE(0x0042E0E0, editor_should_render_transparent_geometry);
HOOK_DECLARE(0x0042E0F0, editor_should_resolve_block_references);
HOOK_DECLARE(0x0042E100, editor_should_resolve_tag_reference);
HOOK_DECLARE(0x0042E110, editor_switch_zone_set);
HOOK_DECLARE(0x0042E120, editor_update);
HOOK_DECLARE(0x0042E130, game_in_editor);
HOOK_DECLARE(0x0042E140, game_in_editor_player_simulation);

//.text:0042DF80 ; 
void __cdecl editor_change_pvs(const s_game_cluster_bit_vectors* old_cluster_activation, const s_game_cluster_bit_vectors* new_cluster_activation, bool local)
{
}

//.text:0042DF90 ; 
void __cdecl editor_dispose()
{
}

//.text:0042DFA0 ; 
void __cdecl editor_dispose_from_old_map()
{
}

//.text:0042DFB0 ; 
void __cdecl editor_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
}

//.text:0042DFC0 ; 

//.text:0042DFD0 ; 
void __cdecl editor_get_cluster_color(const s_cluster_reference* cluster_reference, real_argb_color* cluster_color)
{
}

//.text:0042DFE0 ; 
const char* __cdecl editor_get_map_name()
{
	return "";
}

//.text:0042DFF0 ; 
void __cdecl editor_handle_tag_reload(bool reloading_scenario)
{
}

// used in `input_update`
// something like `editor_can_update_input`?
//.text:0042E000 ; 
bool __cdecl sub_42E000()
{
	return false;
}

//.text:0042E010 ; 
void __cdecl editor_initialize()
{
}

//.text:0042E020 ; 
void __cdecl editor_initialize_for_new_map()
{
}

//.text:0042E030 ; 
void __cdecl editor_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
}

//.text:0042E040 ; 
bool __cdecl editor_input_inhibited()
{
	return true;
}

//.text:0042E050 ; 

//.text:0042E060 ; 
void __cdecl editor_orphan_object(e_object_type object_type, int32 scenario_datum_index)
{
}

//.text:0042E070 ; 
void __cdecl editor_prepare_for_tag_reload(bool reloading_scenario)
{
}

//.text:0042E080 ; 
void __cdecl editor_register_new_object(e_object_type object_type, int32 scenario_datum_index, int32 new_object_index)
{
}

//.text:0042E090 ; 
//.text:0042E0A0 ; 
//.text:0042E0B0 ; 

void __cdecl editor_reset_script_referenced_blocks()
{

}

//.text:0042E0C0 ; 
void __cdecl editor_save_progress()
{
}

//.text:0042E0D0 ; 
bool __cdecl editor_should_render_object(int32 object_index)
{
	return true;
}

//.text:0042E0E0 ; 
bool __cdecl editor_should_render_transparent_geometry()
{
	return false;
}

//.text:0042E0F0 ; 
bool __cdecl editor_should_resolve_block_references(const s_tag_field* block_field)
{
	return true;
}

//.text:0042E100 ; 
bool __cdecl editor_should_resolve_tag_reference(const s_tag_field* reference_field)
{
	return true;
}

//.text:0042E110 ; 
bool __cdecl editor_switch_zone_set(int32 zone_set_index)
{
	return scenario_switch_zone_set(zone_set_index);
}

//.text:0042E120 ; 
void __cdecl editor_update()
{
}

//.text:0042E130 ; 
bool __cdecl game_in_editor()
{
	return false;
}

//.text:0042E140 ; 
bool __cdecl game_in_editor_player_simulation()
{
	return false;
}

