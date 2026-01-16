#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"

enum e_game_state_proc_state
{
	_game_state_proc_state_none = 0,
	_game_state_proc_state_loading,
	_game_state_proc_state_saving,

	k_number_of_game_state_proc_states
};

struct s_game_state_proc_globals
{
	c_synchronized_long state; // e_game_state_proc_state
	uns32 wait_for_render_thread;
};
COMPILE_ASSERT(sizeof(s_game_state_proc_globals) == 0x8);

extern s_game_state_proc_globals& g_game_state_proc_globals;

extern void __cdecl game_state_call_after_load_procs(int32 game_state_proc_flags);
extern void __cdecl game_state_call_after_save_procs(int32 game_state_proc_flags);
extern void __cdecl game_state_call_before_load_procs(int32 game_state_proc_flags);
extern void __cdecl game_state_call_before_save_procs(int32 game_state_proc_flags);
extern e_game_state_proc_state __cdecl game_state_proc_globals_get_state();
extern void __cdecl game_state_proc_globals_initialize();

