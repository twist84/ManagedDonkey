#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct light_datum : s_datum_header
{
	byte __data[0xE2];
};
static_assert(sizeof(light_datum) == 0xE4);

struct lights_game_globals_definition
{
	byte __data[0x40];
};
static_assert(sizeof(lights_game_globals_definition) == 0x40);

struct s_nondeterministic_light_data
{
	byte __data[0x2580];
};
static_assert(sizeof(s_nondeterministic_light_data) == 0x2580);

