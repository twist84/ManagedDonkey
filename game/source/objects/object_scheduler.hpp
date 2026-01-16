#pragma once

#include "cseries/cseries.hpp"

struct s_object_schedule
{
	// all names subject to change

	real32 phase_time;
	int32 object_index;
	int32 phase_index_update_time;
	int16 phase_index;
	int16 phase_count;
};
COMPILE_ASSERT(sizeof(s_object_schedule) == 0x10);

class c_schedule_iterator
{
public:
	// all names subject to change

	int32 phase_index;
	c_static_flags<32> phases_allocated;
};
COMPILE_ASSERT(sizeof(c_schedule_iterator) == 0x8);

struct s_object_schedule_globals
{
	// all names subject to change

	c_static_array<s_object_schedule, 32> schedules;
	c_static_array<real32, 30> phases;
	c_static_flags<30> phases_allocated;
};
COMPILE_ASSERT(sizeof(s_object_schedule_globals) == 0x27C);

extern void __cdecl object_scheduler_dispose();
extern void __cdecl object_scheduler_dispose_from_old_map();
extern void __cdecl object_scheduler_initialize();
extern void __cdecl object_scheduler_initialize_for_new_map();
extern void __cdecl object_scheduler_update();

