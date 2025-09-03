#include "config/version.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

#include "config/version_git_commit.hpp"

//#define ORIGINAL_PROJECT_VERSION
#if defined(ORIGINAL_PROJECT_VERSION)
#define VERSION_BUILD_STRING "1.106708 cert_ms23"
#define VERSION_BUILD_NUMBER 106708
#define VERSION_BUILD_DATE "Mar 20 2015"
#define VERSION_BUILD_TIME "17:24:42"
#define VERSION_TARGET_APPLICATION "blam"
#define VERSION_TARGET_VARIANT "cache"
#define VERSION_TARGET_CONFIGURATION "release"
#define VERSION_TARGET_PLATFORM "pc"
#define VERSION_BUILD_NUMBER_VERSION 1
#define VERSION_PROJECT_NAME "cert_ms23"
#define VERSION_BUILD_NAME ""
#define VERSION_PROJECT_VERSION "1.106708"
#define VERSION_FULL_STRING "blam cache release pc 1.106708 cert_ms23  Mar 20 2015 17:24:42"
#define VERSION_TRACKED_STRING "1.106708_cert_ms23___release"
#define VERSION_UNTRACKED_STRING "untracked_cert_ms23__release"
#endif

#if !defined(VERSION_BUILD_DATE)
#define VERSION_BUILD_DATE __DATE__
#endif

#if !defined(VERSION_BUILD_TIME)
#define VERSION_BUILD_TIME __TIME__
#endif

#if !defined(VERSION_TARGET_APPLICATION)
#define VERSION_TARGET_APPLICATION "donkey"
#endif

#if !defined(VERSION_TARGET_VARIANT)
#define VERSION_TARGET_VARIANT "cache"
#endif

#if !defined(VERSION_TARGET_CONFIGURATION)
#if defined(_DEBUG)
#define VERSION_TARGET_CONFIGURATION "debug"
#else
#define VERSION_TARGET_CONFIGURATION "release"
#endif
#endif

#if !defined(VERSION_TARGET_PLATFORM)
#define VERSION_TARGET_PLATFORM "pc"
#endif

#if !defined(VERSION_BUILD_NUMBER_VERSION)
#define VERSION_BUILD_NUMBER_VERSION 1
#endif

#if !defined(VERSION_BUILD_NUMBER)
#define VERSION_BUILD_NUMBER 106708
#endif

#if !defined(VERSION_PROJECT_NAME)
#define VERSION_PROJECT_NAME "cert_ms23"
#endif

#if !defined(VERSION_PROJECT_VERSION)
#define VERSION_PROJECT_VERSION STRINGIFY(VERSION_BUILD_NUMBER_VERSION) "." STRINGIFY(VERSION_BUILD_NUMBER)
#endif

#if !defined(VERSION_BUILD_STRING)
#define VERSION_BUILD_STRING VERSION_PROJECT_VERSION " " VERSION_PROJECT_NAME
#endif

#if !defined(VERSION_BUILD_NAME)
#define VERSION_BUILD_NAME ""
#endif

#if defined(VERSION_GIT_COMMIT)
#define VERSION_BUILD_COMMIT STRINGIFY(VERSION_GIT_COMMIT)
#else
#define VERSION_BUILD_COMMIT ""
#endif

#if !defined(VERSION_FULL_STRING)
#define VERSION_FULL_STRING VERSION_TARGET_APPLICATION " " VERSION_TARGET_VARIANT " " VERSION_TARGET_CONFIGURATION " " VERSION_TARGET_PLATFORM " " VERSION_BUILD_STRING " " VERSION_BUILD_NAME " " VERSION_BUILD_DATE " " VERSION_BUILD_TIME " " VERSION_BUILD_COMMIT
#endif

#if !defined(VERSION_TRACKED_STRING)
#define VERSION_TRACKED_STRING VERSION_BUILD_STRING "_" VERSION_BUILD_NAME "__" VERSION_TARGET_CONFIGURATION
#endif

#if !defined(VERSION_UNTRACKED_STRING)
#define VERSION_UNTRACKED_STRING "untracked_" VERSION_PROJECT_NAME "__" VERSION_TARGET_CONFIGURATION
#endif

char const k_version_target_application[] = VERSION_TARGET_APPLICATION;
char const k_version_target_variant[] = VERSION_TARGET_VARIANT;
char const k_version_target_configuration[] = VERSION_TARGET_CONFIGURATION;
char const k_version_target_platform[] = VERSION_TARGET_PLATFORM;

int32 const k_version_build_number_version = VERSION_BUILD_NUMBER_VERSION;
int32 const k_version_build_number = VERSION_BUILD_NUMBER;

char const k_version_project_name[] = VERSION_PROJECT_NAME;
char const k_version_project_version[] = VERSION_PROJECT_VERSION;

char const k_version_build_string[] = VERSION_BUILD_STRING;
char const k_version_build_name[] = VERSION_BUILD_NAME;
char const k_version_full_string[] = VERSION_FULL_STRING;

char const k_version_tracked_string[] = VERSION_TRACKED_STRING;
char const k_version_untracked_string[] = VERSION_UNTRACKED_STRING;

REFERENCE_DECLARE(0x0189CFD0, bool, k_tracked_build) = true;

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

byte const k_build_date[] = VERSION_BUILD_DATE;
byte const k_build_time[] = VERSION_BUILD_TIME;
DATA_PATCH_DECLARE(0x016704A8, build_date, k_build_date);
DATA_PATCH_DECLARE(0x016704B4, build_time, k_build_time);

const char* __cdecl version_get_target_variant()
{
	//return "cache";
	return k_version_target_variant;
}

const char* __cdecl version_get_full_string()
{
	//return "blam cache release pc 1.106708 cert_ms23  Mar 20 2015 17:24:42";
	return k_version_full_string;
}

const char* __cdecl version_get_project_name()
{
	//return "cert_ms23";
	return k_version_project_name;
}

const char* __cdecl version_get_build_identifier()
{
	//return version_is_tracked_build() ? "1.106708_cert_ms23___release" : "untracked_cert_ms23__release";
	return version_is_tracked_build() ? k_version_tracked_string : k_version_untracked_string;
}

const char* __cdecl version_get_build_name()
{
	//return "";
	return k_version_build_name;
}

int32 __cdecl version_get_build_number()
{
	//return 106708;
	return k_version_build_number;
}

int64 __cdecl version_get_build_number_identifier()
{
	return make_int64(version_get_build_number(), version_get_build_number_sequence_identifier());
}

int32 __cdecl version_get_build_number_sequence_identifier()
{
	return 2;
}

const char* __cdecl version_get_build_string()
{
	//return "1.106708 cert_ms23";
	return k_version_build_string;
}

const char* __cdecl version_get_target_configuration()
{
	//return "release";
	return k_version_target_configuration;
}

bool version_is_tracked_build()
{
	return k_tracked_build;
}

