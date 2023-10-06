#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

#define HS_TYPE_IS_TAG_REFERENCE(_hs_type) ((_hs_type) >= _hs_type_effect && (_hs_type) <= _hs_type_bink_definition)
#define HS_TYPE_IS_ENUM(_hs_type) ((_hs_type) >= _hs_type_game_difficulty && (_hs_type) <= _hs_type_secondary_skull)
#define HS_TYPE_IS_OBJECT(_hs_type) ((_hs_type) >= _hs_type_object && (_hs_type) <= _hs_type_effect_scenery)
#define HS_TYPE_IS_OBJECT_NAME(_hs_type) ((_hs_type) >= _hs_type_object_name && (_hs_type) <= _hs_type_effect_scenery_name)
#define HS_TYPE_IS_BUDGET_REFERENCE(_hs_type) ((_hs_type) >= _hs_type_animation_budget_reference && (_hs_type) <= _hs_type_sound_budget_reference)
#define hs_type_valid(_hs_type) ((_hs_type) >= _hs_type_void && (_hs_type) < k_hs_type_count)

enum e_hs_type
{
	_hs_unparsed = 0,
	_hs_special_form,
	_hs_function_name,
	_hs_passthrough,

	// actual types start
	_hs_type_void,
	_hs_type_boolean,
	_hs_type_real,
	_hs_type_short_integer,
	_hs_type_long_integer,
	_hs_type_string,
	_hs_type_script,
	_hs_type_string_id,
	_hs_type_unit_seat_mapping,
	_hs_type_trigger_volume,
	_hs_type_cutscene_flag,
	_hs_type_cutscene_camera_point,
	_hs_type_cutscene_title,
	_hs_type_cutscene_recording,
	_hs_type_device_group,
	_hs_type_ai,
	_hs_type_ai_command_list,
	_hs_type_ai_command_script,
	_hs_type_ai_behavior,
	_hs_type_ai_orders,
	_hs_type_ai_line,
	_hs_type_starting_profile,
	_hs_type_conversation,
	_hs_type_zone_set,
	_hs_type_designer_zone,
	_hs_type_point_ref,
	_hs_type_style,
	_hs_type_object_list,
	_hs_type_folder,

	// tag reference type
	_hs_type_sound,
	_hs_type_effect,
	_hs_type_damage,
	_hs_type_looping_sound,
	_hs_type_animation_graph,
	_hs_type_damage_effect,
	_hs_type_object_definition,
	_hs_type_bitmap,
	_hs_type_shader,
	_hs_type_render_model,
	_hs_type_structure_definition,
	_hs_type_lightmap_definition,
	_hs_type_cinematic_definition,
	_hs_type_cinematic_scene_definition,
	_hs_type_bink_definition,
	_hs_type_any_tag,
	_hs_type_any_tag_not_resolving,

	// enum
	_hs_type_game_difficulty,
	_hs_type_team,
	_hs_type_mp_team,
	_hs_type_controller,
	_hs_type_button_preset,
	_hs_type_joystick_preset,
	_hs_type_player_character_type,
	_hs_type_voice_output_setting,
	_hs_type_voice_mask,
	_hs_type_subtitle_setting,
	_hs_type_actor_type,
	_hs_type_model_state,
	_hs_type_event,
	_hs_type_character_physics,
	_hs_type_primary_skull,
	_hs_type_secondary_skull,

	// object
	_hs_type_object,
	_hs_type_unit,
	_hs_type_vehicle,
	_hs_type_weapon,
	_hs_type_device,
	_hs_type_scenery,
	_hs_type_effect_scenery,

	// object name
	_hs_type_object_name,
	_hs_type_unit_name,
	_hs_type_vehicle_name,
	_hs_type_weapon_name,
	_hs_type_device_name,
	_hs_type_scenery_name,
	_hs_type_effect_scenery_name,

	_hs_type_cinematic_lightprobe,

	// budget reference
	_hs_type_animation_budget_reference,
	_hs_type_looping_sound_budget_reference,
	_hs_type_sound_budget_reference,

	k_hs_type_count,

	k_hs_tag_reference_type_count = _hs_type_any_tag - _hs_type_sound + 1,
	k_hs_type_enum_count = _hs_type_secondary_skull - _hs_type_game_difficulty + 1,
	k_hs_type_object_count = _hs_type_effect_scenery - _hs_type_object + 1,
	k_hs_type_object_name_count = _hs_type_effect_scenery_name - _hs_type_object_name + 1,
	k_hs_type_budget_reference_count = _hs_type_sound_budget_reference - _hs_type_animation_budget_reference + 1
};
static_assert(k_hs_type_object_count == k_hs_type_object_name_count);
#define NUMBER_OF_HS_OBJECT_TYPES k_hs_type_object_count

enum e_hs_script_type
{
	_hs_script_type_startup = 0,
	_hs_script_type_dormant,
	_hs_script_type_continuous,
	_hs_script_type_static,
	_hs_script_type_command_script,
	_hs_script_type_stub,

	k_hs_script_type_count
};

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

extern bool const _hs_type_boolean_default;
extern real const _hs_type_real_default;
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
extern c_typed_data_array<hs_syntax_node>*& g_hs_syntax_data;

extern hs_function_definition const* hs_function_get(short function_index);
extern hs_function_definition_debug const* hs_function_get_debug(short function_index);
extern hs_syntax_node* __cdecl hs_syntax_get(long datum_index);
extern short hs_find_script_by_name(char const* name, short parameter_count);
extern short hs_find_function_by_name(char const* name, short parameter_count);
extern short hs_script_find_parameter_by_name(long script_index, char const* name);
extern hs_global_external* hs_global_external_get(short global_index);
extern hs_global_external_debug* hs_global_external_get_debug(short global_index);
extern short hs_find_global_by_name(char const* name);
extern char const* hs_global_get_name(short global_index);
extern short hs_global_get_type(short global_index);

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

