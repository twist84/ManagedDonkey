#include "networking/network_utilities.hpp"

#include "config/version.hpp"
#include "memory/module.hpp"
#include "shell/shell_windows.hpp"
#include "text/unicode.hpp"
#include "tools/network_debug_dump.hpp"

HOOK_DECLARE(0x0042ED50, network_get_machine_name);

short __cdecl _random_range(dword* seed_ptr, char const* name, char const* filename, dword line, short range_begin, short range_end)
{
	return INVOKE(0x0042EC90, _random_range, seed_ptr, name, filename, line, range_begin, range_end);
}

bool __cdecl network_get_machine_name(wchar_t* machine_name, long machine_name_len)
{
	//return INVOKE(0x0042ED50, network_get_machine_name, machine_name, machine_name_len);

	char system_identifier[264];
	if (!shell_get_system_identifier(system_identifier, 256) || !system_identifier[0])
		return false;

	ascii_string_to_wchar_string(system_identifier, machine_name, machine_name_len, nullptr);
	return true;
}

void __cdecl network_remote_reporting_dispose()
{
	INVOKE(0x0042F170, network_remote_reporting_dispose);

	//netdebug_dispose();
}

void __cdecl network_remote_reporting_initialize()
{
	INVOKE(0x0042F180, network_remote_reporting_initialize);

	//char system_identifier[160]{};
	//if (!shell_get_system_identifier(system_identifier, sizeof(system_identifier)))
	//	csstrnzcpy(system_identifier, "unknown_system", sizeof(system_identifier));
	//
	//netdebug_initialize(shell_get_target(), version_get_build_identifier(), system_identifier);
}

void bandwidth_profiler_render()
{
	// #TODO: implement
}

