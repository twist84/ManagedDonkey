#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"

struct s_network_message_view_establishment
{
	dword establishment_mode;
	dword establishment_identifier;

	bool signature_exists;
	dword signature_size;
	byte signature_data[0x3C];
};
static_assert(sizeof(s_network_message_view_establishment) == 0x4C);

struct s_network_message_player_acknowledge
{
	dword player_valid_mask;
	dword player_in_game_mask;
	c_static_array<s_player_identifier, 16> player_identifiers;
};
static_assert(sizeof(s_network_message_player_acknowledge) == 0x88);

