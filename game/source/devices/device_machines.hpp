#pragma once

#include "cseries/cseries.hpp"
#include "devices/devices.hpp"
#include "objects/objects.hpp"

struct _machine_datum
{
	dword_flags flags;
	long door_open_ticks;
	short pathfinding_policy;
	short door_machine_first_portal_index;
	short door_machine_portal_index_count;
	short door_machine_portal_structure_bsp_index;
};
static_assert(sizeof(_machine_datum) == 0x10);

struct machine_datum
{
	long definition_index;
	_object_datum object;
	_device_datum device;
	_machine_datum machine;
};
static_assert(sizeof(machine_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_device_datum) + sizeof(_machine_datum));

