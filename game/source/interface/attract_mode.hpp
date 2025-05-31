#pragma once

#include "cseries/cseries.hpp"

enum e_movie_type
{
	_bink_attract1_movie = 0,
	_bink_attract2_movie,
	_bink_attract3_movie,

	k_attract_mode_movie_count,

	_bink_intro_movie = k_attract_mode_movie_count,
	_bink_outro_movie,
	_bink_logo_microsoft_movie,
	_bink_logo_343_movie,
	_bink_loading_loop_movie,

	k_bink_movie_count,

	k_no_movie = -1
};

extern bool& g_should_music_be_playing;
extern e_movie_type& last_attract_movie;
extern uns32& g_attract_mode_countdown_timer;

extern bool __cdecl attract_mode_get_built_in_movie_path(e_movie_type movie, c_static_string<256>* out_fullpath);
extern int32 __cdecl attract_mode_get_countdown_time_milliseconds();
extern bool __cdecl attract_mode_get_movie_path(const char* movie_name, c_static_string<256>* out_fullpath);
extern void __cdecl attract_mode_reset_timer();
extern void __cdecl attract_mode_set_countdown_length(int32 countdown_length);
extern bool __cdecl attract_mode_should_music_be_playing();
extern bool __cdecl attract_mode_should_start();
extern void __cdecl attract_mode_start();
extern void __cdecl game_end_credits_start();

