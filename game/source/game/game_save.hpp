#pragma once

#include "cseries/cseries.hpp"

enum
{
	_game_save_priority_none = 0,
	_game_save_priority_safe,
	_game_save_priority_no_timeout,
	_game_save_priority_immediate,
	_game_save_priority_cinematic_skip,

	NUMBER_OF_GAME_SAVE_PRIORITIES
};

struct s_game_save_globals
{
	int32 save_priority;
	int32 ticks_until_next_save_check;
	int32 save_start_time;
	int32 safe_interval_count;
	int32 time_of_last_game_save;
	int32 unsafe_object_index;
};
COMPILE_ASSERT(sizeof(s_game_save_globals) == 0x18);

extern bool debug_game_save;

extern void __cdecl __tls_set_g_game_save_globals_allocator(void* address);
extern bool __cdecl game_all_quiet();
extern bool __cdecl game_safe_to_save();
extern bool __cdecl game_safe_to_save_internal(bool a1);
extern bool __cdecl game_safe_to_speak();
extern void __cdecl game_save(int32 save_priority);
extern void __cdecl game_save_cancel();
extern void __cdecl game_save_cinematic_skip();
extern void __cdecl game_save_dispose();
extern void __cdecl game_save_dispose_from_old_map();
extern void __cdecl game_save_immediate();
extern void __cdecl game_save_initialize();
extern void __cdecl game_save_initialize_for_new_map();
extern void __cdecl game_save_no_timeout();
extern void __cdecl game_save_safe();
extern void __cdecl game_save_update();
extern bool __cdecl game_saving();
extern bool __cdecl not_enough_time_since_last_save();

