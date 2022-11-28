#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_player_profile.hpp"

struct s_player_identifier;
struct c_controller_interface
{
	bool in_use();
	bool is_signed_in_to_machine();
	short get_user_index();
	void get_player_identifier(s_player_identifier* out_player_identifier);
	qword get_player_xuid();

	word_flags m_flags;
	short m_user_index;
	dword __unknown4;
	c_player_profile_interface m_player_profile;
	wchar_t m_display_name[16];
	byte __data1420[40];
};
static_assert(sizeof(c_controller_interface) == 0x1448);

struct s_controller_globals
{
	c_controller_interface m_controllers[k_number_of_controllers];
};
static_assert(sizeof(s_controller_globals) == 0x5120);

extern s_controller_globals& g_controller_globals;

enum e_controller_index;

extern c_controller_interface* __cdecl controller_get(e_controller_index controller_index);
extern void __cdecl controllers_render();

