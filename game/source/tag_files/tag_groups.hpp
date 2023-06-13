#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_group_definitions.hpp"

struct s_tag_block
{
	long count;
	byte(&elements)[];
	long : 32; // byte* definition;
};
static_assert(sizeof(s_tag_block) == 0xC);

struct s_tag_reference
{
	tag group_tag;
	long : 32; // char const* name;
	long : 32; // long name_length;
	long index;

	void* get_definition();

	template<typename t_type>
	t_type* cast_to()
	{
		return static_cast<t_type*>(get_definition());
	}

	char const* get_name();
	char const* get_group_name();
};
static_assert(sizeof(s_tag_reference) == 0x10);

struct s_tag_data
{
	long size;
	long : 32; //  flags;
	long : 32; // long stream_position;
	byte(&base)[];
	long : 32; // byte* definition;
};
static_assert(sizeof(s_tag_data) == 0x14);

template<typename t_element_type, dword ...t_extra>
//using c_typed_tag_block = s_tag_block;
struct c_typed_tag_block
{
public:
	long count()
	{
		return m_count;
	}

	t_element_type* begin()
	{
		return m_elements;
	}

	t_element_type* end()
	{
		return m_elements + m_count;
	}

	t_element_type& operator[](long index)
	{
		ASSERT(VALID_INDEX(index, m_count));

		return m_elements[index];
	}

	void clear()
	{
		csmemset(m_elements, 0, sizeof(m_elements) * m_count);
		m_count = 0;
	}

//protected:
	long m_count;
	t_element_type* m_elements;
	long : 32; // byte* definition;
};

template<tag ...k_group_tags>
using c_typed_tag_reference = s_tag_reference;

template<typename t_data_type, dword ...t_extra>
//using c_typed_tag_data = s_tag_data;
struct c_typed_tag_data : s_tag_data
{
	t_data_type* get()
	{
		return reinterpret_cast<t_data_type*>(base);
	}
};

extern void __cdecl tag_load_missing_tags_report();
extern char const* __cdecl tag_name_strip_path(char const* path);
extern wchar_t const* __cdecl tag_name_strip_path(wchar_t const* path);

