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
	uns32 flags;
	int16 hud_override_index;
	uns16 pad;
};
COMPILE_ASSERT(sizeof(_control_datum) == 0x8);

struct control_datum
{
	int32 definition_index;
	_object_datum object;
	_device_datum device;
	_control_datum control;
};
COMPILE_ASSERT(sizeof(control_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_device_datum) + sizeof(_control_datum));

struct s_scenario_control;

extern void __cdecl control_destroyed(int32 control_index);
extern int32 __cdecl control_get_action_string(int32 control_index);
extern void __cdecl control_place(int32 control_index, const s_scenario_control* scenario_control);
extern void __cdecl control_toggle(int32 control_index);
extern void __cdecl control_touched(int32 control_index, int32 unit_index);

