#pragma once

#include "cseries/cseries.hpp"

struct squad_patrol_datum :
	s_datum_header
{
	byte __data[0x6C2];
};
static_assert(sizeof(squad_patrol_datum) == 0x6C4);

struct s_squad_patrol_definition;

