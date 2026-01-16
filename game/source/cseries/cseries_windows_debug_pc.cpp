#include "cseries/cseries_windows_debug_pc.hpp"

#include "config/version.hpp"
#include "cseries/cseries_event_logs.hpp"
#include "editor/editor_stubs.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "multithreading/synchronized_value.hpp"
#include "multithreading/threads.hpp"
#include "rasterizer/rasterizer.hpp"
#include "tag_files/files.hpp"
#include "tag_files/files_windows.hpp"
#include "tag_files/tag_groups.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <dbghelp.h>
#include <float.h>
#include <Psapi.h>

struct s_exception_type_info
{
	const char* exception_string;
	uns32 exception_parameters[3];
};
COMPILE_ASSERT(sizeof(s_exception_type_info) == 0x10);

struct s_exception_information
{
	c_synchronized_long exception_occurred;
	int32 thread_id;
	CONTEXT context_record;
	uns32 exception_code;
	uns32 exception_flags;
	void* exception_address;
	uns32 number_parameters;
	union
	{
		s_exception_type_info exception_type_info;
		s_thread_assert_arguments thread_assert_arguments;
	};
};
COMPILE_ASSERT(sizeof(s_exception_information) == 0x2F4);

REFERENCE_DECLARE(0x0238E878, bool, g_set_always_a_debugger_present);
REFERENCE_DECLARE(0x0238E879, bool, g_set_never_a_debugger_present);
REFERENCE_DECLARE(0x0238E87C, uns32, g_exception_time);
REFERENCE_DECLARE(0x0238E880, PEXCEPTION_POINTERS, g_exception_pointers);
REFERENCE_DECLARE(0x0238E884, c_synchronized_long, g_exception_caching_in_progress);
REFERENCE_DECLARE(0x0238E888, s_exception_information, g_exception_information);

HOOK_DECLARE(0x0051C020, exceptions_update);

const char* const k_screenshot_file = "crash_report\\crash_screenshot.bmp";

const char* GetExceptionFlagsString(DWORD exception)
{
	switch (exception)
	{
	case EXCEPTION_NONCONTINUABLE:           return "EXCEPTION_NONCONTINUABLE";
	case EXCEPTION_UNWINDING:                return "EXCEPTION_UNWINDING";
	case EXCEPTION_EXIT_UNWIND:              return "EXCEPTION_EXIT_UNWIND";
	case EXCEPTION_STACK_INVALID:            return "EXCEPTION_STACK_INVALID";
	case EXCEPTION_NESTED_CALL:              return "EXCEPTION_NESTED_CALL";
	case EXCEPTION_TARGET_UNWIND:            return "EXCEPTION_TARGET_UNWIND";
	case EXCEPTION_COLLIDED_UNWIND:          return "EXCEPTION_COLLIDED_UNWIND";
	case EXCEPTION_SOFTWARE_ORIGINATE:       return "EXCEPTION_SOFTWARE_ORIGINATE";
	case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";
	}

	return "";
}

void exception_print_recursive(PEXCEPTION_RECORD exception_record, s_file_reference file)
{
	if (!exception_record)
		return;

	file_printf(&file, "ExceptionAddress: 0x%08X\n", (uns32)exception_record->ExceptionAddress);
	file_printf(&file, "   ExceptionCode: 0x%08X\n", (uns32)exception_record->ExceptionCode);
	file_printf(&file, "  ExceptionFlags: 0x%08X, %s\n", (uns32)exception_record->ExceptionFlags, GetExceptionFlagsString(exception_record->ExceptionFlags));

	for (DWORD i = 0; i < exception_record->NumberParameters; i++)
		file_printf(&file, "\tExceptionInformation[%i]: 0x%08X\n", i, (uns32)exception_record->ExceptionInformation[i]);

	file_printf(&file, "\n");

	exception_print_recursive(exception_record->ExceptionRecord, file);
}

void __cdecl build_exception_information(_EXCEPTION_POINTERS* exception_pointers, s_exception_information* exception_information)
{
	//INVOKE(0x0051BC10, build_exception_information, exception_pointers, exception_information);

	if (exception_information)
	{
		csmemset(exception_information, 0, sizeof(s_exception_information));
		if (exception_pointers)
		{
			exception_information->thread_id = GetCurrentThreadId();
			csmemcpy(&exception_information->context_record, exception_pointers->ContextRecord, sizeof(exception_information->context_record));
			exception_information->exception_code = exception_pointers->ExceptionRecord->ExceptionCode;
			exception_information->exception_flags = exception_pointers->ExceptionRecord->ExceptionFlags;
			exception_information->exception_address = exception_pointers->ExceptionRecord->ExceptionAddress;
			if (exception_pointers->ExceptionRecord != (PEXCEPTION_RECORD)-0x14)
			{
				exception_information->number_parameters = min((sizeof(s_exception_type_info) / sizeof(ULONG_PTR)), exception_pointers->ExceptionRecord->NumberParameters);
				csmemcpy(&exception_information->exception_type_info, exception_pointers->ExceptionRecord->ExceptionInformation, (exception_information->number_parameters * sizeof(ULONG_PTR)));
			}
			exception_information->exception_occurred = true;
		}
	}
}

void __cdecl cache_exception_information(_EXCEPTION_POINTERS* exception_pointers)
{
	//INVOKE(0x0051BCC0, cache_exception_information, exception_pointers);

	if (!g_exception_caching_in_progress.set(1) && !has_cached_exception())
	{
		build_exception_information(exception_pointers, &g_exception_information);
		g_exception_information.exception_occurred = true;
		g_exception_time = system_milliseconds();
	}
}

bool __cdecl debug_get_binary_filename(c_static_string<260>* binary_filename)
{
	return INVOKE(0x0051BD40, debug_get_binary_filename, binary_filename);

	//if (binary_filename)
	//{
	//	//// halo 3
	//	//binary_filename->print(shell_get_target());
	//	//binary_filename->append("_");
	//	//binary_filename->append(shell_get_target_variant());g
	//	//binary_filename->append("_");
	//	//binary_filename->append(shell_get_configuration());
	//
	//	//// halo reach
	//	//const char* target_application = version_get_target_application();
	//	//if (!strcmp(target_application, "blam"))
	//	//	target_application = version_get_project_executable_name();
	//	//binary_filename->print(target_application);
	//	//binary_filename->append("_");
	//	//binary_filename->append(version_get_target_variant());
	//	//binary_filename->append("_");
	//	//binary_filename->append(version_get_target_configuration());
	//
	//	//// halo online
	//	//const char* target = "unknown_target";
	//	//if (game_is_halo3() || game_is_client())
	//	//	target = "halo3";
	//	//else if (game_is_sapien())
	//	//	target = "sapien";
	//	//else if (game_is_guerilla())
	//	//	target = "guerilla";
	//	//else if (game_is_tool())
	//	//	target = "tool";
	//	//else if (game_is_dedicated_server())
	//	//	target = "hf2p_dedicated_server";
	//	//binary_filename->print(target);
	//	//binary_filename->append("_");
	//	//const char* target_variant = shell_get_target_variant();
	//	//if (string_is_not_empty(target_variant))
	//	//{
	//	//	binary_filename->append(target_variant);
	//	//	binary_filename->append("_");
	//	//}
	//	//binary_filename->append(shell_get_configuration());
	//
	//	return true;
	//}
	//return false;
}

const char* __cdecl exception_code_get_string(uns32 code)
{
	return INVOKE(0x0051BE40, exception_code_get_string, code);

	//switch (code)
	//{
	//case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT";
	//case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT";
	//case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP";
	//case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";
	//case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
	//case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
	//case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND";
	//case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
	//case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT";
	//case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION";
	//case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW";
	//case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK";
	//case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW";
	//case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO";
	//case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW";
	//case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION";
	//case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION";
	//case STATUS_FLOAT_MULTIPLE_FAULTS:       return "STATUS_FLOAT_MULTIPLE_FAULTS";
	//case STATUS_FLOAT_MULTIPLE_TRAPS:        return "STATUS_FLOAT_MULTIPLE_TRAPS";
	//case 0xC06D007E:                         return "VCPPEXCEPTION_MODULE_NOT_FOUND";
	//case 0xC06D007F:                         return "VCPPEXCEPTION_PROC_NOT_FOUND";
	//}
	//return nullptr;
}

int32 __cdecl exceptions_update()
{
	//INVOKE(0x0051C020, exceptions_update);

	if (!has_cached_exception())
	{
		return 0;
	}

	main_loop_pregame_disable(true);

	uns32 code = g_exception_information.exception_code;
	const char* exception_code_string = exception_code_get_string(g_exception_information.exception_code);
	if (!g_catch_exceptions)
	{
		g_exception_information.exception_occurred = false;
		main_loop_pregame_disable(false);
		return 0;
	}

	if (code == 'stk')
	{
		if (version_is_tracked_build() || g_force_upload_even_if_untracked)
		{
			rasterizer_dump_display_to_bmp(k_screenshot_file);
			release_locks_safe_for_crash_release();
		}

		g_exception_information.exception_occurred = false;
		main_loop_pregame_disable(false);
		return -1;
	}

	c_static_string<1156> crash_info;
	const char* thread_name = get_thread_name_from_thread_id(g_exception_information.thread_id);
	int32 thread_id = g_exception_information.thread_id;

	_clearfp();
	event_logs_flush();
	release_locks_safe_for_crash_release();

	event(_event_message, "crash: ");
	event(_event_message, "crash: FATAL ERROR ENCOUNTERED");
	event(_event_message, "crash: ");
	event(_event_message, "crash: ");

	uns32 exception_address = (uns32)g_exception_information.exception_address;
	const char* symbol_name = symbol_name_from_address(exception_address, nullptr);
	event(_event_message, "crash: %s",
		version_get_full_string());

	crash_info.print("version:\r\n%s\r\n",
		version_get_full_string());

	event(_event_message, "crash:   thread information: thread_name: %s thread_id: %08lx",
		thread_name,
		thread_id);
	crash_info.append_print("thread information:\r\n thread_name: %s, thread_id: %08lx\r\n",
		thread_name,
		thread_id);

	if (code == 'asrt' && g_exception_information.number_parameters >= 4)
	{
		const char* exception_string = g_exception_information.thread_assert_arguments.statement;
		const char* file = g_exception_information.thread_assert_arguments.file;
		int32 line = g_exception_information.thread_assert_arguments.line;
		bool assertion_failed = g_exception_information.thread_assert_arguments.fatal;

		event(_event_message, "crash: %s at %s,#%d",
			assertion_failed ? "### ASSERTION FAILED: " : "### RUNTIME WARNING: ",
			file,
			line);

		crash_info.append_print("halt:\r\n%s at %s,#%d\r\n",
			assertion_failed ? "### ASSERTION FAILED: " : "### RUNTIME WARNING: ",
			file,
			line);

		if (exception_string)
		{
			event(_event_message, "crash:   %s",
				exception_string);
			crash_info.append_print("halt information:\r\n  %s\r\n",
				exception_string);
		}
	}
	else
	{
		if (exception_code_string)
		{
			event(_event_message, "crash: ### RUNTIME ERROR: %s at %08lX",
				exception_code_string,
				exception_address);
			event(_event_message, "crash:   (%s)",
				symbol_name);

			crash_info.append_print("halt:\r\n### RUNTIME ERROR: %s at %08lX\r\n",
				exception_code_string,
				exception_address);
			crash_info.append_print("halt information:\r\n  (%s)\r\n",
				symbol_name);
		}
		else
		{
			event(_event_message, "crash: ### RUNTIME ERROR: UNKNOWN EXCEPTION %08lX at %08lX",
				exception_code_string,
				exception_address);
			event(_event_message, "crash:   (%s)",
				symbol_name);

			crash_info.append_print("halt:\r\n### RUNTIME ERROR: UNKNOWN EXCEPTION %08lX at %08lX\r\n",
				code,
				exception_address);
			crash_info.append_print("halt information:\r\n  (%s)\r\n",
				symbol_name);
		}

		if (code == EXCEPTION_ACCESS_VIOLATION)
		{
			if (g_exception_information.number_parameters >= 2)
			{
				event(_event_message, "crash:   tried to %s address %08lx",
					g_exception_information.exception_type_info.exception_string ? "write" : "read",
					g_exception_information.exception_type_info.exception_parameters[0]);

				crash_info.append_print("crash:   tried to %s address %08lx\r\n",
					g_exception_information.exception_type_info.exception_string ? "write" : "read",
					g_exception_information.exception_type_info.exception_parameters[0]);
			}
		}
		else if (code == 0xC06D007E && g_exception_information.number_parameters) // VCPPEXCEPTION_MODULE_NOT_FOUND
		{
			event(_event_message, "crash:   tried to load %s",
				*((const char**)g_exception_information.exception_type_info.exception_string + 3));
			crash_info.append_print("  tried to load %s\r\n",
				*((const char**)g_exception_information.exception_type_info.exception_string + 3));
		}
	}

	main_write_stack_to_crash_info_status_file(crash_info.get_string(), &g_exception_information.context_record);
	editor_save_progress();
	//call_fatal_error_callbacks();
	if (version_is_tracked_build() || g_force_upload_even_if_untracked)
	{
		rasterizer_dump_display_to_bmp(k_screenshot_file);
		release_locks_safe_for_crash_release();
	}

	if (is_debugger_present())
		__debugbreak();
	else
		main_halt_and_catch_fire();

	int32 result = !is_debugger_present() ? 1 : -1;

	g_exception_information.exception_occurred = false;
	main_loop_pregame_disable(false);

	return result;
}

void __cdecl force_debugger_always_present(bool debugger_always_present)
{
	//INVOKE(0x0051C1E0, force_debugger_always_present, debugger_always_present);

	g_set_always_a_debugger_present = debugger_always_present;
	if (debugger_always_present)
		g_set_never_a_debugger_present = false;
}

void __cdecl force_debugger_not_present(bool debugger_not_present)
{
	//INVOKE(0x0051C210, force_debugger_not_present, debugger_not_present);

	g_set_never_a_debugger_present = debugger_not_present;
	if (debugger_not_present)
		g_set_always_a_debugger_present = false;
}

bool __cdecl is_debugger_present()
{
	if (g_set_always_a_debugger_present)
		return true;

	if (g_set_never_a_debugger_present)
		return false;

	return IsDebuggerPresent();
}

bool __cdecl has_cached_exception()
{
	//INVOKE(0x0051C300, has_cached_exception);

	return g_exception_information.exception_occurred.peek() != 0;
}

