#pragma once

class c_scenario_resource_registry;

struct s_game_non_bsp_zone_set;
struct s_location;
struct s_scenario_object;

union real_point3d;

struct s_object_placement_globals
{
	byte __data[0x320];
};
COMPILE_ASSERT(sizeof(s_object_placement_globals) == 0x320);

enum e_object_placement_zone_set_create_mode
{
	_object_placement_zone_set_create_always = 0,
	_object_placement_zone_set_create_default,
	_object_placement_zone_set_create_during_cinematic,

	k_number_of_object_placement_create_modes
};

extern bool __cdecl object_definition_can_be_placed(int32 object_definition_index, int32 model_variant_index);
extern bool __cdecl object_placement_attached_to_valid_parent(const s_scenario_object* scenario_object);
extern int32 __cdecl object_placement_bsp_index_key_from_bsp_indices(int32 first_bsp_index, int32 second_bsp_index);
extern void __cdecl object_placement_create_active_zone_set_objects(e_object_placement_zone_set_create_mode create_mode);
extern void __cdecl object_placement_create_active_zone_set_skies(int32 scenario_index, int32 active_bsp_mask);
extern void __cdecl object_placement_create_global_objects(e_game_mode game_mode, bool test_object_existence_before_creation);
extern void __cdecl object_placement_deactivate_on_zone_set_switch(int32 object_index);
extern void __cdecl object_placement_dispose();
extern void __cdecl object_placement_dispose_from_old_map();
extern void __cdecl object_placement_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern bool __cdecl object_placement_find_initial_location(uns32 active_bsp_mask, const real_point3d* center, real32 radius, s_location* out_location);
extern void __cdecl object_placement_initialize();
extern void __cdecl object_placement_initialize_for_new_map();
extern void __cdecl object_placement_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern void __cdecl object_placement_mark_name_on_create(int16 object_name_index);
extern void __cdecl object_placement_mark_object_on_delete(int32 object_index);
extern void __cdecl object_placement_prepare_for_new_zone_set(uns32 old_active_structure_bsp_mask, uns32 new_active_structure_bsp_mask);
extern void __cdecl object_placement_prepare_for_non_bsp_zone_set_switch(const s_game_non_bsp_zone_set* old_non_bsp_zone_set, const s_game_non_bsp_zone_set* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry);
extern void __cdecl object_placement_reset_from_teleport(int32 object_index);

