#include "networking/network_utilities.hpp"

#include "config/version.hpp"
#include "memory/module.hpp"
#include "shell/shell_windows.hpp"
#include "text/unicode.hpp"
#include "tools/network_debug_dump.hpp"

HOOK_DECLARE(0x0042ED50, network_get_machine_name);

//.text:0042EC70 ; 
//.text:0042EC80 ; 

short __cdecl _random_range(uint32* seed_ptr, char const* name, char const* filename, uint32 line, short range_begin, short range_end)
{
	return INVOKE(0x0042EC90, _random_range, seed_ptr, name, filename, line, range_begin, range_end);
}

//.text:0042ECC0 ; 
//.text:0042ECD0 ; 
//.text:0042ECE0 ; bool __cdecl network_build_is_compatible(long, long, long)
//.text:0042ED10 ; 
//.text:0042ED20 ; void __cdecl network_get_build_identifiers(long*, long*, long*)

bool __cdecl network_get_machine_name(wchar_t* machine_name, long machine_name_len)
{
	//return INVOKE(0x0042ED50, network_get_machine_name, machine_name, machine_name_len);

	char system_identifier[264];
	if (!shell_get_system_identifier(system_identifier, 256) || !system_identifier[0])
		return false;

	ascii_string_to_wchar_string(system_identifier, machine_name, machine_name_len, nullptr);
	return true;
}

//.text:0042ED60 ; void __cdecl network_get_player_colors(e_game_team, bool, c_enum<e_player_color_index, char, -1, 32> const* const, real_rgb_color* const)
//.text:0042EE60 ; bool __cdecl network_get_random_player_name(wchar_t*, long)
//.text:0042EF10 ; real_rgb_color __cdecl network_get_team_color(e_game_team)
//.text:0042EF60 ; bool __cdecl network_get_unique_player_name(wchar_t*, long, long, wchar_t const**)
//.text:0042F0A0 ; bool __cdecl network_is_random_player_name(wchar_t const*)
//.text:0042F160 ; bool __cdecl network_joining_should_be_disabled()

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

//.text:0042F1F0 ; void __cdecl network_set_machine_name(wchar_t const*)
//.text:0042F200 ; 

void bandwidth_profiler_render()
{
	// $TODO: implement
}

