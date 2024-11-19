#pragma once

#include "cseries/cseries.hpp"
#include "devices/devices.hpp"
#include "objects/objects.hpp"

struct _terminal_datum
{
	word_flags flags;
	short state;
	short start_timer;
	short screen_state;
	short screen_state_timer;
	long reader_unit_index;
	real activation_value;
	real error_value;
	real frame_color_value;
	vector2d scroll_velocity;
	real_point2d scroll_position;
	real reading_mode;
	bool desired_reading_mode;
	short reading_mode_transition_ticks;
	short reading_mode_max_transition_ticks;
	real overlay_intensity;
	real hex_intensity;
	bool currently_seeking;
	bool finished_seek;
	short seek_transition_ticks;
	short seek_transition_max_ticks;
	short seek_ticks;
	vector2d seek_desired_velocity;
	vector2d seek_original_velocity;
	bool seek_transitioning_to_zero;
	real_point2d initial_position;
	vector2d b;
	vector2d c;
	bool can_exit;
};
static_assert(sizeof(_terminal_datum) == 0x78);

struct terminal_datum
{
	long definition_index;
	_object_datum object;
	_device_datum device;
	_terminal_datum terminal;
};
static_assert(sizeof(terminal_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_device_datum) + sizeof(_terminal_datum));

