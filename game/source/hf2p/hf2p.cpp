#include "hf2p/hf2p.hpp"

#include "cseries/cseries.hpp"
#include "fmod/fmod.hpp"

bool& g_hf2p_first_run = *reinterpret_cast<bool*>(0x018B59D4);

void __cdecl game_statistics_reset()
{
	INVOKE(0x00853FC0, game_statistics_reset);
}

void* hp2p_ui_proxy = reinterpret_cast<void*>(0x0244ED28);

void __cdecl hf2p_initialize()
{
	//INVOKE(0x00600630, hf2p_initialize);

	game_statistics_reset();
	hp2p_ui_proxy = nullptr;

	// crashes up the call stack
	// #TODO: investigate
	fmod_initialize();
}

void __cdecl hf2p_dispose()
{
	//INVOKE(0x00600790, hf2p_dispose);

	fmod_terminate();
}