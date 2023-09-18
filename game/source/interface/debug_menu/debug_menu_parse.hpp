#pragma once

#include "cseries/cseries.hpp"

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

enum e_token
{
	_token_none = 0,
	
	_token_min,
	_token_max,
	_token_inc,

	_token_menu,
	_token_zone_set_menu,

	// ODST
	//_token_create_zone_set_menu,
	//_token_error_geometry_settings_menu,

	// Reach
	//_token_saved_film_marker_goto_menu,
	//_token_scenario_loader,
	//_token_object_dropper,

	_token_item,

	_token_caption,
	_token_name,
	_token_variable,
	_token_color,

	_token_type,
	_token_global,
	_token_command,

	// CR LF
	_token_crlf,

	// LF CR
	_token_lfcr,

	// CR
	_token_carriage_return,

	// LF
	_token_line_feed,

	k_token_count
};

enum e_property
{
	_property_none = 0,
	_property_type,
	_property_color,
	_property_caption,
	_property_name,
	_property_variable,
	_property_min,
	_property_max,
	_property_inc,

	k_property_count
};

enum e_property_owner
{
	_property_owner_none = 0,
	_property_owner_item,
	_property_owner_menu,
	_property_owner_zone_set_menu,

	// ODST
	//_property_owner_create_zone_set_menu,
	//_property_owner_error_geometry_settings_menu,

	// Reach
	//_property_owner_saved_film_marker_goto_menu,
	//_property_owner_scenario_loader,
	//_property_owner_object_dropper,

	k_property_owner_count
};

enum e_item_type
{
	_item_type_none = 0,
	_item_type_global,
	_item_type_command,

	k_item_type_count
};

struct s_parser_state
{
	void reset();

	enum
	{
		// halo3 08172.07.03.08.2240.delta
		//k_string_length = 128,

		// reach 11883.10.10.25.1227.dlc_1_ship
		k_string_length = 2048
	};

	c_enum<e_property, short, _property_none, k_property_count> m_property;
	c_enum<e_property_owner, long, _property_owner_none, k_property_owner_count> m_property_owner;

	char m_string_buffer[k_string_length];
	short m_string_buffer_index;

	char m_number_buffer[k_string_length];
	short m_number_buffer_index;

	bool m_name;
	char m_name_buffer[k_string_length];

	bool m_color;
	char m_color_buffer[k_string_length];

	bool m_variable;
	char m_variable_buffer[k_string_length];

	bool m_caption;
	char m_caption_buffer[k_string_length];

	bool m_min;
	real m_min_value;

	bool m_max;
	real m_max_value;

	bool m_inc;
	real m_inc_value;

	bool m_item;
	c_enum<e_item_type, long, _item_type_none, k_item_type_count> m_item_type;
};
//static_assert(sizeof(s_parser_state) == 0x330);
static_assert(sizeof(s_parser_state) == 0x3030);

extern char const* const g_token_names[k_token_count];

extern s_parser_state g_parser_state;

class c_debug_menu;
extern void debug_menu_parse(c_debug_menu* root_menu, char const* file_name);

