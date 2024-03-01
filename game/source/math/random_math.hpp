#pragma once

#include "cseries/cseries.hpp"

extern dword __cdecl generate_random_seed();
extern dword __cdecl get_local_random_seed();
extern dword* __cdecl get_local_random_seed_address();
extern dword __cdecl get_random_seed();
extern dword* __cdecl get_random_seed_address();

