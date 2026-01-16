#pragma once

#include "cseries/cseries.hpp"

struct hkMoppCode
{
	byte __data[0x30];
};
COMPILE_ASSERT(sizeof(hkMoppCode) == 0x30);

