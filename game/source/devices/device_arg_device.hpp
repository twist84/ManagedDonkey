#pragma once

#include "cseries/cseries.hpp"
#include "devices/devices.hpp"

struct _arg_device_datum
{
	uint16 flags;
	short pressed_script_index;
	short held_script_index;
	short pad;
	long unit_index;
	long unit_last_time_touched;
};
static_assert(sizeof(_arg_device_datum) == 0x10);

struct arg_device_datum
{
	long definition_index;
	_object_datum object;
	_device_datum device;
	_arg_device_datum arg_device;
};
static_assert(sizeof(arg_device_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_device_datum) + sizeof(_arg_device_datum));

struct object_placement_data;
struct s_scenario_arg_device;

extern long __cdecl device_arg_device_get_action_string(long arg_device_index);
extern bool __cdecl device_arg_device_new(long arg_device_index, object_placement_data* data, bool* out_of_memory);
extern void __cdecl device_arg_device_place(long arg_device_index, s_scenario_arg_device* scenario_arg_device);
extern void __cdecl device_arg_device_touch(long arg_device_index);
extern void __cdecl device_arg_device_touched(long arg_device_index, long unit_index, bool held_script);
extern bool __cdecl device_arg_device_touched_by_unit(long arg_device_index, long unit_index, long time);

