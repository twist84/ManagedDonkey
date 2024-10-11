#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"

struct c_user_interface_main_menu_music
{
public:
	enum e_music_state
	{
		_music_state_stopped = 0,
		_music_state_playing,

		k_music_state_count
	};

	c_user_interface_main_menu_music();

private:
	void change_state(e_music_state state);
	void update_state();

public:
	real fade_out_progress();

private:
	long get_music_index();
	void handle_music_for_loading_screen();

public:
	bool music_done_fading_out();
	void __thiscall render();

private:
	void start();
	void stop();

public:
	void update();

private:
	void update_game_shell_music_state();

public:
//protected:
	long __unknown0;
	long m_state;
	long m_game_shell_music_state;
	long __unknownC;
	long m_looping_sound_index;
	long __unknown14;
	c_synchronized_long __unknown18;
};
static_assert(sizeof(c_user_interface_main_menu_music) == 0x1C);

