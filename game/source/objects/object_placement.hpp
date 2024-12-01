#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

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
struct c_scenario_resource_registry;

extern bool __cdecl object_definition_can_be_placed(long object_definition_index, long model_variant_index);
extern void __cdecl object_placement_create_active_zone_set_objects(e_object_placement_zone_set_create_mode create_mode);
extern void __cdecl object_placement_create_global_objects(e_game_mode game_mode, bool test_object_existence_before_creation);
extern void __cdecl object_placement_dispose();
extern void __cdecl object_placement_dispose_from_old_map();
extern void __cdecl object_placement_dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask);
extern void __cdecl object_placement_initialize();
extern void __cdecl object_placement_initialize_for_new_map();
extern void __cdecl object_placement_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask);
extern void __cdecl object_placement_prepare_for_new_zone_set(dword old_active_structure_bsp_mask, dword new_active_structure_bsp_mask);
extern void __cdecl object_placement_prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const* old_non_bsp_zone_set, s_game_non_bsp_zone_set const* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry);

