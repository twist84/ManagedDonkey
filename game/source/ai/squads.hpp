#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct squad_datum : s_datum_header
{
	byte __data[0xEA];
};
static_assert(sizeof(squad_datum) == 0xEC);

struct squad_group_datum : s_datum_header
{
	byte __data[0x22];
};
static_assert(sizeof(squad_group_datum) == 0x24);

