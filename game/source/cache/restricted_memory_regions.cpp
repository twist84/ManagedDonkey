#include "cache/restricted_memory_regions.hpp"

#include "cache/restricted_memory.hpp"
#include "cseries/cseries.hpp"
#include "memory/thread_local.hpp"

void* __cdecl restricted_memory_get_address(long index, unsigned long offset)
{
	return INVOKE(0x0059FF70, restricted_memory_get_address, index, offset);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//void* address = g_restricted_address[index];
	//ASSERT(address != NULL);
	//ASSERT(offset < g_restricted_regions[index].subsection_size());
	//return (byte*)address + offset;
}

void __cdecl restricted_memory_set_base_address(long index, void* address)
{
	INVOKE(0x0059FFA0, restricted_memory_set_base_address, index, address);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//g_restricted_address[index] = address;
}

void __cdecl restricted_region_add_alias(long index)
{
	INVOKE(0x0059FFD0, restricted_region_add_alias, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_alias);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//ASSERT(g_restricted_address[index] == NULL);
	//g_restricted_address[index] = g_restricted_regions[index].add_alias();
	//g_restricted_alias[index] = true;
}

long __cdecl restricted_region_add_member(long index, char const* name, char const* type, unsigned int allocation, long alignment_bits, void(__cdecl* tls_update_callback)(void*), void(__cdecl* tls_pre_overwrite_fixup_callback)(void*), void(__cdecl* tls_post_copy_fixup_callback)(void*))
{
	return INVOKE(0x005A0010, restricted_region_add_member, index, name, type, allocation, alignment_bits, tls_update_callback, tls_pre_overwrite_fixup_callback, tls_post_copy_fixup_callback);

	//ASSERT(allocation);
	//ASSERT(tls_update_callback);
	//ASSERT(alignment_bits >= 0 && alignment_bits < 6);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//long member_index = g_restricted_regions[index].add_member(name, type, allocation, alignment_bits, tls_update_callback, tls_pre_overwrite_fixup_callback, tls_post_copy_fixup_callback);
	//ASSERT(member_index != NONE);
	//void* address = restricted_region_get_member_address(index, member_index);
	//ASSERT(align_pointer(address, alignment_bits) == address);
	//tls_update_callback(address);
	//return member_index;
}

void __cdecl restricted_region_add_mirror(long index, c_restricted_section* mirror_section)
{
	INVOKE(0x005A0080, restricted_region_add_mirror, index, mirror_section);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//g_restricted_regions[index].add_mirror(mirror_section);
}

bool __cdecl restricted_region_aliased_for_current_thread(long index)
{
	return INVOKE(0x005A00A0, restricted_region_aliased_for_current_thread, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_alias);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//ASSERT(g_restricted_regions[index].initialized());
	//return g_restricted_address[index] && g_restricted_alias[index];
}

void __cdecl restricted_region_begin_aliasing(long index)
{
	INVOKE(0x005A00E0, restricted_region_begin_aliasing, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//ASSERT(g_restricted_address[index] != NULL);
	//g_restricted_regions[index].begin_aliasing();
}

void __cdecl restricted_region_clear_membership(long index)
{
	INVOKE(0x005A0100, restricted_region_clear_membership, index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//g_restricted_regions[index].clear_membership();
}

void __cdecl restricted_region_create(long index, c_restricted_section* primary_section, long critical_section_index, c_restricted_memory_callbacks* callbacks)
{
	INVOKE(0x005A0120, restricted_region_create, index, primary_section, critical_section_index, callbacks);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//g_restricted_regions[index].initialize(index, primary_section, critical_section_index, callbacks);
}

void __cdecl restricted_region_destroy(long index)
{
	INVOKE(0x005A0150, restricted_region_destroy, index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//g_restricted_regions[index].destroy();
}

void __cdecl restricted_region_end_aliasing(long index)
{
	INVOKE(0x005A0170, restricted_region_end_aliasing, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//ASSERT(g_restricted_address[index] != NULL);
	//g_restricted_regions[index].end_aliasing();
}

long __cdecl restricted_region_find_address(void* address)
{
	return INVOKE(0x005A0190, restricted_region_find_address, address);
}

void __cdecl restricted_region_free_member(long index, long member_index)
{
	INVOKE(0x005A0200, restricted_region_free_member, index, member_index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//g_restricted_regions[index].free_member(member_index);
}

void* __cdecl restricted_region_get_member_address(long index, long member_index)
{
	return INVOKE(0x005A0220, restricted_region_get_member_address, index, member_index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//void* address = g_restricted_address[index];
	//ASSERT(address != NULL);
	//return (byte*)address + g_restricted_regions[index].get_member_offset(member_index);
}

unsigned long __cdecl restricted_region_get_member_offset(long index, long member_index)
{
	return INVOKE(0x005A0260, restricted_region_get_member_offset, index, member_index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//return g_restricted_regions[index].get_member_offset(member_index);
}

long __cdecl restricted_region_get_mirror_count(long index)
{
	return INVOKE(0x005A0280, restricted_region_get_mirror_count, index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//return g_restricted_regions[index].mirror_count();
}

char const* __cdecl restricted_region_get_name(long index)
{
	return INVOKE(0x005A02A0, restricted_region_get_name, index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//return g_restricted_region_names[index];
}

unsigned int __cdecl restricted_region_get_sector_size(long index)
{
	return INVOKE(0x005A02B0, restricted_region_get_sector_size, index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//return g_restricted_regions[index].subsection_size();
}

unsigned int __cdecl restricted_region_get_total_size(long index)
{
	return INVOKE(0x005A02D0, restricted_region_get_total_size, index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//return g_restricted_regions[index].total_size();
}

void __cdecl restricted_region_handle_crash_of_current_thread()
{
	INVOKE(0x005A02F0, restricted_region_handle_crash_of_current_thread);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//for (long index = 0; index < k_total_restricted_memory_regions; index++)
	//{
	//	g_restricted_address[index] = NULL;
	//	if (g_restricted_regions[index].initialized())
	//		g_restricted_regions[index].handle_crash_of_current_thread();
	//}
}

void __cdecl restricted_region_handle_gamestate_load(long index)
{
	INVOKE(0x005A0340, restricted_region_handle_gamestate_load, index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//g_restricted_regions[index].handle_gamestate_load();
}

bool __cdecl restricted_region_lock_mirror(long index)
{
	return INVOKE(0x005A0360, restricted_region_lock_mirror, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//void* base_address = g_restricted_regions[index].lock_mirror();
	//ASSERT(g_restricted_address[index] == NULL);
	//g_restricted_address[index] = base_address;
	//return base_address != NULL;
}

void __cdecl restricted_region_lock_primary(long index)
{
	INVOKE(0x005A03A0, restricted_region_lock_primary, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//void* base_address = g_restricted_regions[index].lock_primary();
	//ASSERT(base_address);
	//ASSERT(g_restricted_address[index] == NULL);
	//g_restricted_address[index] = base_address;
}

bool __cdecl restricted_region_locked_for_current_thread(long index)
{
	return INVOKE(0x005A03E0, restricted_region_locked_for_current_thread, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//ASSERT(g_restricted_regions[index].initialized());
	//return g_restricted_address[index] != NULL;
}

bool __cdecl restricted_region_mirror_locked_for_current_thread(long index)
{
	return INVOKE(0x005A0410, restricted_region_mirror_locked_for_current_thread, index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//ASSERT(g_restricted_regions[index].initialized());
	//return g_restricted_regions[index].mirror_locked_for_current_thread();
}

bool __cdecl restricted_region_primary_locked_for_current_thread(long index)
{
	return INVOKE(0x005A0430, restricted_region_primary_locked_for_current_thread, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//return g_restricted_address[index] && !g_restricted_regions[index].mirror_locked_for_current_thread();
}

bool __cdecl restricted_region_publish_to_mirror(long index)
{
	return INVOKE(0x005A0470, restricted_region_publish_to_mirror, index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//return g_restricted_regions[index].mirror_contents();
}

void __cdecl restricted_region_remove_alias(long index)
{
	INVOKE(0x005A0490, restricted_region_remove_alias, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_alias);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//ASSERT(restricted_region_aliased_for_current_thread(index));
	//g_restricted_regions[index].remove_alias();
	//g_restricted_address[index] = NULL;
	//g_restricted_alias[index] = false;
}

void __cdecl restricted_region_reset_mirrors(long index)
{
	INVOKE(0x005A04D0, restricted_region_reset_mirrors, index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//g_restricted_regions[index].reset_mirrors();
}

bool __cdecl restricted_region_try_and_lock_mirror(long index)
{
	return INVOKE(0x005A04F0, restricted_region_try_and_lock_mirror, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//void* base_address = g_restricted_regions[index].try_and_lock_mirror();
	//ASSERT(g_restricted_address[index] == NULL);
	//g_restricted_address[index] = base_address;
	//return base_address != NULL;
}

bool __cdecl restricted_region_try_and_lock_primary(long index)
{
	return INVOKE(0x005A0530, restricted_region_try_and_lock_primary, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//void* base_address = g_restricted_regions[index].try_and_lock_primary();
	//ASSERT(g_restricted_address[index] == NULL);
	//g_restricted_address[index] = base_address;
	//return base_address != NULL;
}

void __cdecl restricted_region_unlock_mirror(long index)
{
	INVOKE(0x005A0570, restricted_region_unlock_mirror, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//ASSERT(g_restricted_address[index] != NULL);
	//g_restricted_address[index] = NULL;
	//g_restricted_regions[index].unlock_mirror();
}

void __cdecl restricted_region_unlock_primary(long index)
{
	INVOKE(0x005A05B0, restricted_region_unlock_primary, index);

	//TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//ASSERT(g_restricted_address[index] != NULL);
	//g_restricted_address[index] = NULL;
	//g_restricted_regions[index].unlock_primary();
}

bool __cdecl restricted_region_valid(long index)
{
	return INVOKE(0x005A05F0, restricted_region_valid, index);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//return g_restricted_regions[index].initialized();
}

bool __cdecl restricted_region_validate_address(long index, void* address)
{
	return INVOKE(0x005A0610, restricted_region_validate_address, index, address);
}

bool __cdecl restricted_region_wait_for_available_mirror(long index, unsigned long a2)
{
	return INVOKE(0x005A0660, restricted_region_wait_for_available_mirror, index, a2);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//g_restricted_regions[index].wait_for_available_mirror(a2);
}

bool __cdecl restricted_region_wait_for_new_write_mirror(long index, unsigned long a2)
{
	return INVOKE(0x005A0680, restricted_region_wait_for_new_write_mirror, index, a2);

	//ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	//g_restricted_regions[index].wait_for_new_write_mirror(a2);
}

