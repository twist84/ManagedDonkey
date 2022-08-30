#include "game/game_globals.hpp"

#include "memory/thread_local.hpp"

game_globals_storage* game_globals_get()
{
	s_thread_local_storage* tls = get_tls();

	return (tls && tls->game_globals) ? tls->game_globals : nullptr;
}
