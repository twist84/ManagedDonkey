#pragma once

#include "cseries/cseries.hpp"
#include "devices/devices.hpp"
#include "objects/objects.hpp"

struct _control_datum
{
	dword_flags flags;
	short hud_override_index;
	word pad;
};
static_assert(sizeof(_control_datum) == 0x8);

struct control_datum
{
	long definition_index;
	_object_datum object;
	_device_datum device;
	_control_datum control;
};
static_assert(sizeof(control_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_device_datum) + sizeof(_control_datum));

