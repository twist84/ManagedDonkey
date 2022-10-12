#pragma once

#include "cseries/cseries.hpp"

struct s_online_user
{
	bool initialized;
	qword player_xuid;
	wchar_t player_name[16];
};
static_assert(sizeof(s_online_user) == 0x30);

extern s_online_user& g_online_user;

extern void __cdecl online_update();