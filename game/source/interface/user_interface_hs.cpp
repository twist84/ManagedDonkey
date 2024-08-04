#include "interface/user_interface_hs.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "hs/hs_runtime.hpp"
#include "scenario/scenario.hpp"

long start_script(hs_script const* script, long index)
{
	ASSERT(script);
	ASSERT(index != NONE);

	long thread_index = hs_runtime_script_begin(static_cast<short>(index), script->script_type, _hs_thread_type_script);
	if (thread_index == NONE)
	{
		generate_event(_event_level_error, "ui:hs: failed to start script %s", script->name.get_string());
	}
	else
	{
		generate_event(_event_level_message, "ui:hs: started script %s (thread 0x%08X)", script->name.get_string(), thread_index);
	}

	return thread_index;
}

long user_interface_start_hs_script_by_name(char const* name)
{
	struct scenario* scenario = global_scenario_try_and_get();
	if (!scenario)
		return NONE;

	long script_index;
	hs_script const* script = nullptr;
	for (script_index = 0; script_index < scenario->scripts.count(); script_index++)
	{
		script = &scenario->scripts[script_index];
		if (script->name.is_equal(name))
			break;
	}

	if (script)
		return start_script(script, script_index);

	generate_event(_event_level_warning, "ui:hs: no such script \"%s\"", name);
	return NONE;
}

