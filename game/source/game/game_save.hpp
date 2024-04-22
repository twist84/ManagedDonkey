#pragma once

#include "cseries/cseries.hpp"

enum e_game_save_type
{
	_game_save_type_cancel = 0,
	_game_save_type_safe,
	_game_save_type_no_timeout,
	_game_save_type_immediate,
	_game_save_type_cinematic_skip,

	k_game_save_type_count
};

struct s_game_save_globals
{
	// e_game_save_type
	long game_save_type;

	// timeout?
	long __unknown4;

	// timeout?
	long __unknown8;

	// attempt count?
	long __unknownC;

	long time_of_last_game_save;

	long unsafe_object_index;
};
static_assert(sizeof(s_game_save_globals) == 0x18);

extern bool debug_game_save;

extern void __cdecl __tls_set_g_game_save_globals_allocator(void* address);
extern bool __cdecl game_all_quiet();
extern bool __cdecl game_safe_to_save();
extern bool __cdecl game_safe_to_save_internal(bool a1);
extern bool __cdecl game_safe_to_speak();
extern void __cdecl game_save(long game_save_type);
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

