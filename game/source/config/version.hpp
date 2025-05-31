#pragma once

extern bool& k_tracked_build;

extern const char* __cdecl version_get_target_variant();
extern const char* __cdecl version_get_full_string();
extern const char* __cdecl version_get_project_name();
extern const char* __cdecl version_get_build_identifier();
extern const char* __cdecl version_get_build_name();
extern int32 __cdecl version_get_build_number();
extern int64 __cdecl version_get_build_number_identifier();
extern int32 __cdecl version_get_build_number_sequence_identifier();
extern const char* __cdecl version_get_build_string();
extern const char* __cdecl version_get_target_configuration();

extern bool version_is_tracked_build();