#pragma once

#include "cseries/cseries.hpp"

extern bool& g_available_squads_ready;

extern bool __cdecl user_interface_available_squads_initialize(long controller_index, dword_flags available_squad_search_flags, char category);

