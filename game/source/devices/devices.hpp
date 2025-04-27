#pragma once

#include "animations/mixing_board/channels/animation_channel.hpp"
#include "cseries/cseries.hpp"
#include "objects/objects.hpp"

struct device_group_datum :
	s_datum_header
{
	uint16 flags;
	real32 desired_value;
	real32 initial_value;
	uint32 __unknownC;
};
static_assert(sizeof(device_group_datum) == 0x10);

struct s_device_animation_control
{
	real32 current_position;
	real32 current_velocity;
	real32 time_remaining;
	real32 cruise_start_time;
	real32 cruise_end_time;
	real32 cruise_velocity;
	real32 exit_velocity;
	real32 exit_pos;
};
static_assert(sizeof(s_device_animation_control) == 0x20);

struct _device_datum
{
	uint32 flags;
	long power_group_index;
	real32 power;
	real32 power_velocity;
	long position_group_index;
	real32 position;
	real32 position_velocity;
	short delay_ticks;
	uint16 pad;
	s_device_animation_control position_track;
	s_device_animation_control power_and_overlay_track;
	c_animation_channel position_channel;
	c_animation_channel power_and_overlay_track_channel;
};
static_assert(sizeof(_device_datum) == 0xD8);

struct device_datum
{
	long definition_index;
	_object_datum object;
	_device_datum device;
};
static_assert(sizeof(device_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_device_datum));

extern long __cdecl device_group_get_from_scenario_index(short device_index);

