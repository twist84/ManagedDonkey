#pragma once

#include "cseries/cseries.hpp"
#include "render/render_visibility_collection.hpp"

enum e_object_type;

extern void __cdecl editor_change_pvs(s_game_cluster_bit_vectors const* a1, s_game_cluster_bit_vectors const* a2, bool a3);
extern void __cdecl editor_dispose();
extern void __cdecl editor_dispose_from_old_map();
extern void __cdecl editor_dispose_from_old_structure_bsp(dword structure_bsp_mask);
extern void __cdecl editor_get_cluster_color(s_cluster_reference const* cluster_reference, real_argb_color* cluster_color);
extern char const* __cdecl editor_get_map_name();
extern bool __cdecl sub_42E000();
extern void __cdecl editor_initialize();
extern void __cdecl editor_initialize_for_new_map();
extern void __cdecl editor_initialize_for_new_structure_bsp(dword a1);
extern bool __cdecl editor_input_inhibited();
extern void __cdecl editor_orphan_object(e_object_type type, long a2);
extern void __cdecl editor_register_new_object(e_object_type type, long a2, long a3);
extern void __cdecl editor_save_progress();
extern bool __cdecl editor_should_render_object(long object_index);
extern bool __cdecl editor_should_render_transparent_geometry();
extern bool __cdecl editor_switch_zone_set(long zoneset_index);
extern void __cdecl editor_update();
extern bool __cdecl game_in_editor();

