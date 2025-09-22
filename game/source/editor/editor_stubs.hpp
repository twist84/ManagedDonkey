#pragma once

#include "cseries/cseries.hpp"
#include "render/render_visibility_collection.hpp"

struct s_tag_field;

extern void __cdecl editor_change_pvs(const s_game_cluster_bit_vectors* old_cluster_activation, const s_game_cluster_bit_vectors* new_cluster_activation, bool local);
extern void __cdecl editor_dispose();
extern void __cdecl editor_dispose_from_old_map();
extern void __cdecl editor_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl editor_get_cluster_color(const s_cluster_reference* cluster_reference, real_argb_color* cluster_color);
extern const char* __cdecl editor_get_map_name();
extern void __cdecl editor_handle_tag_reload(bool reloading_scenario);
extern bool __cdecl sub_42E000();
extern void __cdecl editor_initialize();
extern void __cdecl editor_initialize_for_new_map();
extern void __cdecl editor_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern bool __cdecl editor_input_inhibited();
extern void __cdecl editor_orphan_object(e_object_type object_type, int32 scenario_datum_index);
extern void __cdecl editor_prepare_for_tag_reload(bool reloading_scenario);
extern void __cdecl editor_register_new_object(e_object_type object_type, int32 scenario_datum_index, int32 new_object_index);
extern void __cdecl editor_reset_script_referenced_blocks();
extern void __cdecl editor_save_progress();
extern bool __cdecl editor_should_render_object(int32 object_index);
extern bool __cdecl editor_should_render_transparent_geometry();
extern bool __cdecl editor_should_resolve_block_references(const s_tag_field* block_field);
extern bool __cdecl editor_should_resolve_tag_reference(const s_tag_field* reference_field);
extern bool __cdecl editor_switch_zone_set(int32 zone_set_index);
extern void __cdecl editor_update();
extern bool __cdecl game_in_editor();
extern bool __cdecl game_in_editor_player_simulation();

