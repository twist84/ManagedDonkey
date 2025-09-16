#include "interface/user_interface_hs.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "hs/hs_runtime.hpp"
#include "scenario/scenario.hpp"

int32 start_script(const hs_script* script, int32 index)
{
	ASSERT(script);
	ASSERT(index != NONE);

	int32 thread_index = hs_runtime_script_begin((int16)(index & 0xFFFF), (e_hs_script_type)script->script_type, _hs_thread_type_script);
	if (thread_index == NONE)
	{
		event(_event_error, "ui:hs: failed to start script %s", script->name);
	}
	else
	{
		event(_event_message, "ui:hs: started script %s (thread 0x%08X)", script->name, thread_index);
	}

	return thread_index;
}

int32 user_interface_start_hs_script_by_name(const char* name)
{
	struct scenario* scenario = global_scenario_try_and_get();
	if (!scenario)
		return NONE;

	int32 script_index;
	const hs_script* script = nullptr;
	for (script_index = 0; script_index < scenario->scripts.count; script_index++)
	{
		if (ascii_stricmp(name, scenario->scripts[script_index].name) == 0)
		{
			script = &scenario->scripts[script_index];
			break;
		}
	}

	if (script)
		return start_script(script, script_index);

	event(_event_warning, "ui:hs: no such script \"%s\"", name);
	return NONE;
}

