#include "cseries/cseries_windows_debug_pc.hpp"

#include "memory/module.hpp"
#include "tag_files/files.hpp"
#include "tag_files/files_windows.hpp"

#include <Windows.h>

REFERENCE_DECLARE(0x0238E880, PEXCEPTION_POINTERS, ExceptionPointers);

HOOK_DECLARE(0x0051C020, exceptions_update);

bool g_set_always_a_debugger_present = false;
bool g_set_never_a_debugger_present = false;

bool is_debugger_present()
{
	if (g_set_always_a_debugger_present)
		return true;

	if (g_set_never_a_debugger_present)
		return false;

	return IsDebuggerPresent();
}

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

void exception_print_recursive(PEXCEPTION_RECORD ExceptionRecord, s_file_reference file)
{
	if (!ExceptionRecord)
		return;

	file_printf(&file, "ExceptionAddress: 0x%08X\n", ExceptionRecord->ExceptionAddress);
	file_printf(&file, "   ExceptionCode: 0x%08X\n", ExceptionRecord->ExceptionCode);
	file_printf(&file, "  ExceptionFlags: 0x%08X, %s\n", ExceptionRecord->ExceptionFlags, GetExceptionFlagsString(ExceptionRecord->ExceptionFlags));

	for (DWORD i = 0; i < ExceptionRecord->NumberParameters; i++)
		file_printf(&file, "\tExceptionInformation[%i]: 0x%08X\n", i, ExceptionRecord->ExceptionInformation[i]);

	file_printf(&file, "\n");

	exception_print_recursive(ExceptionRecord->ExceptionRecord, file);
}

long __cdecl exceptions_update()
{
	if (ExceptionPointers)
	{
		s_file_reference file;
		if (file_reference_create_from_path(&file, "exception.txt", false))
		{
			if (!file_exists(&file))
				file_create(&file);

			dword error = 0;
			if (file_exists(&file) && file_open(&file, FLAG(_file_open_flag_desired_access_write), &error))
			{
				exception_print_recursive(ExceptionPointers->ExceptionRecord, file);
			}

			file_close(&file);
		}
	}

	long result = 0;
	HOOK_INVOKE(result =, exceptions_update);

	return result;
}


