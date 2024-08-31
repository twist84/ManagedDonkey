#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "units/units.hpp"

struct animation_playback_controller
{
	byte __data[0xC];
};
static_assert(sizeof(animation_playback_controller) == 0xC);

struct animation_thread :
	s_datum_header
{
	long unit_index;
	word ticks_left;
	word_flags flags;
	long relative_ticks;
	char const* event_stream;
	unit_control_data control;
	animation_playback_controller playback_controller;
	short playback_codec;
};
static_assert(sizeof(animation_thread) == 0xA4);

extern bool debug_recording;

extern bool __cdecl recorded_animation_controlling_unit(long object_index);
extern void __cdecl recorded_animations_dispose();
extern void __cdecl recorded_animations_dispose_from_old_map();
extern void __cdecl recorded_animations_initialize();
extern void __cdecl recorded_animations_initialize_for_new_map();
extern void __cdecl recorded_animations_update();

extern void render_debug_recording();

