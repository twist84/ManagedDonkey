#include "scenario/scenario_trigger_volumes.hpp"

bool __cdecl trigger_volume_get_matrix(scenario_trigger_volume const* volume, real_matrix4x3* matrix)
{
	return INVOKE(0x005FACC0, trigger_volume_get_matrix, volume, matrix);
}
