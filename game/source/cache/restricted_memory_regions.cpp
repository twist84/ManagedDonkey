#include "cache/restricted_memory_regions.hpp"

#include "cseries/cseries.hpp"

// 0059FFD0; void __cdecl restricted_region_add_alias(long)
// 005A0010; long __cdecl restricted_region_add_member(long, char const*, char const*, unsigned int, long, void(__cdecl*)(void*), void(__cdecl*)(void*), void(__cdecl*)(void*))
// 005A0080; void __cdecl restricted_region_add_mirror(long, class c_restricted_section *)
// 005A00A0; UNREFERENCED
// 005A00E0; UNREFERENCED
// 005A0100; UNREFERENCED
// 005A0120; void __cdecl restricted_region_create(long, class c_restricted_section *, long, class c_restricted_memory_callbacks *)
// 005A0150; void __cdecl restricted_region_destroy(long)
// 005A0170; UNREFERENCED
// 005A0190; UNREFERENCED
// 005A0200; void __cdecl restricted_region_free_member(long, long)
// 005A0220; void * __cdecl restricted_region_get_member_address(long, long)
// 005A0260; unsigned long __cdecl restricted_region_get_member_offset(long, long)
// 005A0280; long __cdecl restricted_region_get_mirror_count(long)
// 005A02A0; char const * __cdecl restricted_region_get_name(long)
// 005A02B0; UNREFERENCED
// 005A02D0; UNREFERENCED
// 005A02F0; void __cdecl restricted_region_handle_crash_of_current_thread(void)
// 005A0340; void __cdecl restricted_region_handle_gamestate_load(long)
// 005A0360; bool __cdecl restricted_region_lock_mirror(long)
// 005A03A0; void __cdecl restricted_region_lock_primary(long)
// 005A03E0; bool __cdecl restricted_region_locked_for_current_thread(long)
// 005A0410; bool __cdecl restricted_region_mirror_locked_for_current_thread(long)
// 005A0430; bool __cdecl restricted_region_primary_locked_for_current_thread(long)
// 005A0470; bool __cdecl restricted_region_publish_to_mirror(long)
// 005A0490; void __cdecl restricted_region_remove_alias(long)
// 005A04D0; UNREFERENCED
// 005A04F0; UNREFERENCED
// 005A0530; bool __cdecl restricted_region_try_and_lock_primary(long)
// 005A0570; void __cdecl restricted_region_unlock_mirror(long)

void __cdecl restricted_region_unlock_primary(long index)
{
	INVOKE(0x005A05B0, restricted_region_unlock_primary, index);
}

// 005A05F0; bool __cdecl restricted_region_valid(long)
// 005A0610; UNREFERENCED
// 005A0660; bool __cdecl restricted_region_wait_for_available_mirror(long, unsigned long)
// 005A0680; UNREFERENCED

