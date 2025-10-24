#include "interface/attract_mode.hpp"

#include "bink/bink_playback.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/user_interface.hpp"
#include "memory/module.hpp"
#include "sound/sound_manager.hpp"

REFERENCE_DECLARE(0x0191D300, bool, g_should_music_be_playing) = true;
REFERENCE_DECLARE(0x0191D330, e_movie_type, last_attract_movie) = k_no_movie;
REFERENCE_DECLARE(0x05260708, uns32, g_attract_mode_countdown_timer) = 0;

HOOK_DECLARE(0x00A99120, attract_mode_get_built_in_movie_path);
HOOK_DECLARE(0x00A99220, attract_mode_get_countdown_time_milliseconds);
HOOK_DECLARE(0x00A99250, attract_mode_get_movie_path);
HOOK_DECLARE(0x00A99270, attract_mode_reset_timer);
HOOK_DECLARE(0x00A99280, attract_mode_set_countdown_length);
HOOK_DECLARE(0x00A99290, attract_mode_should_music_be_playing);
//HOOK_DECLARE(0x00A992A0, attract_mode_should_start);
//HOOK_DECLARE(0x00A993A0, attract_mode_start);
HOOK_DECLARE(0x00A99460, game_end_credits_start);

bool __cdecl attract_mode_get_built_in_movie_path(e_movie_type movie, c_static_string<256>* out_fullpath)
{
	//return INVOKE(0x00A99120, attract_mode_get_built_in_movie_path, movie, out_fullpath);

	ASSERT(movie >= 0 && movie < k_bink_movie_count);
	ASSERT(out_fullpath);

	switch (movie)
	{
	case _bink_attract1_movie:
		attract_mode_get_movie_path("attract_1", out_fullpath);
		return true;
	case _bink_attract2_movie:
		attract_mode_get_movie_path("attract_2", out_fullpath);
		return true;
	case _bink_attract3_movie:
		attract_mode_get_movie_path("attract_3", out_fullpath);
		return true;
	case _bink_intro_movie:
		attract_mode_get_movie_path("intro", out_fullpath);
		return true;
	case _bink_outro_movie:
		attract_mode_get_movie_path("credits", out_fullpath);
		return true;
	case _bink_logo_microsoft_movie:
		attract_mode_get_movie_path("logo_microsoft", out_fullpath);
		return true;
	case _bink_logo_343_movie:
		attract_mode_get_movie_path("logo_343", out_fullpath);
		return true;
	case _bink_loading_loop_movie:
		attract_mode_get_movie_path("loading_loop", out_fullpath);
		return true;
	default:
		UNREACHABLE();
		break;
	}
	return false;
}

int32 __cdecl attract_mode_get_countdown_time_milliseconds()
{
	//return INVOKE(0x00A99220, attract_mode_get_countdown_time_milliseconds);

	int32 attract_mode_delay_seconds = 75;
	if (const s_user_interface_shared_globals* ui_shared_globals = user_interface_shared_tag_globals_try_and_get())
	{
		attract_mode_delay_seconds = ui_shared_globals->attract_mode_delay_seconds;
	}

	return 1000 * attract_mode_delay_seconds;
}

bool __cdecl attract_mode_get_movie_path(const char* movie_name, c_static_string<256>* out_fullpath)
{
	//return INVOKE(0x00A99250, attract_mode_get_movie_path, movie_name, out_fullpath);

	ASSERT(movie_name);
	ASSERT(out_fullpath);

	out_fullpath->print("bink\\%s.bik", movie_name);
	return true;
}

void __cdecl attract_mode_reset_timer()
{
	//INVOKE(0x00A99270, attract_mode_reset_timer);

	g_attract_mode_countdown_timer = user_interface_milliseconds();
}

void __cdecl attract_mode_set_countdown_length(int32 countdown_length)
{
	//INVOKE(0x00A99280, attract_mode_set_countdown_length, countdown_length);

	event(_event_warning, "ui: attract_mode_set_countdown_length() no longer does anything");
}

bool __cdecl attract_mode_should_music_be_playing()
{
	//return INVOKE(0x00A99290, attract_mode_should_music_be_playing);

	return g_should_music_be_playing;
}

bool __cdecl attract_mode_should_start()
{
	return INVOKE(0x00A992A0, attract_mode_should_start);
}

void __cdecl attract_mode_start()
{
	INVOKE(0x00A993A0, attract_mode_start);
}

void __cdecl game_end_credits_start()
{
	//INVOKE(0x00A99460, game_end_credits_start);

	sound_stop_all(0);
	c_static_string<256> movie_path;
	attract_mode_get_movie_path("credits", &movie_path);
	constexpr uns32 flags
		= FLAG(_bink_playback_button_click_stops_movie_bit)
		| FLAG(_bink_playback_prevent_events_to_ui_bit)
		| FLAG(_bink_playback_dont_render_ui_bit)
		| FLAG(_bink_playback_full_screen_bit)
		| FLAG(_bink_playback_return_to_main_menu_when_finished_bit);
	bink_playback_start(movie_path.get_string(), NONE, flags);
}

