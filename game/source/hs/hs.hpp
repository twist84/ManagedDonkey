#pragma once

#define HS_TYPE_IS_TAG_REFERENCE(_hs_type) ((_hs_type) >= _hs_type_effect && (_hs_type) <= _hs_type_bink_definition)
#define HS_TYPE_IS_ENUM(_hs_type) ((_hs_type) >= _hs_type_game_difficulty && (_hs_type) <= _hs_type_secondary_skull)
#define HS_TYPE_IS_OBJECT(_hs_type) ((_hs_type) >= _hs_type_object && (_hs_type) <= _hs_type_effect_scenery)
#define HS_TYPE_IS_OBJECT_NAME(_hs_type) ((_hs_type) >= _hs_type_object_name && (_hs_type) <= _hs_type_effect_scenery_name)
#define HS_TYPE_IS_BUDGET_REFERENCE(_hs_type) ((_hs_type) >= _hs_type_animation_budget_reference && (_hs_type) <= _hs_type_sound_budget_reference)
#define hs_type_valid(_hs_type) ((_hs_type) >= _hs_type_void && (_hs_type) < k_hs_type_count)

struct hs_enum_definition
{
	short count;
	char const* const* names;
};
static_assert(sizeof(hs_enum_definition) == 0x8);

struct hs_syntax_node;
struct hs_function_definition;
struct hs_function_definition_debug;
struct hs_global_external;
struct hs_global_external_debug;
struct s_tag_block;
struct s_data_array;

extern bool const _hs_type_boolean_default;
extern real32 const _hs_type_real_default;
extern short const _hs_type_short_integer_default;
extern long const _hs_type_long_integer_default;
extern short const _hs_type_script_default;
extern long const _hs_type_string_id_default;
extern long const _hs_type_unit_seat_mapping_default;
extern short const _hs_type_trigger_volume_default;
extern short const _hs_type_cutscene_flag_default;
extern short const _hs_type_cutscene_camera_point_default;
extern short const _hs_type_cutscene_title_default;
extern short const _hs_type_cutscene_recording_default;
extern long const _hs_type_device_group_default;
extern long const _hs_type_ai_default;
extern short const _hs_type_ai_command_list_default;
extern short const _hs_type_ai_command_script_default;
extern short const _hs_type_ai_behavior_default;
extern short const _hs_type_ai_orders_default;
extern long const _hs_type_ai_line_default;
extern short const _hs_type_starting_profile_default;
extern short const _hs_type_conversation_default;
extern short const _hs_type_zone_set_default;
extern short const _hs_type_designer_zone_default;
extern long const _hs_type_point_ref_default;
extern long const _hs_type_style_default;
extern long const _hs_type_object_list_default;
extern long const _hs_type_folder_default;
extern long const _hs_type_sound_default;
extern long const _hs_type_looping_sound_default;
extern long const _hs_type_effect_default;
extern long const _hs_type_damage_default;
extern long const _hs_type_animation_graph_default;
extern long const _hs_type_damage_effect_default;
extern long const _hs_type_object_definition_default;
extern long const _hs_type_bitmap_default;
extern long const _hs_type_shader_default;
extern long const _hs_type_render_model_definition_default;
extern long const _hs_type_structure_bsp_definition_default;
extern long const _hs_type_structure_lightmap_definition_default;
extern long const _hs_type_cinematic_definition_default;
extern long const _hs_type_cinematic_scene_definition_default;
extern long const _hs_type_bink_definition_default;
extern long const _hs_type_any_tag_default;
extern short const _hs_type_enum_game_difficulty_default;
extern short const _hs_type_enum_team_default;
extern short const _hs_type_enum_mp_team_default;
extern short const _hs_type_enum_controller_default;
extern short const _hs_type_enum_button_preset_default;
extern short const _hs_type_enum_joystick_preset_default;
extern short const _hs_type_enum_player_character_type_default;
extern short const _hs_type_enum_voice_output_setting_default;
extern short const _hs_type_enum_subtitle_setting_default;
extern short const _hs_type_enum_actor_type_default;
extern short const _hs_type_enum_model_state_default;
extern short const _hs_type_enum_event_default;
extern short const _hs_type_enum_character_physics_override_default;
extern short const _hs_type_enum_primary_skull_default;
extern short const _hs_type_enum_secondary_skull_default;
extern short const _hs_type_object_name_default;
extern long const _hs_type_object_default;
extern long const _hs_type_unit_default;
extern long const _hs_type_vehicle_default;
extern long const _hs_type_weapon_default;
extern long const _hs_type_device_default;
extern long const _hs_type_scenery_default;
extern long const _hs_type_effect_scenery_default;
extern long const _hs_type_cinematic_lightprobe_default;
extern long const _hs_type_budget_reference_animation_graph_default;
extern long const _hs_type_budget_reference_looping_sound_default;
extern long const _hs_type_budget_reference_sound_default;
extern tag const hs_budget_reference_type_group_tags[k_hs_type_budget_reference_count];
extern short const hs_type_sizes[k_hs_type_count];
extern short const hs_object_type_masks[k_hs_type_object_count];
extern long const hs_tag_reference_type_group_tags[k_hs_tag_reference_type_count];
extern char const* const hs_type_names[k_hs_type_count];
extern char const* const hs_script_type_names[k_hs_script_type_count];
extern hs_enum_definition const hs_enum_table[k_hs_type_enum_count];
extern s_data_array*& g_hs_syntax_data;

extern hs_function_definition const* hs_function_get(short function_index);
extern hs_function_definition_debug const* hs_function_get_debug(short function_index);
extern hs_syntax_node* __cdecl hs_syntax_get(long datum_index);
extern void __cdecl hs_dispose();
extern void __cdecl hs_dispose_from_old_map();
extern short __cdecl hs_find_script_by_name(char const* name, short num_arguments);
extern short __cdecl hs_global_get_type(short global_index);
extern void __cdecl hs_initialize();
extern void __cdecl hs_initialize_for_new_map();
extern void __cdecl hs_node_gc();
extern void __cdecl hs_update();
extern short hs_find_function_by_name(char const* name, short parameter_count);
extern short hs_script_find_parameter_by_name(long script_index, char const* name);
extern hs_global_external* hs_global_external_get(short global_index);
extern hs_global_external_debug* hs_global_external_get_debug(short global_index);
extern short hs_find_global_by_name(char const* name);
extern char const* hs_global_get_name(short global_index);

extern void hs_tokens_enumerate_add_string(char const* string);
extern void hs_enumerate_from_string_list(char const* const* string_list, short starting_index, short count);
extern void hs_enumerate_block_data(s_tag_block const* block, short offset, long size);
extern void hs_enumerate_scenario_data(short scenario_offset, short block_offset, long block_size);
extern void hs_enumerate_block_data_string_id(s_tag_block const* block, short offset, long size);
extern void hs_enumerate_scenario_data_string_id(short scenario_offset, short block_offset, long block_size);
extern short hs_tokens_enumerate(char const* token, long type_mask, char const** matching_items, short matching_item_count);

extern bool __cdecl sort_by_found_index(long look_inside1, long look_inside2, void const* look_for);
extern void __cdecl hs_enumerate_special_form_names(void);
extern void __cdecl hs_enumerate_script_type_names(void);
extern void __cdecl hs_enumerate_type_names(void);
extern void __cdecl hs_enumerate_function_names(void);
extern void __cdecl hs_enumerate_script_names(void);
extern void __cdecl hs_enumerate_variable_names(void);
extern void __cdecl hs_enumerate_ai_names(void);
extern void __cdecl hs_enumerate_ai_command_list_names(void);
extern void __cdecl hs_enumerate_ai_command_script_names(void);
extern void __cdecl hs_enumerate_ai_behavior_names(void);
extern void __cdecl hs_enumerate_starting_profile_names(void);
extern void __cdecl hs_enumerate_conversation_names(void);
extern void __cdecl hs_enumerate_object_names(void);
extern void __cdecl hs_enumerate_trigger_volume_names(void);
extern void __cdecl hs_enumerate_cutscene_flag_names(void);
extern void __cdecl hs_enumerate_cutscene_camera_point_names(void);
extern void __cdecl hs_enumerate_cutscene_title_names(void);
extern void __cdecl hs_enumerate_cutscene_recording_names(void);
extern void __cdecl hs_enumerate_enum_skull_names(void);

