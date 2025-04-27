#include "saved_games/game_state_procs.hpp"

#include "cseries/cseries.hpp"

void __cdecl game_state_call_after_load_procs(int32 game_state_proc_flags)
{
	INVOKE(0x0058A4B0, game_state_call_after_load_procs, game_state_proc_flags);
}

void __cdecl game_state_call_after_save_procs(int32 game_state_proc_flags)
{
	INVOKE(0x0058A550, game_state_call_after_save_procs, game_state_proc_flags);
}

void __cdecl game_state_call_before_load_procs(int32 game_state_proc_flags)
{
	INVOKE(0x0058A5F0, game_state_call_before_load_procs, game_state_proc_flags);
}

void __cdecl game_state_call_before_save_procs(int32 game_state_proc_flags)
{
	INVOKE(0x0058A6A0, game_state_call_before_save_procs, game_state_proc_flags);
}

void __cdecl game_state_proc_globals_dispose()
{
	INVOKE(0x0058A760, game_state_proc_globals_dispose);
}

e_game_state_proc_state __cdecl game_state_proc_globals_get_state()
{
	return INVOKE(0x0058A770, game_state_proc_globals_get_state);

	//return static_cast<e_game_state_proc_state>(g_game_state_proc_globals.state.peek());
}

void __cdecl game_state_proc_globals_initialize()
{
	INVOKE(0x0058A780, game_state_proc_globals_initialize);

	//g_game_state_proc_globals.state = _game_state_proc_state_none;
}

//.text:015967E0 ; `dynamic initializer for 'g_game_state_proc_globals''
//.text:015F9B70 ; `dynamic atexit destructor for 'g_game_state_proc_globals''

