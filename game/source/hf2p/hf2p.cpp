#include "hf2p/hf2p.hpp"

#include "cseries/cseries.hpp"
#include "fmod/fmod.hpp"

bool& g_hf2p_first_run = *reinterpret_cast<bool*>(0x018B59D4);

void __cdecl hf2p_initialize()
{
	INVOKE(0x00600630, hf2p_initialize);
}

void __cdecl hf2p_dispose()
{
	INVOKE(0x00600790, hf2p_dispose);
}