#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_player_profile.hpp"

struct c_controller_interface
{
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
	c_controller_interface m_controllers[4];
};
static_assert(sizeof(s_controller_globals) == 0x5120);

extern s_controller_globals& g_controller_globals;