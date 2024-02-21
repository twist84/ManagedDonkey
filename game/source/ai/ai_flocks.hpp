#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct flock_datum :
	s_datum_header
{
	byte __data[0x4A];
};
static_assert(sizeof(flock_datum) == 0x4C);

struct s_flock_instance;
struct s_flock_palette_entry;

