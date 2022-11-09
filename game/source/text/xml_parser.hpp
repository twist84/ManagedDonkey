#pragma once

const wchar_t k_xml_tag_start_character = '<';

template<typename t_character_type>
struct c_xml_tag_parser
{
public:
	virtual bool parse_tag(wchar_t const*, long*, wchar_t*, long, long*);
	virtual ~c_xml_tag_parser();

protected:
	wchar_t* m_tag_text;
	//wchar_t m_magic_character;
};
static_assert(sizeof(c_xml_tag_parser<wchar_t>) == 0x8);
//static_assert(sizeof(c_xml_tag_parser<wchar_t>) == 0xC);

