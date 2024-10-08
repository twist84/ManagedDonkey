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
	_object_placement_zone_set_create_mode_unknown0 = 0,
	_object_placement_zone_set_create_mode_unknown1,
	_object_placement_zone_set_create_mode_unknown2,

	k_object_placement_zone_set_create_mode_count
};

struct s_game_non_bsp_zone_set;
struct c_scenario_resource_registry;

extern bool __cdecl object_definition_can_be_placed(long object_definition_index, long model_variant_index);
extern void __cdecl object_placement_create_active_zone_set_objects(e_object_placement_zone_set_create_mode create_mode);
extern void __cdecl object_placement_create_global_objects(e_game_mode game_mode, bool a2);
extern void __cdecl object_placement_dispose();
extern void __cdecl object_placement_dispose_from_old_map();
extern void __cdecl object_placement_dispose_from_old_structure_bsp(dword old_structure_bsp_mask);
extern void __cdecl object_placement_initialize();
extern void __cdecl object_placement_initialize_for_new_map();
extern void __cdecl object_placement_initialize_for_new_structure_bsp(dword new_structure_bsp_mask);
extern void __cdecl object_placement_prepare_for_new_zone_set(dword old_structure_bsp_mask, dword new_structure_bsp_mask);
extern void __cdecl object_placement_prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const* old_non_bsp_zone_set, s_game_non_bsp_zone_set const* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry);

