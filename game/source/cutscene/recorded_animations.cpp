#include "cutscene/recorded_animations.hpp"

#include "cseries/cseries.hpp"

bool __cdecl recorded_animation_controlling_unit(long object_index)
{
	return INVOKE(0x00681AF0, recorded_animation_controlling_unit, object_index);
}
