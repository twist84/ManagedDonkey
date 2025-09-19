#pragma once

#include "cseries/cseries.hpp"

extern bool ai_print_scripting;

extern void __cdecl ai_script_dispose();
extern void __cdecl ai_script_dispose_from_old_map();
extern void __cdecl ai_script_initialize();
extern void __cdecl ai_script_initialize_for_new_map();
extern void __cdecl ai_scripting_allegiance(int16 team1_index, int16 team2_index);
extern void __cdecl ai_scripting_erase_all();

extern bool ai_index_from_string(struct scenario* scenario, const char* ai_string, int32* ai_index_reference);

