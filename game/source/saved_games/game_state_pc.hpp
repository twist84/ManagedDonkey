#pragma once

#include "cseries/cseries.hpp"

struct s_pc_game_state_globals
{
	bool buffer_allocated;
	bool allocated_virtually;
	void* allocation;
	int32 allocation_size;
	int32 buffer_size_to_persist;
	bool storage_is_valid;
};
COMPILE_ASSERT(sizeof(s_pc_game_state_globals) == 0x14);

extern s_pc_game_state_globals& pc_game_state_globals;

extern void* __cdecl game_state_allocate_buffer(int32 cpu_size, int32 persist_size, int32* out_guard_page_size);
extern void* __cdecl game_state_allocate_temporary_buffer(int32 buffer_size);
extern void __cdecl game_state_dispose_storage();
extern void __cdecl game_state_free_buffer();
extern void __cdecl game_state_free_temporary_buffer(void* buffer, int32 buffer_size);
extern int32 __cdecl game_state_get_storage_count();
extern void __cdecl game_state_initialize_storage();
extern bool __cdecl game_state_read_from_file_storage(int32 storage_index, int32 game_state_proc_flags);
extern bool __cdecl game_state_read_from_storage(int32 storage_index, int32 game_state_proc_flags);
extern void __cdecl game_state_set_buffer_protection(void* buffer, int32 cpu_size, int32 guard_page_size);
extern bool __cdecl game_state_storage_is_valid(int32 storage_index);
extern void __cdecl game_state_write_to_file_storage(int32 storage_index);
extern void __cdecl game_state_write_to_storage(int32 storage_index);

