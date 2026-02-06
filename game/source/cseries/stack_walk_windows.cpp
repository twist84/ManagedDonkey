#include "cseries/stack_walk_windows.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "tag_files/files.hpp"
#include "tag_files/files_windows.hpp"
#include "tag_files/tag_groups.hpp"

#include <windows.h>
#include <dbghelp.h>
#include <Psapi.h>

int32 const k_max_stack_depth = 256;

const char* symbol_name_from_address(uns32 routine_address, int32* out_symbol_offset)
{
	static char buffer[16384]{};
	csstrnzcpy(buffer, "<unknown>", sizeof(buffer));

	SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char));
	if (!symbol)
		return buffer;

	HANDLE process = GetCurrentProcess();
	SymInitialize(process, nullptr, TRUE);

	csmemset(symbol, 0, sizeof(SYMBOL_INFO) + 256 * sizeof(char));

	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	DWORD64 displacement;
	SymFromAddr(process, routine_address, &displacement, symbol);

	HMODULE module_handles[128]{};
	DWORD cbNeeded = 0;
	if (!EnumProcessModules(process, module_handles, sizeof(module_handles), &cbNeeded))
	{
		SymCleanup(process);
		free(symbol);

		return buffer;
	}

	for (uns32 module_index = 0; module_index < (cbNeeded / sizeof(HMODULE)); module_index++)
	{
		MODULEINFO module_info{};
		char module_filename[MAX_PATH]{};

		if (!module_handles[module_index])
			continue;

		if (!GetModuleInformation(process, module_handles[module_index], &module_info, sizeof(module_info)))
			continue;

		if (!IN_RANGE_INCLUSIVE(routine_address, (uns32)module_info.lpBaseOfDll, (uns32)module_info.lpBaseOfDll + module_info.SizeOfImage))
			continue;

		if (!GetModuleFileNameExA(process, module_handles[module_index], module_filename, sizeof(module_filename) / sizeof(char)))
			continue;

		csnzprintf(buffer, sizeof(buffer), "%s!", tag_name_strip_path(module_filename));

		break;
	}

	if (*symbol->Name)
	{
		csnzappendf(buffer, sizeof(buffer), "%s", symbol->Name);
		if (displacement > 5)
			csnzappendf(buffer, sizeof(buffer), "+0x%X", displacement - 5 /* take off size of call instruction */);
	}
	else
	{
		csnzappendf(buffer, sizeof(buffer), "%08X()", routine_address - 5 /* take off size of call instruction */);
	}

	SymCleanup(process);
	free(symbol);

	if (out_symbol_offset)
		*out_symbol_offset = 0;

	return buffer;
}

void walk_stack(uns32* routine_addresses, uns32 number_of_levels, int16 levels_to_ignore, int32* levels_dumped)
{
	levels_to_ignore++;

	ASSERT(routine_addresses);
	ASSERT(levels_dumped);
	ASSERT(number_of_levels <= k_max_stack_depth);

	void* stack[k_max_stack_depth]{};

	int32 stack_trace_result = static_cast<int32>(CaptureStackBackTrace(static_cast<DWORD>(levels_to_ignore), k_max_stack_depth, stack, nullptr));
	ASSERT(SUCCEEDED(stack_trace_result));

	csmemcpy(routine_addresses, stack, sizeof(uns32) * number_of_levels);
	*levels_dumped = stack_trace_result;
}

void stack_walk_with_context_internal(const s_file_reference* file, int16 levels_to_ignore, _CONTEXT* context, int32 number_of_levels, uns32* routine_addresses, int32* out_levels_dumped)
{
	levels_to_ignore++;

	int32 levels_dumped = 0;
	walk_stack(routine_addresses, number_of_levels, levels_to_ignore, &levels_dumped);

	if (out_levels_dumped)
		*out_levels_dumped = levels_dumped;
}

void stack_walk_print(s_file_reference* file, _CONTEXT* context, int32 levels_dumped, const uns32* routine_addresses)
{
	for (int32 i = levels_dumped - 1; i >= 0; i--)
	{
		uns32 routine_address = routine_addresses[i];

		int32 symbol_offset = 0;
		const char* symbol_name = symbol_name_from_address(routine_address, &symbol_offset);

		if (file)
		{
			c_static_string<1024> stack_string;
			stack_string.print("%08lX %s\r\n", routine_address - 5 /* take off size of call instruction */, symbol_name);
			file_write(file, stack_string.length(), stack_string.get_string());
		}
		else
		{
			event(_event_message, "crash: %08lX %s", routine_address - 5 /* take off size of call instruction */, symbol_name);
		}
	}
}

void stack_walk_with_context_common(s_file_reference* file, int16 levels_to_ignore, _CONTEXT* context)
{
	levels_to_ignore++;

	uns32 routine_addresses[64]{};
	int32 levels_dumped = 0;
	stack_walk_with_context_internal(file, levels_to_ignore, context, NUMBEROF(routine_addresses), routine_addresses, &levels_dumped);
	stack_walk_print(file, context, levels_dumped, routine_addresses);
}

void stack_walk_with_context(s_file_reference* file, int16 levels_to_ignore, _CONTEXT* context)
{
	levels_to_ignore++;

	stack_walk_with_context_common(file, levels_to_ignore, context);
}

void stack_walk_to_file(s_file_reference* file, int16 levels_to_ignore)
{
	levels_to_ignore++;

	stack_walk_with_context_common(file, levels_to_ignore, nullptr);
}

void stack_walk(int16 levels_to_ignore)
{
	levels_to_ignore++;

	stack_walk_with_context_common(nullptr, levels_to_ignore, nullptr);
}

