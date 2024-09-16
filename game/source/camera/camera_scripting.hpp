#pragma once

#include "cseries/cseries.hpp"

enum e_camera_script_mode
{
	// scripted_camera_initialize_for_new_map
	_camera_script_mode_none = 0,

	// scripted_camera_initialize_for_control_point
	_camera_script_mode_point,

	// scripted_camera_initialize_for_constant_pan
	_camera_script_mode_point_pan,

	// scripted_camera_initialize_for_animation
	_camera_script_mode_animation,

	// scripted_camera_initialize_for_first_person_camera
	_camera_script_mode_first_person,

	// scripted_camera_initialize_for_cinematic_camera
	_camera_script_mode_cinematic,

	k_camera_script_mode_count
};

struct s_scripted_camera_globals
{
	byte __unknown0;
	byte __unknown1;

	c_enum<e_camera_script_mode, short, _camera_script_mode_none, k_camera_script_mode_count> mode;

	real camera_frame_start_time;
	real camera_frame_end_time;
	real camera_field_of_view;
	angle camera_angle; // in radians
	long camera_ticks;

	// union of camera data then other data
	byte __data18[0xD8];
};
static_assert(sizeof(s_scripted_camera_globals) == 0xF0);

extern void __cdecl scripted_camera_game_tick();
extern void __cdecl scripted_camera_initialize();

