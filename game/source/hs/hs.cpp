#include "hs/hs.hpp"

#include "cseries/cseries_events.hpp"
#include "hs/hs_scenario_definitions.hpp"
#include "objects/objects.hpp"
#include "scenario/scenario.hpp"
#include "shell/shell.hpp"

//REFERENCE_DECLARE_ARRAY(, short const, hs_type_sizes, k_hs_type_count);
//REFERENCE_DECLARE_ARRAY(0x, short const, hs_object_type_masks, k_hs_type_object_count);
//REFERENCE_DECLARE_ARRAY(0x, long const, hs_tag_reference_type_group_tags, k_hs_tag_reference_type_count);
//REFERENCE_DECLARE_ARRAY(0x, hs_enum_definition const, hs_enum_table, k_hs_type_enum_count);
//REFERENCE_DECLARE_ARRAY(0x, char const* const, hs_type_names, k_hs_type_count);
//REFERENCE_DECLARE_ARRAY(0x, char const* const, hs_script_type_names, k_hs_script_type_count);
REFERENCE_DECLARE(0x024B06D4, c_typed_data_array<hs_syntax_node>*, g_hs_syntax_data);

hs_syntax_node* __cdecl hs_syntax_get(long expression_index)
{
	if (DATUM_INDEX_TO_ABSOLUTE_INDEX(expression_index) > g_hs_syntax_data->maximum_count)
		return &g_hs_syntax_data->data[DATUM_INDEX_TO_ABSOLUTE_INDEX(expression_index)];

	return nullptr;

	//return INVOKE(0x00598A10, hs_syntax_get, expression_index);
}

short hs_find_script_by_name(char const* name, short parameter_index)
{
	if (global_scenario_index_get() != NONE)
	{
		c_typed_tag_block<hs_script>& scripts = global_scenario_get()->scripts;
		for (long script_index = 0; script_index < scripts.count(); script_index++)
		{
			hs_script& script = scripts[script_index];
			if (script.name.equals(name) && parameter_index == NONE || parameter_index == script.parameters.count())
				return static_cast<short>(script_index);
		}
	}

	return NONE;
}

// 0166D710
short const hs_type_sizes[k_hs_type_count]
{
	0, // unparsed
	0, // special_form
	0, // function_name
	0, // passthrough

	0, // void
	1, // boolean
	4, // real
	2, // short
	4, // long
	4, // string
	2, // script
	4, // string_id

	4, // unit_seat_mapping
	2, // trigger_volume
	2, // cutscene_flag
	2, // cutscene_camera_point
	2, // cutscene_title
	2, // cutscene_recording
	4, // device_group
	4, // ai
	2, // ai_command_list
	2, // ai_command_script
	2, // ai_behavior
	2, // ai_orders
	4, // ai_line
	2, // starting_profile
	2, // conversation
	2, // zone_set
	2, // designer_zone
	4, // point_reference
	4, // style
	4, // object_list
	4, // folder

	4, // sound
	4, // effect
	4, // damage
	4, // looping_sound
	4, // animation_graph
	4, // damage_effect
	4, // object_definition
	4, // bitmap
	4, // shader
	4, // render_model
	4, // structure_definition
	4, // lightmap_definition
	4, // cinematic_definition
	4, // cinematic_scene_definition
	4, // bink_definition
	4, // any_tag
	4, // any_tag_not_resolving

	2, // game_difficulty
	2, // team
	2, // mp_team
	2, // controller
	2, // button_preset
	2, // joystick_preset
	2, // player_character_type
	2, // voice_output_setting
	2, // voice_mask
	2, // subtitle_setting
	2, // actor_type
	2, // model_state
	2, // event
	2, // character_physics
	2, // primary_skull
	2, // secondary_skull

	4, // object
	4, // unit
	4, // vehicle
	4, // weapon
	4, // device
	4, // scenery
	4, // effect_scenery

	2, // object_name
	2, // unit_name
	2, // vehicle_name
	2, // weapon_name
	2, // device_name
	2, // scenery_name
	2, // effect_scenery_name

	4, // cinematic_lightprobe

	4, // animation_budget_reference
	4, // looping_sound_budget_reference
	4  // sound_budget_reference
};

// 0166D7B8
short const hs_object_type_masks[k_hs_type_object_count]
{
	_object_mask_object,        // object
	_object_mask_unit,          // unit
	_object_mask_vehicle,       // vehicle
	_object_mask_weapon,        // weapon
	_object_mask_device,        // device
	_object_mask_scenery,       // scenery
	_object_mask_effect_scenery // effect_scenery
};

// 0166D7C8
long const hs_tag_reference_type_group_tags[k_hs_tag_reference_type_count]
{
	SOUND_TAG,                            // sound
	EFFECT_TAG,                           // effect
	DAMAGE_EFFECT_TAG,                    // damage
	SOUND_LOOPING_TAG,                    // looping_sound
	MODEL_ANIMATION_GRAPH_TAG,            // animation_graph
	DAMAGE_EFFECT_TAG,                    // damage_effect
	OBJECT_TAG,                           // object_definition
	BITMAP_TAG,                           // bitmap
	RENDER_METHOD_TAG,                    // shader
	RENDER_MODEL_TAG,                     // render_model
	SCENARIO_STRUCTURE_BSP_TAG,           // structure_definition
	SCENARIO_STRUCTURE_LIGHTING_INFO_TAG, // lightmap_definition
	CINEMATIC_TAG,                        // cinematic_definition
	CINEMATIC_SCENE_TAG,                  // cinematic_scene_definition
	BINK_TAG,                             // bink_definition
	NONE                                  // any_tag, any_tag_not_resolving
};

// 0166D808
hs_enum_definition const hs_enum_table[k_hs_type_enum_count]
{
#define MAKE_ENUM_TABLE_ENTRY(name){ NUMBEROF((name)), (name) }
	MAKE_ENUM_TABLE_ENTRY(global_campaign_difficulty_level_names),
	MAKE_ENUM_TABLE_ENTRY(global_campaign_team_names),
	MAKE_ENUM_TABLE_ENTRY(global_multiplayer_team_names),
	MAKE_ENUM_TABLE_ENTRY(global_controller_index_names),
	MAKE_ENUM_TABLE_ENTRY(global_button_preset_names),
	MAKE_ENUM_TABLE_ENTRY(global_joystick_preset_names),
	MAKE_ENUM_TABLE_ENTRY(global_player_model_choice_names),
	MAKE_ENUM_TABLE_ENTRY(global_voice_output_setting_names),
	MAKE_ENUM_TABLE_ENTRY(global_voice_mask_names),
	MAKE_ENUM_TABLE_ENTRY(global_subtitle_setting_names),
	MAKE_ENUM_TABLE_ENTRY(global_actor_type_names),
	MAKE_ENUM_TABLE_ENTRY(global_model_state_names),
	MAKE_ENUM_TABLE_ENTRY(k_event_level_names),
	MAKE_ENUM_TABLE_ENTRY(global_character_physics_override_names),
	MAKE_ENUM_TABLE_ENTRY(global_primary_skull_names),
	MAKE_ENUM_TABLE_ENTRY(global_secondary_skull_names)
#undef MAKE_ENUM_TABLE_ENTRY
};

// 018BEC94
char const* const hs_type_names[k_hs_type_count]
{
	"unparsed",
	"special_form",
	"function_name",
	"passthrough",

	"void",
	"boolean",
	"real",
	"short",
	"long",
	"string",
	"script",
	"string_id",

	"unit_seat_mapping",
	"trigger_volume",
	"cutscene_flag",
	"cutscene_camera_point",
	"cutscene_title",
	"cutscene_recording",
	"device_group",
	"ai",
	"ai_command_list",
	"ai_command_script",
	"ai_behavior",
	"ai_orders",
	"ai_line",
	"starting_profile",
	"conversation",
	"zone_set",
	"designer_zone",
	"point_reference",
	"style",
	"object_list",
	"folder",

	"sound",
	"effect",
	"damage",
	"looping_sound",
	"animation_graph",
	"damage_effect",
	"object_definition",
	"bitmap",
	"shader",
	"render_model",
	"structure_definition",
	"lightmap_definition",
	"cinematic_definition",
	"cinematic_scene_definition",
	"bink_definition",
	"any_tag",
	"any_tag_not_resolving",

	"game_difficulty",
	"team",
	"mp_team",
	"controller",
	"button_preset",
	"joystick_preset",
	"player_character_type",
	"voice_output_setting",
	"voice_mask",
	"subtitle_setting",
	"actor_type",
	"model_state",
	"event",
	"character_physics",
	"primary_skull",
	"secondary_skull",

	"object",
	"unit",
	"vehicle",
	"weapon",
	"device",
	"scenery",
	"effect_scenery",

	"object_name",
	"unit_name",
	"vehicle_name",
	"weapon_name",
	"device_name",
	"scenery_name",
	"effect_scenery_name",

	"cinematic_lightprobe",

	"animation_budget_reference",
	"looping_sound_budget_reference",
	"sound_budget_reference"
};

// 018BEDE4
char const* const hs_script_type_names[k_hs_script_type_count]
{
	"startup",
	"dormant",
	"continuous",
	"static",
	"command_script",
	"stub"
};

