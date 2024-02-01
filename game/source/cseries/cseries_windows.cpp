#include "cseries/cseries_windows.hpp"

#include "cseries/cseries.hpp"
#include "main/main.hpp"

#include <windows.h>
#include <time.h>

void __cdecl display_debug_string(const char* format, ...)
{
	va_list list;
	va_start(list, format);

	if (IsDebuggerPresent())
	{
		c_static_string<4096> output = {};
		output.append_vprint(format, list);
		output.append("\n");

		OutputDebugStringA(output.get_string());
	}

	va_end(list);
}

void __cdecl system_get_date_and_time(char* buffer, short buffer_size, bool short_date_and_time)
{
	INVOKE(0x004EBF70, system_get_date_and_time, buffer, buffer_size, short_date_and_time);
}

unsigned long __cdecl system_milliseconds()
{
	//return INVOKE(0x004EC110, system_milliseconds);

	return timeGetTime();
}

unsigned long __cdecl system_seconds()
{
	//return INVOKE(0x004EC260, system_seconds);

	return static_cast<unsigned long>(_time64(0));
}

void __cdecl system_abort()
{
	main_halt_and_catch_fire();
}

// __trap()
void __cdecl system_exit()
{
	exit(1);
}

void __cdecl system_set_dll_directory()
{
	//INVOKE(0x0051CF30, system_set_dll_directory);

	char dll_directory[1024]{};
	csstrnzcat(dll_directory, ".\\bin", 1024);
	SetDllDirectoryA(dll_directory);

	//generate_event(_event_level_message, "system: dll directory={ %s }", dll_directory);
	printf("system: dll directory={ %s }\n", dll_directory);
}

