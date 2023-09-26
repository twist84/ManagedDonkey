#include "hs/hs.hpp"

#include "cseries/cseries_events.hpp"
#include "hs/hs_scenario_definitions.hpp"
#include "scenario/scenario.hpp"

//REFERENCE_DECLARE_ARRAY(, short const, hs_type_sizes, k_hs_type_count);
//REFERENCE_DECLARE_ARRAY(0x, short const, hs_object_type_masks, k_hs_type_object_count);
//REFERENCE_DECLARE_ARRAY(0x, long const, hs_tag_reference_type_group_tags, k_hs_tag_reference_type_count);
//REFERENCE_DECLARE_ARRAY(0x, hs_enum_definition const, hs_enum_table, k_hs_type_enum_count);
//REFERENCE_DECLARE_ARRAY(0x, char const* const, hs_type_names, k_hs_type_count);
//REFERENCE_DECLARE_ARRAY(0x, char const* const, hs_script_type_names, k_hs_script_type_count);
REFERENCE_DECLARE(0x024B06D4, c_typed_data_array<hs_syntax_node>*, g_hs_syntax_data);

hs_syntax_node* __cdecl hs_syntax_get(long datum_index)
{
	if (DATUM_INDEX_TO_ABSOLUTE_INDEX(datum_index) > g_hs_syntax_data->maximum_count)
		return &g_hs_syntax_data->data[DATUM_INDEX_TO_ABSOLUTE_INDEX(datum_index)];

	return nullptr;

	//return INVOKE(0x00598A10, hs_syntax_get, datum_index);
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
	short(0xFFFF), // object
	short(0x2003), // unit
	short(0x2),    // vehicle
	short(0x4),    // weapon
	short(0x330),  // device
	short(0x80),   // scenery
	short(0x4000)  // effect_scenery
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

// 0189DF28
char const* const global_campaign_difficulty_level_names[4]
{
	"easy",
	"normal",
	"heroic",
	"legendary"
};

// 018BE868
char const* const global_campaign_team_names[16]
{
	"default",
	"player",
	"human",
	"covenant",
	"flood",
	"sentinel",
	"heretic",
	"prophet",
	"guilty",
	"unused9",
	"unused10",
	"unused11",
	"unused12",
	"unused13",
	"unused14",
	"unused15"
};

// 0189EECC
char const* const global_multiplayer_team_names[8]
{
	"mp_team_red",
	"mp_team_blue",
	"mp_team_green",
	"mp_team_yellow",
	"mp_team_purple",
	"mp_team_orange",
	"mp_team_brown",
	"mp_team_grey"
};

// 0191CB00
char const* const global_controller_index_names[4]
{
	"controller1",
	"controller2",
	"controller3",
	"controller4"
};

// 0191CB10
char const* const global_button_preset_names[6]
{
	"standard",
	"south_paw",
	"boxer",
	"green_thumb",
	"professional",
	"walkie-talkie"
};

// 0191CB28
char const* const global_joystick_preset_names[4]
{
	"standard",
	"south_paw",
	"legacy",
	"legacy_south_paw"
};

// 0189DD38
char const* const global_player_model_choice_names[2]
{
	"spartan",
	"elite"
};

// 0191CB38
char const* const global_voice_output_setting_names[2]
{
	"default",
	"headset"
};

// 0191CB40
char const* const global_voice_mask_names[2]
{
	"none",
	"anonymous"
};

// 0191CB48
char const* const global_subtitle_setting_names[3]
{
	"automatic",
	"enabled",
	"disabled"
};

// 01992AF8
char const* const global_actor_type_names[25]
{
	"elite",
	"jackal",
	"grunt",
	"hunter",
	"engineer",
	"assassin",
	"player",
	"marine",
	"crew",
	"combat_form",
	"infection_form",
	"carrier_form",
	"monitor",
	"sentinel",
	"none",
	"mounted_weapon",
	"brute",
	"prophet",
	"bugger",
	"juggernaut",
	"pure_form_stealth",
	"pure_form_tank",
	"pure_form_ranged",
	"scarab",
	"guardian"
};

// 018ECC80
char const* const global_model_state_names[5]
{
	"standard",
	"minor damage",
	"medium damage",
	"major damage",
	"destroyed"
};

// 018ECCEC
char const* const global_character_physics_override_names[3]
{
	"NONE",
	"player",
	"biped"
};

// 018BAE24
char const* const global_primary_skull_names[9]
{
	"skull_iron",
	"skull_black_eye",
	"skull_tough_luck",
	"skull_catch",
	"skull_fog",
	"skull_famine",
	"skull_thunderstorm",
	"skull_tilt",
	"skull_mythic"
};

// 018BAE48
char const* const global_secondary_skull_names[7]
{
	"skull_assassin",
	"skull_blind",
	"skull_superman",
	"skull_birthday_party",
	"skull_daddy",
	"skull_third_person",
	"skull_directors_cut"
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

