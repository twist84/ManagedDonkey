#pragma once

#include "cseries/cseries.hpp"
#include "devices/devices.hpp"
#include "objects/objects.hpp"

enum e_terminal_flags
{
	_terminal_start_bit = 0,
	_terminal_start_scripted_scroll_bit,
	_terminal_scripted_scroll_bit,
	_terminal_disallow_error_bit,

	k_terminal_flags_count
};

enum e_terminal_state
{
	_terminal_state_default = 0,
	_terminal_state_fly_in,
	_terminal_state_ui,

	k_terminal_state_count
};

enum e_terminal_screen_state
{
	_terminal_screen_inactive = 0,
	_terminal_screen_intro,
	_terminal_screen_dummy,
	_terminal_screen_error,
	_terminal_screen_seek,
	_terminal_screen_story,

	k_terminal_screen_state_count
};

using c_terminal_flags = c_flags<e_terminal_flags, int16, k_terminal_flags_count>;
using c_terminal_state = c_enum<e_terminal_state, int16, _terminal_state_default, k_terminal_state_count>;
using c_terminal_screen_state = c_enum<e_terminal_screen_state, int16, _terminal_screen_inactive, k_terminal_screen_state_count>;

struct _terminal_datum
{
	c_terminal_flags flags;
	c_terminal_state state;
	int16 start_timer;
	c_terminal_screen_state screen_state;
	int16 screen_state_timer;
	int32 reader_unit_index;
	real32 activation_value;
	real32 error_value;
	real32 frame_color_value;
	real_vector2d scroll_velocity;
	real_point2d scroll_position;
	real32 reading_mode;
	bool desired_reading_mode;
	int16 reading_mode_transition_ticks;
	int16 reading_mode_max_transition_ticks;
	real32 overlay_intensity;
	real32 hex_intensity;
	bool currently_seeking;
	bool finished_seek;
	int16 seek_transition_ticks;
	int16 seek_transition_max_ticks;
	int16 seek_ticks;
	real_vector2d seek_desired_velocity;
	real_vector2d seek_original_velocity;
	bool seek_transitioning_to_zero;
	real_point2d initial_position;
	real_vector2d b;
	real_vector2d c;
	bool can_exit;
};
COMPILE_ASSERT(sizeof(_terminal_datum) == 0x78);

struct terminal_datum
{
	int32 definition_index;
	_object_datum object;
	_device_datum device;
	_terminal_datum terminal;
};
COMPILE_ASSERT(sizeof(terminal_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_device_datum) + sizeof(_terminal_datum));

