#pragma once

#include "cseries/cseries.hpp"

struct s_content_item
{
	uint8 __data[0x240];
};
static_assert(sizeof(s_content_item) == 0x240);