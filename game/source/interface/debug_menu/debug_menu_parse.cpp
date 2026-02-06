#include "interface/debug_menu/debug_menu_parse.hpp"

#include "cseries/cseries_events.hpp"
#include "interface/debug_menu/debug_menu.hpp"
#include "interface/debug_menu/debug_menu_item.hpp"
#include "interface/debug_menu/debug_menu_item_hs_command.hpp"
#include "interface/debug_menu/debug_menu_item_numbered.hpp"
#include "interface/debug_menu/debug_menu_item_type.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/debug_menu/debug_menu_scroll.hpp"
#include "interface/debug_menu/debug_menu_zone_sets.hpp"

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

const char* const g_token_names[k_token_count]
{
	"none",
	"min",
	"max",
	"inc",
	"menu",
	"zone_set_menu",

	// Halo 3: ODST
	//"create_zone_set_menu",
	//"error_geometry_settings_menu",

	// Halo: Reach
	//"saved_film_marker_goto_menu",
	//"scenario_loader",
	//"object_dropper",

	// Halo 4
	//"effects_browser",
	//"cinematics_menu",

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

int32 debug_menu_memory_available()
{
	return 4 * (k_debug_menu_stack_size - g_debug_menu_stack.count());
}

void debug_menu_look_ahead_read_token(FILE* file, int32 file_char, char* token_buffer, int32 token_buffer_count)
{
	int32 file_size = ftell(file);
	*token_buffer = char(file_char);
	int32 characters_read = fread(token_buffer + 1, sizeof(char), token_buffer_count - 1, file);
	fseek(file, file_size, 0);

	ASSERT(IN_RANGE_INCLUSIVE(characters_read, 0, token_buffer_count - 1));

	int32 token_buffer_end = characters_read + 1;
	if (characters_read + 1 > token_buffer_count - 1)
	{
		token_buffer_end = token_buffer_count - 1;
	}
	token_buffer[token_buffer_end] = 0;
}

bool string_in_string_case_insensitive(const char* source, const char* find)
{
	ASSERT(source && find);

	bool result = false;

	int32 source_length = 0;
	while (source[source_length])
	{
		source_length += 1;
	}

	int32 find_length = 0;
	while (find[find_length])
	{
		find_length += 1;
	}

	if (find_length == 0)
	{
		result = true;
	}
	else
	{
		bool match = true;
		int32 i = 0;
		while (i < source_length && i < find_length)
		{
			char source_char = tolower(source[i]);
			char find_char = tolower(find[i]);
			if (source_char != find_char)
			{
				match = false;
				break;
			}
			i += 1;
		}
		if (match && i == find_length)
		{
			result = true;
		}
	}

	return result;
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
	{
		g_parser_state.m_has_min = true;
		g_parser_state.m_min = (real32)atof(g_parser_state.m_number_buffer);
	}
	break;
	case _property_max:
	{
		g_parser_state.m_has_max = true;
		g_parser_state.m_max = (real32)atof(g_parser_state.m_number_buffer);
	}
	break;
	case _property_inc:
	{
		g_parser_state.m_has_inc = true;
		g_parser_state.m_inc = (real32)atof(g_parser_state.m_number_buffer);
	}
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

	g_parser_state.m_string_buffer[g_parser_state.m_string_buffer_index] = 0;
	switch (g_parser_state.m_current_property_type.get())
	{
	case _property_color:
	{
		g_parser_state.m_has_color = true;
		csstrnzcpy(g_parser_state.m_color, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
	}
	break;
	case _property_caption:
	{
		g_parser_state.m_has_caption = true;
		csstrnzcpy(g_parser_state.m_caption, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
	}
	break;
	case _property_name:
	{
		g_parser_state.m_has_name = true;
		csstrnzcpy(g_parser_state.m_name, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
	}
	break;
	case _property_variable:
	{
		g_parser_state.m_has_variable = true;
		csstrnzcpy(g_parser_state.m_variable, g_parser_state.m_string_buffer, s_parser_state::k_string_length);
	}
	break;
	}
	g_parser_state.m_current_property_type = _property_none;
}

const char* debug_menu_build_item_hs_variable_global(c_debug_menu* menu, char* error_buffer, int32 error_buffer_size)
{
	if (!g_parser_state.m_has_variable)
	{
		csnzprintf(error_buffer, error_buffer_size, "global variable debug menu items must define a variable");
		return error_buffer;
	}

	const char* name = g_parser_state.m_has_name ? g_parser_state.m_name : g_parser_state.m_variable;

	e_hs_type type = _hs_unparsed;
	for (int16 global_index = 0; global_index < k_hs_external_global_count; global_index++)
	{
		if (csstricmp(g_parser_state.m_variable, hs_external_globals[global_index]->name) == 0 && hs_external_globals[global_index]->pointer)
		{
			type = (e_hs_type)hs_external_globals[global_index]->type;
			break;
		}
	}

	c_debug_menu_item* item = nullptr;

	switch (type)
	{
	case _hs_type_boolean:
	{
		item = DEBUG_MENU_MALLOC(c_debug_menu_item_type_bool, menu, name, false, g_parser_state.m_variable);
	}
	break;
	case _hs_type_real:
	{
		real32 inc_value = g_parser_state.m_has_inc ? g_parser_state.m_inc : 0.1f;
		real32 max_value = g_parser_state.m_has_max ? g_parser_state.m_max : k_real_max;
		real32 min_value = g_parser_state.m_has_min ? g_parser_state.m_min : k_real_min;

		item = DEBUG_MENU_MALLOC(c_debug_menu_item_type_real, menu, name, nullptr, g_parser_state.m_variable, min_value, max_value, inc_value);
	}
	break;
	case _hs_type_short_integer:
	{
		int16 inc_value = g_parser_state.m_has_inc ? (int16)g_parser_state.m_inc : 1;
		int16 max_value = g_parser_state.m_has_max ? (int16)g_parser_state.m_max : SHRT_MAX - 1;
		int16 min_value = g_parser_state.m_has_min ? (int16)g_parser_state.m_min : SHRT_MIN + 1;

		item = DEBUG_MENU_MALLOC(c_debug_menu_item_type_short, menu, name, nullptr, g_parser_state.m_variable, min_value, max_value, inc_value);
	}
	break;
	case _hs_type_long_integer:
	{
		int32 inc_value = g_parser_state.m_has_inc ? (int32)g_parser_state.m_inc : 1;
		int32 max_value = g_parser_state.m_has_max ? (int32)g_parser_state.m_max : LONG_MAX - 1;
		int32 min_value = g_parser_state.m_has_min ? (int32)g_parser_state.m_min : LONG_MIN + 1;

		item = DEBUG_MENU_MALLOC(c_debug_menu_item_type_long, menu, name, nullptr, g_parser_state.m_variable, min_value, max_value, inc_value);
	}
	break;
	}

	if (!item)
	{
		char undefined_name[1024]{};
		csnzprintf(undefined_name, sizeof(undefined_name), "UNDEFINED: %s", g_parser_state.m_variable);
		item = DEBUG_MENU_MALLOC(c_debug_menu_item, menu, undefined_name, nullptr, false);
	}
	ASSERT(item != nullptr);

	menu->add_item(item);

	return nullptr;
}

const char* debug_menu_build_item_command(c_debug_menu* menu, char* error_buffer, int32 error_buffer_size)
{
	if (!g_parser_state.m_has_variable)
	{
		csnzprintf(error_buffer, error_buffer_size, "command menu items must define a variable");
		return error_buffer;
	}

	const char* name = g_parser_state.m_has_name ? g_parser_state.m_name : g_parser_state.m_variable;
	const char* command = g_parser_state.m_variable;
	menu->add_item(DEBUG_MENU_MALLOC(c_debug_menu_item_hs_command, menu, name, command));

	return nullptr;
}

const char* debug_menu_build_item(c_debug_menu* menu, char* error_buffer, int32 error_buffer_size)
{
	if (!g_parser_state.m_has_item_type)
	{
		csnzprintf(error_buffer, error_buffer_size, "menu items must supply a type");
		return error_buffer;
	}

	const char* result = nullptr;
	switch (g_parser_state.m_item_type.get())
	{
	case _item_type_hs_variable_global:
	{
		result = debug_menu_build_item_hs_variable_global(menu, error_buffer, error_buffer_size);
	}
	break;
	case _item_type_command:
	{
		result = debug_menu_build_item_command(menu, error_buffer, error_buffer_size);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	return result;
}

c_debug_menu* debug_menu_build_menu(e_property_owners property_owner, c_debug_menu* menu)
{
	const char* name = g_parser_state.m_has_name ? g_parser_state.m_name : "untitled menu";
	const char* caption = g_parser_state.m_has_caption ? g_parser_state.m_caption : "";
	c_debug_menu* child = nullptr;

	for (int32 i = 0; i < s_parser_state::k_string_length; i++)
	{
		if (g_parser_state.m_name[i] == '\t')
		{
			g_parser_state.m_name[i] = ',';
		}
	}

	switch (property_owner)
	{
	case _property_owner_menu:
	{
		child = DEBUG_MENU_MALLOC(c_debug_menu_scroll, menu, 26, name);
	}
	break;
	case _property_owner_zone_set_menu:
	{
		child = DEBUG_MENU_MALLOC(c_debug_menu_zone_sets, menu, 26, name);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	child->set_caption(caption);
	menu->add_item(DEBUG_MENU_MALLOC(c_debug_menu_item_numbered, menu, name, child));

	return child;
}

void debug_menu_display_error(const char* error_text, bool fatal)
{
	event(fatal ? _event_critical : _event_warning, "%s: %s", fatal ? "DEBUG_MENU_ERROR" : "DEBUG_MENU_WARNING", error_text);
}

const char* debug_menu_build_recursive(FILE* menu_file, int32& file_char, c_debug_menu* menu, int32* line_count, char* error_buffer, int32 error_buffer_length)
{
	const char* parse_error = nullptr;

	parse_stack_t parse_stack{};
	parse_stack.push_back(_parse_state_none);

	g_parser_state.reset();

	ASSERT(menu_file != nullptr);
	ASSERT(menu != nullptr);
	ASSERT(line_count != nullptr);
	ASSERT(error_buffer != nullptr);

	while (file_char && file_char != NONE && !parse_error)
	{
		int32 advance_distance = 0;
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
				{
					file_char = fgetc(menu_file);
				}
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
					switch (file_char)
					{
					case _symbol_random_whitespace:
					case _symbol_tab:
					case _symbol_white_space:
					{
						advance_distance = 1;
						advance_process_type = _advance_type_process_distance;
					}
					break;
					case _symbol_quote:
					{
						int32 state = *parse_stack.get_top();
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
					case _symbol_minus:
					case _symbol_period:
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
					case _symbol_back_slash:
					{
						advance_distance = 1;
						advance_process_type = _advance_type_process_distance;

						PARSER_ASSERT_WITH_MESSAGE(*parse_stack.get_top() == _parse_state_reading_tag, "unexpected symbol back slash")
						{
							parse_stack.push_back(_parse_state_reading_close_tag);
						}
					}
					break;
					case _symbol_less_than:
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
					case _symbol_equals:
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
					case _symbol_greater_than:
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
										const char* recursive_build_error = debug_menu_build_recursive(menu_file, file_char, built_menu, line_count, error_buffer, error_buffer_length);
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
								return nullptr;
							}
						}
					}
					break;
					case _symbol_forward_slash:
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

			int32 maximum_token_name_length = 0;
			for (int32 i = 0; i < NUMBEROF(g_token_names); i++)
			{
				int32 token_name_length = strlen_debug(g_token_names[i]);
				if (token_name_length > maximum_token_name_length)
				{
					maximum_token_name_length = token_name_length;
				}
			}
			ASSERT(maximum_token_name_length + 1 < NUMBEROF(token_buffer));

			debug_menu_look_ahead_read_token(menu_file, file_char, token_buffer, maximum_token_name_length + 1);
			for (int32 i = 0; i < NUMBEROF(g_token_names); i++)
			{
				if (string_in_string_case_insensitive(token_buffer, g_token_names[i]))
				{
					token = e_tokens(i);

					advance_distance = strlen_debug(g_token_names[i]);
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
				{
					file_char = fgetc(menu_file);
				}
			}
			else ASSERT(advance_process_type < k_advance_type_count, unreachable);
		}
	}

	return parse_error ? parse_error : nullptr;
}

void debug_menu_parse(c_debug_menu* root_menu, const char* file_name)
{
	ASSERT(file_name != nullptr);
	ASSERT(root_menu != nullptr);

	FILE* file = nullptr;
	if (fopen_s(&file, file_name, "rb") == 0 && file)
	{
		char error_buffer[1024]{};

		int32 line_count = 1;
		int32 file_char = (int32)fgetc(file);
		debug_menu_build_recursive(file, file_char, root_menu, &line_count, error_buffer, sizeof(error_buffer));
		fclose(file);
	}
}

#undef TOKEN_CASE_NEW_LINE
#undef TOKEN_CASE_TYPE
#undef TOKEN_CASE_PROPERTY_OWNER
#undef TOKEN_CASE_PROPERTY

#undef PARSER_ASSERT_WITH_MESSAGE
#undef PARSER_ASSERT

