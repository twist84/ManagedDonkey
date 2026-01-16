#pragma once

#include "cseries/cseries.hpp"

const wchar_t k_xml_tag_start_character = '<';

template<typename t_character_type>
class c_xml_tag_parser
{
public:
	virtual bool parse_tag(const t_character_type* a1, int32* a2, t_character_type* a3, int32 a4, int32* a5) = 0;
	virtual ~c_xml_tag_parser() = default;

	c_xml_tag_parser(const t_character_type* xml_tag) :
		m_xml_tag(xml_tag)
	{
		DECLFUNC(0x014E8230, void, __thiscall, c_xml_tag_parser<t_character_type>*, const t_character_type*)(this, xml_tag);
	}

protected:
	const t_character_type* m_xml_tag;
	//wchar_t m_magic_character;
};
COMPILE_ASSERT(sizeof(c_xml_tag_parser<wchar_t>) == 0x8);
//COMPILE_ASSERT(sizeof(c_xml_tag_parser<wchar_t>) == 0xC);

