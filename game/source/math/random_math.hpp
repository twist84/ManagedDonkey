#pragma once

#include "cseries/cseries.hpp"

struct game_options;

extern uns32 __cdecl generate_random_seed();
extern uns32 __cdecl get_local_random_seed();
extern uns32* __cdecl get_local_random_seed_address();
extern uns32 __cdecl get_random_seed();
extern uns32* __cdecl get_random_seed_address();
extern uns32 __cdecl random_direction_table_count();
extern bool __cdecl random_direction_table_sample(int32 sample_index, real_vector3d* out_sample);
extern void __cdecl random_math_dispose();
extern void __cdecl random_math_dispose_tool();
extern void __cdecl random_math_initialize();
extern void __cdecl random_math_initialize_internal();
extern void __cdecl random_math_initialize_tool();
extern void __cdecl set_random_seed(uns32 random_seed);

extern real32 _real_random_range(uns32* seed, const char* purpose, const char* source_file, uns32 source_line, real32 lower_bound, real32 upper_bound);
#define REAL_RANDOM_RANGE(SEED, PURPOSE, LOWER_BOUND, UPPER_BOUND) _real_random_range((SEED), (PURPOSE), __FILE__, __LINE__, (LOWER_BOUND), (UPPER_BOUND))

extern bool random_seed_usable();
extern void random_seed_allow_use();
extern void random_seed_disallow_use();
extern void random_seed_debug_log_begin(const game_options* options);
extern void random_seed_debug_log_end();

