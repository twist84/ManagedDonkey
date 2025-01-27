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

