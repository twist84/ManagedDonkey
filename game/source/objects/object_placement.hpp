#pragma once

struct s_object_placement_globals
{
	byte __data[0x320];
};
static_assert(sizeof(s_object_placement_globals) == 0x320);

enum e_object_placement_zone_set_create_mode
{
	_object_placement_zone_set_create_always = 0,
	_object_placement_zone_set_create_default,
	_object_placement_zone_set_create_during_cinematic,

	k_number_of_object_placement_create_modes
};

struct s_game_non_bsp_zone_set;
class c_scenario_resource_registry;

extern bool __cdecl object_definition_can_be_placed(int32 object_definition_index, int32 model_variant_index);
extern void __cdecl object_placement_create_active_zone_set_objects(e_object_placement_zone_set_create_mode create_mode);
extern void __cdecl object_placement_create_global_objects(e_game_mode game_mode, bool test_object_existence_before_creation);
extern void __cdecl object_placement_dispose();
extern void __cdecl object_placement_dispose_from_old_map();
extern void __cdecl object_placement_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl object_placement_initialize();
extern void __cdecl object_placement_initialize_for_new_map();
extern void __cdecl object_placement_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern void __cdecl object_placement_mark_name_on_create(int16 object_name_index);
extern void __cdecl object_placement_mark_object_on_delete(int32 object_index);
extern void __cdecl object_placement_prepare_for_new_zone_set(uns32 old_active_structure_bsp_mask, uns32 new_active_structure_bsp_mask);
extern void __cdecl object_placement_prepare_for_non_bsp_zone_set_switch(const s_game_non_bsp_zone_set* old_non_bsp_zone_set, const s_game_non_bsp_zone_set* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry);
extern void __cdecl object_placement_reset_from_teleport(int32 object_index);

