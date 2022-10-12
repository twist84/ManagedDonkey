#pragma once

extern bool& k_tracked_build;

extern char const* __cdecl version_get_target_variant();
extern char const* __cdecl version_get_full_string();
extern char const* __cdecl version_get_project_name();
extern char const* __cdecl version_get_build_identifier();
extern char const* __cdecl version_get_build_name();
extern long __cdecl version_get_build_number();
extern __int64 __cdecl version_get_build_number_identifier();
extern long __cdecl version_get_build_number_sequence_identifier();
extern char const* __cdecl version_get_build_string();
extern char const* __cdecl version_get_target_configuration();

extern bool version_is_tracked_build();