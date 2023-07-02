#include "cseries/cseries_windows_debug_pc.hpp"

#include "config/version.hpp"
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

#include <windows.h>
#include <dbghelp.h>
#include <Psapi.h>

struct s_exception_type_info
{
	char const* exception_string;
	dword exception_parameters[3];
};
static_assert(sizeof(s_exception_type_info) == 0x10);

struct s_exception_information
{
	c_synchronized_long exception_occurred;
	long thread_id;
	CONTEXT context_record;
	dword exception_code;
	dword exception_flags;
	dword exception_address;
	dword number_parameters;
	s_exception_type_info exception_type_info;
};
static_assert(sizeof(s_exception_information) == 0x2F4);

REFERENCE_DECLARE(0x0238E878, bool, g_set_always_a_debugger_present);
REFERENCE_DECLARE(0x0238E879, bool, g_set_never_a_debugger_present);
REFERENCE_DECLARE(0x0238E87C, dword, g_exception_time);
REFERENCE_DECLARE(0x0238E880, PEXCEPTION_POINTERS, g_exception_pointers);
REFERENCE_DECLARE(0x0238E884, c_synchronized_long, g_exception_caching_in_progress);
REFERENCE_DECLARE(0x0238E888, s_exception_information, g_exception_information);

HOOK_DECLARE(0x0051C020, exceptions_update);

char const* GetExceptionFlagsString(DWORD exception)
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

char const* GetExceptionString(dword code)
{
	char const* exception_code_string = nullptr;

	if (code > 0xC0000005)
	{
		if (code > 0xC00002B4)
		{
			switch (code)
			{
			case 0xC00002B5:
				exception_code_string = "STATUS_FLOAT_MULTIPLE_TRAPS";
				break;
			case 0xC06D007E:
				exception_code_string = "VCPPEXCEPTION_MODULE_NOT_FOUND";
				break;
			case 0xC06D007F:
				exception_code_string = "VCPPEXCEPTION_PROC_NOT_FOUND";
				break;
			}
		}
		else if (code == 0xC00002B4)
		{
			exception_code_string = "STATUS_FLOAT_MULTIPLE_FAULTS";
		}
		else
		{
			switch (code)
			{
			case 0xC000001D:
				exception_code_string = "EXCEPTION_ILLEGAL_INSTRUCTION";
				break;
			case 0xC0000025:
				exception_code_string = "EXCEPTION_NONCONTINUABLE_EXCEPTION";
				break;
			case 0xC000008C:
				exception_code_string = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
				break;
			case 0xC000008D:
				exception_code_string = "EXCEPTION_FLT_DENORMAL_OPERAND";
				break;
			case 0xC000008E:
				exception_code_string = "EXCEPTION_FLT_DIVIDE_BY_ZERO";
				break;
			case 0xC000008F:
				exception_code_string = "EXCEPTION_FLT_INEXACT_RESULT";
				break;
			case 0xC0000090:
				exception_code_string = "EXCEPTION_FLT_INVALID_OPERATION";
				break;
			case 0xC0000091:
				exception_code_string = "EXCEPTION_FLT_OVERFLOW";
				break;
			case 0xC0000092:
				exception_code_string = "EXCEPTION_FLT_STACK_CHECK";
				break;
			case 0xC0000093:
				exception_code_string = "EXCEPTION_FLT_UNDERFLOW";
				break;
			case 0xC0000094:
				exception_code_string = "EXCEPTION_INT_DIVIDE_BY_ZERO";
				break;
			case 0xC0000095:
				exception_code_string = "EXCEPTION_INT_OVERFLOW";
				break;
			case 0xC0000096:
				exception_code_string = "EXCEPTION_PRIV_INSTRUCTION";
				break;
			default:
				break;
			}
		}
	}
	else
	{
		switch (code)
		{
		case 0xC0000005:
			exception_code_string = "EXCEPTION_ACCESS_VIOLATION";
			break;
		case 0x80000002:
			exception_code_string = "EXCEPTION_DATATYPE_MISALIGNMENT";
			break;
		case 0x80000003:
			exception_code_string = "EXCEPTION_BREAKPOINT";
			break;
		case 0x80000004:
			exception_code_string = "EXCEPTION_SINGLE_STEP";
			break;
		}
	}

	return exception_code_string;
}

void exception_print_recursive(PEXCEPTION_RECORD exception_record, s_file_reference file)
{
	if (!exception_record)
		return;

	file_printf(&file, "ExceptionAddress: 0x%08X\n", (dword)exception_record->ExceptionAddress);
	file_printf(&file, "   ExceptionCode: 0x%08X\n", (dword)exception_record->ExceptionCode);
	file_printf(&file, "  ExceptionFlags: 0x%08X, %s\n", (dword)exception_record->ExceptionFlags, GetExceptionFlagsString(exception_record->ExceptionFlags));

	for (DWORD i = 0; i < exception_record->NumberParameters; i++)
		file_printf(&file, "\tExceptionInformation[%i]: 0x%08X\n", i, (dword)exception_record->ExceptionInformation[i]);

	file_printf(&file, "\n");

	exception_print_recursive(exception_record->ExceptionRecord, file);
}

long __cdecl exceptions_update()
{
	if (!has_cached_exception())
		return 0;

	main_loop_pregame_disable(true);

	dword code = g_exception_information.exception_code;
	char const* exception_code_string = GetExceptionString(g_exception_information.exception_code);
	if (!g_catch_exceptions)
	{
		g_exception_information.exception_occurred = 0;
		main_loop_pregame_disable(false);
		return 0;
	}

	if (code == 'stk')
	{
		if (version_is_tracked_build() || g_force_upload_even_if_untracked)
		{
			rasterizer_dump_display_to_bmp("crash_report\\crash_screenshot.bmp");
			release_locks_safe_for_crash_release();
		}

		g_exception_information.exception_occurred = 0;
		main_loop_pregame_disable(false);
		return -1;
	}

	c_static_string<1156> crash_info;
	char const* thread_name = get_thread_name_from_thread_id(g_exception_information.thread_id);
	long thread_id = g_exception_information.thread_id;

	_clearfp();
	//event_logs_flush();
	release_locks_safe_for_crash_release();

	c_console::write_line("crash: ");
	c_console::write_line("crash: FATAL ERROR ENCOUNTERED");
	c_console::write_line("crash: ");

	dword exception_address = g_exception_information.exception_address;
	char const* symbol_name = symbol_name_from_address(g_exception_information.exception_address, nullptr);

	c_console::write_line("crash: ");

	c_console::write_line("crash: %s",
		version_get_full_string());
	crash_info.print("version:\r\n%s\r\n",
		version_get_full_string());

	c_console::write_line("crash:   thread information: thread_name: %s thread_id: %08lx",
		thread_name,
		thread_id);

	if (code == 'asrt' && g_exception_information.number_parameters >= 4)
	{
		char const* exception_string = g_exception_information.exception_type_info.exception_string;
		char const* parameter0 = (char const*)g_exception_information.exception_type_info.exception_parameters[0];
		dword parameter1 = g_exception_information.exception_type_info.exception_parameters[1];
		bool parameter2 = !!g_exception_information.exception_type_info.exception_parameters[2];

		c_console::write_line("crash: %s at %s,#%d",
			parameter2 ? "### ASSERTION FAILED: " : "### RUNTIME WARNING: ",
			parameter0,
			parameter1);

		crash_info.append_print("halt:\r\n%s at %s,#%d\r\n",
			parameter2 ? "### ASSERTION FAILED: " : "### RUNTIME WARNING: ",
			parameter0,
			parameter1);

		if (exception_string)
		{
			c_console::write_line("crash:   %s", exception_string);

			crash_info.append_print("halt information:\r\n  %s\r\n", exception_string);
		}
	}
	else
	{
		if (exception_code_string)
		{
			c_console::write_line("crash: ### RUNTIME ERROR: %s at %08lX",
				exception_code_string,
				exception_address);
			c_console::write_line("crash:   (%s)", symbol_name);

			crash_info.append_print("halt:\r\n### RUNTIME ERROR: %s at %08lX\r\n",
				exception_code_string,
				exception_address);
			crash_info.append_print("halt information:\r\n  (%s)\r\n", symbol_name);
		}
		else
		{
			if (code == EXCEPTION_ACCESS_VIOLATION)
			{
				if (g_exception_information.number_parameters >= 2)
				{
					c_console::write_line("crash:   tried to %s address %08lx",
						g_exception_information.exception_type_info.exception_string ? "write" : "read",
						g_exception_information.exception_type_info.exception_parameters[0]);

					crash_info.append_print("crash:   tried to %s address %08lx\r\n",
						g_exception_information.exception_type_info.exception_string ? "write" : "read",
						g_exception_information.exception_type_info.exception_parameters[0]);
				}
			}
			else if (code == 0xC06D007E && g_exception_information.number_parameters)
			{
				crash_info.append_print("%s - tried to load %s",
					exception_code_string,
					*((char const**)g_exception_information.exception_type_info.exception_string + 3));
			}
		}
	}

	main_write_stack_to_crash_info_status_file(crash_info, &g_exception_information.context_record);
	editor_save_progress();
	//call_fatal_error_callbacks();
	if (version_is_tracked_build() || g_force_upload_even_if_untracked)
	{
		rasterizer_dump_display_to_bmp("crash_report\\crash_screenshot.bmp");
		release_locks_safe_for_crash_release();
	}

	main_halt_and_catch_fire();

	g_exception_information.exception_occurred = 0;
	main_loop_pregame_disable(false);
	return !is_debugger_present() ? 1 : -1;

	//long result = 0;
	//HOOK_INVOKE(result =, exceptions_update);
	//return result;
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

