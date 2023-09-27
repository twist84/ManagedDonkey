#pragma once

#include "hs/hs_scenario_definitions.hpp"

struct
{
	struct s_unknown_struct
	{
		byte __data[0x14];
	};

	bool initialized;

	long compiled_source_size;
	char* compiled_source;

	char __padC[0xC];

	char const* error_message;
	long error_offset;
	char error_message_buffer[1024];

	bool __unknown420;
	bool __unknown421;
	bool illegal_block;
	bool illegal_variable_value_set;
	bool __unknown424;

	long some_reference_count;

	long current_script_index;
	long current_global_index;

	s_unknown_struct* __unknown434;
	long* __unknown438; // 1024
	long* __unknown43C; // 256
	short __unknown434_count;

} hs_compile_globals;
static_assert(sizeof(hs_compile_globals) == 0x444);

using hs_type_primitive_parser_t = bool __cdecl(long expression_index);
extern hs_type_primitive_parser_t* hs_type_primitive_parsers[k_hs_type_count];

extern bool hs_parse_boolean(long expression_index);
extern bool hs_parse_real(long expression_index);
extern bool hs_parse_integer(long expression_index);
extern bool hs_parse_string(long expression_index);
extern bool hs_parse_script(long expression_index);
extern bool hs_parse_string_id(long expression_index);
extern bool hs_parse_unit_seat_mapping(long expression_index);
extern bool hs_parse_trigger_volume(long expression_index);
extern bool hs_parse_cutscene_flag(long expression_index);
extern bool hs_parse_cutscene_camera_point(long expression_index);
extern bool hs_parse_cutscene_title(long expression_index);
extern bool hs_parse_cutscene_recording(long expression_index);
extern bool hs_parse_device_group(long expression_index);
extern bool hs_parse_ai(long expression_index);
extern bool hs_parse_ai_command_list(long expression_index);
extern bool hs_parse_ai_command_script(long expression_index);
extern bool hs_parse_ai_behavior(long expression_index);
extern bool hs_parse_ai_orders(long expression_index);
extern bool hs_parse_ai_line(long expression_index);
extern bool hs_parse_starting_profile(long expression_index);
extern bool hs_parse_conversation(long expression_index);
extern bool hs_parse_zone_set(long expression_index);
extern bool hs_parse_designer_zone(long expression_index);
extern bool hs_parse_point_ref(long expression_index);
extern bool hs_parse_style(long expression_index);
extern bool hs_parse_object_list(long expression_index);
extern bool hs_parse_folder(long expression_index);
extern bool hs_parse_sound_tag_reference(long expression_index);
extern bool hs_parse_tag_reference(long expression_index);
extern bool hs_parse_tag_reference_not_resolving(long expression_index);
extern bool hs_parse_enum(long expression_index);
extern bool hs_parse_object(long expression_index);
extern bool hs_parse_object_name(long expression_index);
extern bool hs_parse_cinematic_lightprobe(long expression_index);
extern bool hs_parse_budget_reference(long expression_index);

