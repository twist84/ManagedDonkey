#pragma once

#include "cseries/cseries.hpp"

enum e_token
{
	_token_none = 0,
	
	_token_min,
	_token_max,
	_token_inc,

	_token_menu,
	_token_zone_set_menu,
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

extern char const* const g_token_names[k_token_count];

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

	// 1: type
	// 
	// string property
	// 2: color,    m_color = true
	// 3: caption,  m_caption = true
	// 4: name,     m_name = true
	// 5: variable, m_variable = true
	// 
	// // number property
	// 6: min,      m_min = true
	// 7: max,      m_max = true
	// 8: inc,      m_inc = true
	short m_xml_attribute;

	// 1: item
	// 2: menu
	// 3: zone_set_menu
	long m_xml_element;

	char m_string_buffer[2048];
	short m_string_buffer_index;

	char m_number_buffer[2048];
	short m_number_buffer_index;

	bool m_name;
	char m_name_buffer[2048];

	bool m_color;
	char m_color_buffer[2048];

	bool m_variable;
	char m_variable_buffer[2048];

	bool m_caption;
	char m_caption_buffer[2048];

	bool m_min;
	real m_min_value;

	bool m_max;
	real m_max_value;

	bool m_inc;
	real m_inc_value;

	bool m_item_type;
	long m_item_type_value; // 1: global, 2: command
};
//static_assert(sizeof(s_parser_state) == 0x330);
static_assert(sizeof(s_parser_state) == 0x3030);

extern s_parser_state g_parser_state;

class c_debug_menu;
extern void debug_menu_parse(c_debug_menu* root_menu, char const* file_name);

