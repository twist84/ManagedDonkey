#pragma once

#include "cseries/cseries.hpp"

class c_user_interface_main_menu_music
{
public:
	void __thiscall render_();

public:
	enum e_music_state
	{
		_music_state_stopped = 0,
		_music_state_playing,

		k_music_state_count
	};

	c_user_interface_main_menu_music();

private:
	void change_state(e_music_state new_state);
	void change_state_update();

public:
	real32 fade_out_progress();
	static int32 __cdecl get_music_fade_time_milliseconds();

private:
	int32 get_music_index();
	void handle_music_for_loading_screen();

public:
	bool music_done_fading_out();
	void render();
	void reset();

private:
	void start();
	void stop();

public:
	void update();

private:
	void update_game_shell_music_state();

public:
//protected:
	e_music_state m_music_state;
	e_music_state m_next_music_state;
	e_music_state m_game_shell_music_state;
	int32 m_last_music_state_set_time;
	int32 m_looping_sound_index;
	uns32 m_msecs_when_stopped;

	// >= profile builds
	//bool m_debug_render_music_state;
	static bool m_debug_render_music_state;
};
COMPILE_ASSERT(sizeof(c_user_interface_main_menu_music) == 0x18);

