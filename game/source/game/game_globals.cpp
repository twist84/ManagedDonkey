#include "game/game_globals.hpp"

#include "cache/cache_files.hpp"
#include "cseries/console.hpp"
#include "memory/thread_local.hpp"
#include "scenario/scenario.hpp"

game_globals_storage* game_globals_get()
{
	if (!get_tls())
		return nullptr;

	TLS_REFERENCE(game_globals);
	return game_globals;
}

long get_map_minor_version()
{
	if (global_scenario)
		return cache_files_get_header()->minor_version;
	else
		return 0xFFFFFFFF;
}