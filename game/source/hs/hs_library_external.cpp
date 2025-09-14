#include "hs/hs_library_external.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game_time.hpp"
#include "hs/hs.hpp"
#include "hs/hs_runtime.hpp"
#include "interface/terminal.hpp"
#include "memory/module.hpp"

void __cdecl hs_evaluate_library_external(int16 script_index);

HOOK_DECLARE(0x0096D3E0, hs_debug_variable);
HOOK_DECLARE(0x0096D870, hs_evaluate_library_external);
HOOK_DECLARE(0x0096D8B0, hs_log_print);
HOOK_DECLARE(0x0096EF60, hs_print);
HOOK_DECLARE(0x0096F0F0, hs_trigger_volume_test_objects_all);
HOOK_DECLARE(0x0096F150, hs_trigger_volume_test_objects_any);

void __cdecl hs_evaluate_library_external(int16 script_index)
{
	hs_evaluate(script_index);
}

void __cdecl hs_debug_variable(const char* s, bool debug)
{
	bool variable_not_enclosed = false;
	if (*s == '\'')
	{
		char global_name[1024]{};
	
		char* pos = global_name;
		char* end = global_name + sizeof(global_name);
		csstrnzcpy(global_name, s + 1, sizeof(global_name));
		if (global_name[0])
		{
			do
			{
				if (*pos == '\'')
				{
					break;
				}
	
				if (pos >= end)
				{
					break;
				}
	
				pos++;
	
			} while (*pos);
		}
	
		if (*pos && pos < end)
		{
			*pos = 0;
		}
		else
		{
			variable_not_enclosed = true;
		}
	
		int16 global_index = hs_find_global_by_name(global_name);
		if (global_index != NONE)
		{
			debug_globals_all = true;
			debug_global_variables[global_index] = debug;
		}
	}
	else
	{
		variable_not_enclosed = true;
	}
	
	if (variable_not_enclosed)
	{
		event(_event_error, "ai: please enclose the variable to be watched in single quotes");
	}
}

void __cdecl hs_evaluate(int16 script_index)
{
	//INVOKE(0x0096D870, hs_evaluate, script_index);

	hs_script* script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->scripts, script_index, hs_script);
	if (script->parameters.count > 0)
	{
		event(_event_warning, "hs: tried to pass a script to 'evaluate' which has arguments");
	}
	else if (script->script_type != _hs_script_type_static)
	{
		event(_event_warning, "hs: tried to pass a script to 'evaluate' which isn't static");
	}
	else
	{
		hs_runtime_push_script(script_index);
	}
}

void __cdecl hs_log_print(const char* s)
{
	event(_event_warning, "hs: %s", s);
}

void __cdecl hs_print(const char* s)
{
	terminal_printf(global_real_argb_green, s);
}

bool __cdecl hs_trigger_volume_test_objects(int16 trigger_volume_index, int32 object_index, bool a3)
{
	bool result = INVOKE(0x0096F080, hs_trigger_volume_test_objects, trigger_volume_index, object_index, a3);

	hs_debug_data.activated_trigger_volumes.set(trigger_volume_index, result);

	return result;
}

bool __cdecl hs_trigger_volume_test_objects_all(int16 trigger_volume_index, int32 object_index)
{
	return hs_trigger_volume_test_objects(trigger_volume_index, object_index, true);
}

bool __cdecl hs_trigger_volume_test_objects_any(int16 trigger_volume_index, int32 object_index)
{
	return hs_trigger_volume_test_objects(trigger_volume_index, object_index, false);
}

