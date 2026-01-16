#pragma once

#include "cseries/cseries.hpp"
#include "units/units.hpp"

struct direction_playback_controller
{
	int16 yaw;
	int16 pitch;
};
COMPILE_ASSERT(sizeof(direction_playback_controller) == 0x4);

struct animation_playback_controller
{
	direction_playback_controller facing_control;
	direction_playback_controller aiming_control;
	direction_playback_controller looking_control;
};
COMPILE_ASSERT(sizeof(animation_playback_controller) == 0xC);

struct animation_thread :
	s_datum_header
{
	int32 unit_index;
	uns16 ticks_left;
	uns16 flags;
	int32 relative_ticks;
	const char* event_stream;
	unit_control_data controller;
	animation_playback_controller animation_state;
	int16 version;
};
COMPILE_ASSERT(sizeof(animation_thread) == 0xA4);

extern bool debug_recording;

extern bool __cdecl recorded_animation_controlling_unit(int32 object_index);
extern void __cdecl recorded_animations_dispose();
extern void __cdecl recorded_animations_dispose_from_old_map();
extern void __cdecl recorded_animations_initialize();
extern void __cdecl recorded_animations_initialize_for_new_map();
extern void __cdecl recorded_animations_update();

extern void render_debug_recording();

