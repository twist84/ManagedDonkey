#pragma once

extern void __cdecl hs_debug_variable(const char* s, bool debug);
extern void __cdecl hs_evaluate(int16 script_index);
extern void __cdecl hs_log_print(const char* s);
extern void __cdecl hs_print(const char* s);
extern bool __cdecl hs_trigger_volume_test_objects(int16 trigger_volume_index, int32 object_index, bool a3);
extern bool __cdecl hs_trigger_volume_test_objects_all(int16 trigger_volume_index, int32 object_index);
extern bool __cdecl hs_trigger_volume_test_objects_any(int16 trigger_volume_index, int32 object_index);

