#pragma once

extern void __cdecl hs_debug_variable(char const* s, bool debug);
extern void __cdecl hs_log_print(char const* s);
extern void __cdecl hs_print(char const* s);
extern bool __cdecl hs_trigger_volume_test_objects(short trigger_volume_index, long object_index, bool a3);
extern bool __cdecl hs_trigger_volume_test_objects_all(short trigger_volume_index, long object_index);
extern bool __cdecl hs_trigger_volume_test_objects_any(short trigger_volume_index, long object_index);

