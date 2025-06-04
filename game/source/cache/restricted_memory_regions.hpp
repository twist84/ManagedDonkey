#pragma once

class c_restricted_section;
class c_restricted_memory_callbacks;

extern void* __cdecl restricted_memory_get_address(int32 index, uns32 offset);
extern void __cdecl restricted_memory_set_base_address(int32 index, void* address);
extern void __cdecl restricted_region_add_alias(int32 index);
extern int32 __cdecl restricted_region_add_member(int32 index, const char* name, const char* type, unsigned int allocation, int32 alignment_bits, void(__cdecl* tls_update_callback)(void*), void(__cdecl* tls_pre_overwrite_fixup_callback)(void*), void(__cdecl* tls_post_copy_fixup_callback)(void*));
extern void __cdecl restricted_region_add_mirror(int32 index, c_restricted_section* mirror_section);
extern bool __cdecl restricted_region_aliased_for_current_thread(int32 index);
extern void __cdecl restricted_region_begin_aliasing(int32 index);
extern void __cdecl restricted_region_clear_membership(int32 index);
extern void __cdecl restricted_region_create(int32 index, c_restricted_section* primary_section, int32 critical_section_index, c_restricted_memory_callbacks* callbacks);
extern void __cdecl restricted_region_destroy(int32 index);
extern void __cdecl restricted_region_end_aliasing(int32 index);
extern int32 __cdecl restricted_region_find_address(void* address);
extern void __cdecl restricted_region_free_member(int32 index, int32 member_index);
extern void* __cdecl restricted_region_get_member_address(int32 index, int32 member_index);
extern uns32 __cdecl restricted_region_get_member_offset(int32 index, int32 member_index);
extern int32 __cdecl restricted_region_get_mirror_count(int32 index);
extern const char* __cdecl restricted_region_get_name(int32 index);
extern unsigned int __cdecl restricted_region_get_sector_size(int32 index);
extern unsigned int __cdecl restricted_region_get_total_size(int32 index);
extern void __cdecl restricted_region_handle_crash_of_current_thread();
extern void __cdecl restricted_region_handle_gamestate_load(int32 index);
extern bool __cdecl restricted_region_lock_mirror(int32 index);
extern void __cdecl restricted_region_lock_primary(int32 index);
extern bool __cdecl restricted_region_locked_for_current_thread(int32 index);
extern bool __cdecl restricted_region_mirror_locked_for_current_thread(int32 index);
extern bool __cdecl restricted_region_primary_locked_for_current_thread(int32 index);
extern bool __cdecl restricted_region_publish_to_mirror(int32 index);
extern void __cdecl restricted_region_remove_alias(int32 index);
extern void __cdecl restricted_region_reset_mirrors(int32 index);
extern bool __cdecl restricted_region_try_and_lock_mirror(int32 index);
extern bool __cdecl restricted_region_try_and_lock_primary(int32 index);
extern void __cdecl restricted_region_unlock_mirror(int32 index);
extern void __cdecl restricted_region_unlock_primary(int32 index);
extern bool __cdecl restricted_region_valid(int32 index);
extern bool __cdecl restricted_region_validate_address(int32 index, void* address);
extern bool __cdecl restricted_region_wait_for_available_mirror(int32 index, uns32 timeout);
extern bool __cdecl restricted_region_wait_for_new_write_mirror(int32 index, uns32 timeout);

