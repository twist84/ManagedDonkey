#include "interface/user_interface_main_menu_music.hpp"

#include "memory/module.hpp"
#include "text/draw_string.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AD57E0, c_user_interface_main_menu_music, render_);

bool c_user_interface_main_menu_music::m_debug_render_music_state = false;

void __thiscall c_user_interface_main_menu_music::render_()
{
	c_user_interface_main_menu_music::render();
}

c_user_interface_main_menu_music::c_user_interface_main_menu_music() :
	m_music_state(_music_state_stopped),
	m_next_music_state(_music_state_stopped),
	m_game_shell_music_state(_music_state_stopped),
	m_last_music_state_set_time(0),
	m_looping_sound_index(NONE),
	m_msecs_when_stopped(0)
{
	//DECLFUNC(0x00AD54F0, void, __thiscall, c_user_interface_main_menu_music*)(this);

	m_debug_render_music_state = false;
	//c_user_interface_main_menu_music::reset();
}

void c_user_interface_main_menu_music::change_state(e_music_state new_state)
{
	//INVOKE_CLASS_MEMBER(0x00AD5520, c_user_interface_main_menu_music, change_state, new_state);

	m_next_music_state = new_state;
}

void c_user_interface_main_menu_music::change_state_update()
{
	INVOKE_CLASS_MEMBER(0x00AD5530, c_user_interface_main_menu_music, change_state_update);
}

real32 c_user_interface_main_menu_music::fade_out_progress()
{
	return (real32)DECLFUNC(0x00AD55E0, real64, __thiscall, c_user_interface_main_menu_music*)(this);
}

int32 __cdecl c_user_interface_main_menu_music::get_music_fade_time_milliseconds()
{
	return INVOKE(0x00AD5690, c_user_interface_main_menu_music::get_music_fade_time_milliseconds);
}

int32 c_user_interface_main_menu_music::get_music_index()
{
	return INVOKE_CLASS_MEMBER(0x00AD56C0, c_user_interface_main_menu_music, get_music_index);
}

void c_user_interface_main_menu_music::handle_music_for_loading_screen()
{
	INVOKE_CLASS_MEMBER(0x00AD56E0, c_user_interface_main_menu_music, handle_music_for_loading_screen);
}

bool c_user_interface_main_menu_music::music_done_fading_out()
{
	return INVOKE_CLASS_MEMBER(0x00AD5790, c_user_interface_main_menu_music, music_done_fading_out);
}

void c_user_interface_main_menu_music::render()
{
	//INVOKE_CLASS_MEMBER(0x00AD57E0, c_user_interface_main_menu_music, render);

	if (m_debug_render_music_state)
	{
		static const char* music_states[k_music_state_count]
		{
			"music state stopped",
			"music state playing"
		};

		c_simple_font_draw_string draw_string;
		draw_string.set_color(global_real_argb_yellow);
		draw_string.draw(nullptr, music_states[m_next_music_state]);
	}
}

void c_user_interface_main_menu_music::reset()
{
	INVOKE_CLASS_MEMBER(0x00AD57F0, c_user_interface_main_menu_music, reset);

	//m_music_state = _music_state_stopped;
	//m_next_music_state = _music_state_stopped;
	//m_last_music_state_set_time = 0;
	//m_game_shell_music_state = _music_state_stopped;
	//m_looping_sound_index = NONE;
}

void c_user_interface_main_menu_music::start()
{
	INVOKE_CLASS_MEMBER(0x00AD5820, c_user_interface_main_menu_music, start);
}

void c_user_interface_main_menu_music::stop()
{
	INVOKE_CLASS_MEMBER(0x00AD5850, c_user_interface_main_menu_music, stop);
}

void c_user_interface_main_menu_music::update()
{
	INVOKE_CLASS_MEMBER(0x00AD5880, c_user_interface_main_menu_music, update);
}

void c_user_interface_main_menu_music::update_game_shell_music_state()
{
	INVOKE_CLASS_MEMBER(0x00AD58F0, c_user_interface_main_menu_music, update_game_shell_music_state);
}


