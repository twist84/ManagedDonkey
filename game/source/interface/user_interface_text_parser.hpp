#pragma once

#include "cseries/cseries.hpp"
#include "text/xml_parser.hpp"

struct c_game_tag_parser :
	public c_xml_tag_parser<wchar_t>
{
public:
	virtual bool get_magic_string(wchar_t* buffer, long buffer_length) = 0;

	c_game_tag_parser(wchar_t const* magic_character);

protected:
	wchar_t m_magic_character;
};
static_assert(sizeof(c_game_tag_parser) == 0xC);

struct c_magic_string_game_tag_parser :
	public c_game_tag_parser
{
public:
	virtual bool parse_tag(wchar_t const* a1, long* a2, wchar_t* a3, long a4, long* a5) override
	{
		return INVOKE_CLASS_MEMBER(0x00AA67C0, c_magic_string_game_tag_parser, parse_tag, a1, a2, a3, a4, a5);
	}

	virtual bool get_magic_string(wchar_t* buffer, long buffer_length) override;

	c_magic_string_game_tag_parser(wchar_t const* xml_tag, void* game_data, bool(*game_data_function)(void*, wchar_t*, long));

protected:
	void* m_game_data;
	bool(__cdecl* m_game_data_function)(void* this_ptr, wchar_t* buffer, long buffer_size);
};
static_assert(sizeof(c_magic_string_game_tag_parser) == sizeof(c_game_tag_parser) + 0x8);

struct c_user_interface_text_parse_manager
{
	void register_parser(c_game_tag_parser* parser, long screen_index);
	void unregister_parser(c_game_tag_parser* parser, long screen_index);

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

extern void __cdecl user_interface_text_parser_register(c_game_tag_parser* parser, long screen_index);
extern void __cdecl user_interface_text_parser_unregister(c_game_tag_parser* parser, long screen_index);

