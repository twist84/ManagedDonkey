#include "interface/debug_menu/debug_menu_parse.hpp"

#include "interface/debug_menu/debug_menu.hpp"
#include "interface/debug_menu/debug_menu_item.hpp"
#include "interface/debug_menu/debug_menu_item_hs_command.hpp"
#include "interface/debug_menu/debug_menu_item_numbered.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "hs/hs_scenario_definitions.hpp"
#include "main/console.hpp"

#include <ctype.h>
#include <math.h>
#include <string.h>

s_parser_state g_parser_state = {};

void s_parser_state::reset()
{
	m_name = 0;
	m_color = 0;
	m_variable = 0;
	m_caption = 0;
	m_min = 0;
	m_max = 0;
	m_inc = 0;
	m_item_type = 0;
	m_xml_attribute = 0;
	m_xml_element = 0;
}

char const* const g_token_names[k_token_count]
{
	"none",
	"min",
	"max",
	"inc",
	"menu",
	"zone_set_menu",
	"item",
	"caption",
	"name",
	"variable",
	"color",
	"type",
	"global",
	"command",
	"\r\n",
	"\n\r",
	"\r",
	"\n",
};

enum e_parse_state
{
	_parse_state_none = 0,
	_parse_state_reading_tag,
	_parse_state_reading_open_tag,
	_parse_state_reading_forward_slash,
	_parse_state_reading_property,
	_parse_state_reading_property_found_eqauls,
	_parse_state_unknown6,
	_parse_state_reading_string,
	_parse_state_reading_escape_sequence,
	_parse_state_reading_number,

	k_parse_state_count
};

using parse_stack_t = c_static_stack<e_parse_state, k_parse_state_count>;

long debug_menu_memory_available()
{
	return 4 * (64000 - g_debug_menu_stack.count());
}

void debug_menu_look_ahead_read_token(FILE* menu_file, long c, char* token_buffer, long token_buffer_count)
{
	// #TODO: implement this
}

bool string_in_string_case_insensitive(char const* source, char const* find)
{
	ASSERT(source && find);

	while (*source && *find)
	{
		char source_char = tolower(*source);
		char find_char = tolower(*find);

		if (source_char != find_char)
			return false;

		source++;
		find++;
	}

	return (*find == 0);
}

void debug_menu_store_number_property(parse_stack_t* parse_stack)
{
	ASSERT(*parse_stack->get_top() == _parse_state_reading_number);

	parse_stack->pop();
	ASSERT(*parse_stack->get_top() == _parse_state_reading_property_found_eqauls);

	parse_stack->pop();
	ASSERT(*parse_stack->get_top() == _parse_state_reading_property);

	parse_stack->pop();
	ASSERT(*parse_stack->get_top() == _parse_state_reading_open_tag);

	ASSERT(VALID_INDEX(g_parser_state.m_number_buffer_index, s_parser_state::k_string_length));

	g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index] = 0;
	switch (g_parser_state.m_xml_attribute)
	{
	case 6:
		g_parser_state.m_min = true;
		g_parser_state.m_min_value = real(atof(g_parser_state.m_number_buffer));
		break;
	case 7:
		g_parser_state.m_max = true;
		g_parser_state.m_max_value = real(atof(g_parser_state.m_number_buffer));
		break;
	case 8:
		g_parser_state.m_inc = true;
		g_parser_state.m_inc_value = real(atof(g_parser_state.m_number_buffer));
		break;
	}
}

void debug_menu_store_string_property(parse_stack_t* parse_stack)
{
	ASSERT(*parse_stack->get_top() == _parse_state_reading_string);

	parse_stack->pop();
	ASSERT(*parse_stack->get_top() == _parse_state_reading_property_found_eqauls);

	parse_stack->pop();
	ASSERT(*parse_stack->get_top() == _parse_state_reading_property);

	parse_stack->pop();
	ASSERT(*parse_stack->get_top() == _parse_state_reading_open_tag);

	ASSERT(VALID_INDEX(g_parser_state.m_string_buffer_index, s_parser_state::k_string_length));

	g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index] = 0;
	switch (g_parser_state.m_xml_attribute)
	{
	case 2:
		g_parser_state.m_color = true;
		csstrnzcpy(g_parser_state.m_color_buffer, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
		break;
	case 3:
		g_parser_state.m_caption = true;
		csstrnzcpy(g_parser_state.m_caption_buffer, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
		break;
	case 4:
		g_parser_state.m_name = true;
		csstrnzcpy(g_parser_state.m_name_buffer, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
		break;
	case 5:
		g_parser_state.m_variable = true;
		csstrnzcpy(g_parser_state.m_variable_buffer, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
		break;
	}
}

char const* debug_menu_build_item_hs_variable_global(c_debug_menu* menu, char* error_buffer, long error_buffer_size)
{
	if (!g_parser_state.m_variable)
	{
		csnzprintf(error_buffer, error_buffer_size, "global variable debug menu items must define a variable");
		return error_buffer;
	}

	char const* name = g_parser_state.m_name ? g_parser_state.m_name_buffer : g_parser_state.m_variable_buffer;

	c_debug_menu_item* item = NULL;

	long console_global_index = NONE; // find_console_global_by_name(g_parser_state.m_variable_buffer, NULL);
	if (console_global_index != NONE && k_console_globals[console_global_index]->pointer)
	{
		e_hs_type type = k_console_globals[console_global_index]->type;
		switch (type)
		{
		case _hs_type_boolean:
		{
			item = new c_debug_menu_item_type_bool(menu, name, false, g_parser_state.m_variable_buffer);
		}
		break;
		case _hs_type_real:
		{
			real inc_value = g_parser_state.m_inc ? g_parser_state.m_inc : 0.1f;
			real max_value = g_parser_state.m_max ? g_parser_state.m_max_value : 3.4028235e38f;
			real min_value = g_parser_state.m_min ? g_parser_state.m_min_value : -3.4028235e38f;

			item = new c_debug_menu_item_type_real(menu, name, NULL, g_parser_state.m_variable_buffer, min_value, max_value, inc_value);
		}
		break;
		case _hs_type_short_integer:
		{
			short inc_value = g_parser_state.m_inc ? g_parser_state.m_inc : 1;
			short max_value = g_parser_state.m_max ? short(g_parser_state.m_max_value) : INT16_MAX - 1;
			short min_value = g_parser_state.m_min ? short(g_parser_state.m_min_value) : ~(INT16_MAX - 1);

			item = new c_debug_menu_item_type_short(menu, name, NULL, g_parser_state.m_variable_buffer, min_value, max_value, inc_value);
		}
		break;
		case _hs_type_long_integer:
		{
			long inc_value = g_parser_state.m_inc ? g_parser_state.m_inc : 1;
			long max_value = g_parser_state.m_max ? long(g_parser_state.m_max_value) : INT32_MAX - 1;
			long min_value = g_parser_state.m_min ? long(g_parser_state.m_min_value) : ~(INT32_MAX - 1);

			item = new c_debug_menu_item_type_long(menu, name, NULL, g_parser_state.m_variable_buffer, min_value, max_value, inc_value);
		}
		break;
		}
	}

	if (!item)
	{
		char undefined_name[1024]{};
		csnzprintf(undefined_name, sizeof(undefined_name), "UNDEFINED: %s", g_parser_state.m_variable_buffer);
		item = new c_debug_menu_item(menu, undefined_name, NULL, false);
	}
	ASSERT(item != NULL);

	menu->add_item(item);

	return NULL;
}

char const* debug_menu_build_item_command(c_debug_menu* menu, char* error_buffer, long error_buffer_size)
{
	if (!g_parser_state.m_variable)
	{
		csnzprintf(error_buffer, error_buffer_size, "command menu items must define a variable");
		return error_buffer;
	}

	char const* name = g_parser_state.m_name ? g_parser_state.m_name_buffer : g_parser_state.m_variable_buffer;
	char const* command = g_parser_state.m_variable_buffer;
	menu->add_item(new c_debug_menu_item_hs_command(menu, name, command));

	return NULL;
}

char const* debug_menu_build_item(c_debug_menu* menu, char* error_buffer, long error_buffer_size)
{
	if (!g_parser_state.m_item_type)
	{
		csnzprintf(error_buffer, error_buffer_size, "menu items must supply a type");
		return error_buffer;
	}

	switch (g_parser_state.m_item_type_value)
	{
	case 1:
		return debug_menu_build_item_hs_variable_global(menu, error_buffer, error_buffer_size);
	case 2:
		return debug_menu_build_item_command(menu, error_buffer, error_buffer_size);
	default:
		ASSERT2(unreachable);
		break;
	}

	return NULL;
}

c_debug_menu* debug_menu_build_menu(long xml_element, c_debug_menu* item_menu)
{
	// #TODO: implement this
	return NULL;
}

void debug_menu_display_error(char const* error_text, bool log_as_warning)
{
	// #TODO: implement this
}

char const* debug_menu_build_recursive(FILE* menu_file, long& char_ref, c_debug_menu* menu, long* line_count, char* error_buffer, long error_buffer_size)
{
	char const* result = NULL;

	char const* error_text = NULL;
	char const* error = NULL;

	c_static_stack<e_parse_state, 10> parse_stack;
	parse_stack.push_back(_parse_state_none);

	g_parser_state.reset();

	ASSERT(menu_file != NULL);
	ASSERT(menu != NULL);
	ASSERT(line_count != NULL);
	ASSERT(error_buffer != NULL);

	long c;
	while (char_ref && char_ref != NONE && !error_text)
	{
		long advance_distance = 0;
		long v15 = 0;

		if (debug_menu_memory_available() < 7168)
		{
			error = "out of memory, please make your debug menu smaller";
			break;
		}

		if (*parse_stack.get_top() == _parse_state_reading_number)
		{
			if (char_ref >= '0' && char_ref <= '9' || char_ref == '.')
			{
				ASSERT(VALID_INDEX(g_parser_state.m_number_buffer_index, NUMBEROF(g_parser_state.m_number_buffer)));
				g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index++] = char(char_ref);

				v15 = 1;
				advance_distance = 1;
			}
			else
			{
				debug_menu_store_number_property(&parse_stack);
				if (char_ref)
					char_ref = fgetc(menu_file);
			}
		}

		if (*parse_stack.get_top() == _parse_state_reading_number)
			goto LABEL_124;


		if (*parse_stack.get_top() == _parse_state_reading_escape_sequence)
		{
			parse_stack.pop();

			if (*parse_stack.get_top() != _parse_state_reading_string)
			{
				error = "*parse_stack.get_top()==_parse_state_reading_string";
				break;
			}

			if (VALID_INDEX(g_parser_state.m_string_buffer_index, s_parser_state::k_string_length))
			{
				error = "VALID_INDEX(g_parser_state.m_string_buffer_index, s_parser_state::k_string_length)";
				break;
			}

			g_parser_state.m_string_buffer[g_parser_state.m_string_buffer_index++] = char(char_ref);

			v15 = 1;
			advance_distance = 1;
			goto LABEL_124;
		}

		if (*parse_stack.get_top() == _parse_state_reading_string && char_ref != '"')
		{
			if (char_ref == '\\')
			{
				if (*parse_stack.get_top() != _parse_state_reading_string)
				{
					error = "can not use escape sequences outside of string declaration";
					break;
				}

				parse_stack.push_back(_parse_state_reading_escape_sequence);
			}
			else
			{
				if (VALID_INDEX(g_parser_state.m_string_buffer_index, s_parser_state::k_string_length))
				{
					error = "VALID_INDEX(g_parser_state.m_string_buffer_index, s_parser_state::k_string_length)";
					break;
				}
			}

			v15 = 1;
			advance_distance = 1;
			goto LABEL_124;
		}

		if (char_ref >= '0' && char_ref <= '9')
		{
			if (*parse_stack.get_top() != _parse_state_reading_property_found_eqauls)
			{
				error = "losse number not assigned to property";
				break;
			}

			parse_stack.push_back(_parse_state_reading_number);
			g_parser_state.m_number_buffer_index = 0;
			g_parser_state.m_number_buffer[0] = char(char_ref);
			g_parser_state.m_number_buffer_index = 1;

			v15 = 1;
			advance_distance = 1;
			goto LABEL_124;
		}

		c = char_ref;
		if (c <= '/')
		{
			if (c == '/')
			{
				v15 = 1;
				advance_distance = 1;
				if (*parse_stack.get_top() == _parse_state_reading_tag)
				{
					error = "unexpected symbol back slash";
					break;
				}
				parse_stack.push_back(_parse_state_reading_forward_slash);
			}
			else if (c > ' ')
			{
				if (c == '"')
				{
					e_parse_state v35 = *parse_stack.get_top();
					if (v35 == _parse_state_reading_property_found_eqauls)
					{
						v15 = 1;
						advance_distance = 1;
						g_parser_state.m_string_buffer_index = 0;
						parse_stack.push_back(_parse_state_reading_string);
					}
					else
					{
						if (v35 != _parse_state_reading_string)
						{
							error = "unexpected symbol \"";
							break;
						}

						v15 = 1;
						advance_distance = 1;
						debug_menu_store_string_property(&parse_stack);
					}
				}
				else if (c > ',')
				{
					if (*parse_stack.get_top() != _parse_state_reading_property_found_eqauls)
					{
						error = "losse number not assigned to property";
						break;
					}
					parse_stack.push_back(_parse_state_reading_number);
					g_parser_state.m_number_buffer_index = 0;
					g_parser_state.m_number_buffer[0] = char(char_ref);
					g_parser_state.m_number_buffer_index = 1;
					v15 = 1;
					advance_distance = 1;
				}
			}
			else if (c == ' ' || c == '\xCD' || c == '\t')
			{
				v15 = 1;
				advance_distance = 1;
			}

			goto LABEL_124;
		}

		if (c > '>')
		{
			if (c == '\\')
			{
				error = "can not use escape sequences outside of string declaration";
				break;
			}

		LABEL_124:
			if (!v15)
			{
				long v19 = 0;
				long maximum_token_name_length = 0;
				for (long i = 0; i < NUMBEROF(g_token_names); i++)
				{
					long token_name_length = strlen(g_token_names[i]);
					if (token_name_length > maximum_token_name_length)
						maximum_token_name_length = token_name_length;
				}

				char token_buffer[1024]{};
				ASSERT(maximum_token_name_length + 1 < NUMBEROF(token_buffer));

				debug_menu_look_ahead_read_token(menu_file, char_ref, token_buffer, maximum_token_name_length + 1);

				for (long i = 0; i < NUMBEROF(g_token_names); i++)
				{
					if (string_in_string_case_insensitive(token_buffer, g_token_names[i]))
					{
						v19 = i;

						advance_distance = strlen(g_token_names[i]);
						v15 = 1;
						break;
					}
				}

				// #TODO: implement this
				//switch (v19)
				//{
				//case 0:
				//	break;
				//}
			}

			long v44 = v15;
			long v45 = v15;
			if (v15)
			{
				if (v45 == 1)
				{
					ASSERT(advance_distance != 0);
					while (advance_distance > 0)
					{
						char_ref = fgetc(menu_file);
						advance_distance--;
					}
				}
				else if (v45 > 2)
				{
					ASSERT2(unreachable);
				}
			}
			else
			{
				error = "unexpected token";
			}

			break;
		}

		if (c != '>')
		{
			if (c == '<')
			{
				v15 = 1;
				advance_distance = 1;

				if (*parse_stack.get_top() == _parse_state_reading_tag)
				{
					error = "unexpected symbol less than";
					break;
				}

				parse_stack.push_back(_parse_state_reading_tag);
				g_parser_state.reset();
			}
			else if (c == '=')
			{
				v15 = 1;
				advance_distance = 1;

				if (*parse_stack.get_top() != _parse_state_reading_property)
				{
					error = "= sign expected";
					break;
				}

				if (*parse_stack.get_top() != _parse_state_reading_property)
				{
					error = "*parse_stack.get_top() == _parse_state_reading_property";
					break;
				}

				parse_stack.push_back(_parse_state_reading_property_found_eqauls);
			}

			goto LABEL_124;
		}

		v15 = 1;
		advance_distance = 1;

		if (*parse_stack.get_top() == _parse_state_reading_open_tag)
		{
			parse_stack.pop();
			if (*parse_stack.get_top() != _parse_state_reading_tag)
			{
				error = "*parse_stack.get_top() == _parse_state_reading_tag";
				break;
			}
			parse_stack.pop();
			if (g_parser_state.m_xml_element < 2 || g_parser_state.m_xml_element > 11)
			{
				char build_item_error[1024]{};
				if (debug_menu_build_item(menu, build_item_error, sizeof(build_item_error)))
				{
					error = build_item_error;
					break;
				}
				else
				{
					csstrnzcpy(build_item_error, "", sizeof(build_item_error));
					c_debug_menu* built_menu = debug_menu_build_menu(g_parser_state.m_xml_element, menu);
					if (!built_menu)
					{
						error = build_item_error;
						break;
					}

					v15 = 2;
					char_ref = fgetc(menu_file);
					if (char const* v18 = debug_menu_build_recursive(menu_file, char_ref, built_menu, line_count, error_buffer, error_buffer_size))
					{
						result = v18;
						break;
					}
				}
			}

			goto LABEL_124;
		}

		if (*parse_stack.get_top() == _parse_state_reading_forward_slash)
		{
			parse_stack.pop();
			if (*parse_stack.get_top() == _parse_state_reading_tag)
			{
				parse_stack.pop();
				char_ref = fgetc(menu_file);
				result = error_text;

				break;
			}

			error = "*parse_stack.get_top() == _parse_state_reading_tag";
		}
		else
		{
			error = "unexpected symbol greater than";
		}
	}

	if (error)
	{
		csnzprintf(error_buffer, error_buffer_size, "ln %d: %s", *line_count, error);
		error_text = error_buffer;
		debug_menu_display_error(error_buffer, true);
	}

	if (error_text)
		result = error_text;

	return result;
}

void debug_menu_parse(c_debug_menu* root_menu, char const* file_name)
{
	ASSERT(file_name != NULL);
	ASSERT(root_menu != NULL);

	FILE* menu_file;
	if (fopen_s(&menu_file, file_name, "rt") == 0 && menu_file)
	{
		char error_buffer[1024]{};

		long line_count = 1;
		long c = (long)fgetc(menu_file);
		debug_menu_build_recursive(menu_file, c, root_menu, &line_count, error_buffer, sizeof(error_buffer));
		fclose(menu_file);
	}
}

