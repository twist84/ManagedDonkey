#include "interface/user_interface_text_parser.hpp"

REFERENCE_DECLARE(0x05260730, c_user_interface_text_parse_manager, g_parse_manager);

c_game_tag_parser::c_game_tag_parser(wchar_t const* xml_tag) :
	c_xml_tag_parser(xml_tag),
	m_magic_character(0)
{
	DECLFUNC(0x00AA6490, c_game_tag_parser*, __thiscall, c_game_tag_parser*, wchar_t const*)(this, xml_tag);
}

bool c_magic_string_game_tag_parser::get_magic_string(wchar_t* buffer, long buffer_length)
{
	return DECLFUNC(0x00AA6490, bool, __thiscall, c_magic_string_game_tag_parser*, wchar_t*, long)(this, buffer, buffer_length);
}

c_magic_string_game_tag_parser::c_magic_string_game_tag_parser(wchar_t const* xml_tag, void* game_data, bool(*game_data_function)(void*, wchar_t*, long)) :
	c_game_tag_parser(xml_tag),
	m_game_data(game_data),
	m_game_data_function(game_data_function)
{
	DECLFUNC(0x00AA64C0, c_magic_string_game_tag_parser*, __thiscall, c_magic_string_game_tag_parser*, wchar_t const*, void*, decltype(game_data_function))(this, xml_tag, game_data, game_data_function);
}

