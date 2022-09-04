#pragma once

#include "cseries/cseries.hpp"

#pragma pack(push, 1)
struct c_network_time_statistics
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
	dword __unknown10;
	qword __unknown14;
	dword m_period_duration_msec;
	dword m_interval_duration_msec;
	real __unknown24;
	dword m_next_interval_index;
	byte __data2C[160];
	qword __unknownCC;
	byte __dataD4[4];
};
static_assert(sizeof(c_network_time_statistics) == 0xD8);
#pragma pack(pop)