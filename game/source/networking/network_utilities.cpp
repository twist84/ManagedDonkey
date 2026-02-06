#include "networking/network_utilities.hpp"

#include "config/version.hpp"
#include "memory/module.hpp"
#include "shell/shell_windows.hpp"
#include "text/unicode.hpp"
#include "tools/network_debug_dump.hpp"

HOOK_DECLARE(0x0042ED50, network_get_machine_name);

//.text:0042EC70 ; 
//.text:0042EC80 ; 

int16 __cdecl _random_range(uns32* seed, const char* purpose, const char* source_file, uns32 source_line, int16 lower_bound, int16 upper_bound)
{
	return INVOKE(0x0042EC90, _random_range, seed, purpose, source_file, source_line, lower_bound, upper_bound);
}

//.text:0042ECC0 ; 
//.text:0042ECD0 ; 
//.text:0042ECE0 ; bool __cdecl network_build_is_compatible(int32, int32, int32)
//.text:0042ED10 ; 

void __cdecl network_get_build_identifiers(int32* executable_type, int32* executable_version, int32* compatible_version)
{
	INVOKE(0x0042ED20, network_get_build_identifiers, executable_type, executable_version, compatible_version);
}

bool __cdecl network_get_machine_name(wchar_t* name, int32 name_length)
{
	//return INVOKE(0x0042ED50, network_get_machine_name, machine_name, machine_name_len);

	char system_identifier[264];
	bool valid_machine_name = shell_get_system_identifier(system_identifier, 256) && system_identifier[0];
	if (valid_machine_name)
	{
		ascii_string_to_wchar_string(system_identifier, name, name_length, nullptr);
	}
	return valid_machine_name;
}

//.text:0042ED60 ; void __cdecl network_get_player_colors(e_game_team, bool, const c_enum<e_player_color_index, int8, _player_color_none, k_player_color_index_count>* const, real_rgb_color* const)
//.text:0042EE60 ; bool __cdecl network_get_random_player_name(wchar_t*, int32)
//.text:0042EF10 ; real_rgb_color __cdecl network_get_team_color(e_game_team)
//.text:0042EF60 ; bool __cdecl network_get_unique_player_name(wchar_t*, int32, int32, const wchar_t**)
//.text:0042F0A0 ; bool __cdecl network_is_random_player_name(const wchar_t*)
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
	//{
	//	csstrnzcpy(system_identifier, "unknown_system", sizeof(system_identifier));
	//}
	//netdebug_initialize(shell_get_target(), version_get_build_identifier(), system_identifier);
}

//.text:0042F1F0 ; void __cdecl network_set_machine_name(const wchar_t*)
//.text:0042F200 ; 

void bandwidth_profiler_render()
{
	// $IMPLEMENT
}

