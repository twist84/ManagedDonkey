#pragma once

#include "cseries/cseries.hpp"

#define PROFILER(...)

enum e_profile_summary_modes
{
	_profile_summary_all = 0,
	_profile_summary_objects,
	_profile_summary_graphics,
	_profile_summary_effects,
	_profile_summary_ai,
	_profile_summary_game_state,
	_profile_summary_environment_artist,

	k_profile_summary_count,
	k_profile_summary_first = _profile_summary_all,
};

extern bool profile_summary_enabled;
extern bool profile_summary_objects_enabled;
extern bool profile_summary_graphics_enabled;
extern bool profile_summary_effects_enabled;
extern bool profile_summary_ai_enabled;
extern bool profile_summary_environment_artist_enabled;
extern e_profile_summary_modes g_profile_summary_mode;
extern char const* const profile_dump_directory;
extern char const* const k_profile_summary_names[k_profile_summary_count];

extern void profiler_initialize();
extern void profiler_dispose();
extern void profiler_initialize_for_new_map();
extern void profiler_dispose_from_old_map();
extern void profile_render(rectangle2d const* screen_pixel_bounds, rectangle2d const* screen_safe_pixel_bounds);
extern void profile_summary_cycle();

