#pragma once

#include "cseries/cseries.hpp"

struct c_motor_request
{
	byte __unknown0;
	byte __unknown1;
	byte __data2[0x2];
	long __unknown4;
	long __unknown8;
	long __unknownC;
	byte __data10[0xC];
};
static_assert(sizeof(c_motor_request) == 0x1C);

extern long __cdecl motor_system_submit(long motor_index, c_motor_request const* request);

