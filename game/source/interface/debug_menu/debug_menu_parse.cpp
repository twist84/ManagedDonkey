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

#include <climits>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define PARSER_ASSERT(STATEMENT) PARSER_ASSERT_WITH_MESSAGE(STATEMENT, #STATEMENT)
#define PARSER_ASSERT_WITH_MESSAGE(STATEMENT, MESSAGE) \
if (!(STATEMENT)) \
{ \
	csnzprintf(error_buffer, error_buffer_length, "ln %d: %s", *line_count, MESSAGE); \
	parse_error = error_buffer; \
	debug_menu_display_error(error_buffer, true); \
	continue; \
} else 

#define CHAR_CASE(CHAR) CHAR + 51

#define TOKEN_CASE_PROPERTY(PROPERTY) \
case _token_##PROPERTY: \
{ \
	g_parser_state.m_current_property_type = _property_##PROPERTY; \
	PARSER_ASSERT(*parse_stack.get_top() == _parse_state_reading_open_tag) \
	{ \
		parse_stack.push_back(_parse_state_reading_property); \
	} \
} \
break
#define TOKEN_CASE_PROPERTY_OWNER(PROPERTY_OWNER, CHECK_FORWORD_SLASH) \
case _token_##PROPERTY_OWNER: \
{ \
	if (CHECK_FORWORD_SLASH && *parse_stack.get_top() == _parse_state_reading_close_tag) \
		break; \
	g_parser_state.m_current_property_owner = _property_owner_##PROPERTY_OWNER; \
	PARSER_ASSERT(*parse_stack.get_top() == _parse_state_reading_tag) \
	{ \
		parse_stack.push_back(_parse_state_reading_open_tag); \
	} \
} \
break
#define TOKEN_CASE_TYPE(TYPE) \
case _token_##TYPE: \
{ \
	PARSER_ASSERT_WITH_MESSAGE(*parse_stack.get_top() == _parse_state_reading_property_found_eqauls, "unexpected token \"global\"") \
	{ \
		g_parser_state.m_has_item_type = true; \
		g_parser_state.m_item_type = e_item_types((token - (_token_type + 1)) + 1); \
		PARSER_ASSERT(*parse_stack.get_top() == _parse_state_reading_property_found_eqauls) \
		{ \
			parse_stack.pop(); \
			PARSER_ASSERT(*parse_stack.get_top() == _parse_state_reading_property) \
			{ \
				parse_stack.pop(); \
				PARSER_ASSERT(*parse_stack.get_top() == _parse_state_reading_open_tag) \
				{ \
					break; \
				} \
			} \
		} \
	} \
} \
break
#define TOKEN_CASE_NEW_LINE(TOKEN) case _token_##TOKEN: ++*line_count; break

s_parser_state g_parser_state = {};

void s_parser_state::reset()
{
	m_has_name = 0;
	m_has_color = 0;
	m_has_variable = 0;
	m_has_caption = 0;
	m_has_min = 0;
	m_has_max = 0;
	m_has_inc = 0;
	m_item_type = _item_type_none;
	m_current_property_type = _property_none;
	m_current_property_owner = _property_owner_none;
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
	switch (g_parser_state.m_current_property_type.get())
	{
	case _property_min:
		g_parser_state.m_has_min = true;
		g_parser_state.m_min = real(atof(g_parser_state.m_number_buffer));
		break;
	case _property_max:
		g_parser_state.m_has_max = true;
		g_parser_state.m_max = real(atof(g_parser_state.m_number_buffer));
		break;
	case _property_inc:
		g_parser_state.m_has_inc = true;
		g_parser_state.m_inc = real(atof(g_parser_state.m_number_buffer));
		break;
	}
	g_parser_state.m_current_property_type = _property_none;
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
	switch (g_parser_state.m_current_property_type.get())
	{
	case _property_color:
		g_parser_state.m_has_color = true;
		csstrnzcpy(g_parser_state.m_color, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
		break;
	case _property_caption:
		g_parser_state.m_has_caption = true;
		csstrnzcpy(g_parser_state.m_caption, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
		break;
	case _property_name:
		g_parser_state.m_has_name = true;
		csstrnzcpy(g_parser_state.m_name, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
		break;
	case _property_variable:
		g_parser_state.m_has_variable = true;
		csstrnzcpy(g_parser_state.m_variable, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
		break;
	}
	g_parser_state.m_current_property_type = _property_none;
}

char const* debug_menu_build_item_hs_variable_global(c_debug_menu* menu, char* error_buffer, long error_buffer_size)
{
	if (!g_parser_state.m_has_variable)
	{
		csnzprintf(error_buffer, error_buffer_size, "global variable debug menu items must define a variable");
		return error_buffer;
	}

	char const* name = g_parser_state.m_has_name ? g_parser_state.m_name : g_parser_state.m_variable;

	long console_global_index = NONE;
	for (long i = 0; i < k_console_global_count; i++)
	{
		if (!csstricmp(g_parser_state.m_variable, k_console_globals[i].name) && k_console_globals[i].pointer)
		{
			console_global_index = i;
			break;
		}
	}

	c_debug_menu_item* item = NULL;
	if (console_global_index != NONE && k_console_globals[console_global_index].pointer)
	{
		e_hs_type type = k_console_globals[console_global_index].type;
		switch (type)
		{
		case _hs_type_boolean:
		{
			item = new c_debug_menu_item_type_bool(menu, name, false, g_parser_state.m_variable);
		}
		break;
		case _hs_type_real:
		{
			real inc_value = g_parser_state.m_has_inc ? g_parser_state.m_inc : 0.1f;
			real max_value = g_parser_state.m_has_max ? g_parser_state.m_max : k_real_max;
			real min_value = g_parser_state.m_has_min ? g_parser_state.m_min : k_real_min;

			item = new c_debug_menu_item_type_real(menu, name, NULL, g_parser_state.m_variable, min_value, max_value, inc_value);
		}
		break;
		case _hs_type_short_integer:
		{
			short inc_value = short(g_parser_state.m_has_inc ? g_parser_state.m_inc : 1);
			short max_value = short(g_parser_state.m_has_max ? g_parser_state.m_max : SHRT_MAX - 1);
			short min_value = short(g_parser_state.m_has_min ? g_parser_state.m_min : ~(SHRT_MAX - 1));

			item = new c_debug_menu_item_type_short(menu, name, NULL, g_parser_state.m_variable, min_value, max_value, inc_value);
		}
		break;
		case _hs_type_long_integer:
		{
			long inc_value = long(g_parser_state.m_has_inc ? g_parser_state.m_inc : 1);
			long max_value = long(g_parser_state.m_has_max ? g_parser_state.m_max : LONG_MAX - 1);
			long min_value = long(g_parser_state.m_has_min ? g_parser_state.m_min : ~(LONG_MAX - 1));

			item = new c_debug_menu_item_type_long(menu, name, NULL, g_parser_state.m_variable, min_value, max_value, inc_value);
		}
		break;
		}
	}

	if (!item)
	{
		char undefined_name[1024]{};
		csnzprintf(undefined_name, sizeof(undefined_name), "UNDEFINED: %s", g_parser_state.m_variable);
		item = new c_debug_menu_item(menu, undefined_name, NULL, false);
	}
	ASSERT(item != NULL);

	menu->add_item(item);

	return NULL;
}

char const* debug_menu_build_item_command(c_debug_menu* menu, char* error_buffer, long error_buffer_size)
{
	if (!g_parser_state.m_has_variable)
	{
		csnzprintf(error_buffer, error_buffer_size, "command menu items must define a variable");
		return error_buffer;
	}

	char const* name = g_parser_state.m_has_name ? g_parser_state.m_name : g_parser_state.m_variable;
	char const* command = g_parser_state.m_variable;
	menu->add_item(new c_debug_menu_item_hs_command(menu, name, command));

	return NULL;
}

char const* debug_menu_build_item(c_debug_menu* menu, char* error_buffer, long error_buffer_size)
{
	if (!g_parser_state.m_has_item_type)
	{
		csnzprintf(error_buffer, error_buffer_size, "menu items must supply a type");
		return error_buffer;
	}

	switch (g_parser_state.m_item_type.get())
	{
	case _item_type_hs_variable_global:
		return debug_menu_build_item_hs_variable_global(menu, error_buffer, error_buffer_size);
	case _item_type_command:
		return debug_menu_build_item_command(menu, error_buffer, error_buffer_size);
	default:
		ASSERT2("unreachable");
		break;
	}

	return NULL;
}

c_debug_menu* debug_menu_build_menu(e_property_owners property_owner, c_debug_menu* menu)
{
	char const* name = g_parser_state.m_has_name ? g_parser_state.m_name : "untitled menu";
	char const* caption = g_parser_state.m_has_caption ? g_parser_state.m_caption : "";
	c_debug_menu* child = NULL;

	for (long i = 0; i < s_parser_state::k_string_length; i++)
	{
		if (g_parser_state.m_name[i] == '\t')
			g_parser_state.m_name[i] = ',';
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
		ASSERT2("unreachable");
		break;
	}

	child->set_caption(caption);
	menu->add_item(new c_debug_menu_item_numbered(menu, name, child));

	return child;
}

void debug_menu_display_error(char const* error_text, bool error)
{
	GENERATE_EVENT(error == false ? _event_warning : _event_critical, "%s: %s", error == 0 ? "DEBUG_MENU_WARNING" : "DEBUG_MENU_ERROR", error_text);
}

char const* debug_menu_build_recursive(FILE* menu_file, long& file_char, c_debug_menu* menu, long* line_count, char* error_buffer, long error_buffer_length)
{
	char const* parse_error = NULL;

	parse_stack_t parse_stack{};
	parse_stack.push_back(_parse_state_none);

	g_parser_state.reset();

	ASSERT(menu_file != NULL);
	ASSERT(menu != NULL);
	ASSERT(line_count != NULL);
	ASSERT(error_buffer != NULL);

	while (file_char && file_char != NONE && !parse_error)
	{
		long advance_distance = 0;
		e_advance_type advance_process_type = _advance_type_process_token;

		if (*parse_stack.get_top() == _parse_state_reading_number)
		{
			if (IN_RANGE_INCLUSIVE(file_char, '0', '9') || file_char == '.')
			{
				g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index++] = char(file_char);
				g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index] = 0;

				advance_distance = 1;
				advance_process_type = _advance_type_process_distance;
			}
			else
			{
				debug_menu_store_number_property(&parse_stack);
				if (file_char)
					file_char = fgetc(menu_file);
			}
		}
		else
		{
			if (*parse_stack.get_top() == _parse_state_reading_escape_character)
			{
				parse_stack.pop();
				PARSER_ASSERT(*parse_stack.get_top() == _parse_state_reading_string)
				{
					PARSER_ASSERT(VALID_INDEX(g_parser_state.m_string_buffer_index, s_parser_state::k_string_length))
					{
						g_parser_state.m_string_buffer[g_parser_state.m_string_buffer_index++] = char(file_char);
						g_parser_state.m_string_buffer[g_parser_state.m_string_buffer_index] = 0;

						advance_distance = 1;
						advance_process_type = _advance_type_process_distance;
					}
				}
			}
			else if (*parse_stack.get_top() == _parse_state_reading_string && file_char != '"')
			{
				if (file_char == '\\')
				{
					PARSER_ASSERT_WITH_MESSAGE(*parse_stack.get_top() == _parse_state_reading_string, "can not use escape sequences outside of string declaration")
					{
						parse_stack.push_back(_parse_state_reading_escape_character);
					}
				}
				else
				{
					PARSER_ASSERT(VALID_INDEX(g_parser_state.m_string_buffer_index, s_parser_state::k_string_length))
					{
						g_parser_state.m_string_buffer[g_parser_state.m_string_buffer_index++] = char(file_char);
						g_parser_state.m_string_buffer[g_parser_state.m_string_buffer_index] = 0;
					}
				}

				advance_distance = 1;
				advance_process_type = _advance_type_process_distance;
			}
			else
			{
				if (IN_RANGE_INCLUSIVE(file_char, '0', '9'))
				{
					PARSER_ASSERT_WITH_MESSAGE(*parse_stack.get_top() == _parse_state_reading_property_found_eqauls, "losse number not assigned to property")
					{
						parse_stack.push_back(_parse_state_reading_number);
						g_parser_state.m_number_buffer_index = 0;
						g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index++] = char(file_char);
						g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index] = 0;

						advance_distance = 1;
						advance_process_type = _advance_type_process_distance;
					}
				}
				else
				{
					switch (CHAR_CASE(file_char))
					{
					case 0:
					case CHAR_CASE('\t'):
					case CHAR_CASE(' '):
					{
						advance_distance = 1;
						advance_process_type = _advance_type_process_distance;
					}
					break;
					case CHAR_CASE('"'):
					{
						long state = *parse_stack.get_top();
						if (state == _parse_state_reading_property_found_eqauls)
						{
							g_parser_state.m_string_buffer_index = 0;
							parse_stack.push_back(_parse_state_reading_string);
						}
						else
						{
							PARSER_ASSERT_WITH_MESSAGE(state == _parse_state_reading_string, "unexpected symbol \"")
							{
								debug_menu_store_string_property(&parse_stack);
							}
						}

						advance_distance = 1;
						advance_process_type = _advance_type_process_distance;
					}
					break;
					case CHAR_CASE('-'):
					case CHAR_CASE('.'):
					{
						PARSER_ASSERT_WITH_MESSAGE(*parse_stack.get_top() == _parse_state_reading_property_found_eqauls, "losse number not assigned to property")
						{
							parse_stack.push_back(_parse_state_reading_number);
							g_parser_state.m_number_buffer_index = 0;
							g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index++] = char(file_char);
							g_parser_state.m_number_buffer[g_parser_state.m_number_buffer_index] = 0;
						}

						advance_distance = 1;
						advance_process_type = _advance_type_process_distance;
					}
					break;
					case CHAR_CASE('/'):
					{
						advance_distance = 1;
						advance_process_type = _advance_type_process_distance;

						PARSER_ASSERT_WITH_MESSAGE(*parse_stack.get_top() == _parse_state_reading_tag, "unexpected symbol back slash")
						{
							parse_stack.push_back(_parse_state_reading_close_tag);
						}
					}
					break;
					case CHAR_CASE('<'):
					{
						advance_distance = 1;
						advance_process_type = _advance_type_process_distance;

						PARSER_ASSERT_WITH_MESSAGE(*parse_stack.get_top() == _parse_state_none, "unexpected symbol less than")
						{
							parse_stack.push_back(_parse_state_reading_tag);
							g_parser_state.reset();
						}
					}
					break;
					case CHAR_CASE('='):
					{
						advance_distance = 1;
						advance_process_type = _advance_type_process_distance;

						PARSER_ASSERT_WITH_MESSAGE(*parse_stack.get_top() == _parse_state_reading_property, "= sign expected")
						{
							PARSER_ASSERT(*parse_stack.get_top() == _parse_state_reading_property)
							{
								parse_stack.push_back(_parse_state_reading_property_found_eqauls);
							}
						}
					}
					break;
					case CHAR_CASE('>'):
					{
						advance_distance = 1;
						advance_process_type = _advance_type_process_distance;

						if (*parse_stack.get_top() == _parse_state_reading_open_tag)
						{
							parse_stack.pop();
							PARSER_ASSERT(*parse_stack.get_top() == _parse_state_reading_tag)
							{
								parse_stack.pop();
								char build_property_error[1024]{};
								if (g_parser_state.m_current_property_owner == _property_owner_item)
								{
									PARSER_ASSERT_WITH_MESSAGE(!debug_menu_build_item(menu, build_property_error, sizeof(build_property_error)), build_property_error);
								}
								else
								{
									csstrnzcpy(build_property_error, "", sizeof(build_property_error));
									c_debug_menu* built_menu = debug_menu_build_menu(g_parser_state.m_current_property_owner, menu);
									PARSER_ASSERT_WITH_MESSAGE(built_menu, build_property_error)
									{
										advance_process_type = _advance_type_process_nothing;

										file_char = fgetc(menu_file);
										char const* recursive_build_error = debug_menu_build_recursive(menu_file, file_char, built_menu, line_count, error_buffer, error_buffer_length);
										PARSER_ASSERT_WITH_MESSAGE(!recursive_build_error, recursive_build_error);
									}
								}
							}

							break;
						}

						PARSER_ASSERT_WITH_MESSAGE(*parse_stack.get_top() == _parse_state_reading_close_tag, "unexpected symbol greater than")
						{
							parse_stack.pop();
							PARSER_ASSERT(*parse_stack.get_top() == _parse_state_reading_tag);
							{
								file_char = fgetc(menu_file);
								return NULL;
							}
						}
					}
					break;
					case CHAR_CASE('\\'):
					{
						PARSER_ASSERT_WITH_MESSAGE(*parse_stack.get_top() == _parse_state_reading_string, "can not use escape sequences outside of string declaration");
					}
					break;
					}
				}
			}
		}

		if (advance_process_type == _advance_type_process_token)
		{
			e_tokens token = _token_none;
			char token_buffer[1024]{};

			long maximum_token_name_length = 0;
			for (long i = 0; i < NUMBEROF(g_token_names); i++)
			{
				long token_name_length = strlen(g_token_names[i]);
				if (token_name_length > maximum_token_name_length)
					maximum_token_name_length = token_name_length;
			}
			ASSERT(maximum_token_name_length + 1 < NUMBEROF(token_buffer));

			debug_menu_look_ahead_read_token(menu_file, file_char, token_buffer, maximum_token_name_length + 1);
			for (long i = 0; i < NUMBEROF(g_token_names); i++)
			{
				if (string_in_string_case_insensitive(token_buffer, g_token_names[i]))
				{
					token = e_tokens(i);

					advance_distance = strlen(g_token_names[i]);
					advance_process_type = _advance_type_process_distance;
					break;
				}
			}

			switch (token)
			{
			TOKEN_CASE_PROPERTY(min);
			TOKEN_CASE_PROPERTY(max);
			TOKEN_CASE_PROPERTY(inc);
			TOKEN_CASE_PROPERTY_OWNER(menu, true);
			TOKEN_CASE_PROPERTY_OWNER(zone_set_menu, true);
			//TOKEN_CASE_PROPERTY_OWNER(create_zone_set_menu, true);
			//TOKEN_CASE_PROPERTY_OWNER(error_geometry_settings_menu, true);
			//TOKEN_CASE_PROPERTY_OWNER(saved_film_marker_goto_menu, true);
			//TOKEN_CASE_PROPERTY_OWNER(scenario_loader, true);
			//TOKEN_CASE_PROPERTY_OWNER(object_dropper, true);
			TOKEN_CASE_PROPERTY_OWNER(item, false);
			TOKEN_CASE_PROPERTY(caption);
			TOKEN_CASE_PROPERTY(name);
			TOKEN_CASE_PROPERTY(variable);
			TOKEN_CASE_PROPERTY(color);
			TOKEN_CASE_PROPERTY(type);
			TOKEN_CASE_TYPE(global);
			TOKEN_CASE_TYPE(command);
			TOKEN_CASE_NEW_LINE(eol_0);
			TOKEN_CASE_NEW_LINE(eol_1);
			TOKEN_CASE_NEW_LINE(eol_2);
			TOKEN_CASE_NEW_LINE(eol_3);
			}
		}

		PARSER_ASSERT_WITH_MESSAGE(advance_process_type, "unexpected token")
		{
			if (advance_process_type == _advance_type_process_distance)
			{
				ASSERT(advance_distance != 0);
				while (advance_distance-- > 0)
					file_char = fgetc(menu_file);
			}
			else ASSERT(advance_process_type < k_advance_type_count, unreachable);
		}
	}

	return parse_error ? parse_error : NULL;
}

void debug_menu_parse(c_debug_menu* root_menu, char const* file_name)
{
	ASSERT(file_name != NULL);
	ASSERT(root_menu != NULL);

	FILE* file;
	if (fopen_s(&file, file_name, "rt") == 0 && file)
	{
		char error_buffer[1024]{};

		long line_count = 1;
		long file_char = (long)fgetc(file);
		debug_menu_build_recursive(file, file_char, root_menu, &line_count, error_buffer, sizeof(error_buffer));
		fclose(file);
	}
}

#undef TOKEN_CASE_NEW_LINE
#undef TOKEN_CASE_TYPE
#undef TOKEN_CASE_PROPERTY_OWNER
#undef TOKEN_CASE_PROPERTY

#undef CHAR_CASE

#undef PARSER_ASSERT_WITH_MESSAGE
#undef PARSER_ASSERT

