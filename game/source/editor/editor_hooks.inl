#include "editor/editor_stubs.hpp"

c_hook_call<0x0042E040> editor_input_inhibited_hook({ .pointer = editor_input_inhibited });
c_data_patch<0x0042E040 + 5> editor_input_inhibited_patch({ 0xC3 });

c_hook_call<0x00B30F75> editor_orphan_object_hook({ .pointer = editor_orphan_object });
c_hook_call<0x00B3121B> editor_register_new_object_hook({ .pointer = editor_register_new_object });
c_hook_call<0x0051C15D> editor_save_progress_hook({ .pointer = editor_save_progress });
c_hook_call<0x006E5025> editor_should_render_transparent_geometry_hook({ .pointer = editor_should_render_transparent_geometry });
c_hook_call<0x00507290> editor_switch_zone_set_hook({ .pointer = editor_switch_zone_set });
c_hook_call<0x00533202> editor_update_hook0({ .pointer = editor_update });
c_hook_call<0x00533800> editor_update_hook1({ .pointer = editor_update });

c_hook_call<0x00533800> game_in_editor_hook({ .pointer = game_in_editor });
c_data_patch<0x0042E130 + 5> game_in_editor_patch({ 0xC3 });

// `g_game_systems` patches
c_data_patch<0x01656AC0> editor_initialize_patch(editor_initialize);
c_data_patch<0x01656AC4> editor_dispose_patch(editor_dispose);
c_data_patch<0x01656AC8> editor_initialize_for_new_map_patch(editor_initialize_for_new_map);
c_data_patch<0x01656ACC> editor_dispose_from_old_map_patch(editor_dispose_from_old_map);
c_data_patch<0x01656AD4> editor_initialize_for_new_structure_bsp_patch(editor_initialize_for_new_structure_bsp);
c_data_patch<0x01656AD8> editor_dispose_from_old_structure_bsp_patch(editor_dispose_from_old_structure_bsp);
c_data_patch<0x01656ADC> editor_change_pvs_patch(editor_change_pvs);