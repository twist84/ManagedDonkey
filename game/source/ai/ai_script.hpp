#pragma once

typedef struct scenario s_scenario;

extern bool ai_print_scripting;

extern bool __cdecl ai_handle_script_verification(bool suppress_errors);
extern bool __cdecl ai_index_from_string(s_scenario* scenario, const char* ai_string, int32* ai_index_reference);
extern void __cdecl ai_script_dispose();
extern void __cdecl ai_script_dispose_from_old_map();
extern void __cdecl ai_script_initialize();
extern void __cdecl ai_script_initialize_for_new_map();
extern void __cdecl ai_scripting_allegiance(int16 team1_index, int16 team2_index);
extern void __cdecl ai_scripting_erase_all();
extern int32 __cdecl object_list_from_ai_reference(int32 ai_index);

