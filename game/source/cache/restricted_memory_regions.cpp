#include "cache/restricted_memory_regions.hpp"

#include "cache/restricted_memory.hpp"
#include "cseries/cseries.hpp"

void __cdecl restricted_region_add_alias(long index)
{
	INVOKE(0x0059FFD0, restricted_region_add_alias, index);
}

long __cdecl restricted_region_add_member(long index, char const* name, char const* type, unsigned int allocation, long alignment_bits, void(__cdecl*tls_update_callback)(void*), void(__cdecl* tls_unknown_callback1)(void*), void(__cdecl* tls_unknown_callback2)(void*))
{
	return INVOKE(0x005A0010, restricted_region_add_member, index, name, type, allocation, alignment_bits, tls_update_callback, tls_unknown_callback1, tls_unknown_callback2);
}

void __cdecl restricted_region_add_mirror(long index, c_restricted_section* mirror_section)
{
	INVOKE(0x005A0080, restricted_region_add_mirror, index, mirror_section);
}

// 005A00A0; UNREFERENCED
// 005A00E0; UNREFERENCED
// 005A0100; UNREFERENCED

void __cdecl restricted_region_create(long index, c_restricted_section* primary_section, long critical_section_index, c_restricted_memory_callbacks* callbacks)
{
	INVOKE(0x005A0120, restricted_region_create, index, primary_section, critical_section_index, callbacks);
}

void __cdecl restricted_region_destroy(long index)
{
	INVOKE(0x005A0150, restricted_region_destroy, index);
}

// 005A0170; UNREFERENCED
// 005A0190; UNREFERENCED

void __cdecl restricted_region_free_member(long index, long member_index)
{
	INVOKE(0x005A0200, restricted_region_free_member, index, member_index);
}

void* __cdecl restricted_region_get_member_address(long index, long member_index)
{
	return INVOKE(0x005A0220, restricted_region_get_member_address, index, member_index);
}

unsigned long __cdecl restricted_region_get_member_offset(long index, long member_index)
{
	return INVOKE(0x005A0260, restricted_region_get_member_offset, index, member_index);
}

long __cdecl restricted_region_get_mirror_count(long index)
{
	return INVOKE(0x005A0280, restricted_region_get_mirror_count, index);
}

char const* __cdecl restricted_region_get_name(long index)
{
	return INVOKE(0x005A02A0, restricted_region_get_name, index);
}

// 005A02B0; UNREFERENCED
// 005A02D0; UNREFERENCED

void __cdecl restricted_region_handle_crash_of_current_thread()
{
	INVOKE(0x005A02F0, restricted_region_handle_crash_of_current_thread);
}

void __cdecl restricted_region_handle_gamestate_load(long index)
{
	INVOKE(0x005A0340, restricted_region_handle_gamestate_load, index);
}

bool __cdecl restricted_region_lock_mirror(long index)
{
	return INVOKE(0x005A0360, restricted_region_lock_mirror, index);
}

void __cdecl restricted_region_lock_primary(long index)
{
	INVOKE(0x005A03A0, restricted_region_lock_primary, index);
}

bool __cdecl restricted_region_locked_for_current_thread(long index)
{
	return INVOKE(0x005A03E0, restricted_region_locked_for_current_thread, index);
}

bool __cdecl restricted_region_mirror_locked_for_current_thread(long index)
{
	return INVOKE(0x005A0410, restricted_region_mirror_locked_for_current_thread, index);
}

bool __cdecl restricted_region_primary_locked_for_current_thread(long index)
{
	return INVOKE(0x005A0430, restricted_region_primary_locked_for_current_thread, index);
}

bool __cdecl restricted_region_publish_to_mirror(long index)
{
	return INVOKE(0x005A0470, restricted_region_publish_to_mirror, index);
}

void __cdecl restricted_region_remove_alias(long index)
{
	INVOKE(0x005A0490, restricted_region_remove_alias, index);
}

// 005A04D0; UNREFERENCED
// 005A04F0; UNREFERENCED

bool __cdecl restricted_region_try_and_lock_primary(long index)
{
	return INVOKE(0x005A0530, restricted_region_try_and_lock_primary, index);
}

void __cdecl restricted_region_unlock_mirror(long index)
{
	INVOKE(0x005A0570, restricted_region_unlock_mirror, index);
}

void __cdecl restricted_region_unlock_primary(long index)
{
	INVOKE(0x005A05B0, restricted_region_unlock_primary, index);
}

bool __cdecl restricted_region_valid(long index)
{
	return INVOKE(0x005A05F0, restricted_region_valid, index);
}

// 005A0610; UNREFERENCED

bool __cdecl restricted_region_wait_for_available_mirror(long index, unsigned long a2)
{
	return INVOKE(0x005A0660, restricted_region_wait_for_available_mirror, index, a2);
}

// 005A0680; UNREFERENCED

