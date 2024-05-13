#pragma once

#include "ai/clump.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_objective :
	s_datum_header
{
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(s_objective) == 0xC);

struct s_task_record
{
	long leader;
	short body_count;
	short lowest_rank;
	short highest_rank;
	word_flags flags; // assume odst flags
	long __timeC;
	long __time10;
	long __time14;
	long __time18;
	long __time1C;
	long __time20;
	long __time24;
	real_point3d position;
	bool position_calculated;
	byte __data35[0x1];
	short __unknown36;
	c_clump_behavior_state state;
};
static_assert(sizeof(s_task_record) == 0x40);

struct s_objective;

