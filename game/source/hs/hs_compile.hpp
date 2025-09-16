#pragma once

int32 const k_hs_compile_error_buffer_size = 1024;
int32 const k_maximum_hs_scripts_per_scenario = 1024;
int32 const k_maximum_hs_globals_per_scenario = 256;

struct s_hs_reference
{
	e_reference_type type;
	bool strong;
	int32 index;
	int32 node_index;
	s_hs_reference* next;
};
static_assert(sizeof(s_hs_reference) == 0x14);

typedef struct
{
	bool initialized;

	int32 compiled_source_size;
	char* compiled_source;

	char* string_constant_buffer;
	int32 string_constant_buffer_offset;
	int32 string_constant_buffer_size;

	const char* error_message;
	int32 error_offset;
	char error_buffer[k_hs_compile_error_buffer_size];

	bool malloced;
	bool permanent;
	bool disallow_blocks;
	bool disallow_sets;
	bool variables_predetermined;
	int32 indent;
	int32 current_script_index;
	int32 current_global_index;

	s_hs_reference* references;
	s_hs_reference** script_references;
	s_hs_reference** global_references;
	int16 allocated_references;
} s_hs_compile_globals;
static_assert(sizeof(s_hs_compile_globals) == 0x444);
extern s_hs_compile_globals hs_compile_globals;

using hs_type_primitive_parser_t = bool __cdecl(int32 expression_index);
extern hs_type_primitive_parser_t* hs_type_primitive_parsers[k_hs_type_count];

struct hs_tokenizer;
struct s_tag_block;

extern bool hs_parse_boolean(int32 expression_index);
extern bool hs_parse_real(int32 expression_index);
extern bool hs_parse_integer(int32 expression_index);
extern bool hs_parse_string(int32 expression_index);
extern bool hs_parse_script(int32 expression_index);
extern bool hs_parse_string_id(int32 expression_index);
extern bool hs_parse_unit_seat_mapping(int32 expression_index);
extern bool hs_parse_trigger_volume(int32 expression_index);
extern bool hs_parse_cutscene_flag(int32 expression_index);
extern bool hs_parse_cutscene_camera_point(int32 expression_index);
extern bool hs_parse_cutscene_title(int32 expression_index);
extern bool hs_parse_cutscene_recording(int32 expression_index);
extern bool hs_parse_device_group(int32 expression_index);
extern bool hs_parse_ai(int32 expression_index);
extern bool hs_parse_ai_command_list(int32 expression_index);
extern bool hs_parse_ai_command_script(int32 expression_index);
extern bool hs_parse_ai_behavior(int32 expression_index);
extern bool hs_parse_ai_orders(int32 expression_index);
extern bool hs_parse_ai_line(int32 expression_index);
extern bool hs_parse_starting_profile(int32 expression_index);
extern bool hs_parse_conversation(int32 expression_index);
extern bool hs_parse_zone_set(int32 expression_index);
extern bool hs_parse_designer_zone(int32 expression_index);
extern bool hs_parse_point_ref(int32 expression_index);
extern bool hs_parse_style(int32 expression_index);
extern bool hs_parse_object_list(int32 expression_index);
extern bool hs_parse_folder(int32 expression_index);
extern bool hs_parse_sound_tag_reference(int32 expression_index);
extern bool hs_parse_tag_reference(int32 expression_index);
extern bool hs_parse_tag_reference_not_resolving(int32 expression_index);
extern bool hs_parse_enum(int32 expression_index);
extern bool hs_parse_object(int32 expression_index);
extern bool hs_parse_object_name(int32 expression_index);
extern bool hs_parse_cinematic_lightprobe(int32 expression_index);
extern bool hs_parse_budget_reference(int32 expression_index);
extern bool hs_parse_variable(int32 expression_index);
extern bool hs_parse_primitive(int32 expression_index);
extern bool hs_parse_nonprimitive(int32 expression_index);
extern bool hs_parse(int32 expression_index, int16 expected_type);
extern bool hs_macro_function_parse(int16 function_index, int32 expression_index);
extern bool hs_compile_get_tag_by_name(const char* group_name, tag* group_tag_out);
extern int16 hs_count_children(int32 expression_index);
extern void hs_compile_add_reference(int32 referred_index, e_reference_type reference_type, int32 expression_index);
extern void hs_parse_call_predicate(int32 expression_index, bool* a2, bool* a3);
extern bool hs_parse_tag_block_element_string_id(int32 expression_index, int32 offset, int32 scenario_index, s_tag_block* block, int32 element_size);
extern bool hs_parse_tag_block_element(int32 expression_index, int32 offset, int32 scenario_index, s_tag_block* block, int32 element_size);
extern int32 hs_tokenize(hs_tokenizer* state);
extern void hs_tokenize_nonprimitive(hs_tokenizer* state, int32 expression_index);
extern void hs_tokenize_primitive(hs_tokenizer* state, int32 expression_index);
extern void hs_token_primitive_until_delimiter(hs_tokenizer* state, int32 expression_index, int end_delimiter, const char* error_message);
extern bool hs_compile_and_evaluate(e_event_level event_level, const char* source, const char* expression, bool interactive);
extern void hs_compile_source_error(const char* file_name, const char* error_message, const char* error_source, const char* source);

template<typename block_index_type_t, typename return_type_t>
return_type_t hs_check_block_index_type_and_return(return_type_t return_value)
{
	return return_value;
}

