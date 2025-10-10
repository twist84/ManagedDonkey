#pragma once

#define HS_TYPE_IS_TAG_REFERENCE(HS_TYPE) (IN_RANGE_INCLUSIVE((HS_TYPE), _hs_type_effect, _hs_type_bink_definition))
#define HS_TYPE_IS_ENUM(HS_TYPE) (IN_RANGE_INCLUSIVE((HS_TYPE), FIRST_HS_ENUM_TYPE, LAST_HS_ENUM_TYPE))
#define HS_TYPE_IS_OBJECT(HS_TYPE) (IN_RANGE_INCLUSIVE((HS_TYPE), FIRST_HS_OBJECT_TYPE, LAST_HS_OBJECT_TYPE))
#define HS_TYPE_IS_OBJECT_NAME(HS_TYPE) (IN_RANGE_INCLUSIVE((HS_TYPE), FIRST_HS_OBJECT_NAME_TYPE, LAST_HS_OBJECT_NAME_TYPE))
#define HS_TYPE_IS_BUDGET_REFERENCE(HS_TYPE) (IN_RANGE_INCLUSIVE((HS_TYPE), FIRST_HS_BUDGET_REFERENCE_TYPE, LAST_HS_BUDGET_REFERENCE_TYPE))
#define hs_type_valid(HS_TYPE) (VALID_INDEX((HS_TYPE), NUMBER_OF_HS_NODE_TYPES))

struct hs_enum_definition
{
	int16 count;
	const char** identifiers;
};
static_assert(sizeof(hs_enum_definition) == 0x8);

struct hs_syntax_node;
struct hs_function_definition;
struct hs_global_external;
struct s_tag_block;
struct s_data_array;

extern const bool _hs_type_boolean_default;
extern const real32 _hs_type_real_default;
extern const int16 _hs_type_short_integer_default;
extern const int32 _hs_type_long_integer_default;
extern const int16 _hs_type_script_default;
extern const int32 _hs_type_string_id_default;
extern const int32 _hs_type_unit_seat_mapping_default;
extern const int16 _hs_type_trigger_volume_default;
extern const int16 _hs_type_cutscene_flag_default;
extern const int16 _hs_type_cutscene_camera_point_default;
extern const int16 _hs_type_cutscene_title_default;
extern const int16 _hs_type_cutscene_recording_default;
extern const int32 _hs_type_device_group_default;
extern const int32 _hs_type_ai_default;
extern const int16 _hs_type_ai_command_list_default;
extern const int16 _hs_type_ai_command_script_default;
extern const int16 _hs_type_ai_behavior_default;
extern const int16 _hs_type_ai_orders_default;
extern const int32 _hs_type_ai_line_default;
extern const int16 _hs_type_starting_profile_default;
extern const int16 _hs_type_conversation_default;
extern const int16 _hs_type_zone_set_default;
extern const int16 _hs_type_designer_zone_default;
extern const int32 _hs_type_point_ref_default;
extern const int32 _hs_type_style_default;
extern const int32 _hs_type_object_list_default;
extern const int32 _hs_type_folder_default;
extern const int32 _hs_type_sound_default;
extern const int32 _hs_type_looping_sound_default;
extern const int32 _hs_type_effect_default;
extern const int32 _hs_type_damage_default;
extern const int32 _hs_type_animation_graph_default;
extern const int32 _hs_type_damage_effect_default;
extern const int32 _hs_type_object_definition_default;
extern const int32 _hs_type_bitmap_default;
extern const int32 _hs_type_shader_default;
extern const int32 _hs_type_render_model_definition_default;
extern const int32 _hs_type_structure_bsp_definition_default;
extern const int32 _hs_type_structure_lightmap_definition_default;
extern const int32 _hs_type_cinematic_definition_default;
extern const int32 _hs_type_cinematic_scene_definition_default;
extern const int32 _hs_type_bink_definition_default;
extern const int32 _hs_type_any_tag_default;
extern const int16 _hs_type_enum_game_difficulty_default;
extern const int16 _hs_type_enum_team_default;
extern const int16 _hs_type_enum_mp_team_default;
extern const int16 _hs_type_enum_controller_default;
extern const int16 _hs_type_enum_button_preset_default;
extern const int16 _hs_type_enum_joystick_preset_default;
extern const int16 _hs_type_enum_player_character_type_default;
extern const int16 _hs_type_enum_voice_output_setting_default;
extern const int16 _hs_type_enum_subtitle_setting_default;
extern const int16 _hs_type_enum_actor_type_default;
extern const int16 _hs_type_enum_model_state_default;
extern const int16 _hs_type_enum_event_default;
extern const int16 _hs_type_enum_character_physics_override_default;
extern const int16 _hs_type_enum_primary_skull_default;
extern const int16 _hs_type_enum_secondary_skull_default;
extern const int16 _hs_type_object_name_default;
extern const int32 _hs_type_object_default;
extern const int32 _hs_type_unit_default;
extern const int32 _hs_type_vehicle_default;
extern const int32 _hs_type_weapon_default;
extern const int32 _hs_type_device_default;
extern const int32 _hs_type_scenery_default;
extern const int32 _hs_type_effect_scenery_default;
extern const int32 _hs_type_cinematic_lightprobe_default;
extern const int32 _hs_type_budget_reference_animation_graph_default;
extern const int32 _hs_type_budget_reference_looping_sound_default;
extern const int32 _hs_type_budget_reference_sound_default;
extern const tag hs_budget_reference_type_group_tags[NUMBER_OF_HS_BUDGET_REFERENCE_TYPES];
extern const int16 hs_type_sizes[NUMBER_OF_HS_NODE_TYPES];
extern const int16 hs_object_type_masks[NUMBER_OF_HS_OBJECT_TYPES];
extern const int32 hs_tag_reference_type_group_tags[NUMBER_OF_HS_TAG_REFERENCE_TYPES];
extern const hs_enum_definition hs_enum_table[NUMBER_OF_HS_ENUM_TYPES];
extern const char* const _hs_type_string_default;
extern const char* const hs_type_names[NUMBER_OF_HS_NODE_TYPES];
extern const char* const hs_script_type_names[NUMBER_OF_HS_SCRIPT_TYPES];
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
extern real64 __cdecl hs_ticks_to_seconds(int32 hs_ticks);
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

