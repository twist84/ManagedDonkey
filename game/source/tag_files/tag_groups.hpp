#pragma once

#include "cseries/cseries.hpp"

extern void __cdecl tag_load_missing_tags_report();

struct tag_block
{
	long count;
	byte* elements;
	byte* definition;
};
static_assert(sizeof(tag_block) == 0xC);

struct tag_reference
{
	tag group_tag;
	const char* name;
	long name_length;
	long index;
};
static_assert(sizeof(tag_reference) == 0x10);

struct tag_data
{
	long size;
	dword_flags flags;
	long stream_position;
	byte* elements;
	byte* definition;
};
static_assert(sizeof(tag_data) == 0x14);

template<typename t_element_type, dword ...t_extra>
//using c_typed_tag_block = tag_block;
struct c_typed_tag_block
{
	long count;
	t_element_type* elements;
	byte* definition;
};

template<tag ...k_group_tags>
using c_typed_tag_reference = tag_reference;

template<typename t_data_type, dword ...t_extra>
using c_typed_tag_data = tag_data;

struct c_string_id
{
public:
	char const* get_string();

protected:
	string_id m_value;
};