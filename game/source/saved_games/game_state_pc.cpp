#include "saved_games/game_state_pc.hpp"

#include "cseries/cseries.hpp"

void* __cdecl game_state_allocate_buffer(long cpu_size, long persist_size, long* out_guard_page_size)
{
	return INVOKE(0x0065D9F0, game_state_allocate_buffer, cpu_size, persist_size, out_guard_page_size);
}

void* __cdecl game_state_allocate_temporary_buffer(long buffer_size)
{
	return INVOKE(0x0065D9F0, game_state_allocate_temporary_buffer, buffer_size);
}

void __cdecl game_state_dispose_storage()
{
	INVOKE(0x0065DA60, game_state_dispose_storage);
}

void __cdecl game_state_free_buffer()
{
	INVOKE(0x0065DA70, game_state_free_buffer);
}

void __cdecl game_state_free_temporary_buffer(void* buffer, long buffer_size)
{
	INVOKE(0x0065DAC0, game_state_free_temporary_buffer, buffer, buffer_size);
}

long __cdecl game_state_get_storage_count()
{
	return INVOKE(0x0065DAD0, game_state_get_storage_count);
}

void __cdecl game_state_initialize_storage()
{
	INVOKE(0x0065DA70, game_state_initialize_storage);
}

bool __cdecl game_state_read_from_storage(long storage_index, long game_state_proc_flags)
{
	return INVOKE(0x0065DB10, game_state_read_from_storage, storage_index, game_state_proc_flags);
}

void __cdecl game_state_set_buffer_protection(void* buffer, long cpu_size, long guard_page_size)
{
	INVOKE(0x0065DBA0, game_state_set_buffer_protection, buffer, cpu_size, guard_page_size);
}

bool __cdecl game_state_storage_is_valid(long storage_index)
{
	return INVOKE(0x0065DBC0, game_state_storage_is_valid, storage_index);
}

void __cdecl game_state_write_to_storage()
{
	INVOKE(0x0065DBE0, game_state_write_to_storage);
}

