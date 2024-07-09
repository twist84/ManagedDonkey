#pragma once

struct c_restricted_section;
struct c_restricted_memory_callbacks;

extern void* __cdecl restricted_memory_get_address(long index, unsigned long offset);
extern void __cdecl restricted_region_add_alias(long index);
extern long __cdecl restricted_region_add_member(long index, char const* name, char const* type, unsigned int allocation, long alignment_bits, void(__cdecl* tls_update_callback)(void*), void(__cdecl* tls_pre_overwrite_fixup_callback)(void*), void(__cdecl* tls_post_copy_fixup_callback)(void*));
extern void __cdecl restricted_region_add_mirror(long index, c_restricted_section* mirror_section);
extern bool __cdecl restricted_region_aliased_for_current_thread(long index);
extern void __cdecl restricted_region_begin_aliasing(long index);
extern void __cdecl restricted_region_clear_membership(long index);
extern void __cdecl restricted_region_create(long index, c_restricted_section* primary_section, long critical_section_index, c_restricted_memory_callbacks* callbacks);
extern void __cdecl restricted_region_destroy(long index);
extern void __cdecl restricted_region_end_aliasing(long index);
extern long __cdecl restricted_region_find_address(void* address);
extern void __cdecl restricted_region_free_member(long index, long member_index);
extern void* __cdecl restricted_region_get_member_address(long index, long member_index);
extern unsigned long __cdecl restricted_region_get_member_offset(long index, long member_index);
extern long __cdecl restricted_region_get_mirror_count(long index);
extern char const* __cdecl restricted_region_get_name(long index);
extern unsigned int __cdecl restricted_region_get_sector_size(long index);
extern unsigned int __cdecl restricted_region_get_total_size(long index);
extern void __cdecl restricted_region_handle_crash_of_current_thread();
extern void __cdecl restricted_region_handle_gamestate_load(long index);
extern bool __cdecl restricted_region_lock_mirror(long index);
extern void __cdecl restricted_region_lock_primary(long index);
extern bool __cdecl restricted_region_locked_for_current_thread(long index);
extern bool __cdecl restricted_region_mirror_locked_for_current_thread(long index);
extern bool __cdecl restricted_region_primary_locked_for_current_thread(long index);
extern bool __cdecl restricted_region_publish_to_mirror(long index);
extern void __cdecl restricted_region_remove_alias(long index);
extern void __cdecl restricted_region_reset_mirrors(long index);
extern bool __cdecl restricted_region_try_and_lock_mirror(long index);
extern bool __cdecl restricted_region_try_and_lock_primary(long index);
extern void __cdecl restricted_region_unlock_mirror(long index);
extern void __cdecl restricted_region_unlock_primary(long index);
extern bool __cdecl restricted_region_valid(long index);
extern bool __cdecl restricted_region_validate_address(long index, void* address);
extern bool __cdecl restricted_region_wait_for_available_mirror(long index, unsigned long a2);
extern bool __cdecl restricted_region_wait_for_new_write_mirror(long index, unsigned long a2);

