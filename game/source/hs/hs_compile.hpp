#pragma once

#include "cseries/cseries.hpp"

int32 const k_hs_compile_error_buffer_size = 1024;

enum e_hs_fakery
{
	_hs_fakery_none = 0,
	_hs_fakery_parenthesize,
	_hs_fakery_set,
	_hs_fakery_begin,

	NUMBER_OF_HS_FAKERY_TYPES,
};

struct s_tag_block;

struct hs_tokenizer
{
	char* cursor;
	const char* source_file_data;
	int32 source_file_size;
};
static_assert(sizeof(hs_tokenizer) == 0xC);

struct s_hs_compile_state
{
	int32 failed_scripts[32];
	int32 failed_globals[9];
};

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

struct c_hs_compile_error_listener
{
public:
	virtual ~c_hs_compile_error_listener() = 0;
	virtual void handle_error(char const*, int32, const char*, const char*, c_wrapped_array<const char>*, const char*) = 0;
};

struct s_hs_static_globals
{
	static const int32 k_maximum_hs_compile_error_listeners = 1;
	c_hs_compile_error_listener* compile_error_listeners[k_maximum_hs_compile_error_listeners];
};

extern s_hs_compile_globals hs_compile_globals;
extern char* g_error_output_buffer;
extern int32 g_error_buffer_length;
extern s_hs_static_globals hs_static_globals;

using hs_type_primitive_parser_t = bool __cdecl(int32 expression_index);
extern hs_type_primitive_parser_t* hs_type_primitive_parsers[NUMBER_OF_HS_NODE_TYPES];

extern bool hs_add_global(int32 expression_index);
extern bool hs_add_script(int32 expression_index);
extern void hs_compile_add_reference(int32 referred_index, e_reference_type reference_type, int32 node_index);
extern char* hs_compile_add_source(int32 source_size, const char* source_data);
extern bool hs_compile_and_evaluate(e_event_level event_level, const char* source, const char* expression, bool interactive);
extern void hs_compile_first_pass(s_hs_compile_state* compile_state, int32 source_file_size, const char* source_file_data, const char** error_message_pointer, int32* error_offset);
extern void hs_compile_dispose();
extern int32 hs_compile_expression(int32 source_size, const char* source_data, const char** error_message_pointer, const char** error_source_pointer);
extern bool hs_compile_get_tag_by_name(const char* group_name, tag* group_tag_out);
extern void hs_compile_initialize(bool permanent);
extern bool hs_compile_register_error_listener(c_hs_compile_error_listener* listener);
extern bool hs_compile_second_pass(s_hs_compile_state* compile_state, bool verbose);
extern bool hs_compile_source(bool fail_on_error, bool verbose);
extern void hs_compile_source_error(const char* file_name, const char* error_message, const char* error_source, const char* source);
extern void hs_compile_state_initialize(struct scenario* scenario, s_hs_compile_state* state);
extern void hs_compile_strip_failed_special_forms(const s_hs_compile_state* compile_state, bool verbose);
extern void hs_compile_unregister_error_listener(c_hs_compile_error_listener* listener);
extern int16 hs_count_children(int32 expression_index);
extern bool hs_macro_function_parse(int16 function_index, int32 expression_index);
extern bool hs_parse(int32 expression_index, int16 expected_type);
extern bool hs_parse_ai(int32 expression_index);
extern bool hs_parse_ai_behavior(int32 expression_index);
extern bool hs_parse_ai_command_list(int32 expression_index);
extern bool hs_parse_ai_command_script(int32 expression_index);
extern bool hs_parse_ai_line(int32 expression_index);
extern bool hs_parse_ai_orders(int32 expression_index);
extern bool hs_parse_boolean(int32 expression_index);
extern bool hs_parse_budget_reference(int32 expression_index);
extern void hs_parse_call_predicate(int32 expression_index, bool* is_function, bool* is_script);
extern bool hs_parse_cinematic_lightprobe(int32 expression_index);
extern bool hs_parse_conversation(int32 expression_index);
extern bool hs_parse_cutscene_camera_point(int32 expression_index);
extern bool hs_parse_cutscene_flag(int32 expression_index);
extern bool hs_parse_cutscene_recording(int32 expression_index);
extern bool hs_parse_cutscene_title(int32 expression_index);
extern bool hs_parse_designer_zone(int32 expression_index);
extern bool hs_parse_device_group(int32 expression_index);
extern bool hs_parse_enum(int32 expression_index);
extern bool hs_parse_folder(int32 expression_index);
extern bool hs_parse_integer(int32 expression_index);
extern bool hs_parse_nonprimitive(int32 expression_index);
extern bool hs_parse_object(int32 expression_index);
extern bool hs_parse_object_and_object_name_internal(int32 expression_index, e_hs_type byteswap_type);
extern bool hs_parse_object_list(int32 expression_index);
extern bool hs_parse_object_name(int32 expression_index);
extern bool hs_parse_point_ref(int32 expression_index);
extern bool hs_parse_primitive(int32 expression_index);
extern bool hs_parse_real(int32 expression_index);
extern bool hs_parse_script(int32 expression_index);
extern bool hs_parse_sound_tag_reference(int32 expression_index);
extern bool hs_parse_special_form(int32 expression_index);
extern bool hs_parse_starting_profile(int32 expression_index);
extern bool hs_parse_string(int32 expression_index);
extern bool hs_parse_string_id(int32 expression_index);
extern bool hs_parse_style(int32 expression_index);
extern bool hs_parse_tag_block_element(int32 expression_index, const char* name_offset, int32 scenario_index, s_tag_block* block, int32 element_size);
extern bool hs_parse_tag_block_element_string_id(int32 expression_index, const int32* string_id_offset, int32 scenario_index, s_tag_block* block, int32 element_size);
extern bool hs_parse_tag_reference(int32 expression_index);
extern bool hs_parse_tag_reference_not_resolving(int32 expression_index);
extern bool hs_parse_trigger_volume(int32 expression_index);
extern bool hs_parse_unit_seat_mapping(int32 expression_index);
extern bool hs_parse_variable(int32 expression_index);
extern bool hs_parse_zone_set(int32 expression_index);
extern int32 hs_source_pointer_get_line_number(const char* source_pointer, const char* source);
extern void hs_token_primitive_until_delimiter(hs_tokenizer* state, int32 expression_index, int end_delimiter, const char* error_message);
extern int32 hs_tokenize(hs_tokenizer* state);
extern void hs_tokenize_nonprimitive(hs_tokenizer* state, int32 expression_index);
extern void hs_tokenize_primitive(hs_tokenizer* state, int32 expression_index);
extern void hs_validify_expression(const char* expression, char* out_valid_expression_buffer, int32 out_expression_length);
extern bool hs_verify_source_offset(int32 source_offset);

