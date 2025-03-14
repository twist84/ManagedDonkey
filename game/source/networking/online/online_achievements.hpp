#pragma once

#include "cseries/cseries.hpp"

enum e_controller_index;
extern void __cdecl achievements_dispose();
extern void __cdecl achievements_initialize();
extern void __cdecl achievements_update(real world_seconds_elapsed);
extern void __cdecl gamer_achievements_begin_retrieval(e_controller_index controller_index);

