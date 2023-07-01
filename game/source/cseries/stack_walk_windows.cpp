#include "cseries/stack_walk_windows.hpp"

#include "cseries/cseries.hpp"
#include "tag_files/files.hpp"
#include "tag_files/files_windows.hpp"
#include "tag_files/tag_groups.hpp"

#include <windows.h>
#include <dbghelp.h>
#include <Psapi.h>

// the following implementation is wrong but this is fine

void stack_walk(s_file_reference* file, short levels_to_ignore, CONTEXT* context)
{
	c_static_string<4096> trace;

	LPVOID stack_frames[128]{};
	WORD frame_count = CaptureStackBackTrace(levels_to_ignore, NUMBEROF(stack_frames), stack_frames, NULL);

	SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256 * sizeof(wchar_t));
	if (!symbol)
		return;

	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);

	for (WORD i = 0; i < frame_count; i++)
	{
		csmemset(symbol, 0, sizeof(SYMBOL_INFO) + 256 * sizeof(wchar_t));

		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		DWORD64 displacement;
		SymFromAddr(process, (DWORD64)(stack_frames[i]), &displacement, symbol);

		HMODULE module_handles[128]{};
		DWORD cbNeeded = 0;
		if (!EnumProcessModules(process, module_handles, sizeof(module_handles), &cbNeeded))
			continue;

		trace.append_print("    ", i);

		for (dword module_index = 0; module_index < (cbNeeded / sizeof(HMODULE)); module_index++)
		{
			MODULEINFO module_info{};
			char module_filename[MAX_PATH]{};

			if (!module_handles[module_index])
				goto module_loop_end;

			if (!GetModuleInformation(process, module_handles[module_index], &module_info, sizeof(module_info)))
				goto module_loop_end;

			if (!IN_RANGE_INCLUSIVE((ULONG64)stack_frames[i], (ULONG64)module_info.lpBaseOfDll, (ULONG64)module_info.lpBaseOfDll + module_info.SizeOfImage))
				goto module_loop_end;

			if (!GetModuleFileNameExA(process, module_handles[module_index], module_filename, sizeof(module_filename) / sizeof(wchar_t)))
				goto module_loop_end;

			trace.append_print("%s!", tag_name_strip_path(module_filename));

		module_loop_end:;
		}

		if (*symbol->Name)
		{
			trace.append_print("%s", symbol->Name);
			if (displacement > 5)
				trace.append_print("+0x%X", displacement - 5 /* take off size of call instruction */);
		}
		else
		{
			trace.append_print("%08X()", (DWORD64)(stack_frames[i]));
		}

		trace.append_line();
	}

	SymCleanup(process);
	free(symbol);

	file_write(file, trace.length(), trace.get_string());
}

