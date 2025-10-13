#include "hs/hs_compile.hpp"

#include "ai/ai_orders.hpp"
#include "ai/ai_script.hpp"
#include "ai/behavior.hpp"
#include "ai/cl_engine.hpp"
#include "ai/cs_scenario_definitions.hpp"
#include "ai/styles.hpp"
#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "cseries/progress.hpp"
#include "devices/devices.hpp"
#include "editor/editor_stubs.hpp"
#include "hs/hs.hpp"
#include "hs/hs_function.hpp"
#include "hs/hs_library_internal_compile.hpp"
#include "hs/hs_runtime.hpp"
#include "hs/hs_unit_seats.hpp"
#include "main/console.hpp"
#include "scenario/scenario.hpp"

#include <algorithm>
#include <ctype.h>

s_hs_compile_globals hs_compile_globals{};
char* g_error_output_buffer = NULL;
int32 g_error_buffer_length = 0;
static s_hs_static_globals hs_static_globals;

enum
{
	_skip_whitespace_state_no_comment = 0,
	_skip_whitespace_state_comment,
	_skip_whitespace_state_block_comment,
	_skip_whitespace_state_done,

	NUMBER_OF_SKIP_WHITESPACE_STATES
};

const char whitespace_characters[] = { ' ', '\t' };
const char eol_characters[] = { '\n', '\r' };

static bool character_in_list(char c, int16 list_count, const char* list)
{
	for (int16 list_index = 0; list_index < list_count; list_index++)
	{
		if (c == list[list_index])
		{
			return true;
		}
	}
	return false;
}

static void skip_whitespace(char** c)
{
	int32 state = _skip_whitespace_state_no_comment;
	while (state != _skip_whitespace_state_done)
	{
		if (state)
		{
			if (state == _skip_whitespace_state_comment)
			{
				if (**c)
				{
					if (character_in_list(**c, NUMBEROF(eol_characters), eol_characters))
					{
						state = _skip_whitespace_state_no_comment;
						++*c;
					}
					else
					{
						state = _skip_whitespace_state_done;
					}
				}
			}
			else if (state < _skip_whitespace_state_done)
			{
				if (**c)
				{
					if (**c == '*' && (*c)[1] == ';')
					{
						state = _skip_whitespace_state_no_comment;
						++*c;
					}
					++ * c;
				}
				else
				{
					state = _skip_whitespace_state_done;
				}
			}
			else UNREACHABLE();
		}
		else if (**c == ';')
		{
			state = _skip_whitespace_state_comment;
			if (*++ * c == '*')
			{
				state = _skip_whitespace_state_done;
				++*c;
			}
		}
		else if (character_in_list(**c, NUMBEROF(whitespace_characters), whitespace_characters) || character_in_list(**c, NUMBEROF(eol_characters), eol_characters))
		{
			++*c;
		}
		else
		{
			state = _skip_whitespace_state_done;
		}
	}
}

template<typename block_index_type_t, typename return_type_t>
return_type_t hs_check_block_index_type_and_return(return_type_t return_value)
{
	return return_value;
}

hs_source_file* source_offset_get_source_file(int32 offset, int32* offset_within_file)
{
	hs_source_file* result = NULL;

	const struct scenario* scenario = global_scenario_get();
	int32 total_source_size = 0;
	{
		for (int16 source_index = 0; source_index < scenario->hs_source_files.count; source_index++)
		{
			hs_source_file* source_file = TAG_BLOCK_GET_ELEMENT(&scenario->hs_source_files, source_index, hs_source_file);
#if 0
			total_source_size += source_file->source.size;
			if (offset < total_source_size)
			{
				result = source_file;
				ASSERT((*offset_within_file >= 0) && (*offset_within_file < source_file->source.size));
				break;
			}
#endif
		}
	}

	return result;
}

bool hs_add_global(int32 expression_index)
{
	bool success = false;

#if 0
	int32 type_index = NONE;
	int32 name_index = NONE;
	int32 initialization_index = NONE;

	if ((type_index = hs_syntax_get(hs_syntax_get(expression_index)->long_value)->next_node_index) != NONE
		&& (name_index = hs_syntax_get(type_index)->next_node_index) != NONE
		&& (initialization_index = hs_syntax_get(name_index)->next_node_index) != NONE
		&& hs_syntax_get(initialization_index)->next_node_index != NONE)
	{
		const hs_syntax_node* type_node = hs_syntax_get(type_index);
		int16 type = string_list_find(&hs_compile_globals.compiled_source[type_node->source_offset], NUMBER_OF_HS_NODE_TYPES, hs_type_names);
		if (hs_type_valid(type))
		{
			const hs_syntax_node* name_node = hs_syntax_get(name_index);
			const char* global_name = &hs_compile_globals.compiled_source[name_node->source_offset];
			if (strlen(global_name) && strlen(global_name) < NUMBEROF(hs_global_internal::name))
			{
				if (hs_find_global_by_name(global_name) == NONE)
				{
					if (hs_find_script_by_name(global_name, NONE) == NONE)
					{
#if defined(NETWORK_EXECUTABLE_TYPE_TAGS)
						hs_compile_globals.disallow_blocks = true;
						hs_compile_globals.disallow_sets = true;

						int16 new_global_index = tag_block_add_element(&global_scenario_get()->hs_globals);
						if (new_global_index != NONE)
						{
							hs_global_internal* new_global = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_globals, new_global_index, hs_global_internal);
							csstrnzcpy(new_global->name, global_name, NUMBEROF(new_global->name));
							new_global->initialization_expression_index = initialization_index;

							success = true;
						}
						else
						{
							hs_compile_globals.error_message = "i couldn't allocate space for this global.";
							hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
						}
#endif

						hs_compile_globals.disallow_blocks = false;
						hs_compile_globals.disallow_sets = false;
					}
					else
					{
						hs_compile_globals.error_message = "there is already a scipt by this name.";
						hs_compile_globals.error_offset = hs_syntax_get(name_index)->source_offset;
					}
				}
				else
				{
					hs_compile_globals.error_message = "there is already a variable by this name.";
					hs_compile_globals.error_offset = hs_syntax_get(name_index)->source_offset;
				}
			}
			else
			{
				hs_compile_globals.error_message = "i expected a global variable name less than 32 characters.";
				hs_compile_globals.error_offset = hs_syntax_get(name_index)->source_offset;
			}
		}
		else
		{
			hs_compile_globals.error_message = "this is not a valid type.";
			hs_compile_globals.error_offset = hs_syntax_get(type_index)->source_offset;
		}
	}
	else
	{
		hs_compile_globals.error_message = "i expected (global<type> <name> <initial value>)";
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
	}
#endif

	return success;
}

bool hs_add_script(int32 expression_index)
{
	bool success = false;

#if 0 // $IMPLEMENT
	int32 type_index = hs_syntax_get(hs_syntax_get(expression_index)->long_value)->next_node_index;
	if (type_index != NONE)
	{
		const hs_syntax_node* type_node = hs_syntax_get(type_index);
		int16 type = string_list_find(&hs_compile_globals.compiled_source[type_node->source_offset], NUMBER_OF_HS_SCRIPT_TYPES, hs_script_type_names);
		if (type != NONE)
		{
			int16 return_type = 0;
			int32 name_index = 0;
			int32 root_expression_index = NONE;
			if (type == _hs_script_static)
			{
				int32 return_type_index = hs_syntax_get(type_index)->next_node_index;
				if (return_type_index != NONE)
				{
					const hs_syntax_node* return_type_node = hs_syntax_get(return_type_index);
					return_type = string_list_find(&hs_compile_globals.compiled_source[return_type_node->source_offset], NUMBER_OF_HS_NODE_TYPES, hs_type_names);
					root_expression_index = hs_syntax_get(return_type_index)->next_node_index;
					if (hs_type_valid(return_type))
					{
						success = true;
					}
					else
					{
						hs_compile_globals.error_message = "this is not a valid return type.";
						hs_compile_globals.error_offset = hs_syntax_get(return_type_index)->source_offset;
					}
				}
				else
				{
					hs_compile_globals.error_message = "i expected (script local <type> <name> <expression(s)>).";
					hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
				}
			}
			else if (type == _hs_script_stub)
			{
				success = true;
			}
			else
			{
				return_type = _hs_type_void;
				root_expression_index = hs_syntax_get(type_index)->next_node_index;
				success = true;
			}

			if (success)
			{
				if (root_expression_index != NONE && hs_syntax_get(root_expression_index)->next_node_index != NONE)
				{
					int32 parameter_set_index;
					const hs_syntax_node* name_node;
					const char* script_name;

					int32 actual_name_index;
					{
						const hs_syntax_node* actual_name_node = hs_syntax_get(actual_name_index);
					}
					{
						struct scenario* scenario;
						int32 script_index;
						int16 num_parameters;
						{
							int32 temp_parameter_set_index;
						}
						{
							int32 global_index;
						}
						{
							hs_script* script;
							{
								int32 implicit_begin_name_index;
								int32 implicit_begin_index;
								{
									hs_syntax_node* implicit_begin;
									hs_syntax_node* implicit_begin_name;
								}
							}
						}
					}
				}
				else
				{
					if (type == _hs_script_static)
					{
						hs_compile_globals.error_message = "i expected (script static <type> <name> <expression(s)>)";
					}
					else
					{
						hs_compile_globals.error_message = "i expected (script <type> <name> <expression(s)>)";
						hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
					}
				}
			}
		}
		else
		{
			hs_compile_globals.error_message = "script type must be \"startup\", \"dormant\", \"continuous\", or \"static\".";
			hs_compile_globals.error_offset = hs_syntax_get(type_index)->source_offset;
		}
	}
	else
	{
		hs_compile_globals.error_message = "i expected (script <type> <name> <expression(s)>)";
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
	}
#endif

	return success;
}

void hs_compile_add_reference(int32 referred_index, e_reference_type reference_type, int32 node_index)
{
	if (reference_type || !TEST_BIT(referred_index, 15))
	{
		if (hs_compile_globals.current_script_index != NONE || hs_compile_globals.current_global_index != NONE)
		{
			ASSERT((hs_compile_globals.current_script_index != NONE) ^ (hs_compile_globals.current_global_index != NONE));

			e_reference_type referer_type = _hs_reference_type_global;
			int32 referer_index = hs_compile_globals.current_global_index;
			if (hs_compile_globals.current_script_index != NONE)
			{
				referer_type = _hs_reference_type_script;
				referer_index = hs_compile_globals.current_script_index;
			}

			s_hs_reference* reference = &hs_compile_globals.references[hs_compile_globals.allocated_references++];
			reference->type = referer_type;
			reference->index = referer_index;
			reference->node_index = node_index;
			reference->next = NULL;

			switch (reference_type)
			{
			case _hs_reference_type_global:
			{
				ASSERT((referred_index >= 0) && (referred_index < k_maximum_hs_globals_per_scenario));

				reference->next = hs_compile_globals.global_references[referred_index];
				hs_compile_globals.global_references[referred_index] = reference;
				reference->strong = true;
			}
			break;
			case _hs_reference_type_script:
			{
				ASSERT((referred_index >= 0) && (referred_index < k_maximum_hs_scripts_per_scenario));

				reference->next = hs_compile_globals.script_references[referred_index];
				hs_compile_globals.script_references[referred_index] = reference;
				hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, referred_index, hs_script);
				reference->strong = script->return_type != _hs_type_void;
			}
			break;
			default:
			{
				UNREACHABLE();
			}
			break;
			}
		}
	}
}

char* hs_compile_add_source(int32 source_size, const char* source_data)
{
	// $IMPLEMENT

#if 1
	char* compiled_new_source = NULL;
#else
	char* compiled_new_source = NULL;
	char* compiled_source = (char*)system_realloc(hs_compile_globals.compiled_source, source_size + hs_compile_globals.compiled_source_size + 1);
	if (compiled_source)
	{
		hs_compile_globals.compiled_source = compiled_source;
		compiled_new_source = &compiled_source[hs_compile_globals.compiled_source_size];
		csmemcpy(compiled_new_source, source_data, source_size);
		hs_compile_globals.compiled_source_size += source_size;
		hs_compile_globals.compiled_source[hs_compile_globals.compiled_source_size] = 0;
	}
#endif
	return compiled_new_source;
}

bool hs_compile_and_evaluate(e_event_level event_level, const char* source, const char* expression, bool interactive)
{
	bool success = false;

	event(event_level, "hs:evaluate: %s: %s", source, expression);

	//random_seed_allow_use();

	char expressionbuf[4096]{};
	hs_validify_expression(expression, expressionbuf, sizeof(expressionbuf));
	if (string_is_not_empty(expressionbuf))
	{
		const char* error_message = NULL;
		const char* error_source = NULL;

		if (g_hs_syntax_data && g_hs_syntax_data->valid && hs_runtime_safe_to_gc())
		{
			hs_node_gc();
		}

		hs_compile_initialize(false);

		hs_syntax_node temporary_syntax_data[128]{};
		if (g_hs_syntax_data && TEST_BIT(g_hs_syntax_data->flags, _data_array_disconnected_bit))
		{
			csmemset(temporary_syntax_data, 0, sizeof(temporary_syntax_data));
			data_connect(g_hs_syntax_data, NUMBEROF(temporary_syntax_data), temporary_syntax_data);
		}

		int32 expression_index = hs_compile_expression(csstrnlen(expressionbuf, sizeof(expressionbuf)), expressionbuf, &error_message, &error_source);
		if (expression_index == NONE)
		{
			if (error_message)
			{
				hs_compile_source_error(NULL, error_message, error_source, expressionbuf);
			}
		}
		else
		{
			success = true;
			hs_runtime_evaluate(expression_index, interactive, false);
		}

		if (g_hs_syntax_data && g_hs_syntax_data->data == temporary_syntax_data)
		{
			data_disconnect(g_hs_syntax_data);
		}

		hs_compile_dispose();
	}

	if (g_recompile_scripts)
	{
		hs_rebuild_and_compile(NULL, false, true);
		g_recompile_scripts = false;
	}

	//random_seed_disallow_use();

	return success;
}

void hs_compile_first_pass(s_hs_compile_state* compile_state, int32 source_file_size, const char* source_file_data, const char** error_message_pointer, int32* error_offset)
{
	hs_tokenizer tokenizer{};
	tokenizer.source_file_data = source_file_data;
	tokenizer.source_file_size = source_file_size;
	if (tokenizer.cursor = hs_compile_add_source(source_file_size, source_file_data))
	{
		hs_compile_globals.error_message = NULL;
		*error_message_pointer = NULL;
		hs_compile_globals.error_offset = NONE;
	
		skip_whitespace(&tokenizer.cursor);
		while (*tokenizer.cursor)
		{
			int32 expression_index = hs_tokenize(&tokenizer);
			skip_whitespace(&tokenizer.cursor);
			if (hs_compile_globals.error_message || !hs_parse_special_form(expression_index))
			{
				VASSERT(!hs_compile_globals.error_message, "tell DAMIAN (or whomever owns HS) that somebody failed to correctly report a parsing error.");
				*error_message_pointer = hs_compile_globals.error_message;
				*error_offset = hs_compile_globals.error_offset;
				break;
			}
		}
	}
	else
	{
		*error_message_pointer = "couldn't allocate memory for compiled source.";
	}
}

//.text:0072F500 ; 
void hs_compile_dispose()
{
	ASSERT(hs_compile_globals.initialized);

	char* compiled_source = hs_compile_globals.compiled_source;
	if (hs_compile_globals.permanent && hs_compile_globals.compiled_source)
	{
		system_free(hs_compile_globals.compiled_source);
		compiled_source = hs_compile_globals.compiled_source;
	}

	if (hs_compile_globals.malloced)
	{
		system_free(compiled_source);
		hs_compile_globals.compiled_source = NULL;
		hs_compile_globals.malloced = false;
	}

	if (hs_compile_globals.references)
	{
		system_free(hs_compile_globals.references);
		hs_compile_globals.references = NULL;
	}

	if (hs_compile_globals.script_references)
	{
		system_free(hs_compile_globals.script_references);
		hs_compile_globals.script_references = NULL;
	}

	if (hs_compile_globals.global_references)
	{
		system_free(hs_compile_globals.global_references);
		hs_compile_globals.global_references = NULL;
	}

	hs_compile_globals.initialized = false;

	if (g_hs_syntax_data->maximum_count + k_maximum_hs_syntax_nodes_free_space < k_maximum_hs_syntax_nodes_per_scenario)
	{
		resize_scenario_syntax_data(g_hs_syntax_data->maximum_count + k_maximum_hs_syntax_nodes_free_space);
	}
	else
	{
		resize_scenario_syntax_data(k_maximum_hs_syntax_nodes_per_scenario);
	}
}

int32 hs_compile_expression(int32 source_size, const char* source_data, const char** error_message_pointer, const char** error_source_pointer)
{
	int32 result_expression_index = NONE;
	hs_tokenizer tokenizer{};

	if (source_size < 4096)
	{
		int32 old_size = 0;

		bool valid_buffer = true;
		if (global_scenario_index_get() == NONE)
		{
			old_size = 0;
			hs_compile_globals.compiled_source = (char*)system_malloc(source_size + 1);
			hs_compile_globals.malloced = true;
			ASSERT(hs_compile_globals.compiled_source);
		}
		else if (global_scenario_get()->hs_string_constants.size < 4096)
		{
			valid_buffer = false;
		}
		else
		{
			old_size = global_scenario_get()->hs_string_constants.size - 4096;
			hs_compile_globals.compiled_source = (char*)global_scenario_get()->hs_string_constants.address;
		}

		if (valid_buffer)
		{
			ASSERT(g_hs_syntax_data);
			csmemcpy(&hs_compile_globals.compiled_source[old_size], source_data, source_size);
			hs_compile_globals.compiled_source_size = old_size + source_size;
			hs_compile_globals.compiled_source[old_size + source_size] = 0;

			*error_message_pointer = NULL;
			*error_source_pointer = NULL;

			hs_compile_globals.error_message = 0;
			hs_compile_globals.error_offset = -1;

			tokenizer.cursor = &hs_compile_globals.compiled_source[old_size];
			tokenizer.source_file_data = 0;
			tokenizer.source_file_size = 0;

			skip_whitespace(&tokenizer.cursor);
			if (*tokenizer.cursor)
			{
				int32 root_expression_index = hs_tokenize(&tokenizer);
				if (!hs_compile_globals.error_message)
				{
					int32 implicit_inspect_index = datum_new(g_hs_syntax_data);
					int32 implicit_inspect_name_index = datum_new(g_hs_syntax_data);
					if (implicit_inspect_index != NONE && implicit_inspect_name_index != NONE)
					{
						hs_syntax_node* implicit_inspect = hs_syntax_get(implicit_inspect_index);
						hs_syntax_node* implicit_inspect_name = hs_syntax_get(implicit_inspect_name_index);

						implicit_inspect->long_value = implicit_inspect_name_index;
						implicit_inspect->next_node_index = NONE;
						implicit_inspect->source_offset = hs_syntax_get(root_expression_index)->source_offset;
						implicit_inspect->flags = 0;

						implicit_inspect_name->next_node_index = root_expression_index;
						implicit_inspect_name->source_offset = NONE;
						implicit_inspect_name->function_index = _hs_function_inspect;
						implicit_inspect_name->type = _hs_function_name;
						implicit_inspect_name->flags = FLAG(_hs_syntax_node_primitive_bit);

						if (hs_parse(implicit_inspect_index, _hs_type_void))
						{
							result_expression_index = implicit_inspect_index;
						}
					}
				}

				if (result_expression_index == NONE)
				{
					*error_message_pointer = hs_compile_globals.error_message;
					if (hs_compile_globals.error_offset != -1)
					{
						hs_compile_globals.error_offset -= old_size;
						ASSERT(hs_compile_globals.error_offset >= 0 && hs_compile_globals.error_offset < source_size);
						*error_source_pointer = &source_data[hs_compile_globals.error_offset];
					}
				}
			}
			else
			{
				hs_runtime_require_gc();
			}
		}
		else
		{
			event(_event_error, "hs: not enough space to allocate a temporary hs compiled-source buffer! You got yourself into a REALLY weird state!(show Damian)");
		}
	}

	return result_expression_index;
}

bool hs_compile_get_tag_by_name(const char* group_name, tag* group_tag_out)
{
	bool success = false;
	tag group_tag_from_name = group_name_to_group_tag(group_name);
	if (group_tag_from_name != NONE)
	{
		*group_tag_out = group_tag_from_name;
		success = true;
	}
	return success;
}


//.text:0072F510 ; 
void hs_compile_initialize(bool permanent)
{
	ASSERT(!hs_compile_globals.initialized);

	hs_compile_globals.initialized = true;
	hs_compile_globals.compiled_source = NULL;
	hs_compile_globals.compiled_source_size = 0;
	hs_compile_globals.permanent = permanent;
	hs_compile_globals.error_message = NULL;
	hs_compile_globals.error_offset = NONE;
	hs_compile_globals.indent = 0;
	hs_compile_globals.current_script_index = NONE;
	hs_compile_globals.current_global_index = NONE;
	hs_compile_globals.references = NULL;
	hs_compile_globals.script_references = NULL;
	hs_compile_globals.global_references = NULL;
	hs_compile_globals.allocated_references = 0;

	if (permanent)
	{
		editor_reset_script_referenced_blocks();
		resize_scenario_syntax_data(k_maximum_hs_syntax_nodes_per_scenario);

		//scenario* scenario = global_scenario_get();
		//tag_block_delete_all_elements(&scenario->hs_scripts);
		//tag_block_delete_all_elements(&scenario->hs_globals);
		//tag_block_delete_all_elements(&scenario->hs_references);
		//tag_block_delete_all_elements(&scenario->hs_unit_seats);
		//tag_data_resize(&scenario->hs_string_constants, 0);
		//data_delete_all(g_hs_syntax_data);
		//hs_runtime_delete_internal_global_datums();

		hs_compile_globals.references =
			(s_hs_reference*)system_malloc(sizeof(s_hs_reference*) * k_maximum_number_of_references);
		hs_compile_globals.script_references =
			(s_hs_reference**)system_malloc(sizeof(s_hs_reference**) * k_maximum_hs_scripts_per_scenario);
		hs_compile_globals.global_references =
			(s_hs_reference**)system_malloc(sizeof(s_hs_reference**) * k_maximum_hs_globals_per_scenario);

		for (int32 script_index = 0; script_index < k_maximum_hs_scripts_per_scenario; script_index++)
		{
			hs_compile_globals.script_references = NULL;
		}

		for (int32 global_index = 0; global_index < k_maximum_hs_globals_per_scenario; global_index++)
		{
			hs_compile_globals.global_references = NULL;
		}
	}
}

//.text:0072F520 ; 
bool hs_compile_postprocess_and_verify(char const** error_message_pointer, char const** error_source_pointer, bool verbose)
{
	bool success = true;

#if defined(NETWORK_EXECUTABLE_TYPE_TAGS)

	hs_compile_globals.compiled_source = (char*)global_scenario_get()->hs_string_constants.address;
	hs_compile_globals.compiled_source_size = global_scenario_get()->hs_string_constants.size - 4096;
	hs_compile_globals.error_message = 0;
	hs_compile_globals.error_offset = NONE;
	hs_compile_globals.variables_predetermined = true;
	hs_compile_globals.current_script_index = NONE;
	hs_compile_globals.current_global_index = NONE;

	*error_message_pointer = NULL;
	*error_source_pointer = NULL;

	for (int32 node_index = data_next_index(g_hs_syntax_data, NONE);
		success&& node_index != NONE;
		node_index = data_next_index(g_hs_syntax_data, node_index))
	{
		hs_syntax_node* node = hs_syntax_get(node_index);
		hs_syntax_node temp_node = *node;

		if (!TEST_BIT(node->flags, _hs_syntax_node_stripped_bit))
		{
			if (hs_type_valid(node->type))
			{
				int16 return_type = 0;

				if (TEST_BIT(hs_syntax_get(node_index)->flags, _hs_syntax_node_primitive_bit))
				{
					if (node->type >= _hs_type_string || TEST_BIT(hs_syntax_get(node_index)->flags, _hs_syntax_node_variable_bit))
					{
						success = hs_verify_source_offset(node->source_offset);
						if (success)
						{
							success = hs_parse_primitive(node_index);
							if (!success && !TEST_BIT(hs_syntax_get(node_index)->flags, _hs_syntax_node_variable_bit))
							{
								if (node->type == _hs_type_script || node->type == _hs_type_ai_command_script)
								{
									success = node->short_value == NONE;
								}

								if (success)
								{
									hs_compile_globals.error_message = NULL;
									hs_compile_globals.error_offset = NONE;
								}
							}
						}
					}

					if (success && TEST_BIT(hs_syntax_get(node_index)->flags, _hs_syntax_node_variable_bit))
					{
						if (TEST_BIT(hs_syntax_get(node_index)->flags, _hs_syntax_node_parameter_bit))
						{
							return_type = node->type;
						}
						else
						{
							return_type = hs_global_get_type(node->data);
						}
					}
					else
					{
						return_type = node->constant_type;
					}
				}
				else
				{
					const char* name = NULL;
					if (TEST_BIT(node->flags, _hs_syntax_node_script_bit))
					{
						hs_script* script = NULL;
						if (VALID_INDEX(node->script_index, global_scenario_get()->hs_scripts.count)
							&& (script = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->hs_scripts, node->script_index, hs_script), script->script_type == _hs_script_static))
						{
							return_type = script->return_type;
							name = script->name;
						}
						else if (node->script_index == NONE)
						{
							return_type = _hs_type_void;
							name = "INVALID CALL";
						}
						else
						{
							hs_compile_globals.error_message = "bad script index (you need to recompile.)";
							hs_compile_globals.error_offset = node->source_offset;
							success = false;
						}
					}
					else
					{
						int32 expression_index = hs_syntax_get(node_index)->long_value;
						if (expression_index == NONE)
						{
							hs_compile_globals.error_message = "corrupt syntax tree (you need to recompile scripts.)";
							hs_compile_globals.error_offset = node->source_offset;
							success = false;
						}
						else
						{
							hs_syntax_node* expression = hs_syntax_get(expression_index);
							if (expression->type == _hs_function_name)
							{
								if (hs_verify_source_offset(expression->source_offset))
								{
									int16 function_index = hs_find_function_by_name(&hs_compile_globals.compiled_source[expression->source_offset],
										hs_count_children(node_index) - 1);
									if (function_index == NONE)
									{
										hs_compile_globals.error_message = "missing function (you need to recompile scripts.)";
										hs_compile_globals.error_offset = node->source_offset;
										success = false;
									}
									else if (node->function_index == function_index)
									{
										name = hs_function_get(node->constant_type)->name;
										return_type = hs_function_get(node->constant_type)->return_type;
									}
									else
									{
										hs_compile_globals.error_message = "function mismatch: missing function (you need to recompile scripts.)";
										hs_compile_globals.error_offset = node->source_offset;
										success = false;
									}
								}
								else
								{
									success = false;
								}
							}
							else
							{
								hs_compile_globals.error_message = "corrupt syntax tree (you need to recompile scripts.)";
								hs_compile_globals.error_offset = node->source_offset;
								success = false;
							}
						}
					}
				}

				if (success)
				{
					if ((hs_type_valid(return_type) || return_type == _hs_passthrough)
						&& hs_can_cast(return_type, node->type))
					{
						success = true;
					}
					else
					{
						hs_compile_globals.error_message = "type is inconsistent with usage (you need to recompile scripts.)";
						hs_compile_globals.error_offset = node->source_offset;
						success = false;
					}
				}
			}

			bool string_mismatch = true;
			if (TEST_BIT(hs_syntax_get(node_index)->flags, _hs_syntax_node_primitive_bit))
			{
				if (hs_syntax_get(node_index)->type == _hs_type_string && !TEST_BIT(hs_syntax_get(node_index)->flags, _hs_syntax_node_variable_bit))
				{
					string_mismatch = false;
				}
			}

			bool mismatch = string_mismatch || temp_node.type != node->type)
				&& (temp_node.data != node->data
					|| temp_node.type != node->type
					|| temp_node.flags != node->flags
					|| temp_node.line_number != node->line_number
					|| temp_node.next_node_index != node->next_node_index
					|| temp_node.source_offset != node->source_offset);

			VASSERT(!mismatch, "node data mismatch during hs_compile_postprocess_and_verify()! (byte swapping problem?)");
		}
	}

	if (!success)
	{
		*error_message_pointer = hs_compile_globals.error_message;
		if (hs_compile_globals.error_offset != NONE)
		{
			*error_source_pointer = &hs_compile_globals.compiled_source[hs_compile_globals.error_offset];
		}

		hs_compile_globals.compiled_source = NULL;
		hs_compile_globals.error_message = NULL;
		hs_compile_globals.error_offset = NONE;
		hs_compile_globals.variables_predetermined = false;
	}

#else
	hs_compile_postprocess_fast();
#endif

	return success;
}

//.text:0072F530 ; 
void hs_compile_postprocess_fast()
{
	for (int32 node_index = data_next_index(g_hs_syntax_data, NONE);
		node_index != NONE;
		node_index = data_next_index(g_hs_syntax_data, node_index))
	{
		hs_syntax_node* node = hs_syntax_get(node_index);
		if (TEST_BIT(node->flags, _hs_syntax_node_primitive_bit)
			&& node->type == _hs_type_string
			&& !TEST_BIT(node->flags, _hs_syntax_node_variable_bit))
		{
			node->string_value = (const char*)offset_pointer(global_scenario_get()->hs_string_constants.address, node->source_offset);
		}
	}
}

bool hs_compile_register_error_listener(c_hs_compile_error_listener* listener)
{
	bool successfully_added = 0;
	for (int32 compile_error_listener_index = 0; compile_error_listener_index < hs_static_globals.k_maximum_hs_compile_error_listeners; compile_error_listener_index++)
	{
		if (hs_static_globals.compile_error_listeners[compile_error_listener_index] == NULL)
		{
			hs_static_globals.compile_error_listeners[compile_error_listener_index] = listener;

			successfully_added = true;
			break;
		}
	}
	return successfully_added;
}

bool hs_compile_second_pass(s_hs_compile_state* compile_state, bool verbose)
{
	bool success = true;

	const struct scenario* scenario = global_scenario_get();
	for (int16 global_index = 0; global_index < scenario->hs_globals.count; global_index++)
	{
		hs_global_internal* global = TAG_BLOCK_GET_ELEMENT(&scenario->hs_globals, global_index, hs_global_internal);

		ASSERT(global->initialization_expression_index != NONE);
		ASSERT(hs_syntax_get(global->initialization_expression_index)->type == _hs_unparsed);

		hs_compile_globals.current_global_index = global_index;
		hs_compile_globals.error_message = 0;
		hs_compile_globals.error_offset = 0;

		if (!hs_parse(global->initialization_expression_index, global->type))
		{
			BIT_VECTOR_OR_FLAG(compile_state->failed_globals, global_index);
			if (verbose)
			{
				VASSERT(hs_compile_globals.error_message, "tell DAMIAN (or whomever owns HS) that somebody failed to correctly report a parsing error.");

				int32 error_offset_within_file = 0;
				hs_source_file const* error_source_file = source_offset_get_source_file(hs_compile_globals.error_offset, &error_offset_within_file);
				if (error_source_file)
				{
#if 0
					char* error_source = TAG_DATA_GET_POINTER(&error_source_file->source, 0, error_source_file->source.size, char*);
					const char* error_text = &error_source[error_offset_within_file];
					hs_compile_source_error(error_source_file->name, hs_compile_globals.error_message, error_text, error_source);
#endif
				}
			}

			success = false;
		}

		hs_compile_globals.current_global_index = NONE;
	}

	for (int16 script_index = 0; script_index < scenario->hs_scripts.count; script_index++)
	{
		hs_script* script = TAG_BLOCK_GET_ELEMENT(&scenario->hs_scripts, script_index, hs_script);

		ASSERT(script->root_expression_index != NONE);
		ASSERT(hs_syntax_get(script->root_expression_index)->type == _hs_unparsed);

		hs_compile_globals.current_script_index = script_index;
		hs_compile_globals.error_message = 0;
		hs_compile_globals.error_offset = 0;

		if (!hs_parse(script->root_expression_index, script->return_type))
		{
			ASSERT(VALID_INDEX(script_index, k_maximum_hs_scripts_per_scenario));
			BIT_VECTOR_OR_FLAG(compile_state->failed_scripts, script_index);

			if (verbose)
			{
				VASSERT(hs_compile_globals.error_message, "tell DAMIAN (or whomever owns HS) that somebody failed to correctly report a parsing error.");

				int32 error_offset_within_file = 0;
				hs_source_file const* error_source_file = source_offset_get_source_file(hs_compile_globals.error_offset, &error_offset_within_file);
				if (error_source_file)
				{
#if 0
					char* error_source = TAG_DATA_GET_POINTER(&error_source_file->source, 0, error_source_file->source.size, char*);
					const char* error_text = &error_source[error_offset_within_file];
					hs_compile_source_error(error_source_file->name, hs_compile_globals.error_message, error_text, error_source);
#endif
				}
			}

			success = false;
		}

		hs_compile_globals.current_script_index = NONE;
	}

	return success;
}

bool hs_compile_source(bool fail_on_error, bool verbose)
{
#if 1
	bool success = false;
#else
	bool success = true;

	progress_new("compiling scripts");
	hs_compile_initialize(true);

	s_hs_compile_state compile_state{};
	hs_compile_state_initialize(global_scenario_get(), &compile_state);
	
	if (g_error_output_buffer && verbose)
	{
		csstrnzcpy(g_error_output_buffer, "", g_error_buffer_length);
	}

	int32 total_source_size = 0;
	struct scenario* scenario = global_scenario_get();
	for (int16 source_index = 0; source_index < (int16)scenario->hs_source_files.count; source_index++)
	{
		hs_source_file* source_file = TAG_BLOCK_GET_ELEMENT(&scenario->hs_source_files, source_index, hs_source_file);

#if 0
		char* source_text = TAG_DATA_GET_POINTER(&source_file->source, 0, source_file->source.size, char*);
		if (source_text)
		{
			ascii_strnlwr(source_text, source_file->source.size);

			char const* error_message = NULL;
			int32 error_offset = NONE;
			hs_compile_first_pass(&compile_state, source_file->source.size, source_text, &error_message, &error_offset);
			total_source_size += source_file->source.size;
			if (error_message)
			{
				success = false;
				int32 error_offset_within_file = 0;
				if (verbose)
				{
					ASSERT(error_offset != NONE);
					hs_source_file* error_source_file = source_offset_get_source_file(error_offset, &error_offset_within_file);
					if (error_source_file)
					{
						char* error_source = TAG_DATA_GET_POINTER(&error_source_file->source, 0, error_source_file->source.size, char*);
						const char* error_text = &error_source[error_offset_within_file];
						hs_compile_source_error(error_source_file->name, error_message, error_text, error_source);
					}
				}
			}
		}
#endif
	}
	
	if (success || !fail_on_error)
	{
		success = hs_compile_second_pass(&compile_state, verbose);
		if (!success)
		{
			hs_compile_strip_failed_special_forms(&compile_state, verbose);
		}
	}
	
	hs_runtime_require_gc();
	
	if (verbose)
	{
		if (success)
		{
			console_printf("scripts successfully compiled.");
		}
		else
		{
			console_printf("script compile errors");
		}
	}
	
	if (!success)
	{
		success = !fail_on_error;
	}
	
	hs_compile_dispose();
	progress_done();
#endif
	
	return success;
}

void hs_compile_source_error(const char* file_name, const char* error_message, const char* error_source, const char* source)
{
	char const* newline = NULL;
	if (error_source)
	{
		newline = strchr(error_source, '\n');
		if (!newline)
		{
			newline = &error_source[strlen_debug(error_source)];
		}
	}

	c_wrapped_array<char const> bounded_expression(error_source, newline);
	if (file_name && newline)
	{
		int16 line_number = 1;
		while (newline > source)
		{
			if (*newline == '\n')
			{
				line_number++;
			}
			newline--;
		}

		event(_event_critical, "design: [%s line %d] %s: %.*s",
			file_name, line_number,
			error_message,
			bounded_expression.count(), bounded_expression.begin());

		if (g_error_output_buffer)
		{
			csnzappendf(g_error_output_buffer, g_error_buffer_length, "design: [%s line %d] %s: %.*s",
				file_name, line_number,
				error_message,
				bounded_expression.count(), bounded_expression.begin());
		}

		for (int32 compile_error_listener_index = 0; compile_error_listener_index < hs_static_globals.k_maximum_hs_compile_error_listeners; compile_error_listener_index++)
		{
			if (hs_static_globals.compile_error_listeners[compile_error_listener_index])
			{
				hs_static_globals.compile_error_listeners[compile_error_listener_index]->handle_error(
					file_name,
					line_number,
					error_message,
					error_source,
					&bounded_expression,
					source);
			}
		}
	}
	else
	{
		event(_event_critical, "design: %s: %.*s",
			error_message,
			bounded_expression.count(),
			bounded_expression.begin());

		if (g_error_output_buffer)
		{
			csnzappendf(g_error_output_buffer, g_error_buffer_length, "%s: %.*s\n",
				error_message,
				bounded_expression.count(),
				bounded_expression.begin());
		}
	}
}

void hs_compile_state_initialize(struct scenario* scenario, s_hs_compile_state* state)
{
	csmemset(state->failed_globals, 0, sizeof(state->failed_globals));
	csmemset(state->failed_scripts, 0, sizeof(state->failed_scripts));
}

void fail_special_form_recursive(int32* strip_globals, int32* strip_scripts, int32 index, e_reference_type type)
{
#if 0
	s_hs_reference* reference = NULL;
	struct scenario* scenario = global_scenario_get();
	switch (type)
	{
	case _hs_reference_type_global:
	{
		if (BIT_VECTOR_TEST_FLAG(strip_globals, index))
		{
			BIT_VECTOR_OR_FLAG(strip_globals, index);
			reference = hs_compile_globals.global_references[index];
			hs_global_internal* global = TAG_BLOCK_GET_ELEMENT(&scenario->hs_globals, index, hs_global_internal);
			fail_syntax_node_recursive(global->initialization_expression_index);
		}
	}
	break;
	case _hs_reference_type_script:
	{
		if (BIT_VECTOR_TEST_FLAG(strip_scripts, index))
		{
			BIT_VECTOR_OR_FLAG(strip_scripts, index);
			reference = hs_compile_globals.hs_compile_globals.script_references[index];
			hs_script* script = TAG_BLOCK_GET_ELEMENT(&scenario->hs_scripts, index, hs_script);
			fail_syntax_node_recursive(script->root_expression_index);
		}
	}
	break;
	}

	for (; reference; reference = reference->next)
	{
		if (reference->strong)
		{
			fail_special_form_recursive(strip_globals, strip_scripts, reference->index, reference->type);
		}
		else
		{
			hs_syntax_node* node = hs_syntax_get(reference->node_index);
			ASSERT(!TEST_BIT(node->flags, _hs_syntax_node_variable_bit));
			if (TEST_BIT(node->flags, _hs_syntax_node_script_bit))
			{
				ASSERT(scenario_get_hs_script(global_scenario_get(), node->script_index)->return_type == _hs_type_void);
				node->constant_type = NONE;
			}
			else if (node->type == _hs_type_script)
			{
				node->short_value = NONE;
			}
			else if (node->type == _hs_type_ai_command_script)
			{
				node->long_value = NONE;
			}
		}
	}
#endif
}

void hs_compile_strip_failed_special_forms(const s_hs_compile_state* compile_state, bool verbose)
{
	return;

	struct scenario* scenario = global_scenario_get();

	int32 strip_globals[9]{};
	int32 strip_scripts[32]{};

	for (int16 global_index = 0; global_index < scenario->hs_globals.count; global_index++)
	{
		if (BIT_VECTOR_TEST_FLAG(compile_state->failed_globals, global_index) && !BIT_VECTOR_TEST_FLAG(strip_globals, global_index))
		{
			fail_special_form_recursive(strip_globals, strip_scripts, global_index, _hs_reference_type_global);
		}
	}

	for (int16 script_index = 0; script_index < scenario->hs_scripts.count; script_index++)
	{
		ASSERT(VALID_INDEX(script_index, k_maximum_hs_scripts_per_scenario));

		if (BIT_VECTOR_TEST_FLAG(compile_state->failed_scripts, script_index) && !BIT_VECTOR_TEST_FLAG(strip_scripts, script_index))
		{
			fail_special_form_recursive(strip_globals, strip_scripts, script_index, _hs_reference_type_script);
		}
	}
}

void hs_compile_unregister_error_listener(c_hs_compile_error_listener* listener)
{
	for (int32 compile_error_listener_index = 0; compile_error_listener_index < hs_static_globals.k_maximum_hs_compile_error_listeners; compile_error_listener_index++)
	{
		if (hs_static_globals.compile_error_listeners[compile_error_listener_index] == listener)
		{
			hs_static_globals.compile_error_listeners[compile_error_listener_index] = NULL;
		}
	}
}

int16 hs_count_children(int32 expression_index)
{
	int16 result = 0;

	for (int32 child_index = hs_syntax_get(expression_index)->long_value;
		child_index != NONE;
		child_index = hs_syntax_get(child_index)->next_node_index)
	{
		result++;
	}

	return result;
}

bool hs_macro_function_parse(int16 function_index, int32 expression_index)
{
	bool success = true;

	const hs_function_definition* definition = hs_function_get(function_index);
	int32 parameters_index = hs_syntax_get(hs_syntax_get(expression_index)->long_value)->next_node_index;

	ASSERT(hs_type_valid(definition->return_type));

	int16 parameter_index;
	for (parameter_index = 0; success && parameter_index < definition->formal_parameter_count && parameters_index != NONE; parameter_index++)
	{
		hs_syntax_node* parameter = hs_syntax_get(parameters_index);
		if (hs_parse(parameters_index, definition->formal_parameters[parameter_index]))
		{
			parameters_index = parameter->next_node_index;
		}
		else
		{
			if (parameter->type == _hs_type_string_id && TEST_BIT(hs_syntax_get(parameters_index)->flags, _hs_syntax_node_primitive_bit))
			{
				csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
					"this is not a valid string for '%s'", definition->name);
				hs_compile_globals.error_message = hs_compile_globals.error_buffer;
				hs_compile_globals.error_offset = parameter->source_offset;
			}

			success = false;
		}
	}

	if (success && (parameter_index != definition->formal_parameter_count || parameters_index != NONE))
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"the \"%s\" call requires exactly %d arguments.", definition->name, definition->formal_parameter_count);
		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = hs_syntax_get(expression_index)->source_offset;
	}

	return success;
}

bool hs_parse(int32 expression_index, int16 expected_type)
{
	ASSERT(!hs_compile_globals.error_message);
	ASSERT(hs_type_valid(expected_type) || expected_type == _hs_special_form || expected_type == _hs_unparsed);

	bool success = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	if (expression->type == _hs_unparsed)
	{
		expression->type = expected_type;
		if (TEST_BIT(hs_syntax_get(expression_index)->flags, _hs_syntax_node_primitive_bit))
		{
			expression->constant_type = expected_type;
			success = hs_parse_primitive(expression_index);
		}
		else
		{
			success = hs_parse_nonprimitive(expression_index);
		}
	}
	return success;
}

bool hs_parse_ai(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	if (!HS_TYPE_IS_OBJECT(expression->type))
	{
		ASSERT(hs_syntax_get(expression_index)->type == _hs_type_ai);
		ASSERT(expression->constant_type == expression->type);
	}
	else
	{
		success = true;
	}

	bool object_type = false;
	if (global_scenario_index_get() != NONE)
	{
		int32 ai_index = NONE;
		if (object_type = ai_index_from_string(global_scenario_get(), source, &ai_index))
		{
			expression->long_value = ai_index;
			if (success)
			{
				expression->constant_type = _hs_type_ai;
			}
		}
	}

	if (!object_type && !success)
	{
		hs_compile_globals.error_message = "this is not a valid ai squad or squad group";
		hs_compile_globals.error_offset = expression->source_offset;
	}

	return object_type;
}

bool hs_parse_ai_behavior(int32 expression_index)
{
	bool result = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	int16 bindex = behavior_index_by_name(source);
	if (bindex != NONE)
	{
		expression->short_value = bindex;
		result = true;
	}
	else
	{
		hs_compile_globals.error_message = "not a valid behavior";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	return result;
}

bool hs_parse_ai_command_list(int32 expression_index)
{
	bool result = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(expression->type == _hs_type_ai_line);

	return result;
}

bool hs_parse_ai_command_script(int32 expression_index)
{
	bool result = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	int16 script_index = hs_find_script_by_name(source, 0);
	if (script_index != NONE)
	{
		hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, script_index, hs_script);
		if (script->script_type == _hs_script_command_script)
		{
			expression->short_value = script_index;
			hs_compile_add_reference(script_index, _hs_reference_type_script, expression_index);
			result = true;
		}
		else
		{
			hs_compile_globals.error_message = "script is not a command-script";
			hs_compile_globals.error_offset = expression->source_offset;
		}
	}
	else
	{
		hs_compile_globals.error_message = "this is not a valid command list script";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	return result;
}

bool hs_parse_ai_line(int32 expression_index)
{
	bool result = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_ai_line);
	ASSERT(expression->constant_type == expression->type);

	string_id parsed_string_id = string_id_retrieve(source);
	expression->string_id_value = parsed_string_id;
	return result;
}

bool hs_parse_ai_orders(int32 expression_index)
{
	bool result = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	int16 orders_index = orders_get_by_name(source);
	if (orders_index != NONE)
	{
		expression->short_value = orders_index;
		result = true;
	}
	else
	{
		hs_compile_globals.error_message = "not a valid order";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	return result;
}

bool hs_parse_boolean(int32 expression_index)
{
	bool result = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_boolean);
	ASSERT(expression->constant_type == expression->type);

	bool success = true;
	if (csstrcmp(source, "true") == 0 || csstrcmp(source, "on") == 0 || csstrcmp(source, "1") == 0)
	{
		success = true;
	}
	else if (csstrcmp(source, "false") == 0 || csstrcmp(source, "off") == 0 || csstrcmp(source, "0") == 0)
	{
		success = false;
	}
	else
	{
		success = false;

		hs_compile_globals.error_message = "i expected \"true\" or \"false\".";
		hs_compile_globals.error_offset = expression->source_offset;
		result = false;
	}
	expression->bool_value = success;

	return result;
}

bool hs_parse_budget_reference(int32 expression_index)
{
	bool result = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	expression->long_value = NONE;
	if (global_scenario_index_get() == NONE)
	{
		ASSERT(HS_TYPE_IS_BUDGET_REFERENCE(expression->type));

		struct scenario* scenario = global_scenario_get();
		for (int32 budget_reference_index = 0; budget_reference_index < scenario->budget_references.count; budget_reference_index++)
		{
			s_scenario_budget_reference& budget_reference = scenario->budget_references[budget_reference_index];
			if (budget_reference.reference.index != NONE && csstrcmp(budget_reference.reference.get_name(), source) == 0)
			{
				expression->long_value = budget_reference.reference.index;
				break;
			}
		}
	}

	return result;
}

void hs_parse_call_predicate(int32 expression_index, bool* is_function, bool* is_script)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32 predicate_index = expression->long_value;
	hs_syntax_node* predicate = hs_syntax_get(predicate_index);
	char* source_offset = &hs_compile_globals.compiled_source[predicate->source_offset];

	if (predicate->type == _hs_function_name)
	{
		ASSERT(predicate->function_index != NONE);
		expression->function_index = predicate->function_index;
	}
	else
	{
		int16 num_arguments = hs_count_children(expression_index) - 1;
		expression->function_index = hs_find_function_by_name(source_offset, num_arguments);
		predicate->type = _hs_function_name;

		if (expression->function_index == NONE)
		{
			expression->script_index = hs_find_script_by_name(source_offset, num_arguments);
			if (expression->script_index == NONE)
			{
				if (is_function)
				{
					*is_function = hs_find_function_by_name(source_offset, num_arguments) != NONE;
				}

				if (is_script)
				{
					*is_script = hs_find_script_by_name(source_offset, num_arguments) != NONE;
				}
			}
			else
			{
				expression->flags |= FLAG(_hs_syntax_node_script_bit);

				if (is_script)
				{
					*is_script = true;
				}
			}
		}
		else if (is_function)
		{
			*is_function = true;
		}

		predicate->constant_type = expression->constant_type;
	}
}

bool hs_parse_cinematic_lightprobe(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cinematic_lightprobe);
	ASSERT(expression->constant_type == expression->type);

	bool success = false;
	string_id cinematic_lightprobe_name_id = string_id_retrieve(source);
	int32 cinematic_light_probe_index = scenario_cinematic_lighting_palette_entry_get_by_name(global_scenario_get(), cinematic_lightprobe_name_id);
	if (cinematic_light_probe_index != NONE)
	{
		expression->long_value = cinematic_light_probe_index;
		success = true;
	}
	else
	{
		hs_compile_globals.error_message = "this is not a lightprobe name.";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	return success;
}

bool hs_parse_conversation(int32 expression_index)
{
	bool success = false;
	return success;
}

bool hs_parse_cutscene_camera_point(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_camera_point);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element(expression_index, (const char*)OFFSETOF(scenario_cutscene_camera_point, name), global_scenario_index_get(), &global_scenario_get()->cutscene_camera_points, sizeof(scenario_cutscene_camera_point)));
}

bool hs_parse_cutscene_flag(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_flag);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element_string_id(expression_index, (const int32*)OFFSETOF(scenario_cutscene_flag, name), global_scenario_index_get(), &global_scenario_get()->cutscene_flags, sizeof(scenario_cutscene_flag)));
}

bool hs_parse_cutscene_recording(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_recording);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element(expression_index, (const char*)OFFSETOF(recorded_animation_definition, name), global_scenario_index_get(), &global_scenario_get()->recorded_animations, sizeof(recorded_animation_definition)));
}

bool hs_parse_cutscene_title(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_cutscene_camera_point);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element_string_id(expression_index, (const int32*)OFFSETOF(s_scenario_cutscene_title, name), global_scenario_index_get(), &global_scenario_get()->cutscene_titles, sizeof(s_scenario_cutscene_title)));
}

bool hs_parse_designer_zone(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_designer_zone);

	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	if (global_scenario_index_get() != NONE)
	{
		const char* designer_zone_name = &hs_compile_globals.compiled_source[expression->source_offset];
		int32 designer_zone_index = NONE;
		if (global_scenario_try_and_get())
		{
			designer_zone_index = scenario_get_designer_zone_index_by_name(global_scenario_get(), designer_zone_name);
		}

		if (designer_zone_index != NONE)
		{
			expression->short_value = (int16)designer_zone_index;
			success = true;
		}
		else
		{
			hs_compile_globals.error_message = "this is not a valid zone set name.";
			hs_compile_globals.error_offset = expression->source_offset;
		}
	}
	else
	{
		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
	}

	return success;
}

bool hs_parse_device_group(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_device_group);

	bool success = hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element(expression_index, (const char*)OFFSETOF(scenario_device_group, name), global_scenario_index_get(), &global_scenario_get()->device_groups, sizeof(scenario_device_group)));
	if (success)
	{
		hs_syntax_node* expression = hs_syntax_get(expression_index);
		expression->long_value = device_group_get_from_scenario_index(expression->short_value);
	}

	return success;
}

bool hs_parse_enum(int32 expression_index)
{
	bool success = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	if (HS_TYPE_IS_ENUM(expression->type))
	{
		int16 enum_index = 0;
		const hs_enum_definition* enum_definition = &hs_enum_table[expression->type - FIRST_HS_ENUM_TYPE];
		ASSERT(enum_definition->count);

		for (; enum_index < enum_definition->count && csstricmp(source, enum_definition->identifiers[enum_index]); enum_index++);

		if (enum_index == enum_definition->count)
		{
			csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
				"%s must be ", hs_type_names[expression->type]);

			for (enum_index = 0; enum_index < enum_definition->count - 1; enum_index++)
			{
				csstrnzcat(hs_compile_globals.error_buffer, "\"", k_hs_compile_error_buffer_size);
				csstrnzcat(hs_compile_globals.error_buffer, enum_definition->identifiers[enum_index], k_hs_compile_error_buffer_size);
				csstrnzcat(hs_compile_globals.error_buffer, "\", ", k_hs_compile_error_buffer_size);
			}

			if (enum_definition->count > 1)
				csstrnzcat(hs_compile_globals.error_buffer, "or ", k_hs_compile_error_buffer_size);

			csstrnzcat(hs_compile_globals.error_buffer, "\"", k_hs_compile_error_buffer_size);
			csstrnzcat(hs_compile_globals.error_buffer, enum_definition->identifiers[enum_index], k_hs_compile_error_buffer_size);
			csstrnzcat(hs_compile_globals.error_buffer, "\".", k_hs_compile_error_buffer_size);

			hs_compile_globals.error_message = hs_compile_globals.error_buffer;
			hs_compile_globals.error_offset = expression->source_offset;
			success = false;
		}

		expression->short_value = enum_index;
	}
	else
	{
		csnzprintf(
			hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"corrupt enum expression (type %d constant-type %d)",
			expression->type,
			expression->constant_type);

		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;
		success = false;
	}

	return success;
}

bool hs_parse_folder(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	if (global_scenario_index_get() != NONE)
	{
		ASSERT(expression->type == _hs_type_folder);

		const char* folder_name = &hs_compile_globals.compiled_source[expression->source_offset];
		struct scenario* scenario = global_scenario_get();
		if (scenario->editor_folders.count > 0)
		{
			for (int32 index = 0; index < scenario->editor_folders.count; index++)
			{
				s_scenario_editor_folder* folder = TAG_BLOCK_GET_ELEMENT(&scenario->editor_folders, index, s_scenario_editor_folder);
				if (folder->name.is_equal(folder_name))
				{
					expression->long_value = index;
					success = true;
					break;
				}
			}
		}
		else
		{
			hs_compile_globals.error_message = "folder not found";
			hs_compile_globals.error_offset = expression->source_offset;
		}
	}
	else
	{
		hs_compile_globals.error_message = "cannot parse editor folder, no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
	}

	return success;
}

bool hs_parse_integer(int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_short_integer || expression->type == _hs_type_long_integer);
	ASSERT(expression->constant_type == expression->type);

	if (*source == '-')
	{
		source++;
	}

	bool result = true;
	bool error_occurred = false;
	while (*source)
	{
		if (!isdigit(*source))
		{
			if (error_occurred || *source != '.')
			{
				hs_compile_globals.error_message = "this is not a valid integer.";
				hs_compile_globals.error_offset = expression->source_offset;

				result = false;
				break;
			}
			error_occurred = true;
		}
		source++;
	}

	int32 source_value = atoi(&hs_compile_globals.compiled_source[expression->source_offset]);
	if (result && !(expression->type == _hs_type_short_integer || IN_RANGE_INCLUSIVE(source_value, -32767, 32768)))
	{
		hs_compile_globals.error_message = "shorts must be in the range [-32767, 32768].";
		hs_compile_globals.error_offset = expression->source_offset;
		result = false;
	}

	if (expression->type == _hs_type_long_integer)
	{
		expression->long_value = (int32)source_value;
	}
	else
	{
		expression->short_value = (int16)source_value;
	}

	return result;
}

bool hs_parse_nonprimitive(int32 expression_index)
{
	bool success = false;

	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32 predicate_index = hs_syntax_get(expression_index)->long_value;
	hs_syntax_node* predicate = hs_syntax_get(predicate_index);

	ASSERT(hs_type_valid(expression->type) || expression->type == _hs_special_form || expression->type == _hs_unparsed);

	hs_compile_globals.indent++;

	if (TEST_BIT(hs_syntax_get(predicate_index)->flags, _hs_syntax_node_primitive_bit))
	{
		bool function_exists = false;
		bool script_exists = false;
		ASSERT(expression->type != _hs_special_form);

		hs_parse_call_predicate(expression_index, &function_exists, &script_exists);
		if (expression->constant_type == NONE)
		{
			if (function_exists)
			{
				hs_compile_globals.error_message = "wrong number of arguments for function";
			}
			else if (script_exists)
			{
				hs_compile_globals.error_message = "wrong number of arguments for script";
			}
			else
			{
				hs_compile_globals.error_message = "this is not a valid function or script name.";
			}

			hs_compile_globals.error_offset = predicate->source_offset;
		}
		else if (TEST_BIT(expression->flags, _hs_syntax_node_script_bit))
		{
			hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, expression->script_index, hs_script);
			if (script->script_type == _hs_script_static)
			{
				if (expression->type && !hs_can_cast(script->return_type, expression->type))
				{
					csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
						"i expected a %s, but this script returns a %s.",
						hs_type_names[expression->type],
						hs_type_names[script->return_type]);
					hs_compile_globals.error_message = hs_compile_globals.error_buffer;
					hs_compile_globals.error_offset = expression->source_offset;
				}
				else
				{
					if (!expression->type)
					{
						expression->type = script->return_type;
					}

					int16 num_arguments = hs_count_children(expression_index) - 1;
					if (num_arguments == script->parameters.count)
					{
						int16 parameter_index = 0;
						int32 parameter_node_index = hs_syntax_get(predicate_index)->next_node_index;
						success = true;
						while (parameter_node_index != NONE && success)
						{
							ASSERT(parameter_index >= 0 && parameter_index < script->parameters.count);
							hs_script_parameter* parameter = TAG_BLOCK_GET_ELEMENT(&script->parameters, parameter_index, hs_script_parameter);
							success = hs_parse(parameter_node_index, parameter->type);
							parameter_node_index = hs_syntax_get(parameter_node_index)->next_node_index;
							parameter_index++;
						}

						if (success)
						{
							hs_compile_add_reference(expression->constant_type, _hs_reference_type_script, expression_index);
						}
					}
					else
					{
						hs_compile_globals.error_message = "wrong number of script arguments";
						hs_compile_globals.error_offset = expression->source_offset;
						success = false;
					}
				}
			}
			else
			{
				hs_compile_globals.error_message = "this is not a static script.";
				hs_compile_globals.error_offset = expression->source_offset;
			}
		}
		else
		{
			const hs_function_definition* function = hs_function_get(expression->function_index);
			if (expression->type && !hs_can_cast(function->return_type, expression->type))
			{
				csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
					"i expected a %s, but this function returns a %s.",
					hs_type_names[expression->type],
					hs_type_names[function->return_type]);

				hs_compile_globals.error_message = hs_compile_globals.error_buffer;
				hs_compile_globals.error_offset = expression->source_offset;
			}
			else if (hs_compile_globals.disallow_blocks
				&& (expression->constant_type == _hs_type_ai || expression->constant_type == _hs_type_ai_command_script))
			{
				hs_compile_globals.error_message = "it is illegal to block in this context.";
				hs_compile_globals.error_offset = expression->source_offset;
			}
			else if (hs_compile_globals.disallow_sets
				&& expression->constant_type == _hs_type_void)
			{
				hs_compile_globals.error_message = "it is illegal to set the value of variables in this context.";
				hs_compile_globals.error_offset = expression->source_offset;
			}
			else if (!TEST_BIT(function->flags, _hs_function_flag_command_script_atom)
				|| hs_compile_globals.current_script_index == NONE
				|| TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, hs_compile_globals.current_script_index, hs_script)->script_type == _hs_script_command_script)
			{
				if (!TEST_BIT(function->flags, _hs_function_flag_debug) || hs_compile_globals.current_script_index == NONE)
				{
					if (expression->type == _hs_unparsed && function->return_type != _hs_passthrough)
					{
						expression->type = function->return_type;
					}

					ASSERT(function->parse);
					success = function->parse(expression->function_index, expression_index);
				}
				else
				{
					hs_compile_globals.error_message = "it is illegal to call this function from a script file";
					hs_compile_globals.error_offset = expression->source_offset;
				}
			}
			else
			{
				hs_compile_globals.error_message = "it is illegal to call a command script atom from outside a command script";
				hs_compile_globals.error_offset = expression->source_offset;
			}
		}
	}
	else
	{
		if (expression->type == _hs_special_form)
		{
			csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
				"i expected %s, but i got an expression.",
				expression->type == _hs_special_form ? "\"script\" or \"global\"" : "a function name");

			hs_compile_globals.error_message = hs_compile_globals.error_buffer;
			hs_compile_globals.error_offset = expression->source_offset;
		}
	}

	hs_compile_globals.indent--;

	return success;
}

bool hs_parse_object(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(HS_TYPE_IS_OBJECT(expression->type));

	if (csstrcmp(&hs_compile_globals.compiled_source[expression->source_offset], "none") != 0)
	{
		expression->type += NUMBER_OF_HS_SCRIPT_TYPES;
		expression->constant_type = expression->type;
		success = hs_parse_object_and_object_name_internal(expression_index, (e_hs_type)expression->constant_type);
		expression->type -= NUMBER_OF_HS_SCRIPT_TYPES;

		if (!success && hs_parse_ai(expression_index))
		{
			hs_compile_globals.error_message = NULL;
			hs_compile_globals.error_offset = NONE;
			success = true;
		}
	}
	else
	{
		expression->long_value = NONE;
	}

	return success;
}

bool hs_parse_object_and_object_name_internal(int32 expression_index, e_hs_type byteswap_type)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	if (global_scenario_index_get() == NONE)
	{
		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	else
	{
		ASSERT(HS_TYPE_IS_OBJECT(byteswap_type) || HS_TYPE_IS_OBJECT_NAME(byteswap_type));
		ASSERT(HS_TYPE_IS_OBJECT_NAME(expression->type));

		int16 name_index = scenario_object_name_index_from_string(global_scenario_get(), source);
		if (name_index == NONE)
		{
			hs_compile_globals.error_message = "this is not a valid object name.";
			hs_compile_globals.error_offset = expression->source_offset;
		}
		else
		{
			scenario_object_name* object_name = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->object_names, name_index, scenario_object_name);
			if (object_name->object_type != NONE)
			{
				hs_compile_globals.error_message = "no object exists with this name.";
				hs_compile_globals.error_offset = expression->source_offset;
			}
			else if (object_name->scenario_datum_index == NONE)
			{
				hs_compile_globals.error_message = "this object no longer exists in the scenario.";
				hs_compile_globals.error_offset = expression->source_offset;
			}
			else if (TEST_BIT(hs_object_type_masks[expression->type - _hs_type_object_name], object_name->object_type))
			{
				if (HS_TYPE_IS_OBJECT(byteswap_type))
				{
					expression->long_value = name_index;
					success = true;
				}
				else if (HS_TYPE_IS_OBJECT_NAME(byteswap_type))
				{
					expression->short_value = name_index;
					success = true;
				}
				else
				{
					hs_compile_globals.error_message = "invalid byteswap type,";
					hs_compile_globals.error_offset = expression->source_offset;
				}
			}
			else
			{
				csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
					"this is not an object of type %s.", hs_type_names[expression->type]);

				hs_compile_globals.error_message = hs_compile_globals.error_buffer;
				hs_compile_globals.error_offset = expression->source_offset;
			}
		}
	}
	return success;
}

bool hs_parse_object_list(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(expression->type == _hs_type_object_list);

	expression->constant_type = _hs_type_object_name;
	expression->type = _hs_type_object_name;
	success = hs_parse_object_and_object_name_internal(expression_index, (e_hs_type)expression->constant_type);
	expression->type = _hs_type_object_list;

	return success;
}

bool hs_parse_object_name(int32 expression_index)
{
	return hs_parse_object_and_object_name_internal(expression_index, _hs_type_object_name);
}

bool hs_parse_point_ref(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	if (global_scenario_index_get() != NONE)
	{
		ASSERT(hs_syntax_get(expression_index)->type == _hs_type_point_ref);
		ASSERT(expression->constant_type == expression->type);

		const char* point_ref = &hs_compile_globals.compiled_source[expression->source_offset];
		const char* separator = strrchr(point_ref, '/');
		if (!separator)
		{
			int16 set_index = cs_point_set_index_by_name(point_ref);
			if (set_index != NONE)
			{
				expression->long_value = (set_index << 16) | UNSIGNED_SHORT_MAX;
				success = true;
			}
		}
		else
		{
			char namebuf[32]{};
			if (separator - point_ref < sizeof(namebuf))
			{
				csstrnzcpy(namebuf, point_ref, (separator - point_ref + 1) >= sizeof(namebuf) ? sizeof(namebuf) : separator - point_ref + 1);
				int16 set_index = cs_point_set_index_by_name(namebuf);
				int16 point_index = NONE;
				if (set_index != NONE)
				{
					point_index = cs_point_index_by_name(cs_get_point_set(set_index), separator + 1);
					if (point_index >= 0)
					{
						expression->long_value = (set_index << 16) | point_index;
						success = true;
					}
				}
			}
		}
	}

	if (!success)
	{
		hs_compile_globals.error_message = "this is not a valid point reference";
		hs_compile_globals.error_offset = expression->source_offset;
	}

	return success;
}

bool hs_parse_primitive(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	ASSERT(hs_type_valid(expression->type) || expression->type == _hs_special_form || expression->type == _hs_unparsed);

	if (expression->type == _hs_special_form)
	{
		hs_compile_globals.error_message = "i expected a script or variable definition.";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	else if (expression->type == _hs_type_void)
	{
		hs_compile_globals.error_message = "the value of this expression (in a <void> slot) can never be used.";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	else
	{
		if (!hs_compile_globals.variables_predetermined || TEST_BIT(expression->flags, _hs_syntax_node_variable_bit))
		{
			success = hs_parse_variable(expression_index);
		}

		if (!success
			&& expression->type
			&& !hs_compile_globals.error_message
			&& (!hs_compile_globals.variables_predetermined || !TEST_BIT(expression->flags, _hs_syntax_node_variable_bit)))
		{
			if (hs_type_primitive_parsers[expression->type])
			{
				success = hs_type_primitive_parsers[expression->type](expression_index);
			}
			else
			{
				csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
					"expressions of type %s are currently unsupported.",
					hs_type_names[expression->type]);

				hs_compile_globals.error_message = hs_compile_globals.error_buffer;
				hs_compile_globals.error_offset = expression->source_offset;
				success = false;
			}
		}
	}

	return success;
}

bool hs_parse_real(int32 expression_index)
{
	bool success = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* c = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_real);
	ASSERT(expression->constant_type == expression->type);

	if (*c == '-')
	{
		c++;
	}

	bool decimal = false;
	while (*c)
	{
		if (!isdigit(*c))
		{
			if (decimal || *c != '.')
			{
				hs_compile_globals.error_message = "this is not a valid real number.";
				hs_compile_globals.error_offset = expression->source_offset;
				success = false;

				break;
			}
			decimal = true;
		}
		c++;
	}

	expression->real_value = (real32)atof(&hs_compile_globals.compiled_source[expression->source_offset]);
	return success;
}

bool hs_parse_script(int32 expression_index)
{
	bool success = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_script);
	ASSERT(expression->constant_type == expression->type);

	int16 script_index = hs_find_script_by_name(source, NONE);
	if (script_index != NONE)
	{
		expression->short_value = script_index;
		hs_compile_add_reference(script_index, _hs_reference_type_script, expression_index);
	}
	else
	{
		hs_compile_globals.error_message = "this is not a valid script name.";
		hs_compile_globals.error_offset = expression->source_offset;
		success = false;
	}

	return success;
}

bool hs_parse_sound_tag_reference(int32 expression_index)
{
	bool parse_success = hs_parse_tag_reference(expression_index);
	if (!parse_success)
	{
		hs_syntax_get(expression_index)->long_value = NONE;
		hs_compile_globals.error_message = 0;
	}
	return parse_success;
}

bool hs_parse_special_form(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	if (!expression->type)
	{
		expression->type = _hs_special_form;
		if (TEST_BIT(hs_syntax_get(expression_index)->flags, _hs_syntax_node_primitive_bit))
		{
			hs_compile_globals.error_message = "i expected a script or variable definition.";
			hs_compile_globals.error_offset = expression->source_offset;
		}
		else
		{
			int32 predicate_index = hs_syntax_get(expression_index)->long_value;
			const hs_syntax_node* predicate = hs_syntax_get(predicate_index);

			hs_compile_globals.indent++;
			if (strcmp(&hs_compile_globals.compiled_source[predicate->source_offset], "global") == 0)
			{
				success = hs_add_global(expression_index);
			}
			else if (strcmp(&hs_compile_globals.compiled_source[predicate->source_offset], "script") == 0)
			{
				success = hs_add_script(expression_index);
			}
			else
			{
				hs_compile_globals.error_message = "i expected \"script\" or \"global\".";
				hs_compile_globals.error_offset = predicate->source_offset;
			}

			hs_compile_globals.indent--;
		}
	}
	return success;
}

bool hs_parse_starting_profile(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_starting_profile);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element(expression_index, (const char*)OFFSETOF(scenario_starting_profile, name), global_scenario_index_get(), &global_scenario_get()->player_starting_profile, sizeof(scenario_starting_profile)));
}

bool hs_parse_string(int32 expression_index)
{
	bool success = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_string);
	ASSERT(expression->constant_type == expression->type);

	expression->string_value = source;
	return success;
}

bool hs_parse_string_id(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_string_id);
	ASSERT(expression->constant_type == expression->type);

	string_id parsed_string_id = string_id_retrieve(source);
	if (parsed_string_id != NONE)
	{
		expression->string_id_value = parsed_string_id;
		success = true;
	}
	else
	{
		hs_compile_globals.error_message = "this is not a valid string parameter.";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	return success;
}

bool hs_parse_style(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_style);
	ASSERT(expression->constant_type == expression->type);

	const char* style_name = &hs_compile_globals.compiled_source[expression->source_offset];
	int32 style_index = style_get_by_name(style_name);
	if (style_index != NONE)
	{
		expression->long_value = style_index;
		success = true;
	}
	else
	{
		hs_compile_globals.error_message = "invalid style";
		hs_compile_globals.error_offset = expression->source_offset;
	}

	return success;
}

bool hs_parse_tag_block_element(int32 expression_index, const char* name_offset, int32 scenario_index, s_tag_block* block, int32 element_size)
{
	int32 offset = (int32)name_offset;

	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(element_size <= SHRT_MAX);
	ASSERT(offset + (k_tag_string_length - 1) < element_size);

	for (int32 block_index = 0; block_index < block->count; block_index++)
	{
		char* element = (char*)tag_block_get_element_with_size(block, block_index, element_size);
		if (ascii_stricmp(element + offset, source) == 0)
		{
			expression->short_value = (int16)block_index;
			success = true;
			break;
		}
	}

	if (success)
	{
		if (hs_compile_globals.permanent)
		{
			if (scenario_index == global_scenario_index_get())
			{
				//editor_register_script_referenced_block(block);
			}
			else
			{
				//tag_group_dependencies_register_dependency(global_scenario_index_get(), scenario_index);
			}
		}
	}
	else
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"this is not a valid %s name", hs_type_names[expression->type]);
		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;
	}

	return success;
}

bool hs_parse_tag_block_element_string_id(int32 expression_index, const int32* string_id_offset, int32 scenario_index, s_tag_block* block, int32 element_size)
{
	bool success = false;
	int32 offset = (int32)string_id_offset;

	hs_syntax_node* expression = hs_syntax_get(expression_index);
	const char* source = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT((offset + (int32)sizeof(string_id)) <= element_size);

	for (int32 block_index = 0; block_index < block->count; block_index++)
	{
		char* element = (char*)tag_block_get_element_with_size(block, block_index, element_size);

		string_id name_id = *(string_id*)(element + offset);
		if (name_id == string_id_retrieve(source))
		{
			expression->short_value = (int16)block_index;
			success = true;
			break;
		}
	}

	if (success)
	{
		if (hs_compile_globals.permanent)
		{
			if (scenario_index == global_scenario_index_get())
			{
				//editor_register_script_referenced_block(block);
			}
			else
			{
				//tag_group_dependencies_register_dependency(global_scenario_index_get(), scenario_index);
			}
		}
	}
	else
	{
		csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
			"this is not a valid %s name", hs_type_names[expression->type]);
		hs_compile_globals.error_message = hs_compile_globals.error_buffer;
		hs_compile_globals.error_offset = expression->source_offset;
	}

	return success;
}

bool hs_parse_tag_reference(int32 expression_index)
{
	bool success = true;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	if (global_scenario_index_get() == NONE)
	{
		expression->long_value = NONE;
	}
	else
	{
		ASSERT(HS_TYPE_IS_TAG_REFERENCE(expression->type));

		tag group_tag = hs_tag_reference_type_group_tags[expression->type - _hs_type_sound];
		const struct scenario* scenario = global_scenario_get();
		const char* tag_name = &hs_compile_globals.compiled_source[expression->source_offset];

		for (int16 reference_index = 0; reference_index < scenario->hs_references.count; reference_index++)
		{
			hs_tag_reference* reference = TAG_BLOCK_GET_ELEMENT(&scenario->hs_references, reference_index, hs_tag_reference);

			if (reference->reference.index != NONE)
			{
				if (csstrcmp(tag_get_name(reference->reference.index), tag_name) == 0 && reference->reference.group_tag == group_tag)
				{
					expression->long_value = reference->reference.index;
					break;
				}
			}
		}

		if (expression->long_value == NONE && !hs_compile_globals.permanent)
		{
			bool explicit_group_success = false;
			const char* separator = strrchr(tag_name, '.');
			if (separator)
			{
				const char* tag_file_type_name = separator + 1;
				int32 tag_name_length = separator - tag_name;
				if (hs_compile_get_tag_by_name(tag_file_type_name, &group_tag))
				{
					c_static_string<256> actual_tag_name;
					actual_tag_name.set_bounded(separator, tag_name_length);
					expression->long_value = tag_loaded(group_tag, actual_tag_name.get_string());
					explicit_group_success = expression->long_value == NONE;
				}
			}

			if (!explicit_group_success)
			{
				expression->long_value = tag_loaded(group_tag, separator);
			}
		}
	}

	return success;
}

bool hs_parse_tag_reference_not_resolving(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	if (global_scenario_index_get() == NONE)
	{
		expression->long_value = NONE;
		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	else if (expression->long_value == NONE && !hs_compile_globals.permanent)
	{
		const char* tag_name = &hs_compile_globals.compiled_source[expression->source_offset];
		tag group_tag = NONE;
		const char* separator = strrchr(tag_name, '.');
		if (separator)
		{
			const char* extension = separator + 1;
			int32 tag_file_type_name = separator - tag_name;
			if (hs_compile_get_tag_by_name(extension, &group_tag))
			{
				c_static_string<256> actual_tag_name;
				actual_tag_name.set_bounded(tag_name, tag_file_type_name);
				expression->long_value = tag_loaded(group_tag, actual_tag_name.get_string());
			}
		}
	}

	if (expression->long_value == NONE)
	{
		hs_compile_globals.error_message = "not a loaded tag";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	else
	{
		success = true;
	}

	return success;
}

bool hs_parse_trigger_volume(int32 expression_index)
{
	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_trigger_volume);

	return hs_check_block_index_type_and_return<int16>(hs_parse_tag_block_element_string_id(expression_index, (const int32*)OFFSETOF(scenario_trigger_volume, name), global_scenario_index_get(), &global_scenario_get()->trigger_volumes, sizeof(scenario_trigger_volume)));
}

bool hs_parse_unit_seat_mapping(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(expression->type == _hs_type_unit_seat_mapping);
	ASSERT(expression->constant_type == expression->type);

	const char* seat_substring = &hs_compile_globals.compiled_source[expression->source_offset];
	if (*seat_substring)
	{
		c_static_stack<s_hs_unit_seat_mapping, 256> unit_seat_mapping_stack;
		tag_iterator unit_iterator{};
		bool valid_mappings = true;

		tag_iterator_new(&unit_iterator, UNIT_TAG);
		for (int32 unit_definition_index = tag_iterator_next(&unit_iterator);
			unit_definition_index != NONE;
			unit_definition_index = tag_iterator_next(&unit_iterator))
		{
			s_hs_unit_seat_mapping potential_mapping{};
			potential_mapping.unit_definition_tag_index = unit_definition_index;
			if (hs_get_unit_seats_from_substring(unit_definition_index, seat_substring, &potential_mapping.unit_seats))
			{
				if (unit_seat_mapping_stack.full())
				{
					hs_compile_globals.error_message = "too many units match this seat substring";
					hs_compile_globals.error_offset = expression->source_offset;
					valid_mappings = false;
					break;
				}
				unit_seat_mapping_stack.push_back(potential_mapping);
			}
		}

		if (!valid_mappings || global_scenario_index_get() == NONE)
		{
			hs_compile_globals.error_message = "no scenario loaded";
			hs_compile_globals.error_offset = expression->source_offset;
		}
		else if (unit_seat_mapping_stack.count() <= 0)
		{
			hs_compile_globals.error_message = "no units match this seat substring";
			hs_compile_globals.error_offset = expression->source_offset;
		}
		else
		{
			struct scenario* scenario = global_scenario_get();
			int32 seat_mapping_start_index = NONE;
			int32 seat_mapping_count = unit_seat_mapping_stack.count();

			if (scenario->hs_unit_seats.count > 0)
			{
				const s_hs_unit_seat_mapping* new_seat_mapping_end = scenario->hs_unit_seats.begin();
				const s_hs_unit_seat_mapping* new_seat_mapping_start = scenario->hs_unit_seats.end();
				const s_hs_unit_seat_mapping* current_seat_mapping_start = unit_seat_mapping_stack.get_elements();
				const s_hs_unit_seat_mapping* current_seat_mapping_end = current_seat_mapping_start + unit_seat_mapping_stack.count();

				const s_hs_unit_seat_mapping* seat_match_start = std::search(new_seat_mapping_end, new_seat_mapping_start, current_seat_mapping_start, current_seat_mapping_end, hs_unit_seat_mappings_match);
				if (seat_match_start != new_seat_mapping_start)
				{
					seat_mapping_start_index = seat_match_start - new_seat_mapping_end;
				}
			}

			if (seat_mapping_start_index != NONE)
			{
				expression->long_value = hs_encode_unit_seat_mapping(seat_mapping_start_index, seat_mapping_count);
				success = true;
			}
		}
	}
	else
	{
		expression->long_value = NONE;
		success = true;
	}

	return success;
}

bool hs_parse_variable(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char* source_offset = hs_compile_globals.compiled_source + expression->source_offset;

	ASSERT(hs_type_valid(expression->type) || expression->type == _hs_unparsed);

	bool is_parameter = false;
	int16 type = NONE;
	if (hs_compile_globals.current_script_index != NONE && global_scenario_index_get() != NONE)
	{
		expression->short_value = hs_script_find_parameter_by_name(hs_compile_globals.current_script_index, source_offset);
		if (expression->short_value != NONE)
		{
			hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, hs_compile_globals.current_script_index, hs_script);
			hs_script_parameter* parameter = TAG_BLOCK_GET_ELEMENT(&script->parameters, expression->short_value, hs_script_parameter);

			type = parameter->type;
			is_parameter = true;
			success = true;
		}
	}

	if (!success && (!hs_compile_globals.variables_predetermined
		|| expression->type == NONE
		|| expression->short_value == NONE
		|| !TEST_BIT(expression->flags, _hs_syntax_node_parameter_bit)))
	{
		expression->short_value = hs_find_global_by_name(source_offset);
		if (expression->short_value != NONE)
		{
			type = hs_global_get_type(expression->short_value);
			success = true;
		}
	}

	if (success)
	{
		ASSERT(type != NONE);
		if (expression->type && !hs_can_cast(type, expression->type))
		{
			csnzprintf(hs_compile_globals.error_buffer, k_hs_compile_error_buffer_size,
				"i expected a value of type %s, but the variable %s has type %s",
				hs_type_names[expression->type],
				hs_global_get_name(expression->short_value),
				hs_type_names[type]);

			hs_compile_globals.error_message = hs_compile_globals.error_buffer;
			hs_compile_globals.error_offset = expression->source_offset;

			success = false;
		}
		else
		{
			if (expression->type == _hs_unparsed)
			{
				expression->type = type;
			}

			expression->flags |= FLAG(_hs_syntax_node_variable_bit);

			if (is_parameter)
			{
				expression->flags |= FLAG(_hs_syntax_node_parameter_bit);
			}
			else
			{
				hs_compile_add_reference(expression->long_value, _hs_reference_type_global, expression_index);
			}

			success = true;
		}
	}
	else if (hs_compile_globals.variables_predetermined)
	{
		if (expression->type == NONE || expression->long_value == NONE || !TEST_BIT(expression->flags, _hs_syntax_node_parameter_bit))
		{
			hs_compile_globals.error_message = "this is not a valid variable name.";
			hs_compile_globals.error_offset = expression->source_offset;

			success = false;
		}
		else
		{
			success = true;
		}
	}

	return success;
}

bool hs_parse_zone_set(int32 expression_index)
{
	bool success = false;
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	ASSERT(hs_syntax_get(expression_index)->type == _hs_type_zone_set);

	if (global_scenario_index_get() == NONE)
	{
		hs_compile_globals.error_message = "no scenario loaded";
		hs_compile_globals.error_offset = expression->source_offset;
	}
	else
	{
		const char* designer_zone_name = &hs_compile_globals.compiled_source[expression->source_offset];
		int32 designer_zone_index = NONE;
		if (global_scenario_try_and_get())
		{
			designer_zone_index = scenario_get_zone_set_index_by_name(global_scenario_get(), designer_zone_name, tag_name_strip_path(designer_zone_name) == designer_zone_name);
		}

		if (designer_zone_index == NONE)
		{
			hs_compile_globals.error_message = "this is not a valid zone set name.";
			hs_compile_globals.error_offset = expression->source_offset;
		}
		else
		{
			expression->short_value = (int16)designer_zone_index;
			success = true;
		}
	}

	return success;
}

int32 hs_source_pointer_get_line_number(const char* source_pointer, const char* source)
{
	int32 line_number = 1;

	ASSERT(source_pointer);

	for (; source < source_pointer; source++)
	{
		if (*source == '\n')
		{
			line_number++;
		}
	}

	return line_number;
}

void hs_token_primitive_until_delimiter(hs_tokenizer* state, int32 expression_index, int end_delimiter, const char* error_message)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	expression->source_offset = ++state->cursor - hs_compile_globals.compiled_source;
	while (*state->cursor && *state->cursor != end_delimiter)
	{
		state->cursor++;
	}

	if (*state->cursor)
	{
		*state->cursor++ = 0;
	}
	else
	{
		hs_compile_globals.error_message = error_message;
		hs_compile_globals.error_offset = expression->source_offset - 1;
	}
}

int32 hs_tokenize(hs_tokenizer* state)
{
	ASSERT(!hs_compile_globals.error_message);
	ASSERT(g_hs_syntax_data);
	
	int32 syntax_index = datum_new(g_hs_syntax_data);
	if (syntax_index == NONE)
	{
		hs_compile_globals.error_message = "i couldn't allocate a syntax node.";
		hs_compile_globals.error_offset = state->cursor - hs_compile_globals.compiled_source;
	}
	else
	{
		hs_syntax_node* node = hs_syntax_get(syntax_index);
		node->type = 0;
		node->flags = 0;
		node->script_index = NONE;
		node->source_offset = NONE;
		node->line_number = NONE;
		node->next_node_index = NONE;

		SET_BIT(node->flags, _hs_syntax_node_primitive_bit, *state->cursor != '(');

		if (TEST_BIT(node->flags, _hs_syntax_node_primitive_bit))
		{
			hs_tokenize_primitive(state, syntax_index);
		}
		else
		{
			hs_tokenize_nonprimitive(state, syntax_index);
		}

		if (node->source_offset != NONE && state->source_file_data)
		{
			int32 offset = state->source_file_size + node->source_offset - hs_compile_globals.compiled_source_size;
			ASSERT(offset >= 0 && offset < state->source_file_size);

			char const* source_pointer = &state->source_file_data[offset];
			node->line_number = (int16)hs_source_pointer_get_line_number(source_pointer, state->source_file_data);
		}
	}
	
	return syntax_index;
}

void hs_tokenize_nonprimitive(hs_tokenizer* state, int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	int32* tail = &expression->long_value;

	expression->source_offset = state->cursor - hs_compile_globals.compiled_source;
	state->cursor++;

	while (!hs_compile_globals.error_message)
	{
		char* last_expression_end = state->cursor;

		skip_whitespace(&state->cursor);
		if (state->cursor != last_expression_end)
		{
			*last_expression_end = 0;
		}

		if (*state->cursor)
		{
			if (*state->cursor == ')')
			{
				*state->cursor++ = 0;
				break;
			}

			*tail = hs_tokenize(state);
			if (*tail != NONE)
			{
				tail = &hs_syntax_get(*tail)->next_node_index;
			}
		}
		else
		{
			hs_compile_globals.error_message = "this left parenthesis is unmatched.";
			hs_compile_globals.error_offset = expression->source_offset;
		}
	}

	if (tail == &expression->long_value && !hs_compile_globals.error_message)
	{
		hs_compile_globals.error_message = "this expression is empty.";
		hs_compile_globals.error_offset = expression->source_offset;
	}
}

void hs_tokenize_primitive(hs_tokenizer* state, int32 expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);

	if (*state->cursor == '"')
	{
		hs_token_primitive_until_delimiter(state, expression_index, '"',
			"this quoted constant is unterminated.");
	}
	else if (*state->cursor == '{')
	{
		hs_token_primitive_until_delimiter(state, expression_index, '}',
			"this superstring constant is unterminated. (did you forget a '}' ?)");
	}
	else
	{
		expression->source_offset = state->cursor - hs_compile_globals.compiled_source;
		while (*state->cursor
			&& *state->cursor != ')'
			&& *state->cursor != ';'
			&& !character_in_list(*state->cursor, NUMBEROF(whitespace_characters), whitespace_characters)
			&& !character_in_list(*state->cursor, NUMBEROF(eol_characters), eol_characters))
		{
			state->cursor++;
		}
	}
}

void hs_validify_expression(const char* expression, char* out_valid_expression_buffer, int32 out_expression_length)
{
	c_wrapped_array<char> out_valid_expression(out_valid_expression_buffer, out_expression_length);

	const char* expression_end = find_string_end(expression, ";");
	const char* expression_begin = find_string_end_not_in_delimiter(expression, " \t\r\n");
	if (expression_begin > expression_end)
	{
		expression_begin = expression;
	}

	while (char_is_delimiter(*(expression_end - 1), " \t\r\n"))
	{
		expression_end--;
	}

	if (expression_begin >= expression_end)
	{
		csstrnzcpy(out_valid_expression.begin(), "", out_valid_expression.count());
	}
	else
	{
		e_hs_fakery fakery = _hs_fakery_none;
		char fakery_buffer[4096]{};
		string_copy_bounded(fakery_buffer, make_wrapped_array(expression_begin, expression_end));

		if (fakery_buffer[0] != '(')
		{
			char* space = strchr(fakery_buffer, ' ');
			if (space)
			{
				*space = 0;
			}

			if (hs_find_global_by_name(fakery_buffer) == NONE)
			{
				fakery = _hs_fakery_parenthesize;
			}
			else if (space)
			{
				fakery = _hs_fakery_set;
			}

			if (space)
			{
				*space = ' ';
			}
		}
		else
		{
			int32 expression_depth = 0;
			int32 expression_count = 0;
			for (char const* expression_character = expression_begin; expression_character < expression_end; ++expression_character)
			{
				if (*expression_character == '(')
				{
					++expression_count;
				}
				else if (*expression_character == ')')
				{
					if (!expression_count)
					{
						break;
					}

					if (!--expression_count)
					{
						++expression_depth;
					}
				}
			}

			if (expression_depth > 1)
			{
				fakery = _hs_fakery_begin;
			}
		}

		switch (fakery)
		{
		case _hs_fakery_none:
		{
			csstrnzcpy(out_valid_expression.begin(), fakery_buffer, out_valid_expression.count());
		}
		break;
		case _hs_fakery_parenthesize:
		{
			csnzprintf(out_valid_expression.begin(), out_valid_expression.count(), "(%s)", fakery_buffer);
		}
		break;
		case _hs_fakery_set:
		{
			csnzprintf(out_valid_expression.begin(), out_valid_expression.count(), "(set %s)", fakery_buffer);
		}
		break;
		case _hs_fakery_begin:
		{
			csnzprintf(out_valid_expression.begin(), out_valid_expression.count(), "(begin %s)", fakery_buffer);
		}
		break;
		default:
		{
			HALT();
		}
		break;
		}

		ascii_strnlwr(out_valid_expression.begin(), out_valid_expression.count());
	}
}

bool hs_verify_source_offset(int32 source_offset)
{
	bool success = true;
	if (source_offset < 0 || source_offset >= hs_compile_globals.compiled_source_size)
	{
		hs_compile_globals.error_message = "bad source offset (you need to recompile.)";
		hs_compile_globals.error_offset = NONE;
		success = false;
	}
	return success;
}

hs_type_primitive_parser_t* hs_type_primitive_parsers[NUMBER_OF_HS_NODE_TYPES]
{
	NULL,                                 // unparsed
	NULL,                                 // special_form
	NULL,                                 // function_name
	NULL,                                 // passthrough

	NULL,                                 // void
	hs_parse_boolean,                     // boolean
	hs_parse_real,                        // real32
	hs_parse_integer,                     // short_integer
	hs_parse_integer,                     // long_integer
	hs_parse_string,                      // string
	hs_parse_script,                      // script
	hs_parse_string_id,                   // string_id
	hs_parse_unit_seat_mapping,           // unit_seat_mapping,
	hs_parse_trigger_volume,              // trigger_volume,
	hs_parse_cutscene_flag,               // cutscene_flag,
	hs_parse_cutscene_camera_point,       // cutscene_camera_point,
	hs_parse_cutscene_title,              // cutscene_title,
	hs_parse_cutscene_recording,          // cutscene_recording,
	hs_parse_device_group,                // device_group,
	hs_parse_ai,                          // ai,
	hs_parse_ai_command_list,             // ai_command_list,
	hs_parse_ai_command_script,           // ai_command_script,
	hs_parse_ai_behavior,                 // ai_behavior,
	hs_parse_ai_orders,                   // ai_orders,
	hs_parse_ai_line,                     // ai_line,
	hs_parse_starting_profile,            // starting_profile,
	hs_parse_conversation,                // conversation,
	hs_parse_zone_set,                    // zone_set,
	hs_parse_designer_zone,               // designer_zone,
	hs_parse_point_ref,                   // point_reference,
	hs_parse_style,                       // style,
	hs_parse_object_list,                 // object_list,
	hs_parse_folder,                      // folder,

	hs_parse_sound_tag_reference,         // sound,
	hs_parse_tag_reference,               // effect,
	hs_parse_tag_reference,               // damage,
	hs_parse_tag_reference,               // looping_sound,
	hs_parse_tag_reference,               // animation_graph,
	hs_parse_tag_reference,               // damage_effect,
	hs_parse_tag_reference,               // object_definition,
	hs_parse_tag_reference,               // bitmap,
	hs_parse_tag_reference,               // shader,
	hs_parse_tag_reference,               // render_model,
	hs_parse_tag_reference,               // structure_definition,
	hs_parse_tag_reference,               // lightmap_definition,
	hs_parse_tag_reference,               // cinematic_definition,
	hs_parse_tag_reference,               // cinematic_scene_definition,
	hs_parse_tag_reference,               // bink_definition,
	hs_parse_tag_reference,               // any_tag,
	hs_parse_tag_reference_not_resolving, // any_tag_not_resolving,

	hs_parse_enum,                        // game_difficulty,
	hs_parse_enum,                        // team,
	hs_parse_enum,                        // mp_team,
	hs_parse_enum,                        // controller,
	hs_parse_enum,                        // button_preset,
	hs_parse_enum,                        // joystick_preset,
	hs_parse_enum,                        // player_character_type,
	hs_parse_enum,                        // voice_output_setting,
	hs_parse_enum,                        // voice_mask,
	hs_parse_enum,                        // subtitle_setting,
	hs_parse_enum,                        // actor_type,
	hs_parse_enum,                        // model_state,
	hs_parse_enum,                        // event,
	hs_parse_enum,                        // character_physics,
	hs_parse_enum,                        // primary_skull,
	hs_parse_enum,                        // secondary_skull,

	hs_parse_object,                      // object,
	hs_parse_object,                      // unit,
	hs_parse_object,                      // vehicle,
	hs_parse_object,                      // weapon,
	hs_parse_object,                      // device,
	hs_parse_object,                      // scenery,
	hs_parse_object,                      // effect_scenery,

	hs_parse_object_name,                 // object_name,
	hs_parse_object_name,                 // unit_name,
	hs_parse_object_name,                 // vehicle_name,
	hs_parse_object_name,                 // weapon_name,
	hs_parse_object_name,                 // device_name,
	hs_parse_object_name,                 // scenery_name,
	hs_parse_object_name,                 // effect_scenery_name,

	hs_parse_cinematic_lightprobe,        // cinematic_lightprobe,

	hs_parse_budget_reference,            // animation_budget_reference,
	hs_parse_budget_reference,            // looping_sound_budget_reference,
	hs_parse_budget_reference,            // sound_budget_reference,
};

