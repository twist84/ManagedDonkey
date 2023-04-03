#include "cseries/cseries_windows_debug_pc.hpp"

#include <Windows.h>

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

