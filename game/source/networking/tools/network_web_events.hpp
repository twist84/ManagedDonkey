#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"

struct s_network_web_event_local_player
{
	long controller_index;
	byte __pad4[0x4];
	qword player_xuid;
	wchar_t name[16];
};
static_assert(sizeof(s_network_web_event_local_player) == 0x30);

struct s_network_web_event_cheating_local_player
{
	qword machine_id;
	s_player_identifier player_identifier;
	wchar_t name[16];
	dword_flags cheat_flags;

	// pad?
	byte __data34[0x4];
};
static_assert(sizeof(s_network_web_event_cheating_local_player) == 0x38);

struct s_network_web_event_cheating_remote_player :
	s_network_web_event_cheating_local_player
{
};
static_assert(sizeof(s_network_web_event_cheating_remote_player) == sizeof(s_network_web_event_cheating_local_player));

