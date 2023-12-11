#pragma once

extern void* __cdecl game_state_allocate_buffer(long cpu_size, long persist_size, long* out_guard_page_size);
extern void* __cdecl game_state_allocate_temporary_buffer(long buffer_size);
extern void __cdecl game_state_dispose_storage();
extern void __cdecl game_state_free_buffer();
extern void __cdecl game_state_free_temporary_buffer(void* buffer, long buffer_size);
extern long __cdecl game_state_get_storage_count();
extern void __cdecl game_state_initialize_storage();
extern bool __cdecl game_state_read_from_storage(long storage_index, long game_state_proc_flags);
extern void __cdecl game_state_set_buffer_protection(void* buffer, long cpu_size, long guard_page_size);
extern bool __cdecl game_state_storage_is_valid(long storage_index);
extern void __cdecl game_state_write_to_storage();

