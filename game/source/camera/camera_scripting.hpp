#pragma once

#include "cseries/cseries.hpp"

enum e_camera_script_mode
{
	_camera_script_mode_none = 0,
	_camera_script_mode_point,
	_camera_script_mode_point_pan,
	_camera_script_mode_animation,
	_camera_script_mode_first_person,
	_camera_script_mode_cinematic,

	k_camera_script_mode_count
};

struct s_cinematic_field_of_view
{
	real32 time_elapsed;
	real32 time_total;
	real32 start;
	real32 end;
};
COMPILE_ASSERT(sizeof(s_cinematic_field_of_view) == 0x10);

struct s_scripted_camera_globals
{
	bool enabled;
	bool first_update;
	c_enum<e_camera_script_mode, int16, _camera_script_mode_none, k_camera_script_mode_count> mode;
	s_cinematic_field_of_view fov;
	int32 game_tick_timer;
	real_point3d point;
	real_vector3d forward;
	real_vector3d up;
	int32 relative_object_index;

	union
	{
		byte storage[0xB0];

		//s_scripted_camera_control_point internal_control_point;
		//s_scripted_camera_constant_pan internal_contant_pan;
		//s_scripted_camera_animation internal_animation;
		//s_scripted_camera_cinematic internal_cinematic;
	};
};
COMPILE_ASSERT(sizeof(s_scripted_camera_globals) == 0xF0);

extern void __cdecl scripted_camera_game_tick();
extern void __cdecl scripted_camera_initialize();

