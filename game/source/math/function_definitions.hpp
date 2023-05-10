#pragma once

#include "tag_files/tag_groups.hpp"

struct real_block
{
	real value;
};
static_assert(sizeof(real_block) == sizeof(real));

struct function_definition_data
{
	char function_type;
	byte_flags flags;
	char function[2];
	rgb_color color[4];
	c_typed_tag_block<real_block> values;
};
static_assert(sizeof(function_definition_data) == 0x20);

struct mapping_function
{
	c_typed_tag_data<function_definition_data> data;
};
static_assert(sizeof(mapping_function) == sizeof(s_tag_data));

struct c_function_definition : mapping_function
{
};
static_assert(sizeof(c_function_definition) == sizeof(mapping_function));

