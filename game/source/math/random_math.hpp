#pragma once

#include "cseries/cseries.hpp"

extern dword __cdecl generate_random_seed();
extern dword __cdecl get_local_random_seed();
extern dword* __cdecl get_local_random_seed_address();
extern dword __cdecl get_random_seed();
extern dword* __cdecl get_random_seed_address();
extern dword __cdecl random_direction_table_count();
extern bool __cdecl random_direction_table_sample(long sample_index, vector3d* out_sample);
extern void __cdecl random_math_dispose();
extern void __cdecl random_math_dispose_tool();
extern void __cdecl random_math_initialize();
extern void __cdecl random_math_initialize_internal();
extern void __cdecl random_math_initialize_tool();

