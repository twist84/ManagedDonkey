#pragma once

#include "cseries/cseries.hpp"

struct object_list_header_datum :
	s_datum_header
{
	int16 __unknown2;
	int16 reference_count;
	int16 count;
	int32 reference_index;
};
static_assert(sizeof(object_list_header_datum) == 0xC);

struct object_list_datum :
	s_datum_header
{
	int32 object_index;
	int32 reference_index;
};
static_assert(sizeof(object_list_datum) == 0xC);

void __cdecl object_list_gc();

