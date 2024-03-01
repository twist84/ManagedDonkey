#include "math/random_math.hpp"

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

