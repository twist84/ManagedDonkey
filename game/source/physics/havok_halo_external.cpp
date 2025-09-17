#include "physics/havok_halo_external.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x0075C490, halo_external_breakpoint);
HOOK_DECLARE(0x0075C4A0, halo_managed_memory_address);
HOOK_DECLARE(0x0075C4D0, halo_verify_reference_count);

void __cdecl halo_external_breakpoint()
{
    VASSERT(0, "havok breakpoint :-)");
}

bool __cdecl halo_managed_memory_address(const hkReferencedObject* object)
{
    int32 total_tags_size = cache_files_get_total_tags_size();
    unsigned int tag_cache_base_address = (unsigned int)cache_file_globals_get_tag_cache_base_address();

    return IN_RANGE_INCLUSIVE((unsigned int)object, tag_cache_base_address, tag_cache_base_address + total_tags_size - 1);
}

// unreferenced
void __cdecl halo_verify_reference_count(hkReferencedObject* object, int32 new_reference_count)
{
    if (halo_managed_memory_address(object))
        ASSERT(new_reference_count == k_minimum_physics_model_shape_reference_count);
}

