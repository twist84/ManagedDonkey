#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"

struct s_online_session_player
{
	c_flags_no_init<e_online_session_player_flags, uint16, k_online_session_player_flags_count> flags;
	uint64 xuid;
};
static_assert(sizeof(s_online_session_player) == 0x10);
static_assert(0x00 == OFFSETOF(s_online_session_player, flags));
static_assert(0x08 == OFFSETOF(s_online_session_player, xuid));

struct s_online_session
{
	e_controller_index controller_index;
	uint16 flags;
	long public_slot_count;
	long private_slot_count;
	s_transport_session_description session_description;
	uint64 nonce;
	void* handle;
	s_online_session_player players[16];
};
static_assert(sizeof(s_online_session) == 0x150);
static_assert(0x00 == OFFSETOF(s_online_session, controller_index));
static_assert(0x04 == OFFSETOF(s_online_session, flags));
static_assert(0x08 == OFFSETOF(s_online_session, public_slot_count));
static_assert(0x0C == OFFSETOF(s_online_session, private_slot_count));
static_assert(0x10 == OFFSETOF(s_online_session, session_description));
static_assert(0x40 == OFFSETOF(s_online_session, nonce));
static_assert(0x48 == OFFSETOF(s_online_session, handle));
static_assert(0x50 == OFFSETOF(s_online_session, players));

