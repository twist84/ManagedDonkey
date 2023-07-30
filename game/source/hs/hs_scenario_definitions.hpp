#pragma once

#include "memory/data.hpp"
#include "tag_files/tag_groups.hpp"

struct hs_tag_reference
{
	c_typed_tag_reference<0xFFFFFFFF> reference;

	void update_reference_names();
};
static_assert(sizeof(hs_tag_reference) == sizeof(s_tag_reference));

enum e_hs_script_type
{
	_hs_script_startup = 0,
	_hs_script_dormant,
	_hs_script_continuous,
	_hs_script_static,
	_hs_script_command_script,
	_hs_script_stub,

	k_hs_script_type_count
};

enum e_hs_type
{
	_hs_type_unparsed = 0,
	_hs_type_special_form,
	_hs_type_function_name,
	_hs_type_passthrough,
	_hs_type_void,
	_hs_type_boolean,
	_hs_type_real,
	_hs_type_short,
	_hs_type_long,
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
	_hs_type_point_reference,
	_hs_type_style,
	_hs_type_object_list,
	_hs_type_folder,
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
	_hs_type_object,
	_hs_type_unit,
	_hs_type_vehicle,
	_hs_type_weapon,
	_hs_type_device,
	_hs_type_scenery,
	_hs_type_effect_scenery,
	_hs_type_object_name,
	_hs_type_unit_name,
	_hs_type_vehicle_name,
	_hs_type_weapon_name,
	_hs_type_device_name,
	_hs_type_scenery_name,
	_hs_type_effect_scenery_name,
	_hs_type_cinematic_lightprobe,
	_hs_type_animation_budget_reference,
	_hs_type_looping_sound_budget_reference,
	_hs_type_sound_budget_reference,

	k_hs_type_count
};

struct hs_script_parameter
{
	string name;
	c_enum<e_hs_type, short, _hs_type_unparsed, k_hs_type_count> return_type;

	// pad
	byte JBG[2];
};
static_assert(sizeof(hs_script_parameter) == 0x24);

struct hs_global_internal
{
	string name;
	c_enum<e_hs_type, short, _hs_type_unparsed, k_hs_type_count> type;

	// pad
	byte EB[2];

	long initialization_expression_index;
};
static_assert(sizeof(hs_global_internal) == 0x28);

struct hs_script
{
	string name;
	c_enum<e_hs_script_type, short, _hs_script_startup, k_hs_script_type_count> script_type;
	c_enum<e_hs_type, short, _hs_type_unparsed, k_hs_type_count> return_type;
	long root_expression_index;
	c_typed_tag_block<hs_script_parameter> parameters;
};
static_assert(sizeof(hs_script) == 0x34);

enum hs_source_file_flags
{
	_source_file_flag_generated_at_runtime = 0,
	_source_file_flag_ai_fragments,

	hs_source_file_flag_count
};

struct hs_string_data_definition
{
#pragma warning(push)
#pragma warning(disable : 4200)
	byte data[];
#pragma warning(pop)
};

struct hs_syntax_node : s_datum_header
{
	union
	{
		short script_index;
		short function_index;
		c_enum<e_hs_type, short, _hs_type_unparsed, k_hs_type_count> constant_type;
	};

	c_enum<e_hs_type, short, _hs_type_unparsed, k_hs_type_count> type;
	word_flags flags;
	long next_node_index;
	long source_offset;
	long data;
	short line_number;
	short HMM;
};
static_assert(sizeof(hs_syntax_node) == 0x18);

struct hs_source_file
{
	c_typed_tag_block<hs_tag_reference> external_references;
};
static_assert(sizeof(hs_source_file) == sizeof(s_tag_block));

struct s_hs_unit_seat_mapping
{
	long unit_definition_tag_index;
	long unit_seats;
	long unit_seats2;
};
static_assert(sizeof(s_hs_unit_seat_mapping) == 0xC);