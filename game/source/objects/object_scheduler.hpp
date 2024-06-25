#pragma once

#include "cseries/cseries.hpp"

struct s_object_schedule
{
	// all names subject to change

	real phase_time;
	long object_index;
	long phase_index_update_time;
	short phase_index;
	short phase_count;
};
static_assert(sizeof(s_object_schedule) == 0x10);

struct c_schedule_iterator
{
	// all names subject to change

	long phase_index;
	c_static_flags<32> phases_allocated;
};
static_assert(sizeof(c_schedule_iterator) == 0x8);

struct s_object_schedule_globals
{
	// all names subject to change

	c_static_array<s_object_schedule, 32> schedules;
	c_static_array<real, 30> phases;
	c_static_flags<30> phases_allocated;
};
static_assert(sizeof(s_object_schedule_globals) == 0x27C);

extern void __cdecl object_scheduler_update();

