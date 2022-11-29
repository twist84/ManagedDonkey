#pragma once

#include "cseries/cseries.hpp"

struct s_chud_navpoint
{
	long object_index;
	long __unknown4;
	long __unknown8;
	long __unknownC;
	dword_flags flags;
	real __unknown14;
	wchar_t str[6];
	real_point3d position;
};

extern void __cdecl chud_submit_navpoint(long, s_chud_navpoint* navpoint);

