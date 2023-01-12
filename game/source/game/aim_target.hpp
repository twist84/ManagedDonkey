#pragma once

#include "cseries/cseries.hpp"

struct c_aim_target_predicted
{
	byte target_type;
	dword aim_target;
	dword target_model;
};
static_assert(sizeof(c_aim_target_predicted) == 0xC);

