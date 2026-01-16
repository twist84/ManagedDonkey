#pragma once

#include "cseries/cseries.hpp"
#include "objects/objects.hpp"

struct _crate_datum
{
	uns32 flags;
	int32 self_destruction_timer;
};
COMPILE_ASSERT(sizeof(_crate_datum) == 0x8);

struct crate_datum
{
	int32 definition_index;
	_object_datum object;
	_crate_datum crate;
};
COMPILE_ASSERT(sizeof(crate_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_crate_datum));

