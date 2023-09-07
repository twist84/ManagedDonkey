#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct prop_ref_datum : s_datum_header
{
	byte __data[0x3A];
};
static_assert(sizeof(prop_ref_datum) == 0x3C);

struct prop_datum : s_datum_header
{
	byte __data[0xC2];
};
static_assert(sizeof(prop_datum) == 0xC4);

struct tracking_datum : s_datum_header
{
	byte __data[0xFE];
};
static_assert(sizeof(tracking_datum) == 0x100);

