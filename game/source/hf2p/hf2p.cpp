#include "hf2p/hf2p.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "fmod/fmod.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x006006F0, hf2p_game_initialize);
HOOK_DECLARE(0x00600790, hf2p_game_dispose);

bool& g_hf2p_first_run = *reinterpret_cast<bool*>(0x018B59D4);

void __cdecl game_statistics_reset()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00853FC0, game_statistics_reset);
}

void* hp2p_ui_proxy = reinterpret_cast<void*>(0x0244ED28);

void __cdecl hf2p_initialize()
{
	FUNCTION_BEGIN(true);

	//INVOKE(0x00600630, hf2p_initialize);

	//game_statistics_reset();
	//hp2p_ui_proxy = nullptr;
	//
	//// crashes up the call stack
	//// #TODO: investigate
	//fmod_initialize();
}

void __cdecl hf2p_game_initialize()
{
	FUNCTION_BEGIN(true);

	//HOOK_INVOKE(, hf2p_game_initialize);
}

void __cdecl hf2p_scenario_tags_load_finished()
{
	FUNCTION_BEGIN(true);

	//INVOKE(0x00600750, hf2p_scenario_tags_load_finished);

	g_hf2p_first_run = true;
	if (!g_hf2p_first_run)
	{
		hf2p_initialize();
		g_hf2p_first_run = true;
	}
}

void __cdecl hf2p_scenario_load()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00600770, hf2p_scenario_load);
}

void __cdecl hf2p_game_dispose()
{
	FUNCTION_BEGIN(true);

	//HOOK_INVOKE(, hf2p_game_dispose);
	//
	//fmod_terminate();
}
