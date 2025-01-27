#pragma once

#include "cseries/cseries.hpp"
#include "devices/devices.hpp"

enum
{
	_control_usable_from_both_sides_bit = 0,
	_control_toggled_bit,
	_control_toggle_only_once_bit,

	NUMBER_OF_CONTROL_DATUM_FLAGS
};

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

struct s_scenario_control;

extern void __cdecl control_destroyed(long control_index);
extern long __cdecl control_get_action_string(long control_index);
extern control_datum* __cdecl control_get_datum(long control_index);
extern void __cdecl control_place(long control_index, s_scenario_control const* scenario_control);
extern void __cdecl control_toggle(long control_index);
extern void __cdecl control_touched(long control_index, long unit_index);

