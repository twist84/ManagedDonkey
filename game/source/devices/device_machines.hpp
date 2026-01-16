#pragma once

#include "cseries/cseries.hpp"
#include "devices/devices.hpp"
#include "objects/objects.hpp"

enum // flags
{
	_machine_does_not_operate_automatically_bit = 0,
	_machine_does_not_close_automatically_bit,
	_machine_one_sided_bit,
	_machine_one_sided_for_player_bit,
	_machine_never_appears_locked_bit,
	_machine_opened_by_melee_attack_bit,
	_machine_in_motion_bit,
	_machine_ignores_player_bit,

	NUMBER_OF_MACHINE_DATUM_FLAGS
};

struct _machine_datum
{
	uns32 flags;
	int32 door_open_ticks;
	int16 pathfinding_policy;
	int16 door_machine_first_portal_index;
	int16 door_machine_portal_index_count;
	int16 door_machine_portal_structure_bsp_index;
};
COMPILE_ASSERT(sizeof(_machine_datum) == 0x10);

struct machine_datum
{
	int32 definition_index;
	_object_datum object;
	_device_datum device;
	_machine_datum machine;
};
COMPILE_ASSERT(sizeof(machine_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_device_datum) + sizeof(_machine_datum));

