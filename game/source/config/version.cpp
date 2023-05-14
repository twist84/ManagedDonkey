#include "config/version.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_console.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x0189CFD0, bool, k_tracked_build);

HOOK_DECLARE(0x0042E480, version_get_target_variant);
HOOK_DECLARE(0x0042E490, version_get_full_string);
HOOK_DECLARE(0x005013E0, version_get_project_name);
HOOK_DECLARE(0x005013F0, version_get_build_identifier);
HOOK_DECLARE(0x00501410, version_get_build_name);
HOOK_DECLARE(0x00501420, version_get_build_number);
HOOK_DECLARE(0x00501430, version_get_build_number_identifier);
HOOK_DECLARE(0x00501450, version_get_build_number_sequence_identifier);
HOOK_DECLARE(0x00501460, version_get_build_string);
HOOK_DECLARE(0x00501470, version_get_target_configuration);

byte const k_build_date[] = __DATE__;
byte const k_build_time[] = __TIME__;
DATA_PATCH_DECLARE(0x016704A8, build_date, k_build_date);
DATA_PATCH_DECLARE(0x016704B4, build_time, k_build_time);

char const* __cdecl version_get_target_variant()
{
	return "cache";
}

char const* __cdecl version_get_full_string()
{
	return "blam cache release pc 1.106708 cert_ms23  Mar 20 2015 17:24:42";
}

char const* __cdecl version_get_project_name()
{
	return "cert_ms23";
}

char const* __cdecl version_get_build_identifier()
{
	if (version_is_tracked_build())
		return "1.106708_cert_ms23___release";

	return "untracked_cert_ms23__release";
}

char const* __cdecl version_get_build_name()
{
	return "";
}

long __cdecl version_get_build_number()
{
	return 106708;
}

__int64 __cdecl version_get_build_number_identifier()
{
	long build_number = version_get_build_number();
	long build_number_sequence_identifier = version_get_build_number_sequence_identifier();

	return make_int64(build_number, build_number_sequence_identifier);
}

long __cdecl version_get_build_number_sequence_identifier()
{
	return 2;
}

char const* __cdecl version_get_build_string()
{
	return "1.106708 cert_ms23";
}

char const* __cdecl version_get_target_configuration()
{
	return "release";
}

bool version_is_tracked_build()
{
	return k_tracked_build;
}