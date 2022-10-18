#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "text/unicode.hpp"

struct s_service_record_identity
{
	c_static_wchar_string<16> player_name;
	s_player_appearance appearance;
	byte __data[0x20];
};
static_assert(sizeof(s_service_record_identity) == 0x6A0);
