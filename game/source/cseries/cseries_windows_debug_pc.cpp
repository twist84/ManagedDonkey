#include "cseries/cseries_windows_debug_pc.hpp"

#include "memory/module.hpp"
#include "tag_files/files.hpp"
#include "tag_files/files_windows.hpp"
#include "tag_files/tag_groups.hpp"

#include <windows.h>
#include <dbghelp.h>
#include <Psapi.h>

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

	file_printf(&file, "ExceptionAddress: 0x%08X\n", (dword)ExceptionRecord->ExceptionAddress);
	file_printf(&file, "   ExceptionCode: 0x%08X\n", (dword)ExceptionRecord->ExceptionCode);
	file_printf(&file, "  ExceptionFlags: 0x%08X, %s\n", (dword)ExceptionRecord->ExceptionFlags, GetExceptionFlagsString(ExceptionRecord->ExceptionFlags));

	for (DWORD i = 0; i < ExceptionRecord->NumberParameters; i++)
		file_printf(&file, "\tExceptionInformation[%i]: 0x%08X\n", i, (dword)ExceptionRecord->ExceptionInformation[i]);

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

		if (ExceptionPointers->ExceptionRecord->ExceptionFlags == EXCEPTION_NONCONTINUABLE)
			return 0;
	}

	long result = 0;
	HOOK_INVOKE(result =, exceptions_update);

	return result;
}

void debug_stack_print(void(*console_write_line)(wchar_t const* format, ...), bool include_offset)
{
	c_static_wchar_string<4096> trace;
	trace.append_line(L"Stack Trace:");

	LPVOID stack_frames[128];
	WORD frame_count = CaptureStackBackTrace(1, NUMBEROF(stack_frames), stack_frames, NULL);

	SYMBOL_INFOW* symbol = (SYMBOL_INFOW*)malloc(sizeof(SYMBOL_INFOW) + 256 * sizeof(wchar_t));
	if (!symbol)
		return;

	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFOW);

	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);

	for (WORD i = 0; i < frame_count; i++)
	{
		trace.append_print(L"[%d]\t", i);

		DWORD64 displacement;
		SymFromAddrW(process, (DWORD64)(stack_frames[i]), &displacement, symbol);

		HMODULE module_handles[128]{};
		DWORD cbNeeded = 0;
		if (!EnumProcessModules(process, module_handles, sizeof(module_handles), &cbNeeded))
			continue;

		for (dword module_index = 0; module_index < (cbNeeded / sizeof(HMODULE)); module_index++)
		{
			MODULEINFO module_info{};
			wchar_t module_filename[MAX_PATH]{};

			if (!module_handles[module_index])
				goto module_loop_end;

			if (!GetModuleInformation(process, module_handles[module_index], &module_info, sizeof(module_info)))
				goto module_loop_end;

			if (!IN_RANGE_INCLUSIVE(symbol->Address, (ULONG64)module_info.lpBaseOfDll, (ULONG64)module_info.lpBaseOfDll + module_info.SizeOfImage))
				goto module_loop_end;

			if (!GetModuleFileNameEx(process, module_handles[module_index], module_filename, sizeof(module_filename) / sizeof(wchar_t)))
				goto module_loop_end;

			trace.append_print(L"%s!", tag_name_strip_path(module_filename));

		module_loop_end:;
		}

		trace.append_print(L"%s", symbol->Name);
		if (include_offset)
		{
			trace.append_print(L"+0x%X", displacement - 5 /* take off size of call instruction */);
		}
		trace.append_line();
	}

	SymCleanup(process);
	free(symbol);

	if (console_write_line)
		console_write_line(trace.get_string());
}


