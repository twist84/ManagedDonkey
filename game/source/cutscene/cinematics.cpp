#include "cutscene/cinematics.hpp"

#include "cseries/cseries.hpp"

bool __cdecl cinematic_in_progress()
{
	return INVOKE(0x0067D270, cinematic_in_progress);
}