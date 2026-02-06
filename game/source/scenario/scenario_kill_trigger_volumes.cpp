#include "scenario/scenario_kill_trigger_volumes.hpp"

#include "memory/thread_local.hpp"

//.text:0068C220 ; 
//.text:0068C240 ; 
//.text:0068C250 ; void __cdecl __tls_set_g_scenario_kill_trigger_volumes_state_allocator(void*)
//.text:0068C270 ; 
//.text:0068C2B0 ; 
//.text:0068C2E0 ; 
//.text:0068C310 ; 

void __cdecl scenario_kill_trigger_volume_disable(int32 trigger_volume_index)
{
	//INVOKE(0x0068C350, scenario_kill_trigger_volume_disable, trigger_volume_index);

	ASSERT(g_scenario_kill_trigger_volumes_state != nullptr);
	ASSERT(g_scenario_kill_trigger_volumes_state->map_initialized);

	if (trigger_volume_index == NONE)
		return;

	scenario_trigger_volume* trigger = &global_scenario_get()->trigger_volumes[trigger_volume_index];
	if (trigger->kill_trigger_volume_index == NONE)
	{
		c_console::write_line("trigger %s is not a kill trigger, you can't disable it", trigger->name.get_string());
	}
	else
	{
		ASSERT(VALID_INDEX(trigger->kill_trigger_volume_index, MAXIMUM_TRIGGER_VOLUMES_PER_SCENARIO));
		g_scenario_kill_trigger_volumes_state->disabled_kill_volumes.set(trigger->kill_trigger_volume_index, true);
	}
}

void __cdecl scenario_kill_trigger_volume_enable(int32 trigger_volume_index)
{
	//INVOKE(0x0068C3B0, scenario_kill_trigger_volume_enable, trigger_volume_index);

	ASSERT(g_scenario_kill_trigger_volumes_state != nullptr);
	ASSERT(g_scenario_kill_trigger_volumes_state->map_initialized);

	if (trigger_volume_index == NONE)
		return;

	scenario_trigger_volume* trigger = &global_scenario_get()->trigger_volumes[trigger_volume_index];
	if (trigger->kill_trigger_volume_index == NONE)
	{
		c_console::write_line("trigger %s is not a kill trigger, you can't enable it", trigger->name.get_string());
	}
	else
	{
		ASSERT(VALID_INDEX(trigger->kill_trigger_volume_index, MAXIMUM_TRIGGER_VOLUMES_PER_SCENARIO));
		g_scenario_kill_trigger_volumes_state->disabled_kill_volumes.set(trigger->kill_trigger_volume_index, false);
	}
}

bool __cdecl scenario_kill_trigger_volume_test_object(int32 object_index, bool* a2)
{
	return INVOKE(0x0068C410, scenario_kill_trigger_volume_test_object, object_index, a2);
}

void __cdecl scenario_kill_trigger_volumes_dispose()
{
	INVOKE(0x0068C520, scenario_kill_trigger_volumes_dispose);
}

void __cdecl scenario_kill_trigger_volumes_dispose_from_old_map()
{
	INVOKE(0x0068C540, scenario_kill_trigger_volumes_dispose_from_old_map);
}

void __cdecl scenario_kill_trigger_volumes_initialize()
{
	INVOKE(0x0068C560, scenario_kill_trigger_volumes_initialize);
}

void __cdecl scenario_kill_trigger_volumes_initialize_for_new_map()
{
	INVOKE(0x0068C5C0, scenario_kill_trigger_volumes_initialize_for_new_map);
}

