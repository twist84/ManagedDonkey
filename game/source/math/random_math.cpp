#include "math/random_math.hpp"

//.text:0051E790 ; public: __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>()
//.text:0051E7B0 ; public: __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::~t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>()
//.text:0051E7C0 ; void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)
//.text:0051E7E0 ; vector3d* __cdecl _distriubuite_vector_in_cone3d(dword*, char const*, char const*, dword, vector3d const*, long, long, long, long, real, real, real, vector3d*)
//.text:0051EAA0 ; vector3d* __cdecl _random_direction3d(dword*, char const*, char const*, dword, vector3d*)
//.text:0051ECA0 ; vector3d* __cdecl _random_vector_in_cone3d(dword*, char const*, char const*, dword, vector3d const*, real, real, vector3d*)
//.text:0051EE80 ; public: virtual void* __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::allocate(unsigned int, char const*)
//.text:0051EEC0 ; public: virtual void __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::deallocate(void*, long)
//.text:0051EEF0 ; double __cdecl distribute_random_stratified(dword*, char const*, char const*, dword, long, long)
//.text:0051EF70 ; public: void __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::free_memory()

dword __cdecl generate_random_seed()
{
    return INVOKE(0x0051EFA0, generate_random_seed);
}

dword __cdecl get_local_random_seed()
{
    return INVOKE(0x0051EFC0, get_local_random_seed);
}

dword* __cdecl get_local_random_seed_address()
{
    return INVOKE(0x0051EFD0, get_local_random_seed_address);
}

dword __cdecl get_random_seed()
{
    return INVOKE(0x0051EFE0, get_random_seed);
}

dword* __cdecl get_random_seed_address()
{
    return INVOKE(0x0051F000, get_random_seed_address);
}

dword __cdecl random_direction_table_count()
{
    return INVOKE(0x0051F020, random_direction_table_count);
}

bool __cdecl random_direction_table_sample(long sample_index, vector3d* out_sample)
{
    return INVOKE(0x0051F030, random_direction_table_sample, sample_index, out_sample);
}

void __cdecl random_math_dispose()
{
    INVOKE(0x0051F070, random_math_dispose);
}

void __cdecl random_math_dispose_tool()
{
    INVOKE(0x0051F080, random_math_dispose_tool);
}

void __cdecl random_math_initialize()
{
    INVOKE(0x0051F0B0, random_math_initialize);
}

void __cdecl random_math_initialize_internal()
{
    INVOKE(0x0051F130, random_math_initialize_internal);
}

void __cdecl random_math_initialize_tool()
{
    INVOKE(0x0051F170, random_math_initialize_tool);
}

//.text:0051F1C0 ; public: void* __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::reserve_memory(char const*, char const*, unsigned int, long)
//.text:0051F200 ; void __cdecl set_local_random_seed(dword local_random_seed)
//.text:0051F210 ; void __cdecl set_random_seed(dword random_seed)
//.text:01596530 ; void __cdecl `dynamic initializer for 'g_deterministic_random_seed_ptr_allocator''()
//.text:015F99B0 ; void __cdecl `dynamic atexit destructor for 'g_deterministic_random_seed_ptr_allocator''()

