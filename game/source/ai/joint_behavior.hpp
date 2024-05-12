#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct invitation_data
{
	byte __data[0x10];
};
static_assert(sizeof(invitation_data) == 0x10);

struct joint_state_datum :
	s_datum_header
{
	byte __data[0xCA];
};
static_assert(sizeof(joint_state_datum) == 0xCC);

