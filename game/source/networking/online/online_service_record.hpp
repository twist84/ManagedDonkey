#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "text/unicode.hpp"

struct s_service_record_identity
{
	c_static_wchar_string<16> player_name;
	s_player_appearance appearance;
	bool extras_portal_debug;
	byte vidmaster;

	byte __data682[0x6];

	long skill_level;
	long player_total_xp;

	byte __data690[0x4];

	long player_rank;
	long player_grade;

	byte __data69C[0x4];
};
static_assert(sizeof(s_service_record_identity) == 0x6A0);

