#pragma once

#include "cseries/cseries.hpp"
#include "text/xml_parser.hpp"

struct c_game_tag_parser : public c_xml_tag_parser<wchar_t>
{
public:
	virtual bool get_magic_string(wchar_t* buffer, long buffer_length);

protected:
	wchar_t m_magic_character;
};
static_assert(sizeof(c_game_tag_parser) == 0xC);

struct c_user_interface_text_parse_manager
{
	long m_tag_parser_count;

	// #TODO: find the proper name of this struct
	struct s_text_parser
	{
		c_game_tag_parser* parser;
		long screen_index;
	};
	c_static_array<s_text_parser, 256> m_tag_parsers;
};
static_assert(sizeof(c_user_interface_text_parse_manager) == 0x804);

extern c_user_interface_text_parse_manager& g_parse_manager;

