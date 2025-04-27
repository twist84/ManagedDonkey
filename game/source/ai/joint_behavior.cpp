#include "ai/joint_behavior.hpp"

void __cdecl joint_behavior_create_for_new_map()
{
	INVOKE(0x014A9EA0, joint_behavior_create_for_new_map);
}

void __cdecl joint_behavior_dispose()
{
	INVOKE(0x014A9EB0, joint_behavior_dispose);
}

void __cdecl joint_behavior_dispose_from_old_map()
{
	INVOKE(0x014A9EC0, joint_behavior_dispose_from_old_map);
}

bool __cdecl joint_behavior_get_participant_limits(int16 behavior_index, int16* min_participants_ref, int16* max_participants_ref)
{
	return INVOKE(0x014A9EE0, joint_behavior_get_participant_limits, behavior_index, min_participants_ref, max_participants_ref);
}

void __cdecl joint_behavior_initialize()
{
	INVOKE(0x014A9F40, joint_behavior_initialize);
}

void __cdecl joint_behavior_initialize_for_new_map()
{
	INVOKE(0x014A9F80, joint_behavior_initialize_for_new_map);
}

void __cdecl joint_behaviors_update()
{
	INVOKE(0x014A9FA0, joint_behaviors_update);
}

