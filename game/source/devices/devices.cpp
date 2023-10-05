#include "devices/devices.hpp"

long __cdecl device_group_get_from_scenario_index(short device_index)
{
	return INVOKE(0x00B93040, device_group_get_from_scenario_index, device_index);
}

