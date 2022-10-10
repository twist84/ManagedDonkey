#include "objects/objects.hpp"

#include "cseries/console.hpp"
#include "physics/havok.hpp"

long __cdecl object_get_ultimate_parent(long object_index)
{
    FUNCTION_BEGIN(true);

    return INVOKE(0x00B2EAB0, object_get_ultimate_parent, object_index);
}

void __cdecl object_set_in_limbo(long object_index, bool deactivate)
{
    INVOKE(0x00B32E20, object_set_in_limbo, object_index, deactivate);
}

void* __cdecl object_try_and_get_and_verify_type(long object_index, dword object_type)
{
    FUNCTION_BEGIN(true);

    return INVOKE(0x00B34490, object_try_and_get_and_verify_type, object_index, object_type);
}

