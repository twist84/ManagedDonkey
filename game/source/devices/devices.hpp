#pragma once

#include "animations/mixing_board/channels/animation_channel.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "objects/objects.hpp"

struct device_group_datum :
	s_datum_header
{
	word_flags flags;
	real __unknown4; // desired value, actual value
	real __unknown8; // desired value, actual value
	dword __unknownC;
};
static_assert(sizeof(device_group_datum) == 0x10);

struct s_device_animation_control
{
	real __unknown0;
	real __unknown4;
	real __unknown8;
	real __unknownC;
	real __unknown10;
	real __unknown14;
	real __unknown18;
	real __unknown1C;
};
static_assert(sizeof(s_device_animation_control) == 0x20);

struct _device_datum
{
	dword_flags flags;

	long power_group_index;
	real power;
	real power_velocity;

	long position_group_index;
	real position;
	real position_velocity;

	short delay_ticks;
	short __unknown1E;

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

