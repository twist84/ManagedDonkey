#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs_scenario_definitions.hpp"

enum e_reference_type
{
	_reference_type_global = 0,
	_reference_type_script,

	k_reference_type_count
};

long const k_hs_compile_error_buffer_size = 1024;
long const k_maximum_hs_scripts_per_scenario = 1024;
long const k_maximum_hs_globals_per_scenario = 256;

struct hs_compile_globals_reference_struct
{
	e_reference_type reference_type;

	// script->return_type != _hs_type_void
	bool has_return_type;

	long index;
	long expression_index;
	hs_compile_globals_reference_struct* __unknown10; // previous/next?
};
static_assert(sizeof(hs_compile_globals_reference_struct) == 0x14);

struct hs_compile_globals_struct
{
	bool initialized;

	long compiled_source_size;
	char* compiled_source;

	char* string_constant_buffer;
	long string_constant_buffer_offset;
	long string_constant_buffer_size;

	char const* error_message;
	long error_offset;
	char error_buffer[k_hs_compile_error_buffer_size];

	bool malloced;
	bool permanent;
	bool disallow_blocks;
	bool disallow_sets;
	bool variables_predetermined;

	long some_reference_count;

	long current_script_index;
	long current_global_index;

	struct
	{
		hs_compile_globals_reference_struct* references;
		hs_compile_globals_reference_struct*(*script_references)[k_maximum_hs_scripts_per_scenario];
		hs_compile_globals_reference_struct*(*global_references)[k_maximum_hs_globals_per_scenario];
		short reference_count; // hs_compile_globals_reference_struct* references
	};
};
static_assert(sizeof(hs_compile_globals_struct) == 0x444);

extern hs_compile_globals_struct hs_compile_globals;

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
extern bool hs_parse_variable(long expression_index);
extern bool hs_parse_primitive(long expression_index);
extern bool hs_parse_nonprimitive(long expression_index);
extern bool hs_parse(long expression_index, short expected_type);
extern bool hs_macro_function_parse(short function_index, long expression_index);
extern bool hs_compile_get_tag_by_name(char const* group_name, tag* group_tag_out);
extern short hs_count_children(long expression_index);
extern void hs_compile_add_reference(long referred_index, e_reference_type reference_type, long expression_index);
extern void hs_parse_call_predicate(long expression_index, bool* a2, bool* a3);
extern bool hs_parse_tag_block_element_string_id(long expression_index, long offset, long scenario_index, s_tag_block* block, long element_size);
extern bool hs_parse_tag_block_element(long expression_index, long offset, long scenario_index, s_tag_block* block, long element_size);

template<typename block_index_type_t, typename return_type_t>
return_type_t hs_check_block_index_type_and_return(return_type_t return_value)
{
	return return_value;
}

