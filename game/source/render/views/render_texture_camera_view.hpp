#pragma once

#include "cseries/cseries.hpp"

struct s_render_texture_camera_globals
{
	bool camera_enable;
	bool camera_dynamic_lights_enable;
	
	short __unknown2;

	long camera_render_mode;

	long __unknown8;
	long __unknownC;
	long __unknown10;
	long __unknown14;
	long __unknown18;

	long camera_object_handle;
	long camera_marker_name;

	long __unknown24;

	real_point3d camera_position_world;
	real_point3d camera_target_object;

	byte __unknown40[0x1C];

	real camera_fov;
	real aspect_ratio;
	long camera_resolution_width;
	long camera_resolution_height;
};
static_assert(sizeof(s_render_texture_camera_globals) == 0x6C);

