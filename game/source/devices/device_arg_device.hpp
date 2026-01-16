#pragma once

#include "cseries/cseries.hpp"
#include "devices/devices.hpp"

struct _arg_device_datum
{
	uns16 flags;
	int16 pressed_script_index;
	int16 held_script_index;
	int16 pad;
	int32 unit_index;
	int32 unit_last_time_touched;
};
COMPILE_ASSERT(sizeof(_arg_device_datum) == 0x10);

struct arg_device_datum
{
	int32 definition_index;
	_object_datum object;
	_device_datum device;
	_arg_device_datum arg_device;
};
COMPILE_ASSERT(sizeof(arg_device_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_device_datum) + sizeof(_arg_device_datum));

struct object_placement_data;
struct s_scenario_arg_device;

extern int32 __cdecl device_arg_device_get_action_string(int32 arg_device_index);
extern bool __cdecl device_arg_device_new(int32 arg_device_index, object_placement_data* data, bool* out_of_memory);
extern void __cdecl device_arg_device_place(int32 arg_device_index, s_scenario_arg_device* scenario_arg_device);
extern void __cdecl device_arg_device_touch(int32 arg_device_index);
extern void __cdecl device_arg_device_touched(int32 arg_device_index, int32 unit_index, bool held_script);
extern bool __cdecl device_arg_device_touched_by_unit(int32 arg_device_index, int32 unit_index, int32 time);

