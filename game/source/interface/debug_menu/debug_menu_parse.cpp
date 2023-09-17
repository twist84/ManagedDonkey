#include "interface/debug_menu/debug_menu_parse.hpp"

#include "cseries/cseries_events.hpp"
#include "interface/debug_menu/debug_menu.hpp"
#include "interface/debug_menu/debug_menu_item.hpp"
#include "interface/debug_menu/debug_menu_item_hs_command.hpp"
#include "interface/debug_menu/debug_menu_item_numbered.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/debug_menu/debug_menu_scroll.hpp"
#include "hs/hs_scenario_definitions.hpp"
#include "main/console.hpp"

#include <ctype.h>
#include <math.h>
#include <string.h>

#define GET_STATE parse_stack[stack_index]
#define PUSH_STATE(STATE) ASSERT(stack_index < stack_size - 1); stack_index++; parse_stack[stack_index] = STATE;
#define POP_STATE ASSERT(stack_index > 0); stack_index--;

#define PARSER_ASSERT(STATEMENT) PARSER_ASSERT_WITH_MESSAGE(STATEMENT, #STATEMENT)
#define PARSER_ASSERT_WITH_MESSAGE(STATEMENT, MESSAGE) \
if (!(STATEMENT)) \
{ \
	csnzprintf(error_buffer, error_buffer_size, "ln %d: %s", *line_count, MESSAGE); \
	parse_error = error_buffer; \
	debug_menu_display_error(error_buffer, true); \
	continue; \
} else 

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
	m_property_owner = _property_owner_none;
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
	return 4 * (262144 - g_debug_menu_stack.count());
}

void debug_menu_look_ahead_read_token(FILE* menu_file, long c, char* token_buffer, long token_buffer_count)
{
	long menu_file_size = ftell(menu_file);
	*token_buffer = char(c);
	token_buffer[1] = 0;
	fread(token_buffer + 1, sizeof(char), token_buffer_count - 1, menu_file);
	fseek(menu_file, menu_file_size, 0);
}

bool string_in_string_case_insensitive(char const* source, char const* find)
{
	ASSERT(source && find);

	char const* source_ = source;
	char const* find_ = find;

	char v3 = *source_;
	if (IN_RANGE_INCLUSIVE(*source_, 'A', 'Z'))
		v3 = *source_ - 'A' + 'a';

	char v6 = *find_;
	if (IN_RANGE_INCLUSIVE(*find_, 'A', 'Z'))
		v6 = *find_ - 'A' + 'a';

	for (char i = v6; v3 == i || !i; i = v6)
	{
		if (!i)
			return true;

		if (!v3)
			return false;

		++source_;
		++find_;

		v3 = *source_;
		if (IN_RANGE_INCLUSIVE(*source_, 'A', 'Z'))
			v3 = *source_ - 'A' + 'a';

		v6 = *find_;
		if (IN_RANGE_INCLUSIVE(*find_, 'A', 'Z'))
			v6 = *find_ - 'A' + 'a';
	}

	return false;
}

void debug_menu_store_number_property(long stack_size, long& stack_index, e_parse_state* parse_stack)
{
	ASSERT(GET_STATE == _parse_state_reading_number);

	POP_STATE;
	ASSERT(GET_STATE == _parse_state_reading_property_found_eqauls);

	POP_STATE;
	ASSERT(GET_STATE == _parse_state_reading_property);

	POP_STATE;
	ASSERT(GET_STATE == _parse_state_reading_open_tag);

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
	g_parser_state.m_xml_attribute = 0;
}

void debug_menu_store_string_property(long stack_size, long& stack_index, e_parse_state* parse_stack)
{
	ASSERT(GET_STATE == _parse_state_reading_string);

	POP_STATE;
	ASSERT(GET_STATE == _parse_state_reading_property_found_eqauls);

	POP_STATE;
	ASSERT(GET_STATE == _parse_state_reading_property);

	POP_STATE;
	ASSERT(GET_STATE == _parse_state_reading_open_tag);

	ASSERT(VALID_INDEX(g_parser_state.m_number_buffer_index, s_parser_state::k_string_length));

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
	g_parser_state.m_xml_attribute = 0;
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
	g_parser_state.m_xml_attribute = 0;
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
	g_parser_state.m_xml_attribute = 0;
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

c_debug_menu* debug_menu_build_menu(e_property_owner property_owner, c_debug_menu* menu)
{
	char const* name = g_parser_state.m_name ? g_parser_state.m_name_buffer : "untitled menu";
	char const* caption = g_parser_state.m_caption ? g_parser_state.m_caption_buffer : "";
	c_debug_menu* child = NULL;

	for (long i = 0; i < s_parser_state::k_string_length; i++)
	{
		if (g_parser_state.m_name_buffer[i] == '\t')
			g_parser_state.m_name_buffer[i] = ',';
	}

	switch (property_owner)
	{
	case _property_owner_menu:
	{
		child = new c_debug_menu_scroll(menu, 26, name);
	}
	break;
	case _property_owner_zone_set_menu:
	{
		child = new c_debug_menu_zone_sets(menu, 26, name);
	}
	break;
	default:
		ASSERT2(unreachable);
		break;
	}

	child->set_caption(caption);
	menu->add_item(new c_debug_menu_item_numbered(menu, name, child));

	return child;
}

void debug_menu_display_error(char const* error_text, bool error)
{
	generate_event(error == false ? _event_level_warning : _event_level_critical, "%s: %s", error == 0 ? "DEBUG_MENU_WARNING" : "DEBUG_MENU_ERROR", error_text);
}

char const* debug_menu_build_recursive(FILE* menu_file, long& c, c_debug_menu* menu, long* line_count, char* error_buffer, long error_buffer_size)
{
	char const* parse_error = NULL;

	long const stack_size = k_parse_state_count;
	long stack_index = 0;
	e_parse_state parse_stack[stack_size]{};

	PUSH_STATE(_parse_state_none);

	g_parser_state.reset();

	ASSERT(menu_file != NULL);
	ASSERT(menu != NULL);
	ASSERT(line_count != NULL);
	ASSERT(error_buffer != NULL);

	while (c && c != NONE && !parse_error)
	{
		long advance_distance = 0;
		long v15 = 0;

		if (GET_STATE == _parse_state_reading_number)
		{
			if (IN_RANGE_INCLUSIVE(c, '0', '9') || c == '.')
			{
				g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index++] = char(c);
				g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index] = 0;

				advance_distance = 1;
				v15 = 1;
			}
			else
			{
				debug_menu_store_number_property(stack_size, stack_index, parse_stack);
				if (c)
					c = fgetc(menu_file);
			}
		}
		else
		{
			if (GET_STATE == _parse_state_reading_escape_sequence)
			{
				POP_STATE;
				PARSER_ASSERT(GET_STATE == _parse_state_reading_string)
				{
					PARSER_ASSERT(VALID_INDEX(g_parser_state.m_string_buffer_index, s_parser_state::k_string_length))
					{
						g_parser_state.m_string_buffer[g_parser_state.m_string_buffer_index++] = char(c);
						g_parser_state.m_string_buffer[g_parser_state.m_string_buffer_index] = 0;

						advance_distance = 1;
						v15 = 1;
					}
				}
			}
			else if (GET_STATE == _parse_state_reading_string && c != '"')
			{
				if (c == '\\')
				{
					PARSER_ASSERT_WITH_MESSAGE(GET_STATE == _parse_state_reading_string, "can not use escape sequences outside of string declaration")
					{
						PUSH_STATE(_parse_state_reading_escape_sequence);
					}
				}
				else
				{
					PARSER_ASSERT(VALID_INDEX(g_parser_state.m_string_buffer_index, s_parser_state::k_string_length))
					{
						g_parser_state.m_string_buffer[g_parser_state.m_string_buffer_index++] = char(c);
						g_parser_state.m_string_buffer[g_parser_state.m_string_buffer_index] = 0;
					}
				}

				advance_distance = 1;
				v15 = 1;
			}
			else
			{
				if (IN_RANGE_INCLUSIVE(c, '0', '9'))
				{
					PARSER_ASSERT_WITH_MESSAGE(GET_STATE == _parse_state_reading_property_found_eqauls, "losse number not assigned to property")
					{
						PUSH_STATE(_parse_state_reading_number);
						g_parser_state.m_number_buffer_index = 0;
						g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index++] = char(c);
						g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index] = 0;

						advance_distance = 1;
						v15 = 1;
					}
				}
				else
				{
#define CHAR_CASE(CHAR) CHAR + 51

					switch (CHAR_CASE(c))
					{
					case 0:
					case CHAR_CASE('\t'):
					case CHAR_CASE(' '):
					{
						advance_distance = 1;
						v15 = 1;
					}
					break;
					case CHAR_CASE('"'):
					{
						long state = GET_STATE;
						if (state == _parse_state_reading_property_found_eqauls)
						{
							g_parser_state.m_string_buffer_index = 0;
							PUSH_STATE(_parse_state_reading_string);
						}
						else
						{
							PARSER_ASSERT_WITH_MESSAGE(state == _parse_state_reading_string, "unexpected symbol \"")
							{
								debug_menu_store_string_property(stack_size, stack_index, parse_stack);
							}
						}

						advance_distance = 1;
						v15 = 1;
					}
					break;
					case CHAR_CASE('-'):
					case CHAR_CASE('.'):
					{
						PARSER_ASSERT_WITH_MESSAGE(GET_STATE == _parse_state_reading_property_found_eqauls, "losse number not assigned to property")
						{
							PUSH_STATE(_parse_state_reading_number);
							g_parser_state.m_number_buffer_index = 0;
							g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index++] = char(c);
							g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index] = 0;
						}

						advance_distance = 1;
						v15 = 1;
					}
					break;
					case CHAR_CASE('/'):
					{
						advance_distance = 1;
						v15 = 1;

						PARSER_ASSERT_WITH_MESSAGE(GET_STATE == _parse_state_reading_tag, "unexpected symbol back slash")
						{
							PUSH_STATE(_parse_state_reading_forward_slash);
						}
					}
					break;
					case CHAR_CASE('<'):
					{
						advance_distance = 1;
						v15 = 1;

						PARSER_ASSERT_WITH_MESSAGE(GET_STATE == _parse_state_none, "unexpected symbol less than")
						{
							PUSH_STATE(_parse_state_reading_tag);
							g_parser_state.reset();
						}
					}
					break;
					case CHAR_CASE('='):
					{
						advance_distance = 1;
						v15 = 1;

						PARSER_ASSERT_WITH_MESSAGE(GET_STATE == _parse_state_reading_property, "= sign expected")
						{
							PARSER_ASSERT(GET_STATE == _parse_state_reading_property)
							{
								PUSH_STATE(_parse_state_reading_property_found_eqauls);
							}
						}
					}
					break;
					case CHAR_CASE('>'):
					{
						advance_distance = 1;
						v15 = 1;

						if (GET_STATE == _parse_state_reading_open_tag)
						{
							POP_STATE;
							PARSER_ASSERT(GET_STATE == _parse_state_reading_tag)
							{
								POP_STATE;
								char build_property_error[1024]{};
								if (g_parser_state.m_property_owner == _property_owner_item)
								{
									PARSER_ASSERT_WITH_MESSAGE(!debug_menu_build_item(menu, build_property_error, sizeof(build_property_error)), build_property_error);
								}
								else
								{
									csstrnzcpy(build_property_error, "", sizeof(build_property_error));
									c_debug_menu* built_menu = debug_menu_build_menu(g_parser_state.m_property_owner, menu);
									PARSER_ASSERT_WITH_MESSAGE(built_menu, build_property_error)
									{
										v15 = 2;
										c = fgetc(menu_file);
										char const* recursive_build_error = debug_menu_build_recursive(menu_file, c, built_menu, line_count, error_buffer, error_buffer_size);
										PARSER_ASSERT_WITH_MESSAGE(!recursive_build_error, recursive_build_error);
									}
								}
							}

							break;
						}

						PARSER_ASSERT_WITH_MESSAGE(GET_STATE == _parse_state_reading_forward_slash, "unexpected symbol greater than")
						{
							POP_STATE;
							PARSER_ASSERT(GET_STATE == _parse_state_reading_tag);
							{
								c = fgetc(menu_file);
								return NULL;
							}
						}
					}
					break;
					case CHAR_CASE('\\'):
					{
						PARSER_ASSERT_WITH_MESSAGE(GET_STATE == _parse_state_reading_string, "can not use escape sequences outside of string declaration");
					}
					break;
					}
#undef CHAR_CASE
				}
			}
		}

		if (!v15)
		{
			long token = 0;
			char token_buffer[1024]{};

			long maximum_token_name_length = 0;
			for (long i = 0; i < NUMBEROF(g_token_names); i++)
			{
				long token_name_length = strlen(g_token_names[i]);
				if (token_name_length > maximum_token_name_length)
					maximum_token_name_length = token_name_length;
			}
			ASSERT(maximum_token_name_length + 1 < NUMBEROF(token_buffer));

			debug_menu_look_ahead_read_token(menu_file, c, token_buffer, maximum_token_name_length + 1);
			for (long i = 0; i < NUMBEROF(g_token_names); i++)
			{
				if (string_in_string_case_insensitive(token_buffer, g_token_names[i]))
				{
					token = i;
					advance_distance = strlen(g_token_names[i]);
					v15 = 1;
					break;
				}
			}

			switch (token)
			{
			case _token_min:
			{
				g_parser_state.m_xml_attribute = 6;

				PARSER_ASSERT(GET_STATE == _parse_state_reading_open_tag)
				{
					PUSH_STATE(_parse_state_reading_property);
				}
			}
			break;
			case _token_max:
			{
				g_parser_state.m_xml_attribute = 7;

				PARSER_ASSERT(GET_STATE == _parse_state_reading_open_tag)
				{
					PUSH_STATE(_parse_state_reading_property);
				}
			}
			break;
			case _token_inc:
			{
				g_parser_state.m_xml_attribute = 8;

				PARSER_ASSERT(GET_STATE == _parse_state_reading_open_tag)
				{
					PUSH_STATE(_parse_state_reading_property);
				}
			}
			break;
			case _token_menu:
			{
				if (GET_STATE != _parse_state_reading_forward_slash)
				{
					g_parser_state.m_property_owner = _property_owner_menu;

					PARSER_ASSERT(GET_STATE == _parse_state_reading_tag)
					{
						PUSH_STATE(_parse_state_reading_open_tag);
					}
				}
			}
			break;
			case _token_zone_set_menu:
			{
				PARSER_ASSERT(GET_STATE != _parse_state_reading_forward_slash)
				{
					g_parser_state.m_property_owner = _property_owner_zone_set_menu;

					PARSER_ASSERT(GET_STATE == _parse_state_reading_tag)
					{
						PUSH_STATE(_parse_state_reading_open_tag);
					}
				}
			}
			break;
			case _token_item:
			{
				g_parser_state.m_property_owner = _property_owner_item;

				PARSER_ASSERT(GET_STATE == _parse_state_reading_tag)
				{
					PUSH_STATE(_parse_state_reading_open_tag);
				}
			}
			break;
			case _token_caption:
			{
				g_parser_state.m_xml_attribute = 3;

				PARSER_ASSERT(GET_STATE == _parse_state_reading_open_tag)
				{
					PUSH_STATE(_parse_state_reading_property);
				}
			}
			break;
			case _token_name:
			{
				g_parser_state.m_xml_attribute = 4;

				PARSER_ASSERT(GET_STATE == _parse_state_reading_open_tag)
				{
					PUSH_STATE(_parse_state_reading_property);
				}
			}
			break;
			case _token_variable:
			{
				g_parser_state.m_xml_attribute = 5;

				PARSER_ASSERT(GET_STATE == _parse_state_reading_open_tag)
				{
					PUSH_STATE(_parse_state_reading_property);
				}
			}
			break;
			case _token_color:
			{
				g_parser_state.m_xml_attribute = 2;

				PARSER_ASSERT(GET_STATE == _parse_state_reading_open_tag)
				{
					PUSH_STATE(_parse_state_reading_property);
				}
			}
			break;
			case _token_type:
			{
				g_parser_state.m_xml_attribute = 1;

				PARSER_ASSERT(GET_STATE == _parse_state_reading_open_tag)
				{
					PUSH_STATE(_parse_state_reading_property);
				}
			}
			break;
			case _token_global:
			case _token_command:
			{
				PARSER_ASSERT_WITH_MESSAGE(GET_STATE == _parse_state_reading_property_found_eqauls, "unexpected token \"global\"")
				{
					g_parser_state.m_item_type = 1;
					g_parser_state.m_item_type_value = (token - _token_global) + 1;

					PARSER_ASSERT(GET_STATE == _parse_state_reading_property_found_eqauls)
					{
						POP_STATE;
						PARSER_ASSERT(GET_STATE == _parse_state_reading_property)
						{
							POP_STATE;
							PARSER_ASSERT(GET_STATE == _parse_state_reading_open_tag)
							{
								break;
							}
						}
					}
				}
			}
			break;
			case _token_crlf:
			case _token_lfcr:
			case _token_carriage_return:
			case _token_line_feed:
				++*line_count;
				break;
			}
		}

		PARSER_ASSERT_WITH_MESSAGE(v15, "unexpected token")
		{
			if (v15 == 1)
			{
				ASSERT(advance_distance != 0);
				do
				{
					c = fgetc(menu_file);
					advance_distance--;
				} while (advance_distance > 0);
			}
			else ASSERT(v15 < 3, unreachable);
		}
	}

	return parse_error ? parse_error : NULL;
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

#undef PARSER_ASSERT_WITH_MESSAGE
#undef PARSER_ASSERT
#undef POP_STATE
#undef PUSH_STATE
#undef GET_STATE

