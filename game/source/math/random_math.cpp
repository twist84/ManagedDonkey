#include "math/random_math.hpp"

dword* __cdecl get_local_random_seed_address()
{
    return INVOKE(0x0051EFD0, get_local_random_seed_address);
}

