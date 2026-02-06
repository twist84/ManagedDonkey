#include "cseries/progress.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x02446088, s_progress_globals, progress_globals);

void __cdecl progress_disable_callbacks(bool disable_callbacks)
{
	INVOKE(0x005F0810, progress_disable_callbacks, disable_callbacks);

	//progress_globals.disable_callbacks = disable_callbacks;
}

void __cdecl progress_done()
{
	INVOKE(0x005F0820, progress_done);

	//if (progress_globals.currently_valid)
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
	//	progress_globals.currently_valid = false;
	//}
	//event(_event_message, "progress: %s, total time: %.2f seconds",
	//	progress_globals.description, ((system_milliseconds() - progress_globals.start_time) / 1000.0f));
}

void __cdecl progress_new(const char* description)
{
	INVOKE(0x005F0880, progress_new, description);

	//progress_done();
	//progress_globals.currently_valid = true;
	//progress_globals.start_time = system_milliseconds();
	//csstrnzcpy(progress_globals.description, description, sizeof(progress_globals.description));
	//progress_globals.last_progress = -1;
	//progress_globals.last_milliseconds = 0;
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

void __cdecl progress_set_custom_callbacks(const progress_callbacks* callbacks)
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
	//		.progress_new_proc = nullptr,
	//		.progress_update_proc = nullptr,
	//		.progress_done_proc = nullptr,
	//		.progress_data = nullptr
	//	};
	//}
}

void __cdecl progress_set_default_callbacks(const progress_callbacks* callbacks)
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
	//		.progress_new_proc = nullptr,
	//		.progress_update_proc = nullptr,
	//		.progress_done_proc = nullptr,
	//		.progress_data = nullptr
	//	};
	//}
}

int32 __cdecl progress_update(int32 current_amount, int32 total_amount)
{
	return INVOKE(0x005F0990, progress_update, current_amount, total_amount);

	//return progress_update_with_description(current_amount, total_amount, nullptr);
}

int32 __cdecl progress_update_with_description(int32 current_amount, int32 total_amount, const char* optional_description)
{
	return INVOKE(0x005F09B0, progress_update_with_description, current_amount, total_amount, optional_description);

	//uns32 time = system_milliseconds();
	//int32 progress = 100 * PIN(current_amount, 0, total_amount) / MAX(total_amount, 1);
	//if (progress_globals.currently_valid && (shell_tool_type() != _shell_tool_command_line || progress >= progress_globals.last_progress + 20 || time - progress_globals.last_milliseconds > 500))
	//{
	//	progress_globals.last_progress = progress;
	//	progress_globals.last_milliseconds = time;
	//	if (!progress_globals.disable_callbacks)
	//	{
	//		if (progress_globals.custom_callbacks.progress_update_proc)
	//		{
	//			progress_globals.custom_callbacks.progress_update_proc(progress_globals.description, optional_description, progress, progress_globals.custom_callbacks.progress_data);
	//		}
	//		else if (progress_globals.default_callbacks.progress_update_proc)
	//		{
	//			progress_globals.default_callbacks.progress_update_proc(progress_globals.description, optional_description, progress, progress_globals.default_callbacks.progress_data);
	//		}
	//	}
	//}
	//return progress;
}

