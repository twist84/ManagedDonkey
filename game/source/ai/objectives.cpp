#include "ai/objectives.hpp"


char const* const inhibit_behavior_flag_names[k_inhibit_behavior_flags]
{
	"Cover",
	"Retreat",
	"Vehicles",
	"Grenades",
	"Berserk",
	"Equipment",
	"Pureform Ranged",
	"Pureform Tank",
	"Pureform Stalker"
};

s_task_record* __cdecl objective_get_task_record(short objective_index, short task_index)
{
	return INVOKE(0x01448840, objective_get_task_record, objective_index, task_index);
}

