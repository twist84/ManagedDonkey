#include "saved_games/game_state_pc.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "saved_games/game_state.hpp"
#include "saved_games/game_state_procs.hpp"
#include "tag_files/files.hpp"

#include <windows.h>

REFERENCE_DECLARE(0x02497CD0, s_pc_game_state_globals, pc_game_state_globals);

HOOK_DECLARE(0x0065DAD0, game_state_get_storage_count);
HOOK_DECLARE(0x0065DB10, game_state_read_from_storage);
HOOK_DECLARE(0x0065DBA0, game_state_set_buffer_protection);
HOOK_DECLARE(0x0065DBC0, game_state_storage_is_valid);
HOOK_DECLARE(0x0065DBE0, game_state_write_to_storage);

void* __cdecl game_state_allocate_buffer(int32 cpu_size, int32 persist_size, int32* out_guard_page_size)
{
	return INVOKE(0x0065D9F0, game_state_allocate_buffer, cpu_size, persist_size, out_guard_page_size);
}

//.text:0065DA40

void* __cdecl game_state_allocate_temporary_buffer(int32 buffer_size)
{
	return INVOKE(0x0065DA50, game_state_allocate_temporary_buffer, buffer_size);
}

void __cdecl game_state_dispose_storage()
{
	INVOKE(0x0065DA60, game_state_dispose_storage);
}

void __cdecl game_state_free_buffer()
{
	INVOKE(0x0065DA70, game_state_free_buffer);
}

void __cdecl game_state_free_temporary_buffer(void* buffer, int32 buffer_size)
{
	INVOKE(0x0065DAC0, game_state_free_temporary_buffer, buffer, buffer_size);
}

int32 __cdecl game_state_get_storage_count()
{
	//return INVOKE(0x0065DAD0, game_state_get_storage_count);

	return 2;
}

void __cdecl game_state_initialize_storage()
{
	INVOKE(0x0065DAE0, game_state_initialize_storage);
}

//.text:0065DAF0
//.text:0065DB00

bool __cdecl game_state_read_from_memory_storage(int32 storage_index, int32 game_state_proc_flags)
{
	return false;
}

bool __cdecl game_state_read_from_file_storage(int32 storage_index, int32 game_state_proc_flags)
{
	if (pc_game_state_globals.buffer_allocated)
	{
		s_file_reference scratch_save_file{};
		file_reference_create_from_path(&scratch_save_file, "core\\scratch_save_file.bin", false);

		uns32 error = 0;
		if (file_open(&scratch_save_file, FLAG(_file_open_flag_desired_access_read), &error))
		{
			game_state_call_before_load_procs(game_state_proc_flags);
			bool file_result = file_read(&scratch_save_file, pc_game_state_globals.buffer_size_to_persist, false, pc_game_state_globals.allocation);
			file_close(&scratch_save_file);

			//bool signature_check_result = game_state_security_verify_signature_insecure(nullptr) && file_result;
			//if (!signature_check_result)
			//	event(_event_critical, "game_state: pc game state read from file failed signature check");

			game_state_buffer_handle_read();
			game_state_call_after_load_procs(game_state_proc_flags);

			//return signature_check_result;
			return file_result;
		}
	}
	else
	{
		event(_event_error, "game_state: can't load game state on PC if it's not allocated or not at a fixed address");
	}

	return false;
}

bool __cdecl game_state_read_from_storage(int32 storage_index, int32 game_state_proc_flags)
{
	//return INVOKE(0x0065DB10, game_state_read_from_storage, storage_index, game_state_proc_flags);

	ASSERT(VALID_INDEX(storage_index, game_state_get_storage_count()));

	bool result = false;
	//if (pc_game_state_globals.game_state_storage_buffer_type == 1)
	//{
	//	result = game_state_read_from_memory_storage(storage_index, game_state_proc_flags);
	//}
	//else if (pc_game_state_globals.game_state_storage_buffer_type == 2)
	//{
		result = game_state_read_from_file_storage(storage_index, game_state_proc_flags);
	//}
	//else
	//{
	//	UNREACHABLE();
	//	result = false;
	//}

	return result;
}

//.text:0065DB90

void __cdecl game_state_set_buffer_protection(void* buffer, int32 cpu_size, int32 guard_page_size)
{
	//INVOKE(0x0065DBA0, game_state_set_buffer_protection, buffer, cpu_size, guard_page_size);

	DWORD old_protect;
	VirtualProtect(buffer, cpu_size + guard_page_size, PAGE_READWRITE, &old_protect);
}

bool __cdecl game_state_storage_is_valid(int32 storage_index)
{
	//return INVOKE(0x0065DBC0, game_state_storage_is_valid, storage_index);

	return pc_game_state_globals.storage_is_valid;
}

//.text:0065DBD0

void __cdecl game_state_write_to_file_memory_storage(int32 storage_index)
{
}

void __cdecl game_state_write_to_file_storage(int32 storage_index)
{
	if (pc_game_state_globals.buffer_allocated)
	{
		s_file_reference scratch_save_file{};
		file_reference_create_from_path(&scratch_save_file, "core\\scratch_save_file.bin", false);
		file_create_parent_directories_if_not_present(&scratch_save_file);

		if (!file_exists(&scratch_save_file))
			file_create(&scratch_save_file);

		uns32 error = 0;
		if (file_open(&scratch_save_file, FLAG(_file_open_flag_desired_access_write), &error))
		{
			bool file_result = file_write(&scratch_save_file, pc_game_state_globals.buffer_size_to_persist, pc_game_state_globals.allocation);
			file_close(&scratch_save_file);
			pc_game_state_globals.storage_is_valid = file_result;
			return;
		}
	}
	else
	{
		event(_event_error, "can't save game state on PC if it's not allocated or not at a fixed address");
	}

	pc_game_state_globals.storage_is_valid = false;
}

void __cdecl game_state_write_to_storage(int32 storage_index)
{
	//INVOKE(0x0065DBE0, game_state_write_to_storage, storage_index);

	ASSERT(VALID_INDEX(storage_index, game_state_get_storage_count()));

	//if (pc_game_state_globals.game_state_storage_buffer_type == 1)
	//{
	//	game_state_write_to_file_memory_storage(storage_index);
	//}
	//else if (pc_game_state_globals.game_state_storage_buffer_type == 2)
	//{
		game_state_write_to_file_storage(storage_index);
	//}
	//else
	//{
	//	UNREACHABLE();
	//}
}

