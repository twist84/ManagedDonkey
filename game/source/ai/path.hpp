#pragma once

#include "cseries/cseries.hpp"

struct path_state;

extern bool __cdecl path_state_find(path_state* state);
extern void __cdecl paths_dispose();
extern void __cdecl paths_dispose_from_old_map();
extern void __cdecl paths_initialize();
extern void __cdecl paths_initialize_for_new_map();

