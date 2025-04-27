#include "interface/user_interface_main_menu_music.hpp"

#include "memory/module.hpp"
#include "text/draw_string.hpp"

bool main_menu_music_render_debug_enabled = false;

c_user_interface_main_menu_music::c_user_interface_main_menu_music() :
	__unknown0(0),
	m_state(_music_state_stopped),
	m_game_shell_music_state(0),
	__unknownC(0),
	m_looping_sound_index(NONE),
	__unknown14(0),
	__unknown18(0)
{
	DECLFUNC(0x00AD54F0, c_user_interface_main_menu_music*, __thiscall, c_user_interface_main_menu_music*)(this);
}

void c_user_interface_main_menu_music::change_state(c_user_interface_main_menu_music::e_music_state state)
{
	INVOKE_CLASS_MEMBER(0x00AD5520, c_user_interface_main_menu_music, change_state, state);
}

void c_user_interface_main_menu_music::update_state()
{
	INVOKE_CLASS_MEMBER(0x00AD5530, c_user_interface_main_menu_music, update_state);
}

real32 c_user_interface_main_menu_music::fade_out_progress()
{
	return (real32)DECLFUNC(0x00AD55E0, real64, __thiscall, c_user_interface_main_menu_music*)(this);
}

//.text:00AD5690 ; 

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

HOOK_DECLARE_CLASS_MEMBER(0x00AD57E0, c_user_interface_main_menu_music, render);

void __thiscall c_user_interface_main_menu_music::render()
{
	//INVOKE_CLASS_MEMBER(0x00AD57E0, c_user_interface_main_menu_music, render);

	if (main_menu_music_render_debug_enabled)
	{
		static char const* music_states[k_music_state_count]
		{
			"music state stopped",
			"music state playing"
		};

		c_simple_font_draw_string draw_string;
		draw_string.set_color(global_real_argb_yellow);
		draw_string.draw(NULL, music_states[m_state]);
	}
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


