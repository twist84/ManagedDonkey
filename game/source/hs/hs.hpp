#pragma once

#define HS_TYPE_IS_TAG_REFERENCE(_hs_type) ((_hs_type) >= _hs_type_effect && (_hs_type) <= _hs_type_bink_definition)
#define HS_TYPE_IS_ENUM(_hs_type) ((_hs_type) >= _hs_type_enum_game_difficulty && (_hs_type) <= _hs_type_enum_secondary_skull)
#define HS_TYPE_IS_OBJECT(_hs_type) ((_hs_type) >= _hs_type_object && (_hs_type) <= _hs_type_effect_scenery)
#define HS_TYPE_IS_OBJECT_NAME(_hs_type) ((_hs_type) >= _hs_type_object_name && (_hs_type) <= _hs_type_effect_scenery_name)
#define HS_TYPE_IS_BUDGET_REFERENCE(_hs_type) ((_hs_type) >= _hs_type_budget_reference_animation_graph && (_hs_type) <= _hs_type_budget_reference_sound)
#define hs_type_valid(_hs_type) ((_hs_type) >= _hs_type_void && (_hs_type) < k_hs_type_count)

struct hs_enum_definition
{
	int16 count;
	const char* const* names;
};
static_assert(sizeof(hs_enum_definition) == 0x8);

struct hs_syntax_node;
struct hs_function_definition_release;
struct hs_function_definition;
struct hs_global_external_release;
struct hs_global_external;
struct s_tag_block;
struct s_data_array;

extern bool const _hs_type_boolean_default;
extern real32 const _hs_type_real_default;
extern int16 const _hs_type_short_integer_default;
extern int32 const _hs_type_long_integer_default;
extern int16 const _hs_type_script_default;
extern int32 const _hs_type_string_id_default;
extern int32 const _hs_type_unit_seat_mapping_default;
extern int16 const _hs_type_trigger_volume_default;
extern int16 const _hs_type_cutscene_flag_default;
extern int16 const _hs_type_cutscene_camera_point_default;
extern int16 const _hs_type_cutscene_title_default;
extern int16 const _hs_type_cutscene_recording_default;
extern int32 const _hs_type_device_group_default;
extern int32 const _hs_type_ai_default;
extern int16 const _hs_type_ai_command_list_default;
extern int16 const _hs_type_ai_command_script_default;
extern int16 const _hs_type_ai_behavior_default;
extern int16 const _hs_type_ai_orders_default;
extern int32 const _hs_type_ai_line_default;
extern int16 const _hs_type_starting_profile_default;
extern int16 const _hs_type_conversation_default;
extern int16 const _hs_type_zone_set_default;
extern int16 const _hs_type_designer_zone_default;
extern int32 const _hs_type_point_ref_default;
extern int32 const _hs_type_style_default;
extern int32 const _hs_type_object_list_default;
extern int32 const _hs_type_folder_default;
extern int32 const _hs_type_sound_default;
extern int32 const _hs_type_looping_sound_default;
extern int32 const _hs_type_effect_default;
extern int32 const _hs_type_damage_default;
extern int32 const _hs_type_animation_graph_default;
extern int32 const _hs_type_damage_effect_default;
extern int32 const _hs_type_object_definition_default;
extern int32 const _hs_type_bitmap_default;
extern int32 const _hs_type_shader_default;
extern int32 const _hs_type_render_model_definition_default;
extern int32 const _hs_type_structure_bsp_definition_default;
extern int32 const _hs_type_structure_lightmap_definition_default;
extern int32 const _hs_type_cinematic_definition_default;
extern int32 const _hs_type_cinematic_scene_definition_default;
extern int32 const _hs_type_bink_definition_default;
extern int32 const _hs_type_any_tag_default;
extern int16 const _hs_type_enum_game_difficulty_default;
extern int16 const _hs_type_enum_team_default;
extern int16 const _hs_type_enum_mp_team_default;
extern int16 const _hs_type_enum_controller_default;
extern int16 const _hs_type_enum_button_preset_default;
extern int16 const _hs_type_enum_joystick_preset_default;
extern int16 const _hs_type_enum_player_character_type_default;
extern int16 const _hs_type_enum_voice_output_setting_default;
extern int16 const _hs_type_enum_subtitle_setting_default;
extern int16 const _hs_type_enum_actor_type_default;
extern int16 const _hs_type_enum_model_state_default;
extern int16 const _hs_type_enum_event_default;
extern int16 const _hs_type_enum_character_physics_override_default;
extern int16 const _hs_type_enum_primary_skull_default;
extern int16 const _hs_type_enum_secondary_skull_default;
extern int16 const _hs_type_object_name_default;
extern int32 const _hs_type_object_default;
extern int32 const _hs_type_unit_default;
extern int32 const _hs_type_vehicle_default;
extern int32 const _hs_type_weapon_default;
extern int32 const _hs_type_device_default;
extern int32 const _hs_type_scenery_default;
extern int32 const _hs_type_effect_scenery_default;
extern int32 const _hs_type_cinematic_lightprobe_default;
extern int32 const _hs_type_budget_reference_animation_graph_default;
extern int32 const _hs_type_budget_reference_looping_sound_default;
extern int32 const _hs_type_budget_reference_sound_default;
extern tag const hs_budget_reference_type_group_tags[k_hs_type_budget_reference_count];
extern int16 const hs_type_sizes[k_hs_type_count];
extern int16 const hs_object_type_masks[k_hs_type_object_count];
extern int32 const hs_tag_reference_type_group_tags[k_hs_tag_reference_type_count];
extern const char* const _hs_type_string_default;
extern const char* const hs_type_names[k_hs_type_count];
extern const char* const hs_script_type_names[NUMBER_OF_HS_SCRIPT_TYPES];
extern hs_enum_definition const hs_enum_table[k_hs_type_enum_count];
extern s_data_array*& g_hs_syntax_data;
extern bool& g_recompile_scripts;

extern bool __cdecl char_is_delimiter(char character, const char* delimiter_string);
extern void __cdecl hs_dispose();
extern void __cdecl hs_dispose_from_old_map();
extern int16 __cdecl hs_find_script_by_name(const char* name, int16 num_arguments);
extern int16 __cdecl hs_global_get_type(int16 global_designator);
extern void __cdecl hs_initialize();
extern void __cdecl hs_initialize_for_new_map();
extern void __cdecl hs_initialize_for_new_map(bool force_recompile, bool verbose);
extern void __cdecl hs_node_gc();
extern void __cdecl hs_recompile();
extern void __cdecl hs_reset_time(int32 previous_time);
extern bool __cdecl hs_scenario_postprocess(bool force_recompile, bool fail_on_error, bool verbose);
extern void __cdecl hs_update();
extern int16 hs_find_function_by_name(const char* name, int16 parameter_count);
extern int16 hs_script_find_parameter_by_name(int32 script_index, const char* name);
extern hs_global_external* hs_global_external_get(int16 global_index);
extern int16 hs_find_global_by_name(const char* name);
extern const char* hs_global_get_name(int16 global_designator);
extern void resize_scenario_syntax_data(int32 count);
extern void hs_doc();
extern const hs_function_definition* hs_function_get(int16 function_index);
extern void hs_get_function_documentation_string(int16 function_index, char* buffer, int32 buffer_size);
extern void hs_get_function_parameters_string(int16 function_index, char* buffer, int32 buffer_size);
extern void hs_help(const char* function_name);

extern void hs_tokens_enumerate_add_string(const char* string);
extern void hs_enumerate_from_string_list(const char* const* string_list, int16 starting_index, int16 count);
extern void hs_enumerate_block_data(const s_tag_block* block, int16 offset, int32 size);
extern void hs_enumerate_scenario_data(int16 scenario_offset, int16 block_offset, int32 block_size);
extern void hs_enumerate_block_data_string_id(const s_tag_block* block, int16 offset, int32 size);
extern void hs_enumerate_scenario_data_string_id(int16 scenario_offset, int16 block_offset, int32 block_size);
extern int16 hs_tokens_enumerate(const char* token, int32 type_mask, const char** matching_items, int16 matching_item_count);

extern bool __cdecl sort_by_found_index(int32 look_inside1, int32 look_inside2, const void* look_for);
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

