#pragma once

#include "cseries/cseries.hpp"
#include "scenario/scenario_trigger_volumes.hpp"

struct s_scenario_kill_trigger_volumes_state
{
	c_static_flags<MAXIMUM_TRIGGER_VOLUMES_PER_SCENARIO> disabled_kill_volumes;
	bool map_initialized;
};
COMPILE_ASSERT(sizeof(s_scenario_kill_trigger_volumes_state) == 0x84);

extern void __cdecl scenario_kill_trigger_volume_disable(int32 trigger_volume_index);
extern void __cdecl scenario_kill_trigger_volume_enable(int32 trigger_volume_index);
extern bool __cdecl scenario_kill_trigger_volume_test_object(int32 object_index, bool* a2);
extern void __cdecl scenario_kill_trigger_volumes_dispose();
extern void __cdecl scenario_kill_trigger_volumes_dispose_from_old_map();
extern void __cdecl scenario_kill_trigger_volumes_initialize();
extern void __cdecl scenario_kill_trigger_volumes_initialize_for_new_map();

