#include "cseries/progress.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "shell/shell.hpp"

REFERENCE_DECLARE(0x02446088, progress_globals_definition, progress_globals);

void __cdecl progress_disable_callbacks(bool disable_callbacks)
{
	INVOKE(0x005F0810, progress_disable_callbacks, disable_callbacks);

	//progress_globals.disable_callbacks = disable_callbacks;
}

void __cdecl progress_done()
{
	INVOKE(0x005F0820, progress_done);

	//if (progress_globals.__unknown28)
	//{
	//	if (!progress_globals.disable_callbacks)
	//	{
	//		if (progress_globals.custom_callbacks.progress_done_proc)
	//		{
	//			progress_globals.custom_callbacks.progress_done_proc(progress_globals.description, progress_globals.custom_callbacks.progress_data);
	//		}
	//		else if (progress_globals.default_callbacks.progress_done_proc)
	//		{
	//			progress_globals.default_callbacks.progress_done_proc(progress_globals.description, progress_globals.default_callbacks.progress_data);
	//		}
	//	}
	//
	//	progress_globals.__unknown28 = false;
	//}
	//generate_event(_event_message, "progress: %s, total time: %.2f seconds",
	//	progress_globals.description, ((system_milliseconds() - progress_globals.start_time) / 1000.0f));
}

void __cdecl progress_new(char const* description)
{
	INVOKE(0x005F0880, progress_new, description);

	//progress_done();
	//progress_globals.__unknown28 = true;
	//progress_globals.start_time = system_milliseconds();
	//csstrnzcpy(progress_globals.description, description, sizeof(progress_globals.description));
	//progress_globals.update_progress = -1;
	//progress_globals.update_time = 0;
	//if (!progress_globals.disable_callbacks)
	//{
	//	if (progress_globals.custom_callbacks.progress_new_proc)
	//	{
	//		progress_globals.custom_callbacks.progress_new_proc(progress_globals.description, progress_globals.custom_callbacks.progress_data);
	//	}
	//	else if (progress_globals.default_callbacks.progress_new_proc)
	//	{
	//		progress_globals.default_callbacks.progress_new_proc(progress_globals.description, progress_globals.default_callbacks.progress_data);
	//	}
	//}
}

void __cdecl progress_set_custom_callbacks(progress_callbacks const* callbacks)
{
	INVOKE(0x005F0910, progress_set_custom_callbacks, callbacks);

	//if (callbacks)
	//{
	//	progress_globals.custom_callbacks = *callbacks;
	//}
	//else
	//{
	//	progress_globals.custom_callbacks =
	//	{
	//		.progress_new_proc = NULL,
	//		.progress_update_proc = NULL,
	//		.progress_done_proc = NULL,
	//		.progress_data = NULL
	//	};
	//}
}

void __cdecl progress_set_default_callbacks(progress_callbacks const* callbacks)
{
	INVOKE(0x005F0950, progress_set_default_callbacks, callbacks);

	//if (callbacks)
	//{
	//	progress_globals.default_callbacks = *callbacks;
	//}
	//else
	//{
	//	progress_globals.default_callbacks =
	//	{
	//		.progress_new_proc = NULL,
	//		.progress_update_proc = NULL,
	//		.progress_done_proc = NULL,
	//		.progress_data = NULL
	//	};
	//}
}

long __cdecl progress_update(long index, long count)
{
	return INVOKE(0x005F0990, progress_update, index, count);

	//return progress_update_with_description(index, count, NULL);
}

long __cdecl progress_update_with_description(long index, long count, char const* description)
{
	return INVOKE(0x005F09B0, progress_update_with_description, index, count, description);

	//dword time = system_milliseconds();
	//long progress = 100 * MIN(MAX(index, 0), count) / MAX(count, 1);
	//if (progress_globals.__unknown28 && (shell_tool_type() != _shell_tool_command_line || progress >= progress_globals.update_progress + 20 || time - progress_globals.update_time > 500))
	//{
	//	progress_globals.update_progress = progress;
	//	progress_globals.update_time = time;
	//	if (!progress_globals.disable_callbacks)
	//	{
	//		if (progress_globals.custom_callbacks.progress_update_proc)
	//		{
	//			progress_globals.custom_callbacks.progress_update_proc(progress_globals.description, description, progress, progress_globals.custom_callbacks.progress_data);
	//		}
	//		else if (progress_globals.default_callbacks.progress_update_proc)
	//		{
	//			progress_globals.default_callbacks.progress_update_proc(progress_globals.description, description, progress, progress_globals.default_callbacks.progress_data);
	//		}
	//	}
	//}
	//return progress;
}

