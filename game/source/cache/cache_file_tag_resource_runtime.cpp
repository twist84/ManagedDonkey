#include "cache/cache_file_tag_resource_runtime.hpp"

#include "memory/module.hpp"

HOOK_DECLARE(0x00563E10, tag_resource_get);

void* __cdecl tag_resource_get(s_tag_resource const* resource)
{
    void* result = nullptr;
    HOOK_INVOKE(result =, tag_resource_get, resource);
    return result;
}
