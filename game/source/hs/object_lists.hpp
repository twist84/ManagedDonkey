#pragma once

#include "cseries/cseries.hpp"

struct object_list_header_datum :
	s_datum_header
{
	short __unknown2;
	short reference_count;
	short count;
	long reference_index;
};
static_assert(sizeof(object_list_header_datum) == 0xC);

struct object_list_datum :
	s_datum_header
{
	long object_index;
	long reference_index;
};
static_assert(sizeof(object_list_datum) == 0xC);

