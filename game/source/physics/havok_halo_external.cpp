#include "physics/havok_halo_external.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x0075C490, halo_external_breakpoint);
HOOK_DECLARE(0x0075C4A0, halo_managed_memory_address);
HOOK_DECLARE(0x0075C4D0, halo_verify_reference_count);

void __cdecl halo_external_breakpoint()
{
    ASSERT2("havok breakpoint :-)");
}

bool __cdecl halo_managed_memory_address(hkReferencedObject const* object)
{
    long total_tags_size = cache_files_get_total_tags_size();
    unsigned int tag_cache_base_address = address_from_pointer(cache_file_globals_get_tag_cache_base_address());

    return IN_RANGE_INCLUSIVE(address_from_pointer(object), tag_cache_base_address, tag_cache_base_address + total_tags_size - 1);
}

// unreferenced
void __cdecl halo_verify_reference_count(hkReferencedObject* object, long new_reference_count)
{
    if (halo_managed_memory_address(object))
        ASSERT(new_reference_count == k_minimum_physics_model_shape_reference_count);
}

