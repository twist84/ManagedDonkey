#pragma once

#include "cseries/cseries.hpp"

struct hkMoppCode
{
	byte __data[0x30];
};
static_assert(sizeof(hkMoppCode) == 0x30);

